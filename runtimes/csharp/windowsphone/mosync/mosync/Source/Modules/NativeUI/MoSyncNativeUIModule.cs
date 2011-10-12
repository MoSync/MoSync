using System;

namespace MoSync
{
    public class NativeUIModule : IIoctlModule
    {
        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            ioctls.maWidgetCreate = delegate(int _widgetType)
            {
                String widgetType = core.GetDataMemory().ReadStringAtAddress(_widgetType);

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetDestroy = delegate(int _widget)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetAddChild = delegate(int _parent, int _child)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetRemoveChild = delegate(int _child)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetInsertChild = delegate(int _parent, int _child, int index)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetStackScreenPush = delegate(int _stackScreen, int _newScreen)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetStackScreenPop = delegate(int _stackScreen)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetSetProperty = delegate(int _widget, int _property, int _value)
            {
                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                String value = core.GetDataMemory().ReadStringAtAddress(_value);

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetGetProperty = delegate(int _widget, int _property, int _value, int _bufSize)
            {
                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                String value = "";


                core.GetDataMemory().WriteStringAtAddress(_value, value, _bufSize);
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetScreenShow = delegate(int _screenHandle)
            {

                return MoSync.Constants.MAW_RES_OK;
            };

        }
    }
}
