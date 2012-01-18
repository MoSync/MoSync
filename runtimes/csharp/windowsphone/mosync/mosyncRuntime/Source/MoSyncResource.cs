using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{

	// Read only, used for ubins.
	public class BoundedStream : Stream
	{
		private readonly Stream mStream;
		private readonly long mOffset;
		private readonly long mLength;
		private long mPosition;
		public BoundedStream(Stream stream, long offset, long length)
		{
			mStream = stream;
			mOffset = offset;
			mLength = length;
			mPosition = 0;
		}

		public override bool CanRead { get { return true; } }
		public override bool CanWrite { get { return false; } }
		public override bool CanSeek { get { return true; } }

		public override long Position
		{
			set
			{
				mPosition = value;
				if (value > mLength || value < 0)
					throw new IOException("Invalid position.");
				mStream.Seek(mOffset + mPosition, SeekOrigin.Begin);
			}

			get
			{
				return mPosition;
			}
		}

		public override long Length
		{
			get
			{
				return mLength;
			}
		}

		public override long Seek(long offset, SeekOrigin origin)
		{
			switch (origin)
			{
				case SeekOrigin.Begin:
					this.Position = offset;
					break;
				case SeekOrigin.Current:
					this.Position = mPosition + offset;
					break;
				case SeekOrigin.End:
					this.Position = mLength - offset;
					break;
			}

			return this.Position;
		}

		public override int Read(byte[] buffer, int offset, int count)
		{
			if (mPosition == mLength)
				return 0;
			if (mPosition + count > mLength)
				count = (int)mLength - (int)mPosition;
			mStream.Seek(mOffset + mPosition, SeekOrigin.Begin);
			int res = mStream.Read(buffer, offset, count);
			mPosition = mStream.Position - mOffset;
			return res;
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
			throw new NotSupportedException();
		}

		public override void Flush()
		{
			throw new NotSupportedException();
		}

		public override void SetLength(long value)
		{
			throw new NotSupportedException();
		}
	}

	// This is a class used to hold a mosync resoruce
	// Each resource has a specific type and a platform
	// specific internal object.
	public class Resource
	{
		protected Object mInternalObject;
		protected int mResourceType;
		protected bool mIsDynamicPlaceholder = false;
		public static readonly Resource Flux = new Resource(null, Constants.RT_FLUX);

		// used for unloaded resources
		BoundedStream mStream;

		public Resource(Object internalObject, int resourceType, bool isDynamicPlaceholder = false, BoundedStream stream = null)
		{
			mInternalObject = internalObject;
			mResourceType = resourceType;
			mIsDynamicPlaceholder = isDynamicPlaceholder;

			mStream = stream;
		}



		// null if label, sprite, ubin
		public BoundedStream GetFileStream()
		{
			return mStream;
		}

		public void SetFileStream(BoundedStream stream)
		{
			mStream = stream;
		}

		public bool IsDynamicPlaceholder()
		{
			return mIsDynamicPlaceholder;
		}

		public void SetIsDynamicPlaceholder(bool s)
		{
			mIsDynamicPlaceholder = s;
		}

		public void SetInternalObject(Object internalObject)
		{
			mInternalObject = internalObject;
		}

		public void SetResourceType(int type)
		{
			mResourceType = type;
		}

		public int GetResourceType()
		{
			return mResourceType;
		}

		public Object GetInternalObject()
		{
			return mInternalObject;
		}
	}
}
