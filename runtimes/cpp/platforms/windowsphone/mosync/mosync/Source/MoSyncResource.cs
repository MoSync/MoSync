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
	public class Resource {
        protected Object mInternalObject;
        protected int mResourceType;

        public Resource(Object internalObject, int resourceType)
        {
            mInternalObject = internalObject;
            mResourceType = resourceType;
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

