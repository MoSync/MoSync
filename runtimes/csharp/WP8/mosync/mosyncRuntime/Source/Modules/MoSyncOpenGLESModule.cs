using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using System.Windows.Navigation;
using System;
using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Media.Imaging;

namespace MoSync
{
    public class OpenGLESModule : IIoctlModule
    {
		GL mGL = new GL();
		Runtime mRuntime;

		// run when xna is initalized.
		public void InitWithGraphicsDevice(GraphicsDevice gd)
		{
			mGL.Init(gd);

			// Send initialized event.
			MoSync.Memory eventData = new MoSync.Memory(8);
			eventData.WriteInt32(MoSync.Struct.MAWidgetEventData.eventType, MoSync.Constants.MAW_EVENT_GL_VIEW_READY);
			eventData.WriteInt32(MoSync.Struct.MAWidgetEventData.widgetHandle, 0);
			mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
		}
#if !LIB
		Syscalls.Delegate_maUpdateScreen mOldUpdateScreenImplementation = null;
#else
        MosyncLibrary.Delegate_maUpdateScreen mOldUpdateScreenImplementation = null;
#endif
		Action maUpdateScreenAction = null;
		public void SetOnUpdateScreenAction(Action a)
		{
			maUpdateScreenAction = a;
		}

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
			mRuntime = runtime;

			ioctls.maOpenGLInitFullscreen = delegate(int _glApi)
			{
				if (_glApi != MoSync.Constants.MA_GL_API_GL1)
					return MoSync.Constants.MA_GL_INIT_RES_ERROR;


				Syscalls syscalls = runtime.GetSyscalls();
				mOldUpdateScreenImplementation = syscalls.maUpdateScreen;

				syscalls.maUpdateScreen = delegate()
				{
					if (maUpdateScreenAction != null)
						maUpdateScreenAction();
				};

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					// GamePage must always exist for fullscreen apps to work.
					if (((PhoneApplicationFrame)Application.Current.RootVisual).Navigate(new Uri("/GamePage.xaml", UriKind.Relative)))
					{
					}
				});

				return MoSync.Constants.MA_GL_INIT_RES_OK;
			};

			ioctls.maOpenGLCloseFullscreen = delegate()
			{

				return MoSync.Constants.MA_GL_INIT_RES_OK;
			};

			ioctls.maOpenGLTexImage2D = delegate(int _res)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, _res);
				WriteableBitmap src = (WriteableBitmap)res.GetInternalObject();
				byte[] pixels = src.ToByteArray();
				mGL.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, src.PixelWidth, src.PixelHeight, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, pixels, 0);
				return MoSync.Constants.MA_GL_TEX_IMAGE_2D_OK;
			};

			ioctls.maOpenGLTexSubImage2D = delegate(int _res)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, _res);
				WriteableBitmap src = (WriteableBitmap)res.GetInternalObject();
				byte[] pixels = src.ToByteArray();
				mGL.glTexSubImage2D(GL.GL_TEXTURE_2D, 0, 0, 0, src.PixelWidth, src.PixelHeight, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, pixels, 0);
				return MoSync.Constants.MA_GL_TEX_IMAGE_2D_OK;
			};

			ioctls.glViewport = delegate(int _x, int _y, int _w, int _h)
			{
				mGL.glViewport(_x, _y, _w, _h);
				return 0;
			};

			ioctls.glGetError = delegate()
			{
				int err = mGL.glGetError();
				if (err != GL.GL_NO_ERROR)
				{
					//int a = 2;
					//err = GL.GL_NO_ERROR;
				}
				return err;
			};

			ioctls.glGetStringHandle = delegate(int _name)
			{

				String str = mGL.glGetString(_name);
				char[] data = str.ToCharArray();
				byte[] bytes = new byte[data.Length + 1];
				bytes[data.Length] = 0;
				for (int i = 0; i < data.Length; i++)
				{
					bytes[i] = (byte)data[i];
				}

				return runtime.AddResource(new Resource(new System.IO.MemoryStream(bytes), MoSync.Constants.RT_BINARY, true));
			};

			ioctls.glMatrixMode = delegate(int mode)
			{
				mGL.glMatrixMode(mode);  return 0;
			};

			ioctls.glPushMatrix = delegate()
			{
				mGL.glPushMatrix(); return 0;
			};

			ioctls.glPopMatrix = delegate()
			{
				mGL.glPopMatrix(); return 0;
			};

			ioctls.glLoadIdentity = delegate()
			{
				mGL.glLoadIdentity(); return 0;
			};

			ioctls.glBlendFunc = delegate(int sfactor, int dfactor)
			{
				mGL.glBlendFunc(sfactor, dfactor); return 0;
			};

			ioctls.glAlphaFunc = delegate(int func, float _ref)
			{
				return 0;
			};

			ioctls.glAlphaFuncx = delegate(int func, int _ref)
			{
				return 0;
			};

			ioctls.glDepthFunc = delegate(int _func)
			{
				mGL.glDepthFunc(_func); return 0;
			};

			ioctls.glDepthMask = delegate(int _flag)
			{
				mGL.glDepthMask(_flag); return 0;
			};

			ioctls.glClearColor = delegate(float r, float g, float b, float a)
			{
				mGL.glClearColor(r, g, b, a); return 0;
			};

			ioctls.glClearColorx = delegate(int r, int g, int b, int a)
			{
				mGL.glClearColorx(r, g, b, a); return 0;
			};

			ioctls.glColor4f = delegate(float r, float g, float b, float a)
			{
				mGL.glColor4f(r, g, b, a); return 0;
			};

			ioctls.glColor4x = delegate(int r, int g, int b, int a)
			{
				mGL.glColor4x(r, g, b, a); return 0;
			};

			ioctls.glColor4ub = delegate(int r, int g, int b, int a)
			{
				mGL.glColor4ub(r, g, b, a); return 0;
			};

			ioctls.glColor4f = delegate(float r, float g, float b, float a)
			{
				mGL.glColor4f(r, g, b, a); return 0;
			};

			ioctls.glClear = delegate(int _mask)
			{
				mGL.glClear(_mask); return 0;
			};

			ioctls.glRotatef = delegate(float angle, float x, float y, float z)
			{
				mGL.glRotatef(angle, x, y, z); return 0;
			};

			ioctls.glRotatex = delegate(int angle, int x, int y, int z)
			{
				mGL.glRotatex(angle, x, y, z); return 0;
			};

			ioctls.glTranslatef = delegate(float x, float y, float z)
			{
				mGL.glTranslatef(x, y, z); return 0;
			};

			ioctls.glTranslatex = delegate(int x, int y, int z)
			{
				mGL.glTranslatex(x, y, z); return 0;
			};

			ioctls.glScalef = delegate(float x, float y, float z)
			{
				mGL.glScalef(x, y, z); return 0;
			};

			ioctls.glScalex = delegate(int x, int y, int z)
			{
				mGL.glScalex(x, y, z); return 0;
			};

			ioctls.glMultMatrixf = delegate(int _matrix)
			{
#if !LIB
				mGL.glMultMatrixf(core.GetDataMemory().GetData(), _matrix);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glMultMatrixx = delegate(int _matrix)
			{
#if !LIB
				mGL.glMultMatrixx(core.GetDataMemory().GetData(), _matrix);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glEnableClientState = delegate(int _array)
			{
				mGL.glEnableClientState(_array); return 0;
			};

			ioctls.glDisableClientState = delegate(int _array)
			{
				mGL.glDisableClientState(_array); return 0;
			};

			ioctls.glTexCoordPointer = delegate(int _size, int _type, int _stride, int _pointer)
			{
#if !LIB
				mGL.glTexCoordPointer(_size, _type, _stride, core.GetDataMemory().GetData(), _pointer);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glVertexPointer = delegate(int _size, int _type, int _stride, int _pointer)
			{
#if !LIB
				mGL.glVertexPointer(_size, _type, _stride, core.GetDataMemory().GetData(), _pointer);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glColorPointer = delegate(int _size, int _type, int _stride, int _pointer)
			{
#if !LIB
				mGL.glColorPointer(_size, _type, _stride, core.GetDataMemory().GetData(), _pointer);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glNormalPointer = delegate(int _type, int _stride, int _pointer)
			{
#if !LIB
				mGL.glNormalPointer(_type, _stride, core.GetDataMemory().GetData(), _pointer);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glDrawArrays = delegate(int _mode, int _first, int _count)
			{
				mGL.glDrawArrays(_mode, _first, _count);
				return 0;
			};

			ioctls.glDrawElements = delegate(int _mode, int _count, int _type, int _indecies)
			{
#if !LIB
				mGL.glDrawElements(_mode, _count, _type, core.GetDataMemory().GetData(), _indecies);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glEnable = delegate(int _e)
			{
				mGL.glEnable(_e);
				return 0;
			};

			ioctls.glDisable = delegate(int _e)
			{
				mGL.glDisable(_e);
				return 0;
			};

			ioctls.glFrustumf = delegate(float _left, float _right, float _bottom, float _top, float _znear, float _zfar)
			{
				mGL.glFrustumf(_left, _right, _bottom, _top, _znear, _zfar);
				return 0;
			};

			ioctls.glOrthof = delegate(float _left, float _right, float _bottom, float _top, float _znear, float _zfar)
			{
				mGL.glOrthof(_left, _right, _bottom, _top, _znear, _zfar);
				return 0;
			};

			ioctls.glFrustumx = delegate(int _left, int _right, int _bottom, int _top, int _znear, int _zfar)
			{
				mGL.glFrustumx(_left, _right, _bottom, _top, _znear, _zfar);
				return 0;
			};

			ioctls.glOrthox = delegate(int _left, int _right, int _bottom, int _top, int _znear, int _zfar)
			{
				mGL.glOrthox(_left, _right, _bottom, _top, _znear, _zfar);
				return 0;
			};

			ioctls.glFlush = delegate()
			{
				return 0;
			};

			ioctls.glFinish = delegate()
			{
				mGL.glFinish();
				return 0;
			};

			ioctls.glGenTextures = delegate(int _n, int _textures)
			{
				int[] handles = new int[_n];
				mGL.glGenTextures(_n, handles);
				for (int i = 0; i < _n; i++)
				{
					core.GetDataMemory().WriteInt32(_textures + i * 4, handles[i]);
				}
				return 0;
			};

			ioctls.glBindTexture = delegate(int _target, int _texture)
			{
				mGL.glBindTexture(_target, _texture);
				return 0;
			};

			ioctls.glTexImage2D = delegate(int _target, int _level, int _internalformat, int _width, int _height, int _border, int _format, int _type, int _pixels)
			{
#if !LIB
				if(_pixels == 0)
					mGL.glTexImage2D(_target, _level, _internalformat, _width, _height, _border, _format, _type, null, _pixels);
				else
					mGL.glTexImage2D(_target, _level, _internalformat, _width, _height, _border, _format, _type, core.GetDataMemory().GetData(), _pixels);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glDeleteTextures = delegate(int _n, int _textures)
			{
				int[] textures = new int[_n];
				for (int i = 0; i < _n; i++)
				{
					textures[i] = core.GetDataMemory().ReadInt32(_textures + i * 4);
				}
				mGL.glDeleteTextures(textures);
				return 0;
			};

			ioctls.glTexSubImage2D = delegate(int _target, int _level, int _xofs, int _yofs, int _width, int _height, int _format, int _type, int _pixels)
			{
#if !LIB
				mGL.glTexSubImage2D(_target, _level, _xofs, _yofs, _width, _height, _format, _type, core.GetDataMemory().GetData(), _pixels);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glCompressedTexImage2D = delegate(int _target, int _level, int _internalformat, int _width, int _height, int _border, int _imageSize, int _data)
			{
				return 0;
			};

			ioctls.glCompressedTexSubImage2D = delegate(int _target, int _level, int _xofs, int _yofs, int _width, int _height, int _format, int _imageSize, int _data)
			{
				return 0;
			};

			ioctls.glGenBuffers = delegate(int _n, int _buffers)
			{
				int[] handles = new int[_n];
				mGL.glGenBuffers(_n, handles);
				for (int i = 0; i < _n; i++)
				{
					core.GetDataMemory().WriteInt32(_buffers + i * 4, handles[i]);
				}
				return 0;
			};

			ioctls.glBindBuffer = delegate(int _target, int _buffer)
			{
				mGL.glBindBuffer(_target, _buffer);
				return 0;
			};

			ioctls.glBufferData = delegate(int _target, int _size, int _data, int _usage)
			{
#if !LIB
				mGL.glBufferData(_target, _size, core.GetDataMemory().GetData(), _data, _usage);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glBufferSubData = delegate(int _target, int _offset, int _size, int _data)
			{
#if !LIB
				mGL.glBufferSubData(_target, _offset, _size, core.GetDataMemory().GetData(), _data);
				return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;
#endif
			};

			ioctls.glMaterialf = delegate(int face, int pname, float value)
			{
				mGL.glMaterialf(face, pname, value); return 0;
			};

			ioctls.glMaterialx = delegate(int face, int pname, int value)
			{
				mGL.glMaterialx(face, pname, value); return 0;
			};

			ioctls.glMaterialfv = delegate(int face, int pname, int data)
			{
#if !LIB
				mGL.glMaterialfv(face, pname, core.GetDataMemory().GetData(), data); return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glMaterialxv = delegate(int face, int pname, int data)
			{
#if !LIB
				mGL.glMaterialxv(face, pname, core.GetDataMemory().GetData(), data); return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glLightf = delegate(int _light, int _pname, float _param)
			{
				mGL.glLightf(_light, _pname, _param); return 0;
			};

			ioctls.glLightx = delegate(int _light, int _pname, int _param)
			{
				mGL.glLightx(_light, _pname, _param); return 0;
			};

			ioctls.glLightfv = delegate(int _light, int _pname, int _pointer)
			{
#if !LIB
				mGL.glLightfv(_light, _pname, core.GetDataMemory().GetData(), _pointer); return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glLightxv = delegate(int _light, int _pname, int _pointer)
			{
#if !LIB
				mGL.glLightxv(_light, _pname, core.GetDataMemory().GetData(), _pointer); return 0;
#else
				return MoSync.Constants.IOCTL_UNAVAILABLE;;
#endif
			};

			ioctls.glShadeModel = delegate(int _mode)
			{
				mGL.glShadeModel(_mode); return 0;
			};
        }
    }
}
