using System.Collections.Generic;

namespace MoSync
{
    public abstract class IWidget
    {
        protected IWidget mParent = null;
        protected List<IWidget> mChildren = new List<IWidget>();

        public IWidget()
        {
        }

        public void AddChild(IWidget child)
        {
        }

        public void InsertChild(IWidget child, int index)
        {
        }

        public void RemoveChild(int index)
        {
        }

        public void RemoveChild(IWidget child)
        {
        }

        public void RemoveFromParent()
        {
        }

        public IWidget GetParent()
        {
            return mParent;
        }
    };

    public class NativeUI
    {
        public NativeUI()
        {
        }

        public IWidget CreateWidget(string widgetName)
        {
            return null;
        }

    }
}