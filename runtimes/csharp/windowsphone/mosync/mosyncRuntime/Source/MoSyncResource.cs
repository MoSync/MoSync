using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
    // This is a class used to hold a mosync resoruce
    // Each resource has a specific type and a platform
    // specific internal object.
    public class Resource
    {
        protected Object mInternalObject;
        protected int mResourceType;
		protected bool mIsDynamicPlaceholder = false;
        public static readonly Resource Flux = new Resource(null, Constants.RT_FLUX);

        public Resource(Object internalObject, int resourceType, bool isDynamicPlaceholder=false)
        {
            mInternalObject = internalObject;
            mResourceType = resourceType;
			mIsDynamicPlaceholder = isDynamicPlaceholder;
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
