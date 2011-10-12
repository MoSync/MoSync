using System.Collections.Generic;
using System;
using System.IO;
using System.Reflection;

namespace MoSync
{
    public interface IWidget
    {
        void AddChild(IWidget child);
        void InsertChild(IWidget child, int index);
        void RemoveChild(int index);
        void RemoveChild(IWidget child);
        void RemoveFromParent();
        void SetParent(IWidget parent);
        IWidget GetParent();
        bool HasParent();
        bool IsChild(IWidget child);

        void SetProperty(String property, String Value);
        String GetProperty(String property);
    }

    public interface IScreen
    {
        void Show();
    }

    public class InvalidPropertyNameException : System.Exception
    {
        public InvalidPropertyNameException() : base() { }
    }

    public class InvalidPropertyValueException : System.Exception
    {
        public InvalidPropertyValueException() : base() { }
    }

    public abstract class WidgetBase : IWidget
    {
        protected IWidget mParent = null;
        protected List<IWidget> mChildren = new List<IWidget>();

        public WidgetBase()
        {
        }

        public void AddChild(IWidget child)
        {
            child.SetParent(mParent);
            mChildren.Add(child);
        }

        public void InsertChild(IWidget child, int index)
        {
            mChildren.Insert(index, child);
        }

        public void RemoveChild(int index)
        {
            mChildren.RemoveAt(index);
        }

        public void RemoveChild(IWidget child)
        {
            mChildren.Remove(child);
        }

        public bool HasParent()
        {
            return mParent != null;
        }

        public bool IsChild(IWidget child)
        {
            return mChildren.Contains(child);
        }

        public void RemoveFromParent()
        {
            if (mParent != null)
            {
                mParent.RemoveChild(this);
            }
        }

        public void SetParent(IWidget parent)
        {
            mParent = parent;
        }

        public IWidget GetParent()
        {
            return mParent;
        }

        public void SetProperty(String property, String stringValue)
        {
            PropertyInfo pinfo = this.GetType().GetProperty(property);
            if (pinfo == null) throw new InvalidPropertyNameException();
            switch (pinfo.PropertyType.Name)
            {
                case "Int32":
                    pinfo.SetValue(this, Convert.ToInt32(stringValue), null);
                    break;
                case "String":
                    pinfo.SetValue(this, stringValue, null);
                    break;
            }
        }

        public String GetProperty(String property)
        {
            PropertyInfo pinfo = this.GetType().GetProperty(property);
            if (pinfo == null || pinfo.CanRead == false) throw new InvalidPropertyNameException();
            Object value = pinfo.GetValue(this, null);
            return value.ToString();
        }
    }

    public abstract class NativeUI
    {
        public NativeUI()
        {
        }

        public IWidget CreateWidget(string widgetName)
        {
            foreach (Type t in Assembly.GetCallingAssembly().GetTypes())
            {
                if (t.GetInterface("MoSync.IWidget", false) != null && t.Name == widgetName)
                {
                    return Activator.CreateInstance(t) as IWidget;
                }
            }

            return null;
        }
    }
}