/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D:\\MoSyncSources\\MoSync\\runtimes\\java\\platforms\\androidJNI\\AndroidProject\\src\\com\\android\\vending\\billing\\IMarketBillingService.aidl
 */
package com.android.vending.billing;
public interface IMarketBillingService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.android.vending.billing.IMarketBillingService
{
private static final java.lang.String DESCRIPTOR = "com.android.vending.billing.IMarketBillingService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.android.vending.billing.IMarketBillingService interface,
 * generating a proxy if needed.
 */
public static com.android.vending.billing.IMarketBillingService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = (android.os.IInterface)obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.android.vending.billing.IMarketBillingService))) {
return ((com.android.vending.billing.IMarketBillingService)iin);
}
return new com.android.vending.billing.IMarketBillingService.Stub.Proxy(obj);
}
public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_sendBillingRequest:
{
data.enforceInterface(DESCRIPTOR);
android.os.Bundle _arg0;
if ((0!=data.readInt())) {
_arg0 = android.os.Bundle.CREATOR.createFromParcel(data);
}
else {
_arg0 = null;
}
android.os.Bundle _result = this.sendBillingRequest(_arg0);
reply.writeNoException();
if ((_result!=null)) {
reply.writeInt(1);
_result.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
}
else {
reply.writeInt(0);
}
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.android.vending.billing.IMarketBillingService
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
/** Given the arguments in bundle form, returns a bundle for results. */
public android.os.Bundle sendBillingRequest(android.os.Bundle bundle) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
android.os.Bundle _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
if ((bundle!=null)) {
_data.writeInt(1);
bundle.writeToParcel(_data, 0);
}
else {
_data.writeInt(0);
}
mRemote.transact(Stub.TRANSACTION_sendBillingRequest, _data, _reply, 0);
_reply.readException();
if ((0!=_reply.readInt())) {
_result = android.os.Bundle.CREATOR.createFromParcel(_reply);
}
else {
_result = null;
}
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
}
static final int TRANSACTION_sendBillingRequest = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
}
/** Given the arguments in bundle form, returns a bundle for results. */
public android.os.Bundle sendBillingRequest(android.os.Bundle bundle) throws android.os.RemoteException;
}
