/* Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file GL.cs
 * @author Niklas Nummelin
 *
 * @platform WP 7.1
 **/

// GLES2XNA
// ----------------------------------------------------------------------------
// Maps OpenGL|ES 1.x calls into their XNA equivalent.
// OpenGL|ES is based on a c-memory model with pointers, so this is emulated
// by using byte arrays with offsets as pointers.
//
// List of known issues
// - Texture matrices won't work as expected (no support in xna 4.0 for windows phone).
//   However they are emulated by transforming texture coordinates when converting them from memory
//   to native xna vertex arrays.
// - Triangle fans aren't supported natively, but will be converted into triangle lists.
//   Unfortunately this process makes things slower.
// - A pool is used for temporary vertex arrays. This might require some memory.
//
// Todos:
// - Fix some kind of simple garbage collection for the memory pool to keep the memory footprint down.
// - Optimize STATIC_DRAW vertex buffers to be cached using native VertexBuffers until the dirty flag
//   (glBufferSubData has been called) has been raised.

using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using System.IO;
using System;
using System.Collections.Generic;

class GL : GLConstants
{
	private const float fix2flt = (1.0f / 65536.0f);

	private class GLMatrixMode
	{
		public const int MODELVIEW = 0;
		public const int PROJECTION = 1;
		public const int TEXTURE = 2;
	};

	protected internal struct GLPointer
	{
		public byte[] data;
		public int offset;
		public int type;
		public int stride;
		public int size;
		public GLBuffer buffer;
		public bool clientStateEnabled;
	};

	protected internal class GLBuffer
	{
		public byte[] data;
		public bool dirty;
		public int usage;
	}

	struct GLVertexType : IVertexType
	{
		public Vector3 pos;
		public Vector3 normal;
		public Vector4 color;
		public Vector2 tcoord;

		public static VertexDeclaration GetVertexDeclaration()
		{
			return new VertexDeclaration
			(
				new VertexElement(0, VertexElementFormat.Vector3, VertexElementUsage.Position, 0),
				new VertexElement(12, VertexElementFormat.Vector3, VertexElementUsage.Normal, 0),
				new VertexElement(24, VertexElementFormat.Vector4, VertexElementUsage.Color, 0),
				new VertexElement(40, VertexElementFormat.Vector2, VertexElementUsage.TextureCoordinate, 0)
			);
		}

		public VertexDeclaration VertexDeclaration
		{
			get
			{
				return GetVertexDeclaration();
			}
		}
	};

	private Matrix[] mMatrices = new Matrix[3];
	private int mCurrentMatrix;

	private Stack<Matrix>[] mMatrixStack = new Stack<Matrix>[3];

	private Color mCurrentColor;
	private float mCurrentClearDepth;
	private int mCurrentClearStencilValue;

	private GraphicsDevice mGraphicsDevice;
	private Color mCurrentClearColor;

	private int mFogMode;
	private float mFogDensity, mFogStart, mFogEnd;
	private Color mFogColor = new Color();

	private int mGLError;

	List<Texture> mTextures = new List<Texture>();
	List<GLBuffer> mBuffers = new List<GLBuffer>();
	int mCurrentBoundElementArrayBuffer = -1;
	int mCurrentBoundArrayBuffer = -1;
	int mCurrentBoundTexture = -1;

	private DepthStencilState mDepthStencilState;

	protected GLPointer mCurrentVertexPointer;
	protected GLPointer mCurrentColorPointer;
	protected GLPointer[] mCurrentTexCoordPointer = new GLPointer[GL.GL_MAX_TEXTURE_UNITS];
	protected GLPointer mCurrentNormalPointer;
	private int mClientActiveTexture;
	bool mNormalizeNormals = false;

	RasterizerState mFrontFaceMode;
	BlendState mCurrentBlendFunc;

	private BasicEffect mBasicEffect;

	public static VertexElementFormat GetVertexElementFormat(int glSize, int glType, out int size)
	{
		size = 16;
		if (glSize == 2)
		{
			size = sizeof(float) * 2;
			return VertexElementFormat.Vector2;
		}
		else if (glSize == 3)
		{
			size = sizeof(float) * 3;
			return VertexElementFormat.Vector3;
		}
		else if (glSize == 4)
		{
			size = sizeof(float) * 4;
			return VertexElementFormat.Vector4;
		}
		else
		{
			size = sizeof(float) * 4;
			return VertexElementFormat.Vector4;
		}
	}

	public static VertexDeclaration GetVertexDeclaration(GL gl)
	{
		List<VertexElement> elements = new List<VertexElement>();
		int offset = 0;

		if (gl.mCurrentVertexPointer.clientStateEnabled)
		{
			int size = 0;
			VertexElementFormat vformat = GetVertexElementFormat(gl.mCurrentVertexPointer.size, gl.mCurrentVertexPointer.type, out size);
			elements.Add(
				new VertexElement(offset,
					vformat,
					VertexElementUsage.Position, 0));
			offset += size;
		}

		for (int i = 0; i < GL.GL_MAX_TEXTURE_UNITS; i++)
		{
			if (gl.mCurrentTexCoordPointer[i].clientStateEnabled)
			{
				int size = 0;
				VertexElementFormat vformat = GetVertexElementFormat(gl.mCurrentTexCoordPointer[i].size, gl.mCurrentTexCoordPointer[i].type, out size);
				elements.Add(
					new VertexElement(offset,
						vformat,
						VertexElementUsage.TextureCoordinate, i));
				offset += size;
			}
		}

		if (gl.mCurrentNormalPointer.clientStateEnabled)
		{
			int size = 0;
			VertexElementFormat vformat = GetVertexElementFormat(3, gl.mCurrentNormalPointer.type, out size);
			elements.Add(
				new VertexElement(offset,
					vformat,
					VertexElementUsage.Normal, 0));
			offset += size;
		}

		if (gl.mCurrentColorPointer.clientStateEnabled)
		{
			int size = 0;
			VertexElementFormat vformat = GetVertexElementFormat(gl.mCurrentColorPointer.size, gl.mCurrentColorPointer.type, out size);
			elements.Add(
				new VertexElement(offset,
					vformat,
					VertexElementUsage.Color, 0));
			offset += size;
		}

		return new VertexDeclaration(offset, elements.ToArray());
	}

	class Pool<T>
	{
		protected static Dictionary<int, Stack<T[]>> mPool = new Dictionary<int, Stack<T[]>>();

		public static T[] GetElements(int numElements)
		{
			Stack<T[]> mSubPool = null;
			if (mPool.TryGetValue(numElements, out mSubPool))
			{
				return mSubPool.Pop();
			}
			else
			{
				return new T[numElements];
			}
		}

		// add some automatic garbage collection here if it becomes too big..
		public static void LeaveElements(T[] elems)
		{
			Stack<T[]> mSubPool = null;
			if (mPool.TryGetValue(elems.Length, out mSubPool))
			{
				mSubPool.Push(elems);
			}
			else
			{
				mSubPool = new Stack<T[]>();
				mPool.Add(elems.Length, mSubPool);
				mSubPool.Push(elems);
			}
		}
	}

	delegate float FloatConverterDelegate(byte[] data, int offset);

	FloatConverterDelegate GetFloatConverter(int glType)
	{
		switch (glType)
		{
			case GL.GL_FLOAT:
				return new FloatConverterDelegate(ReadFloat);
			case GL.GL_SHORT:
				return delegate(byte[] data, int offset)
					{
						return (float)ReadInt16(data, offset);
					};
			case GL.GL_FIXED:
				return delegate(byte[] data, int offset)
				{
					return (float)ReadInt32(data, offset) * fix2flt;
				};
			case GL.GL_UNSIGNED_BYTE:
				return delegate(byte[] data, int offset)
				{
					return (float)data[offset];
				};
			case GL.GL_BYTE:
				return delegate(byte[] data, int offset)
				{
					return (float)((sbyte)data[offset]);
				};
			case GL.GL_UNSIGNED_SHORT:
				return delegate(byte[] data, int offset)
				{
					return (float)ReadUint16(data, offset);
				};
		}

		return null;
	}

	static int GetSizeOfType(int glType)
	{
		switch (glType)
		{
			case GL.GL_FLOAT:
				return 4;
			case GL.GL_FIXED:
				return 4;
			case GL.GL_UNSIGNED_SHORT:
				return 2;
			case GL.GL_SHORT:
				return 2;
			case GL.GL_UNSIGNED_BYTE:
				return 1;
			case GL.GL_BYTE:
				return 1;
		}
		return -1;
	}

	void CreateVertexArrayFromPointers(int startIndex, int numVertices, GLVertexType[] vertices)
	{
		if (mCurrentVertexPointer.clientStateEnabled)
		{
			FloatConverterDelegate vertexConverter = GetFloatConverter(mCurrentVertexPointer.type);
			int vcomponentbytes = GetSizeOfType(mCurrentVertexPointer.type);
			int vstride = mCurrentVertexPointer.stride;
			if (mCurrentVertexPointer.stride == 0)
			{
				vstride = vcomponentbytes * mCurrentVertexPointer.size;
			}

			int vpointer = mCurrentVertexPointer.offset + startIndex * vstride;

			for (int i = 0; i < numVertices; i++)
			{
				vertices[i].pos.X = vertexConverter(mCurrentVertexPointer.data, vpointer + vcomponentbytes * 0);
				vertices[i].pos.Y = vertexConverter(mCurrentVertexPointer.data, vpointer + vcomponentbytes * 1);
				vertices[i].pos.Z = (mCurrentVertexPointer.size > 2) ?
					vertexConverter(mCurrentVertexPointer.data, vpointer + vcomponentbytes * 2) : 0;

				vpointer += vstride;
			}
		}

		if (mCurrentColorPointer.clientStateEnabled)
		{
			FloatConverterDelegate colorConverter = GetFloatConverter(mCurrentColorPointer.type);
			int ccomponentbytes = GetSizeOfType(mCurrentColorPointer.type);
			int cstride = mCurrentColorPointer.stride;
			if (mCurrentColorPointer.stride == 0)
			{
				cstride = ccomponentbytes * mCurrentColorPointer.size;
			}

			int cpointer = mCurrentColorPointer.offset + startIndex * cstride;
			for (int i = 0; i < numVertices; i++)
			{

				vertices[i].color.X = colorConverter(mCurrentColorPointer.data, cpointer + ccomponentbytes * 0);
				vertices[i].color.Y = colorConverter(mCurrentColorPointer.data, cpointer + ccomponentbytes * 1);
				vertices[i].color.Z = colorConverter(mCurrentColorPointer.data, cpointer + ccomponentbytes * 2);
				vertices[i].color.W = colorConverter(mCurrentColorPointer.data, cpointer + ccomponentbytes * 3);
				cpointer += cstride;
			}

		}


		if (mCurrentNormalPointer.clientStateEnabled)
		{
			FloatConverterDelegate normalConverter = GetFloatConverter(mCurrentNormalPointer.type);
			int ncomponentbytes = GetSizeOfType(mCurrentNormalPointer.type);
			int nstride = mCurrentNormalPointer.stride;
			if (mCurrentNormalPointer.stride == 0)
			{
				nstride = ncomponentbytes * mCurrentNormalPointer.size;
			}

			int npointer = mCurrentNormalPointer.offset + startIndex * nstride;

			for (int i = 0; i < numVertices; i++)
			{
				vertices[i].normal.X = normalConverter(mCurrentNormalPointer.data, npointer + ncomponentbytes * 0);
				vertices[i].normal.Y = normalConverter(mCurrentNormalPointer.data, npointer + ncomponentbytes * 1);
				vertices[i].normal.Z = normalConverter(mCurrentNormalPointer.data, npointer + ncomponentbytes * 2);
				if (mNormalizeNormals)
					vertices[i].normal.Normalize();
				npointer += nstride;
			}

		}


		if (mCurrentTexCoordPointer[0].clientStateEnabled)
		{
			FloatConverterDelegate texConverter = GetFloatConverter(mCurrentTexCoordPointer[0].type);
			int tcomponentbytes = GetSizeOfType(mCurrentTexCoordPointer[0].type);
			int tstride = mCurrentTexCoordPointer[0].stride;
			if (mCurrentTexCoordPointer[0].stride == 0)
			{
				tstride = tcomponentbytes * mCurrentTexCoordPointer[0].size;
			}

			bool ShouldTransformCoords = mMatrices[GLMatrixMode.TEXTURE] != Matrix.Identity;

			int tpointer = mCurrentTexCoordPointer[0].offset + startIndex * tstride;
			for (int i = 0; i < numVertices; i++)
			{
				vertices[i].tcoord.X = texConverter(mCurrentTexCoordPointer[0].data, tpointer + tcomponentbytes * 0);
				vertices[i].tcoord.Y = texConverter(mCurrentTexCoordPointer[0].data, tpointer + tcomponentbytes * 1);
				if (ShouldTransformCoords)
					vertices[i].tcoord = Vector2.Transform(vertices[i].tcoord, mMatrices[GLMatrixMode.TEXTURE]);
				tpointer += tstride;
			}
		}
	}

	void CreateIndexArrayFromPointers(byte[] indecies, int indeciesOffset, int vertexCount, int type, short[] indexData, out int maxIndex)
	{
		maxIndex = 0;
		if (type == GL.GL_UNSIGNED_SHORT)
		{
			int pointer = indeciesOffset;
			for (int i = 0; i < vertexCount; i++)
			{
				indexData[i] = ReadInt16(indecies, pointer);
				if (indexData[i] > maxIndex)
					maxIndex = indexData[i];
				pointer += sizeof(ushort);
			}
		}
		else if (type == GL.GL_UNSIGNED_BYTE)
		{
			int pointer = indeciesOffset;
			for (int i = 0; i < vertexCount; i++)
			{
				indexData[i] = indecies[i];
				pointer++;
			}
		}
	}

	public static short ReadInt16(byte[] buf, int offset)
	{
		int a = buf[offset + 0];
		int b = buf[offset + 1];
		return (short)((b << 8) | a);
	}

	public static ushort ReadUint16(byte[] buf, int offset)
	{
		int a = buf[offset + 0];
		int b = buf[offset + 1];
		return (ushort)((b << 8) | a);
	}

	public static uint ReadUint32(byte[] buf, int offset)
	{
		uint a = buf[offset + 0];
		uint b = buf[offset + 1];
		uint c = buf[offset + 2];
		uint d = buf[offset + 3];
		return ((d << 24) | (c << 16) | (b << 8) | a);
	}


	public static int ReadInt32(byte[] buf, int offset)
	{
		uint a = buf[offset + 0];
		uint b = buf[offset + 1];
		uint c = buf[offset + 2];
		uint d = buf[offset + 3];
		return (int)((d << 24) | (c << 16) | (b << 8) | a);
	}

	public static float ReadFloat(byte[] buf, int offset)
	{
		return BitConverter.ToSingle(buf, offset);
	}

	public static double ReadDouble(byte[] buf, int offset)
	{
		return BitConverter.ToDouble(buf, offset);
	}

	Vector4 ReadFloatVector4(byte[] data, int offset)
	{
		return new Vector4(
			ReadFloat(data, offset + 0 * 4),
			ReadFloat(data, offset + 1 * 4),
			ReadFloat(data, offset + 2 * 4),
			ReadFloat(data, offset + 3 * 4)
			);
	}

	Vector4 ReadFixedVector4(byte[] data, int offset)
	{
		return new Vector4(
			ReadInt32(data, offset + 0 * 4) * fix2flt,
			ReadInt32(data, offset + 1 * 4) * fix2flt,
			ReadInt32(data, offset + 2 * 4) * fix2flt,
			ReadInt32(data, offset + 3 * 4) * fix2flt
			);
	}

	Vector3 ToVector3(Vector4 v)
	{
		return new Vector3(v.X, v.Y, v.Z);
	}

	int GetPrimitiveCount(PrimitiveType primitiveType, int vertexCount)
	{
		if (primitiveType == PrimitiveType.LineList)
			return vertexCount / 2;
		else if (primitiveType == PrimitiveType.LineStrip)
			return vertexCount - 1;
		else if (primitiveType == PrimitiveType.TriangleList)
			return vertexCount / 3;
		else if (primitiveType == PrimitiveType.TriangleStrip)
			return vertexCount - 2;
		else
			return 0;
	}

	private bool ConvertGLPrimitiveType(int glType, out PrimitiveType primitiveType)
	{
		switch (glType)
		{
			case GL_TRIANGLES:
				primitiveType = PrimitiveType.TriangleList;
				break;
			case GL_TRIANGLE_STRIP:
				primitiveType = PrimitiveType.TriangleStrip;
				break;
			case GL_LINES:
				primitiveType = PrimitiveType.LineList;
				break;
			case GL_LINE_STRIP:
				primitiveType = PrimitiveType.LineStrip;
				break;
			case GL_TRIANGLE_FAN:
				primitiveType = PrimitiveType.TriangleList;
				break;
			default:
				RaiseGLError(GL_INVALID_VALUE);
				primitiveType = default(PrimitiveType);
				return false;
		}

		return true;
	}

	/*
	private bool GetGLTypeInfo(int glType, out int sizeInBytes, out Type type)
	{
		switch (glType)
		{
			case GL_FLOAT:
				sizeInBytes = sizeof(float);
				type = typeof(float);
				break;
			case GL_BYTE:
				sizeInBytes = sizeof(sbyte);
				type = typeof(sbyte);
				break;
			case GL_UNSIGNED_BYTE:
				sizeInBytes = sizeof(byte);
				type = typeof(byte);
				break;
			case GL_SHORT:
				sizeInBytes = sizeof(short);
				type = typeof(short);
				break;
			case GL_UNSIGNED_SHORT:
				sizeInBytes = sizeof(ushort);
				type = typeof(ushort);
				break;
			default:
				sizeInBytes = 0;
				type = null;
				return false;
		}

		return true;
	}
	*/

	public CompareFunction GetCompareFunction(int glCompare)
	{
		switch (glCompare)
		{
			case GL.GL_NEVER:
				return CompareFunction.Never;
			case GL.GL_LESS:
				return CompareFunction.Less;
			case GL.GL_EQUAL:
				return CompareFunction.Equal;
			case GL.GL_LEQUAL:
				return CompareFunction.LessEqual;
			case GL.GL_GREATER:
				return CompareFunction.Greater;
			case GL.GL_NOTEQUAL:
				return CompareFunction.NotEqual;
			case GL.GL_GEQUAL:
				return CompareFunction.GreaterEqual;
			case GL.GL_ALWAYS:
				return CompareFunction.Always;
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return CompareFunction.Never;
		}
	}

	struct GLBlendFunc
	{
		public GLBlendFunc(int sf, int df)
		{
			sfactor = sf;
			dfactor = df;
		}

		int sfactor;
		int dfactor;
	};

	private Dictionary<GLBlendFunc, BlendState> mBlendStates = new Dictionary<GLBlendFunc, BlendState>();

	public void SetupBlendStates()
	{
		//BlendState blendState = new BlendState();
		//blendState.AlphaSourceBlend = Blend.Zero;
		//blendState.AlphaDestinationBlend = Blend.SourceColor;
		//mBlendStates.Add(new GLBlendFunc(GL_ZERO, GL_SRC_COLOR), blendState);
	}

	public void Init(GraphicsDevice g)
	{
		SetGraphicsDevice(g);

		mCurrentClearColor = new Color(0.0f, 0.0f, 0.0f, 1.0f);
		mCurrentClearStencilValue = 0;
		mCurrentClearDepth = 1.0f;
		mCurrentColor = new Color();

		mMatrixStack[GLMatrixMode.MODELVIEW] = new Stack<Matrix>();
		mMatrixStack[GLMatrixMode.PROJECTION] = new Stack<Matrix>();
		mMatrixStack[GLMatrixMode.TEXTURE] = new Stack<Matrix>();
		glMatrixMode(GL.GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL.GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL.GL_MODELVIEW);
		glLoadIdentity();

		/*
		mMatrices[GLMatrixMode.MODELVIEW] = Matrix.Identity;
		mMatrices[GLMatrixMode.PROJECTION] = Matrix.Identity;
		mMatrices[GLMatrixMode.TEXTURE] = Matrix.Identity;
		mCurrentMatrix = GLMatrixMode.MODELVIEW;
		 */
		mCurrentVertexPointer.clientStateEnabled = false;
		mCurrentNormalPointer.clientStateEnabled = false;		
		for (int i = 0; i < GL.GL_MAX_TEXTURE_UNITS; i++)
			mCurrentTexCoordPointer[0].clientStateEnabled = false;
		mCurrentColorPointer.clientStateEnabled = false;

		mClientActiveTexture = 0; // GL_TEXTURE0

		mFogMode = GL_EXP;
		mFogDensity = 1.0f;
		mFogStart = 0.0f;
		mFogEnd = 1.0f;
		mFogColor = new Color(0.0f, 0.0f, 0.0f, 0.0f);

		mGLError = GL_NO_ERROR;

		mFrontFaceMode = RasterizerState.CullCounterClockwise;
		mGraphicsDevice.RasterizerState = RasterizerState.CullNone;
		mBasicEffect = new BasicEffect(mGraphicsDevice);

#if true
		mBasicEffect.World = Matrix.CreateWorld(new Vector3(0, 0, 0), new Vector3(0, 0, -1), new Vector3(0, 1, 0));
#else
		mBasicEffect.World = Matrix.CreateWorld(new Vector3(0, 0, 0), new Vector3(0, 0, 1), new Vector3(0, 1, 0));
#endif
		mGraphicsDevice.BlendState = BlendState.Opaque;
		mCurrentBlendFunc = BlendState.Opaque;

		mDepthStencilState = DepthStencilState.None;

		glDepthFunc(GL.GL_LESS);

		mNormalizeNormals = false;
		mSmoothShadingEnabled = true;

		SetupBlendStates();
	}

	bool mSmoothShadingEnabled = true;
	public void glShadeModel(int mode)
	{
		if (mode != GL.GL_FLAT && mode != GL.GL_SMOOTH)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (mode == GL.GL_FLAT)
		{
			mSmoothShadingEnabled = false;
		}
		else
		{
			mSmoothShadingEnabled = true;
		}
	}

	public String glGetString(int _enum)
	{
		switch (_enum)
		{
			case GL.GL_VENDOR:
				return "MoSync";
			case GL.GL_RENDERER:
				return "XNA";
			case GL.GL_VERSION:
				return "1.0";
			case GL.GL_EXTENSIONS:
				return "";
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return "";
		}
	}

	public void glAlphaFunc(int func, float _ref)
	{
	}

	public void glAlphaFuncx(int func, int _ref)
	{
	}

	public void glEnable(int what)
	{
		switch (what)
		{
			case GL.GL_DEPTH_TEST:
				mGraphicsDevice.DepthStencilState = DepthStencilState.Default;
				break;
			case GL.GL_ALPHA_TEST:
				break;
			case GL.GL_DITHER:
				break;
			case GL.GL_CULL_FACE:
				mGraphicsDevice.RasterizerState = mFrontFaceMode;
				break;
			case GL.GL_BLEND:
				mGraphicsDevice.BlendState = mCurrentBlendFunc;
				break;
			case GL.GL_TEXTURE_2D:
				mBasicEffect.TextureEnabled = true;
				break;
			case GL.GL_LIGHT0:
				mBasicEffect.DirectionalLight0.Enabled = true;
				break;
			case GL.GL_LIGHT1:
				mBasicEffect.DirectionalLight1.Enabled = true;
				break;
			case GL.GL_LIGHT2:
				mBasicEffect.DirectionalLight2.Enabled = true;
				break;
			case GL.GL_LIGHTING:
				mBasicEffect.LightingEnabled = true;
				break;
			case GL.GL_NORMALIZE:
				mNormalizeNormals = true;
				break;
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return;
		}
	}

	public void glDisable(int what)
	{
		switch (what)
		{
			case GL.GL_DEPTH_TEST:
				mGraphicsDevice.DepthStencilState = DepthStencilState.None;
				//mGraphicsDevice.DepthStencilState = DepthStencilState.None;
				//if (mDepthStencilState != mGraphicsDevice.DepthStencilState)
				//	mGraphicsDevice.DepthStencilState = mDepthStencilState;
				break;
			case GL.GL_DITHER:
				break;
			case GL.GL_ALPHA_TEST:
				break;
			case GL.GL_CULL_FACE:
				mGraphicsDevice.RasterizerState = RasterizerState.CullNone;
				break;
			case GL.GL_BLEND:
				mGraphicsDevice.BlendState = BlendState.Opaque;
				break;
			case GL.GL_TEXTURE_2D:
				mBasicEffect.TextureEnabled = false;
				break;
			case GL.GL_LIGHT0:
				mBasicEffect.DirectionalLight0.Enabled = false;
				break;
			case GL.GL_LIGHT1:
				mBasicEffect.DirectionalLight1.Enabled = false;
				break;
			case GL.GL_LIGHT2:
				mBasicEffect.DirectionalLight2.Enabled = false;
				break;
			case GL.GL_LIGHTING:
				mBasicEffect.LightingEnabled = false;
				break;
			case GL.GL_NORMALIZE:
				mNormalizeNormals = false;
				break;
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return;
		}
	}

	void SetupBasicEffectState()
	{
		mBasicEffect.View = mMatrices[GLMatrixMode.MODELVIEW];
		mBasicEffect.Projection = mMatrices[GLMatrixMode.PROJECTION];
		//mBasicEffect.LightingEnabled = false;

		if (mSmoothShadingEnabled && mCurrentColorPointer.clientStateEnabled)
			mBasicEffect.VertexColorEnabled = true;
		else
		{
			mBasicEffect.VertexColorEnabled = false;
			mBasicEffect.DiffuseColor = mCurrentColor.ToVector3();
			mBasicEffect.Alpha = 1.0f;
		}

		mGraphicsDevice.BlendFactor = mCurrentColor;
		mGraphicsDevice.BlendState = mCurrentBlendFunc;

		//if (mCurrentBoundTexture != -1)
		//	mBasicEffect.Parameters["Texture"].SetValue(mTextures[mCurrentBoundTexture]);
	}

	public void Close()
	{
	}

	public void SetGraphicsDevice(GraphicsDevice g)
	{
		mGraphicsDevice = g;
	}

	private void RaiseGLError(int error)
	{
		mGLError = error;
	}

	public int glGetError()
	{
		return mGLError;
	}

	public void glLoadIdentity()
	{
		mMatrices[mCurrentMatrix] = Matrix.Identity;
	}

	public void glMultMatrixf(byte[] data, int offset)
	{
		Matrix matrix = new Matrix(
		ReadFloat(data, offset + 0 * 16 + 0),
		ReadFloat(data, offset + 0 * 16 + 4),
		ReadFloat(data, offset + 0 * 16 + 8),
		ReadFloat(data, offset + 0 * 16 + 12),
		ReadFloat(data, offset + 1 * 16 + 0),
		ReadFloat(data, offset + 1 * 16 + 4),
		ReadFloat(data, offset + 1 * 16 + 8),
		ReadFloat(data, offset + 1 * 16 + 12),
		ReadFloat(data, offset + 2 * 16 + 0),
		ReadFloat(data, offset + 2 * 16 + 4),
		ReadFloat(data, offset + 2 * 16 + 8),
		ReadFloat(data, offset + 2 * 16 + 12),
		ReadFloat(data, offset + 3 * 16 + 0),
		ReadFloat(data, offset + 3 * 16 + 4),
		ReadFloat(data, offset + 3 * 16 + 8),
		ReadFloat(data, offset + 3 * 16 + 12)
		);

		mMatrices[mCurrentMatrix] = matrix * mMatrices[mCurrentMatrix];
	}

	public void glMultMatrixx(byte[] data, int offset)
	{
		Matrix matrix = new Matrix(
		ReadInt32(data, offset + 0 * 16 + 0) * fix2flt,
		ReadInt32(data, offset + 0 * 16 + 4) * fix2flt,
		ReadInt32(data, offset + 0 * 16 + 8) * fix2flt,
		ReadInt32(data, offset + 0 * 16 + 12) * fix2flt,
		ReadInt32(data, offset + 1 * 16 + 0) * fix2flt,
		ReadInt32(data, offset + 1 * 16 + 4) * fix2flt,
		ReadInt32(data, offset + 1 * 16 + 8) * fix2flt,
		ReadInt32(data, offset + 1 * 16 + 12) * fix2flt,
		ReadInt32(data, offset + 2 * 16 + 0) * fix2flt,
		ReadInt32(data, offset + 2 * 16 + 4) * fix2flt,
		ReadInt32(data, offset + 2 * 16 + 8) * fix2flt,
		ReadInt32(data, offset + 2 * 16 + 12) * fix2flt,
		ReadInt32(data, offset + 3 * 16 + 0) * fix2flt,
		ReadInt32(data, offset + 3 * 16 + 4) * fix2flt,
		ReadInt32(data, offset + 3 * 16 + 8) * fix2flt,
		ReadInt32(data, offset + 3 * 16 + 12) * fix2flt
		);
		mMatrices[mCurrentMatrix] = matrix * mMatrices[mCurrentMatrix];
	}
	public void glPushMatrix()
	{
		mMatrixStack[mCurrentMatrix].Push(mMatrices[mCurrentMatrix]);
	}

	public void glPopMatrix()
	{
		mMatrices[mCurrentMatrix] = mMatrixStack[mCurrentMatrix].Pop();
	}

	public void glRotatef(float angle, float x, float y, float z)
	{
		mMatrices[mCurrentMatrix] = Matrix.CreateFromAxisAngle(new Vector3(x, y, z), angle * 2.0f * (float)Math.PI / 360.0f) * mMatrices[mCurrentMatrix];
	}

	public void glRotatex(int angle, int x, int y, int z)
	{
		glRotatef(angle * fix2flt, x * fix2flt, y * fix2flt, z * fix2flt);
	}

	public void glTranslatef(float x, float y, float z)
	{
		mMatrices[mCurrentMatrix] = Matrix.CreateTranslation(new Vector3(x, y, z)) * mMatrices[mCurrentMatrix];
	}

	public void glTranslatex(int x, int y, int z)
	{
		glTranslatef(x * fix2flt, y * fix2flt, z * fix2flt);
	}

	public void glScalef(float x, float y, float z)
	{
		mMatrices[mCurrentMatrix] = Matrix.CreateScale(x, y, z) * mMatrices[mCurrentMatrix];
	}

	public void glScalex(int x, int y, int z)
	{
		glScalef(x * fix2flt, y * fix2flt, z * fix2flt);
	}

	public void glOrthof(float left, float right, float bottom, float top, float near, float far)
	{
		float width = right - left;
		float height = bottom - top;
		mMatrices[mCurrentMatrix] = Matrix.CreateOrthographic(width, height, near, far) * mMatrices[mCurrentMatrix];
		glScalef(1, -1, 1);
		glTranslatef(-width / 2.0f, -height / 2.0f, 0.0f); // glOrthof is a bit different than CreateOrthographic.
	}

	public void glFrustumf(float xmin, float xmax, float ymin, float ymax, float near, float far)
	{
		mMatrices[mCurrentMatrix] = Matrix.CreatePerspective(xmax - xmin, ymax - ymin, near, far) * mMatrices[mCurrentMatrix];
	}

	public void glFrustumx(int _left, int _right, int _bottom, int _top, int _znear, int _zfar)
	{
		glFrustumf(_left * fix2flt, _right * fix2flt, _bottom * fix2flt, _top * fix2flt, _znear * fix2flt, _zfar * fix2flt);
	}

	public void glOrthox(int _left, int _right, int _bottom, int _top, int _znear, int _zfar)
	{
		glOrthof(_left * fix2flt, _right * fix2flt, _bottom * fix2flt, _top * fix2flt, _znear * fix2flt, _zfar * fix2flt);
	}

	public void glMatrixMode(int matrixMode)
	{
		switch (matrixMode)
		{
			case GL_MODELVIEW:
				mCurrentMatrix = GLMatrixMode.MODELVIEW;
				break;
			case GL_PROJECTION:
				mCurrentMatrix = GLMatrixMode.PROJECTION;
				break;
			case GL_TEXTURE:
				mCurrentMatrix = GLMatrixMode.TEXTURE;
				break;
		}
	}

	public void glFogf(int pname, BinaryReader param)
	{
		switch (pname)
		{
			case GL_FOG_MODE:
				mFogMode = (int)param.ReadSingle();
				break;
			case GL_FOG_DENSITY:
				mFogDensity = param.ReadSingle();
				break;
			case GL_FOG_START:
				mFogStart = param.ReadSingle();
				break;
			case GL_FOG_END:
				mFogEnd = param.ReadSingle();
				break;
			case GL_FOG_COLOR:
				mFogColor = new Color(
					param.ReadSingle(),
					param.ReadSingle(),
					param.ReadSingle(),
					param.ReadSingle());
				break;
		}
	}

	public void glDepthMask(int flag)
	{
		if (flag != 0)
		{
			mDepthStencilState = DepthStencilState.Default;
		}
		else
		{
			mDepthStencilState = DepthStencilState.DepthRead;
		}
		if (mGraphicsDevice.DepthStencilState != mDepthStencilState)
			mGraphicsDevice.DepthStencilState = mDepthStencilState;
	}

	public void glFrontFace(int mode)
	{
		switch (mode)
		{
			case GL.GL_CW:
				mFrontFaceMode = RasterizerState.CullClockwise;
				break;
			case GL.GL_CCW:
				mFrontFaceMode = RasterizerState.CullCounterClockwise;
				break;
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return;
		}

		if (mGraphicsDevice.RasterizerState != RasterizerState.CullNone)
			mGraphicsDevice.RasterizerState = mFrontFaceMode;
	}

	public void glCullFace(int mode)
	{
		switch (mode)
		{
			case GL.GL_FRONT:
				break;
			case GL.GL_BACK:
				break;
			case GL.GL_FRONT_AND_BACK:
				break;
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return;

		}
	}

	public void glBlendFunc(int sfactor, int dfactor)
	{
		if (sfactor == GL.GL_SRC_ALPHA && dfactor == GL.GL_ONE_MINUS_SRC_ALPHA)
		{
			mCurrentBlendFunc = BlendState.AlphaBlend;
			return;
		}
		/*
		BlendState blendState = null;
		if(mBlendStates.TryGetValue(new GLBlendFunc(sfactor, dfactor), out blendState))
		{
			mCurrentBlendFunc = blendState;
		}
		 */
	}

	public void glLightModelfv(int pname, byte[] floats, int offset)
	{
	}

	public void glLightModelxv(int pname, byte[] fixeds, int offset)
	{
	}

	public void glLightModelf(int pname, float param)
	{
	}

	public void glLightModelx(int pname, int param)
	{
	}

	public void glLightfv(int _light, int pname, byte[] floats, int offset)
	{
		DirectionalLight light = null;

		switch (_light)
		{
			case GL.GL_LIGHT0:
				light = mBasicEffect.DirectionalLight0;
				break;
			case GL.GL_LIGHT1:
				light = mBasicEffect.DirectionalLight1;
				break;
			case GL.GL_LIGHT2:
				light = mBasicEffect.DirectionalLight2;
				break;
			default:
				return;
		}

		switch (pname)
		{
			case GL.GL_DIFFUSE:
				light.DiffuseColor = ToVector3(ReadFloatVector4(floats, offset));
				break;
			case GL.GL_SPECULAR:
				light.SpecularColor = ToVector3(ReadFloatVector4(floats, offset));
				break;
			case GL.GL_POSITION:
				Vector4 pos = ReadFloatVector4(floats, offset);
				if (pos.W == 0.0f)
				{
					light.Direction = ToVector3(pos);
				}
				break;

		}
	}

	public void glLightxv(int _light, int pname, byte[] fixeds, int offset)
	{
		DirectionalLight light = null;

		switch (_light)
		{
			case GL.GL_LIGHT0:
				light = mBasicEffect.DirectionalLight0;
				break;
			case GL.GL_LIGHT1:
				light = mBasicEffect.DirectionalLight1;
				break;
			case GL.GL_LIGHT2:
				light = mBasicEffect.DirectionalLight2;
				break;
			default:
				return;
		}

		switch (pname)
		{
			case GL.GL_DIFFUSE:
				light.DiffuseColor = ToVector3(ReadFixedVector4(fixeds, offset));
				break;
			case GL.GL_SPECULAR:
				light.SpecularColor = ToVector3(ReadFixedVector4(fixeds, offset));
				break;
			case GL.GL_POSITION:
				Vector4 pos = ReadFixedVector4(fixeds, offset);
				if (pos.W == 0.0f)
				{
					light.Direction = ToVector3(pos);
				}
				break;
		}
	}

	public void glLightf(int light, int pname, float data)
	{
	}

	public void glLightx(int light, int pname, int data)
	{
	}

	bool VerifyMaterialParams(int face, int pname)
	{
		if (face != GL.GL_FRONT_AND_BACK)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return false;
		}

		if (pname != GL.GL_AMBIENT && pname != GL.GL_DIFFUSE &&
			pname != GL.GL_SPECULAR && pname != GL.GL_EMISSION &&
			pname != GL.GL_AMBIENT_AND_DIFFUSE)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return false;
		}

		return true;
	}

	public void glMaterialf(int face, int pname, float param)
	{
		if (face != GL.GL_FRONT_AND_BACK && pname != GL.GL_SHININESS)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		mBasicEffect.SpecularPower = param;
	}

	public void glMaterialx(int face, int pname, int param)
	{
		if (face != GL.GL_FRONT_AND_BACK && pname != GL.GL_SHININESS)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		mBasicEffect.SpecularPower = param * fix2flt;
	}

	public void glMaterialfv(int face, int pname, byte[] param, int offset)
	{
		if (!VerifyMaterialParams(face, pname))
			return;

		switch (pname)
		{
			case GL.GL_AMBIENT:
				mBasicEffect.AmbientLightColor = ToVector3(ReadFloatVector4(param, offset));
				break;
			case GL.GL_DIFFUSE:
				mBasicEffect.DiffuseColor = ToVector3(ReadFloatVector4(param, offset));
				break;
			case GL.GL_SPECULAR:
				mBasicEffect.SpecularColor = ToVector3(ReadFloatVector4(param, offset));
				break;
			case GL.GL_EMISSION:
				mBasicEffect.EmissiveColor = ToVector3(ReadFloatVector4(param, offset));
				break;
			case GL.GL_SHININESS:
				mBasicEffect.SpecularPower = ReadFloat(param, offset);
				break;
			case GL.GL_AMBIENT_AND_DIFFUSE:
				mBasicEffect.AmbientLightColor = ToVector3(ReadFloatVector4(param, offset));
				mBasicEffect.DiffuseColor = ToVector3(ReadFloatVector4(param, offset));
				break;
		}
	}

	public void glMaterialxv(int face, int pname, byte[] param, int offset)
	{
		if (!VerifyMaterialParams(face, pname))
			return;

		switch (pname)
		{
			case GL.GL_AMBIENT:
				mBasicEffect.AmbientLightColor = ToVector3(ReadFixedVector4(param, offset));
				break;
			case GL.GL_DIFFUSE:
				mBasicEffect.DiffuseColor = ToVector3(ReadFixedVector4(param, offset));
				break;
			case GL.GL_SPECULAR:
				mBasicEffect.SpecularColor = ToVector3(ReadFixedVector4(param, offset));
				break;
			case GL.GL_EMISSION:
				mBasicEffect.EmissiveColor = ToVector3(ReadFixedVector4(param, offset));
				break;
			case GL.GL_SHININESS:
				mBasicEffect.SpecularPower = ReadInt32(param, offset) * fix2flt;
				break;
			case GL.GL_AMBIENT_AND_DIFFUSE:
				mBasicEffect.AmbientLightColor = ToVector3(ReadFixedVector4(param, offset));
				mBasicEffect.DiffuseColor = ToVector3(ReadFixedVector4(param, offset));
				break;
		}
	}
	public void glScissor(int x, int y, int w, int h)
	{
		mGraphicsDevice.ScissorRectangle = new Rectangle(x, y, w, h);
	}

	public void glClearColor(float r, float g, float b, float a)
	{
		mCurrentClearColor = new Color(r, g, b, a);
	}

	public void glClearColorx(int r, int g, int b, int a)
	{
		glClearColor(r * fix2flt, g * fix2flt, b * fix2flt, a * fix2flt);
	}

	public void glClearDepthf(float d)
	{
		mCurrentClearDepth = d;
	}

	public void glClearStencil(int s)
	{
		mCurrentClearStencilValue = s;
	}

	public void glClear(int mask)
	{
		ClearOptions clearOptions = 0;

		if ((mask & GL_DEPTH_BUFFER_BIT) != 0)
		{
			clearOptions |= ClearOptions.DepthBuffer;
		}
		if ((mask & GL_STENCIL_BUFFER_BIT) != 0)
		{
			clearOptions |= ClearOptions.Stencil;
		}
		if ((mask & GL_COLOR_BUFFER_BIT) != 0)
		{
			clearOptions |= ClearOptions.Target;
		}

		mGraphicsDevice.Clear(clearOptions, mCurrentClearColor, mCurrentClearDepth, mCurrentClearStencilValue);
	}

	public void glColor4f(float r, float g, float b, float a)
	{
		mCurrentColor = new Color(r, g, b, a);
	}

	public void glColor4x(int r, int g, int b, int a)
	{
		glColor4f(r * fix2flt, g * fix2flt, b * fix2flt, a * fix2flt);
	}

	public void glColor4ub(int r, int g, int b, int a)
	{
		glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	public void glEnableClientState(int mode)
	{
		if (mode == GL_COLOR_ARRAY)
			mCurrentColorPointer.clientStateEnabled = true;
		else if (mode == GL_TEXTURE_COORD_ARRAY)
			mCurrentTexCoordPointer[mClientActiveTexture].clientStateEnabled = true;
		else if (mode == GL_VERTEX_ARRAY)
			mCurrentVertexPointer.clientStateEnabled = true;
		else if (mode == GL_NORMAL_ARRAY)
			mCurrentNormalPointer.clientStateEnabled = true;
		else
			RaiseGLError(GL_INVALID_VALUE);
	}

	public void glDisableClientState(int mode)
	{
		if (mode == GL_COLOR_ARRAY)
			mCurrentColorPointer.clientStateEnabled = false;
		else if (mode == GL_TEXTURE_COORD_ARRAY)
			mCurrentTexCoordPointer[mClientActiveTexture].clientStateEnabled = false;
		else if (mode == GL_VERTEX_ARRAY)
			mCurrentVertexPointer.clientStateEnabled = false;
		else if (mode == GL_NORMAL_ARRAY)
			mCurrentNormalPointer.clientStateEnabled = false;
		else
			RaiseGLError(GL_INVALID_VALUE);
	}

	public void glVertexPointer(int size, int type, int stride, byte[] vertexPointer, int vertexOffset)
	{
		if (size < 2 || size > 4 || stride < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		if (type != GL.GL_FLOAT && type != GL.GL_BYTE && type != GL.GL_SHORT && type != GL.GL_FIXED)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (mCurrentBoundArrayBuffer != -1)
		{
			mCurrentVertexPointer.buffer = mBuffers[mCurrentBoundArrayBuffer];
			vertexPointer = mBuffers[mCurrentBoundArrayBuffer].data;
		}
		else
		{
			mCurrentVertexPointer.buffer = null;
		}

		mCurrentVertexPointer.size = size;
		mCurrentVertexPointer.type = type;
		mCurrentVertexPointer.stride = stride;
		mCurrentVertexPointer.data = vertexPointer;
		mCurrentVertexPointer.offset = vertexOffset;
	}

	public void glClientActiveTexture(int texture)
	{
		if (texture < GL.GL_TEXTURE0 || texture >= GL_TEXTURE0 + GL.GL_MAX_TEXTURE_UNITS)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		mClientActiveTexture = texture - GL.GL_TEXTURE0;
	}

	public void glTexCoordPointer(int size, int type, int stride, byte[] texPointer, int texOffset)
	{
		if (size < 2 || size > 4 || stride < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		if (type != GL.GL_FLOAT && type != GL.GL_BYTE && type != GL.GL_SHORT && type != GL.GL_FIXED)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (mCurrentBoundArrayBuffer != -1)
		{
			mCurrentTexCoordPointer[mClientActiveTexture].buffer = mBuffers[mCurrentBoundArrayBuffer];
			texPointer = mBuffers[mCurrentBoundArrayBuffer].data;
		}
		else
		{
			mCurrentTexCoordPointer[mClientActiveTexture].buffer = null;
		}

		mCurrentTexCoordPointer[mClientActiveTexture].size = size;
		mCurrentTexCoordPointer[mClientActiveTexture].type = type;
		mCurrentTexCoordPointer[mClientActiveTexture].stride = stride;
		mCurrentTexCoordPointer[mClientActiveTexture].data = texPointer;
		mCurrentTexCoordPointer[mClientActiveTexture].offset = texOffset;
	}


	public void glNormalPointer(int type, int stride, byte[] nPointer, int nOffset)
	{
		if (stride < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		if (type != GL.GL_FLOAT && type != GL.GL_BYTE && type != GL.GL_SHORT && type != GL.GL_FIXED)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (mCurrentBoundArrayBuffer != -1)
		{
			mCurrentNormalPointer.buffer = mBuffers[mCurrentBoundArrayBuffer];
			nPointer = mBuffers[mCurrentBoundArrayBuffer].data;
		}
		else
		{
			mCurrentNormalPointer.buffer = null;
		}

		mCurrentNormalPointer.type = type;
		mCurrentNormalPointer.stride = stride;
		mCurrentNormalPointer.data = nPointer;
		mCurrentNormalPointer.offset = nOffset;
		mCurrentNormalPointer.size = 3;
	}

	public void glColorPointer(int size, int type, int stride, byte[] cPointer, int cOffset)
	{
		if (size != 4 || stride < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		if (type != GL.GL_FLOAT && type != GL.GL_UNSIGNED_BYTE && type != GL.GL_FIXED)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (mCurrentBoundArrayBuffer != -1)
		{
			mCurrentColorPointer.buffer = mBuffers[mCurrentBoundArrayBuffer];
			cPointer = mBuffers[mCurrentBoundArrayBuffer].data;
		}
		else
		{
			mCurrentColorPointer.buffer = null; ;
		}

		mCurrentColorPointer.size = size;
		mCurrentColorPointer.type = type;
		mCurrentColorPointer.stride = stride;
		mCurrentColorPointer.data = cPointer;
		mCurrentColorPointer.offset = cOffset;
	}

	// Maybe use this to calculate hash of vertex data?
	public class Jenkins96
	{
		private static uint a, b, c;

		private static void Mix()
		{
			a -= b; a -= c; a ^= (c >> 13);
			b -= c; b -= a; b ^= (a << 8);
			c -= a; c -= b; c ^= (b >> 13);
			a -= b; a -= c; a ^= (c >> 12);
			b -= c; b -= a; b ^= (a << 16);
			c -= a; c -= b; c ^= (b >> 5);
			a -= b; a -= c; a ^= (c >> 3);
			b -= c; b -= a; b ^= (a << 10);
			c -= a; c -= b; c ^= (b >> 15);
		}

		public static uint ComputeHash(byte[] data, int offset, int length)
		{
			int len = offset + length;
			a = b = 0x9e3779b9;
			c = 0;
			int i = offset;
			while (i + 12 <= len)
			{
				a += (uint)data[i++] |
					((uint)data[i++] << 8) |
					((uint)data[i++] << 16) |
					((uint)data[i++] << 24);
				b += (uint)data[i++] |
					((uint)data[i++] << 8) |
					((uint)data[i++] << 16) |
					((uint)data[i++] << 24);
				c += (uint)data[i++] |
					((uint)data[i++] << 8) |
					((uint)data[i++] << 16) |
					((uint)data[i++] << 24);
				Mix();
			}
			c += (uint)len;
			if (i < len)
				a += data[i++];
			if (i < len)
				a += (uint)data[i++] << 8;
			if (i < len)
				a += (uint)data[i++] << 16;
			if (i < len)
				a += (uint)data[i++] << 24;
			if (i < len)
				b += (uint)data[i++];
			if (i < len)
				b += (uint)data[i++] << 8;
			if (i < len)
				b += (uint)data[i++] << 16;
			if (i < len)
				b += (uint)data[i++] << 24;
			if (i < len)
				c += (uint)data[i++] << 8;
			if (i < len)
				c += (uint)data[i++] << 16;
			if (i < len)
				c += (uint)data[i++] << 24;
			Mix();
			return c;
		}
	}

	public class VertexBufferKey
	{

		private GLPointer mVertexPointer;
		private GLPointer mTexCoordPointer;
		private GLPointer mColorPointer;
		private GLPointer mNormalPointer;
		private int mStartIndex;
		private int mVertexCount;
		private uint mTotalHash;

		public VertexBufferKey(GLPointer v, GLPointer t, GLPointer c, GLPointer n, int startIndex, int vertexCount)
		{
			mVertexPointer = v;
			mTexCoordPointer = t;
			mColorPointer = c;
			mNormalPointer = n;
			mStartIndex = startIndex;
			mVertexCount = vertexCount;

			mTotalHash = ComputeHash(startIndex, vertexCount);
		}

		public uint ComputeHash(int startIndex, int vertexCount)
		{
			uint hash;
			int stride = mVertexPointer.stride;
			if (stride == 0)
				stride = GetSizeOfType(mVertexPointer.type) * mVertexPointer.size;

			if ((mVertexPointer.buffer != null) && (mVertexPointer.buffer.usage == GL.GL_STATIC_DRAW))
				hash = 0;
			else
				hash = Jenkins96.ComputeHash(mVertexPointer.data, mVertexPointer.offset + startIndex * stride, vertexCount * stride);

			if (mTexCoordPointer.clientStateEnabled)
			{
				stride = mTexCoordPointer.stride;
				if (stride == 0)
					stride = GetSizeOfType(mTexCoordPointer.type) * mTexCoordPointer.size;

				if ((mTexCoordPointer.buffer != null) && (mTexCoordPointer.buffer.usage == GL.GL_STATIC_DRAW))
					hash ^= 0;
				else
					hash ^= Jenkins96.ComputeHash(mTexCoordPointer.data, mTexCoordPointer.offset + startIndex * stride, vertexCount * stride);
			}

			if (mColorPointer.clientStateEnabled)
			{
				stride = mColorPointer.stride;
				if (stride == 0)
					stride = GetSizeOfType(mColorPointer.type) * mColorPointer.size;

				if ((mColorPointer.buffer != null) && (mColorPointer.buffer.usage == GL.GL_STATIC_DRAW))
					hash ^= 0;
				else
					hash ^= Jenkins96.ComputeHash(mColorPointer.data, mColorPointer.offset + startIndex * stride, vertexCount * stride);
			}

			if (mNormalPointer.clientStateEnabled)
			{
				stride = mNormalPointer.stride;
				if (stride == 0)
					stride = GetSizeOfType(mNormalPointer.type) * mNormalPointer.size;

				if ((mNormalPointer.buffer != null) && (mNormalPointer.buffer.usage == GL.GL_STATIC_DRAW))
					hash ^= 0;
				else
					hash ^= Jenkins96.ComputeHash(mNormalPointer.data, mNormalPointer.offset + startIndex * stride, vertexCount * stride);
			}
			return hash;
		}

		public class EqualityComparer : IEqualityComparer<VertexBufferKey>
		{
			public bool Equals(VertexBufferKey x, VertexBufferKey y)
			{
				return ((x.mVertexPointer.Equals(y.mVertexPointer)) && (x.mTexCoordPointer.Equals(y.mTexCoordPointer)) &&
						(x.mColorPointer.Equals(y.mColorPointer)) && (x.mNormalPointer.Equals(y.mNormalPointer)) &&
						(x.mStartIndex == y.mStartIndex) && (x.mVertexCount == y.mVertexCount) && (x.mTotalHash == y.mTotalHash));

			}
			public int GetHashCode(VertexBufferKey x)
			{
				return x.mVertexPointer.GetHashCode() ^ x.mTexCoordPointer.GetHashCode() ^
					x.mColorPointer.GetHashCode() ^ x.mNormalPointer.GetHashCode() ^
					x.mStartIndex.GetHashCode() ^ x.mVertexCount.GetHashCode() ^ (int)x.mTotalHash;
			}
		}
	}

	Dictionary<VertexBufferKey, VertexBuffer> mCachedVertices = new Dictionary<VertexBufferKey, VertexBuffer>(new VertexBufferKey.EqualityComparer());

	public void glDrawArrays(int mode, int startIndex, int vertexCount)
	{
		if (mCurrentVertexPointer.clientStateEnabled == false)
			return;

		if (vertexCount < 0)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (vertexCount == 0)
		{
			return;
		}

		PrimitiveType primitiveType;
		if (!ConvertGLPrimitiveType(mode, out primitiveType))
		{
			RaiseGLError(GL_INVALID_VALUE);
			return;
		}

		SetupBasicEffectState();

		mBasicEffect.CurrentTechnique.Passes[0].Apply();
		VertexDeclaration decl = GLVertexType.GetVertexDeclaration();

		// this caching mechanism is only possible using buffers
		// we should raise a flag it the data has been changed in a buffer
		// and re-read it.
#if false
		VertexBuffer vertexBuffer = null;

		VertexBufferKey vbk = new VertexBufferKey(mCurrentVertexPointer, mCurrentTexCoordPointer[0], mCurrentColorPointer, mCurrentNormalPointer,
			startIndex, vertexCount);

		if (!mCachedVertices.TryGetValue(vbk, out vertexBuffer))
		{
			GLVertexType[] vertices = new GLVertexType[vertexCount];
			CreateVertexArrayFromPointers(startIndex, vertexCount, vertices);

			// slow fallback for triangle fans.
			if (mode == GL.GL_TRIANGLE_FAN)
			{
				// We simulate them using a triangle list.
				primitiveType = PrimitiveType.TriangleList;
				vertexCount = (vertices.Length - 2) * 3;
				GLVertexType[] newVertices = new GLVertexType[vertexCount];
				int newIndex = 0;
				for (int i = 2; i < vertices.Length; i++)
				{
					newVertices[newIndex++] = vertices[0];
					newVertices[newIndex++] = vertices[i - 1];
					newVertices[newIndex++] = vertices[i];
				}
				vertices = newVertices;
			}

			// if everything matches except hash. Just upload new data..
			// else create new buffer.
			vertexBuffer = new VertexBuffer(mGraphicsDevice, decl, vertices.Length, BufferUsage.WriteOnly);
			vertexBuffer.SetData<GLVertexType>(vertices);

			mCachedVertices.Add(vbk, vertexBuffer);
		}
		else
		{
			if (mode == GL.GL_TRIANGLE_FAN)
			{
				primitiveType = PrimitiveType.TriangleList;
			}
		}
		mGraphicsDevice.SetVertexBuffer(vertexBuffer);
		int primitiveCount = GetPrimitiveCount(primitiveType, vertexBuffer.VertexCount);
		mGraphicsDevice.DrawPrimitives(primitiveType, 0, primitiveCount);
#else
		GLVertexType[] vertices = new GLVertexType[vertexCount];
		CreateVertexArrayFromPointers(startIndex, vertexCount, vertices);

		// slow fallback for triangle fans.
		if (mode == GL.GL_TRIANGLE_FAN)
		{
			// We simulate them using a triangle list.
			primitiveType = PrimitiveType.TriangleList;
			vertexCount = (vertices.Length - 2) * 3;
			GLVertexType[] newVertices = new GLVertexType[vertexCount];
			int newIndex = 0;
			for (int i = 2; i < vertices.Length; i++)
			{
				newVertices[newIndex++] = vertices[0];
				newVertices[newIndex++] = vertices[i - 1];
				newVertices[newIndex++] = vertices[i];
			}
			vertices = newVertices;
		}
		int primitiveCount = GetPrimitiveCount(primitiveType, vertices.Length);
		if (primitiveCount <= 0)
			return;
		mGraphicsDevice.DrawUserPrimitives<GLVertexType>(primitiveType, vertices, 0, primitiveCount, decl);
#endif
	}

	public void glDrawElements(int mode, int count, int type, byte[] indecies, int indeciesOffset)
	{
		if (mCurrentVertexPointer.clientStateEnabled == false)
			return;

		if (count < 0)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (type != GL.GL_UNSIGNED_BYTE && type != GL.GL_UNSIGNED_SHORT)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		PrimitiveType primitiveType;
		if (!ConvertGLPrimitiveType(mode, out primitiveType))
		{
			RaiseGLError(GL_INVALID_VALUE);
			return;
		}

		SetupBasicEffectState();

		int primitiveCount = GetPrimitiveCount(primitiveType, count);
		mBasicEffect.CurrentTechnique.Passes[0].Apply();

		if (mCurrentBoundElementArrayBuffer != -1)
		{
			indecies = mBuffers[mCurrentBoundElementArrayBuffer].data;
		}

		int maxIndex = 0;
		short[] indexData = new short[count];
		CreateIndexArrayFromPointers(indecies, indeciesOffset, count, type, indexData, out maxIndex);

		if (mode == GL.GL_TRIANGLE_FAN)
		{
			// We simulate them using a triangle list.
			primitiveType = PrimitiveType.TriangleList;
			count = (count - 2) * 3;
			primitiveCount = GetPrimitiveCount(primitiveType, count);
			short[] newIndecies = new short[count];
			int newIndex = 0;
			for (int i = 2; i < indexData.Length; i++)
			{
				newIndecies[newIndex++] = indexData[0];
				newIndecies[newIndex++] = indexData[i - 1];
				newIndecies[newIndex++] = indexData[i];
			}
			indexData = newIndecies;
		}

		VertexDeclaration decl = GLVertexType.GetVertexDeclaration();

		GLVertexType[] vertices = null;
		vertices = new GLVertexType[maxIndex + 1];
		CreateVertexArrayFromPointers(0, maxIndex + 1, vertices);
		mGraphicsDevice.DrawUserIndexedPrimitives<GLVertexType>(primitiveType, vertices, 0, vertices.Length, indexData, 0, primitiveCount);
	}


	public void glViewport(int x, int y, int w, int h)
	{
		Viewport viewport = mGraphicsDevice.Viewport;
		viewport.X = x;
		viewport.Y = y;
		viewport.Width = w;
		viewport.Height = h;
		mGraphicsDevice.Viewport = viewport;
	}

	public void glDepthFunc(int tst)
	{
		//mDepthStencilState.StencilFunction = GetCompareFunction(tst);
		//		mGraphicsDevice.DepthStencilState = mDepthStencilState;
	}

	public void glHint(int type, int value)
	{
	}

	public void glFinish()
	{
	}

	public void glGenBuffers(int nbuffers, int[] handles)
	{
		if (nbuffers < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		for (int i = 0; i < nbuffers; i++)
		{
			handles[i] = mBuffers.Count + 1;
			mBuffers.Add(new GLBuffer { data = null });
		}
	}

	public void glBindBuffer(int target, int buffer)
	{
		if (buffer < 0 || buffer > mBuffers.Count)
		{
			return;
		}

		if (target == GL.GL_ELEMENT_ARRAY_BUFFER)
		{
			mCurrentBoundElementArrayBuffer = buffer - 1;
		}
		else if (target == GL.GL_ARRAY_BUFFER)
		{
			mCurrentBoundArrayBuffer = buffer - 1;
		}
		else
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}
	}

	public void glBufferData(int target, int size, byte[] data, int dataOffset, int usage)
	{
		int buf;
		if (target == GL.GL_ELEMENT_ARRAY_BUFFER)
		{
			buf = mCurrentBoundElementArrayBuffer;
		}
		else if (target == GL.GL_ARRAY_BUFFER)
		{
			buf = mCurrentBoundArrayBuffer;
		}
		else
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (buf == -1)
		{
			RaiseGLError(GL.GL_INVALID_OPERATION);
			return;
		}

		if (usage != GL.GL_STATIC_DRAW && usage != GL.GL_DYNAMIC_DRAW)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (size < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		try
		{
			byte[] dataCopy = new byte[size];
			if (data != null)
			{
				Buffer.BlockCopy(data, dataOffset, dataCopy, 0, size);
			}
			mBuffers[buf].data = dataCopy;
			mBuffers[buf].usage = usage;
		}
		catch (OutOfMemoryException)
		{
			RaiseGLError(GL.GL_OUT_OF_MEMORY);
			return;
		}
	}

	public void glBufferSubData(int target, int offset, int size, byte[] data, int dataOffset)
	{
		int buf;
		if (target == GL.GL_ELEMENT_ARRAY_BUFFER)
		{
			buf = mCurrentBoundElementArrayBuffer;
		}
		else if (target == GL.GL_ARRAY_BUFFER)
		{
			buf = mCurrentBoundArrayBuffer;
		}
		else
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (buf == -1)
		{
			RaiseGLError(GL.GL_INVALID_OPERATION);
			return;
		}

		if (offset < 0 || size < 0 || (offset + size) > mBuffers[buf].data.Length)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		Buffer.BlockCopy(data, dataOffset, mBuffers[buf].data, offset, size);
	}

	public void glGenTextures(int ntextures, int[] handles)
	{
		if (ntextures < 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		for (int i = 0; i < ntextures; i++)
		{
			handles[i] = mTextures.Count + 1;
			mTextures.Add(null);
		}
	}

	public void glBindTexture(int target, int texture)
	{
		if (target != GL.GL_TEXTURE_2D)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (texture < 0 || texture > mTextures.Count)
		{
			int cnt = mTextures.Count;
			for (int i = cnt; i < texture; i++)
			{
				mTextures.Add(null);
			}
		}

		mCurrentBoundTexture = texture - 1;
		if (mCurrentBoundTexture == -1)
			return;

		if (mTextures[mCurrentBoundTexture] != null)
		{
			mBasicEffect.Texture = (Texture2D)mTextures[mCurrentBoundTexture];
		}
	}

	bool GetSurfaceFormat(int glformat, int type, out SurfaceFormat surfaceFormat, out int bytesPerPixel)
	{
		surfaceFormat = SurfaceFormat.Color;
		bytesPerPixel = 4;
		switch (glformat)
		{
			case GL.GL_ALPHA:
				if (type == GL.GL_UNSIGNED_BYTE)
				{
					bytesPerPixel = 1;
					surfaceFormat = SurfaceFormat.Alpha8;
				}
				else
				{
					RaiseGLError(GL.GL_INVALID_VALUE);
					return false;
				}
				break;
			case GL.GL_RGB:
				if (type == GL.GL_UNSIGNED_SHORT_5_6_5)
				{
					bytesPerPixel = 2;
					surfaceFormat = SurfaceFormat.Bgr565;
				}
				else if (type == GL.GL_UNSIGNED_BYTE)
				{
					bytesPerPixel = 3;
					surfaceFormat = SurfaceFormat.Color;
				}
				else
				{
					RaiseGLError(GL.GL_INVALID_VALUE);
					return false;
				}
				break;
			case GL.GL_RGBA:
				if (type == GL.GL_UNSIGNED_SHORT_4_4_4_4)
				{
					bytesPerPixel = 2;
					surfaceFormat = SurfaceFormat.Bgra4444;
				}
				else if (type == GL.GL_UNSIGNED_SHORT_5_5_5_1)
				{
					bytesPerPixel = 2;
					surfaceFormat = SurfaceFormat.Bgra5551;
				}
				else if (type == GL.GL_UNSIGNED_BYTE)
					surfaceFormat = SurfaceFormat.Color;
				else
				{
					RaiseGLError(GL.GL_INVALID_VALUE);
					return false;
				}
				break;
			case GL.GL_LUMINANCE:
			case GL.GL_LUMINANCE_ALPHA:
				if (type == GL.GL_UNSIGNED_BYTE)
				{
					surfaceFormat = SurfaceFormat.Color;
					bytesPerPixel = 4;
				}
				else
				{
					RaiseGLError(GL.GL_INVALID_VALUE);
					return false;
				}
				break;
			default:
				RaiseGLError(GL.GL_INVALID_ENUM);
				return false;
		}
		return true;
	}

	bool SetConvertedPixels(SurfaceFormat surfaceFormat, int bytesPerPixel, int level, Texture2D texture, int format, int type, int xoffset, int yoffset, int width, int height, byte[] textureData, int textureDataOffset)
	{
		if (bytesPerPixel == 2)
		{
			byte[] pixels = new byte[width * height * 2];
			int pitch = width * 2;
			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < pitch; i += 2)
				{
					int rgb565 = (int)(textureData[textureDataOffset + 1] << 8) | (int)textureData[textureDataOffset + 0];
					int r = (rgb565 >> 11) & 31;
					int g = (rgb565 >> 5) & 64;
					int b = rgb565 & 31;
					int bgr565 = (b << 11) | (g << 5) | r;

					pixels[j * pitch + i + 1] = (byte)((bgr565 & 0xff00) >> 8);
					pixels[j * pitch + i + 0] = (byte)(bgr565 & 0xff);
					textureDataOffset += 2;
				}
			}
			textureDataOffset = 0;
			textureData = pixels;
		}
		else if (bytesPerPixel == 3)
		{
			byte[] pixels = new byte[width * height * 4];
			int pitch = width * 4;
			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < pitch; i += 4)
				{
					pixels[j * pitch + i + 3] = 255;
					pixels[j * pitch + i + 2] = textureData[textureDataOffset + 0];
					pixels[j * pitch + i + 1] = textureData[textureDataOffset + 1];
					pixels[j * pitch + i + 0] = textureData[textureDataOffset + 2];
					textureDataOffset += 3;
				}
			}
			textureDataOffset = 0;
			textureData = pixels;
		}
		else if (bytesPerPixel == 4)
		{
			byte[] pixels = new byte[width * height * 4];

			int pitch = width * 4;

			if (format == GL.GL_ALPHA)
			{
				for (int j = 0; j < height; j++)
				{
					for (int i = 0; i < pitch; i += 4)
					{
						pixels[j * pitch + i + 3] = textureData[textureDataOffset];
						pixels[j * pitch + i + 2] = 0;
						pixels[j * pitch + i + 1] = 0;
						pixels[j * pitch + i + 0] = 0;
						textureDataOffset++;
					}
				}
			}
			else if (format == GL.GL_LUMINANCE)
			{
				for (int j = 0; j < height; j++)
				{
					for (int i = 0; i < pitch; i += 4)
					{
						pixels[j * pitch + i + 3] = 1;
						pixels[j * pitch + i + 2] = textureData[textureDataOffset];
						pixels[j * pitch + i + 1] = textureData[textureDataOffset];
						pixels[j * pitch + i + 0] = textureData[textureDataOffset];
						textureDataOffset++;
					}
				}
			}
			else if (format == GL.GL_LUMINANCE_ALPHA)
			{
				for (int j = 0; j < height; j++)
				{
					for (int i = 0; i < pitch; i += 4)
					{
						pixels[j * pitch + i + 3] = textureData[textureDataOffset++];
						pixels[j * pitch + i + 2] = textureData[textureDataOffset];
						pixels[j * pitch + i + 1] = textureData[textureDataOffset];
						pixels[j * pitch + i + 0] = textureData[textureDataOffset];
						textureDataOffset++;
					}
				}
			}
			else
			{
				//for (int j = height - 1; j >= 0; j--)
				for (int j = 0; j < height; j++)
				{
					for (int i = 0; i < pitch; i += 4)
					{
						pixels[j * pitch + i + 3] = textureData[textureDataOffset + 3];
						pixels[j * pitch + i + 2] = textureData[textureDataOffset + 0];
						pixels[j * pitch + i + 1] = textureData[textureDataOffset + 1];
						pixels[j * pitch + i + 0] = textureData[textureDataOffset + 2];
						textureDataOffset += 4;
					}
				}

			}

			textureDataOffset = 0;
			textureData = pixels;
		}

		texture.SetData<byte>(level, new Rectangle(xoffset, yoffset, width, height), textureData, textureDataOffset, width * height * bytesPerPixel);

		return true;
	}

	public void glTexImage2D(int target, int level, int internalformat, int width, int height, int border, int format, int type, byte[] textureData, int textureDataOffset)
	{
		if (target != GL.GL_TEXTURE_2D)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		if (internalformat != format)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		if (mCurrentBoundTexture == -1)
		{
			RaiseGLError(GL.GL_INVALID_OPERATION);
			return;
		}



		// we don't support mipmapping just yet.
#if false
		if (level != 0)
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}
#endif


		Texture2D t = (Texture2D)mTextures[mCurrentBoundTexture];

		int bytesPerPixel;
		SurfaceFormat surfaceFormat;
		if (!GetSurfaceFormat(format, type, out surfaceFormat, out bytesPerPixel))
		{
			RaiseGLError(GL.GL_INVALID_VALUE);
			return;
		}

		if (t == null)
		{
			t = new Texture2D(mGraphicsDevice, width, height, true, surfaceFormat);
			mTextures[mCurrentBoundTexture] = t;
		}

		mGraphicsDevice.Textures[0] = null;

		if(textureData != null)
			SetConvertedPixels(surfaceFormat, bytesPerPixel, level, t,
				format, type, 0, 0, width, height, textureData, textureDataOffset);

		mBasicEffect.Texture = t;
	}

	public void glDeleteTextures(int[] textures)
	{
		for (int i = 0; i < textures.Length; i++)
		{
			mTextures[textures[i]].Dispose();
			mTextures[textures[i]] = null;
		}
	}

	public void glTexSubImage2D(int target, int level, int xoffset, int yoffset,
		int width, int height, int format, int type, byte[] textureData, int textureDataOffset) {
			if (target != GL.GL_TEXTURE_2D)
			{
				RaiseGLError(GL.GL_INVALID_ENUM);
				return;
			}

			if (mCurrentBoundTexture == -1)
			{
				RaiseGLError(GL.GL_INVALID_OPERATION);
				return;
			}

			Texture2D t = (Texture2D)mTextures[mCurrentBoundTexture];
	
			int bytesPerPixel;
			SurfaceFormat surfaceFormat;

			if (!GetSurfaceFormat(format, type, out surfaceFormat, out bytesPerPixel))
			{
				RaiseGLError(GL.GL_INVALID_VALUE);
				return;
			}

			mGraphicsDevice.Textures[0] = null;

			if (xoffset + width > t.Width ||
				yoffset + height > t.Height ||
				xoffset < 0 ||
				yoffset < 0)
			{
				RaiseGLError(GL.GL_INVALID_OPERATION);
				return;
			}

			try
			{
				SetConvertedPixels(surfaceFormat, bytesPerPixel, level, t,
					format, type, xoffset, yoffset, width, height, textureData, textureDataOffset);
			}
			catch (Exception e)
			{
				int a = 2;
			}
			mBasicEffect.Texture = t;
	}

	public void glTexParameterx(int target, int pname, int param)
	{
		if (target != GL.GL_TEXTURE_2D)
		{
			RaiseGLError(GL.GL_INVALID_ENUM);
			return;
		}

		// save samplerstate to texture struct and set it before rendering..
	}
};
