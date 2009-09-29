//
//  Copyright 2002 - 2004, Microsoft Corporation
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#define BLUETOOTH_MAX_NAME_SIZE             (248)
#define BLUETOOTH_MAX_PASSKEY_SIZE          (16)
#define BLUETOOTH_MAX_PASSKEY_BUFFER_SIZE   (BLUETOOTH_MAX_PASSKEY_SIZE + 1)

#ifdef __cplusplus
extern "C" {
#endif

// ***************************************************************************
//
//  Bluetooth Address
//
// ***************************************************************************

typedef ULONGLONG BTH_ADDR;

typedef struct _BLUETOOTH_ADDRESS {
    union {
        BTH_ADDR ullLong;       //  easier to compare again BLUETOOTH_NULL_ADDRESS
        BYTE    rgBytes[ 6 ];   //  easier to format when broken out
    };

} BLUETOOTH_ADDRESS_STRUCT;

#define BLUETOOTH_ADDRESS BLUETOOTH_ADDRESS_STRUCT

#define BLUETOOTH_NULL_ADDRESS ( (ULONGLONG) 0x0 )

// ***************************************************************************
//
//  Radio Enumeration
//
//  Description:
//      This group of APIs enumerates the installed Bluetooth radios.
//
//  Sample Usage:
//      HANDLE hRadio;
//      BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(btfrp) };
//
//      HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio( &btfrp, &hRadio );
//      if ( NULL != hFind )
//      {
//          do
//          {
//              //
//              //  TODO: Do something with the radio handle.
//              //
//
//              CloseHandle( hRadio );
//
//          } while( BluetoothFindNextRadio( hFind, &hRadio ) );
//
//          BluetoothFindRadioClose( hFind );
//      }
//
// ***************************************************************************

typedef struct _BLUETOOTH_FIND_RADIO_PARAMS {
    DWORD   dwSize;             //  IN  sizeof this structure

} BLUETOOTH_FIND_RADIO_PARAMS;

typedef HANDLE      HBLUETOOTH_RADIO_FIND;

//
//  Description:
//      Begins the enumeration of local Bluetooth radios.
//
//  Parameters:
//      pbtfrp
//          A pointer to a BLUETOOTH_FIND_RADIO_PARAMS structure. The dwSize 
//          member of this structure must match the sizeof the of the structure.
//
//      phRadio
//          A pointer where the first radio HANDLE enumerated will be returned.
//
//  Return Values:
//      NULL
//          Error opening radios or no devices found. Use GetLastError() for
//          more info.
//
//          ERROR_INVALID_PARAMETER
//              pbtfrp parameter is NULL.
//
//          ERROR_REVISION_MISMATCH
//              The pbtfrp structure is not the right length.
//
//          ERROR_OUTOFMEMORY
//              Out of memory.
//
//          other Win32 errors.
//
//      any other
//          Success. The return handle is valid and phRadio points to a valid handle.
//
HBLUETOOTH_RADIO_FIND
WINAPI
BluetoothFindFirstRadio(
    BLUETOOTH_FIND_RADIO_PARAMS * pbtfrp,
    HANDLE *                      phRadio
    );

//
//  Description:
//      Finds the next installed Bluetooth radio.
//
//  Parameters:
//      hFind
//          The handle returned by BluetoothFindFirstRadio().
//
//      phRadio
//          A pointer where the next radio HANDLE enumerated will be returned.
//
//  Return Values:
//      TRUE
//          Next device succesfully found. pHandleOut points to valid handle.
//
//      FALSE
//          No device found. pHandleOut points to an invalid handle. Call
//          GetLastError() for more details.
//
//          ERROR_INVALID_HANDLE
//              The handle is NULL.
//
//          ERROR_NO_MORE_ITEMS
//              No more radios found.
//
//          ERROR_OUTOFMEMORY
//              Out of memory.
//
//          other Win32 errors
//
BOOL
WINAPI
BluetoothFindNextRadio(
    HBLUETOOTH_RADIO_FIND hFind,
    HANDLE * phRadio
    );

//
//  Description:
//      Closes the enumeration handle.
//
//  Parameters
//      hFind
//          The handle returned by BluetoothFindFirstRadio().
//
//  Return Values:
//      TRUE
//          Handle succesfully closed.
//
//      FALSE
//          Failure. Check GetLastError() for details.
//
//          ERROR_INVALID_HANDLE
//              The handle is NULL.
//
BOOL
WINAPI
BluetoothFindRadioClose(
    HBLUETOOTH_RADIO_FIND hFind
    );

// ***************************************************************************
//
//  Radio Information
//
// ***************************************************************************

typedef struct _BLUETOOTH_RADIO_INFO {
    DWORD dwSize;                               // Size, in bytes, of this entire data structure

    BLUETOOTH_ADDRESS address;                  // Address of the local radio

    WCHAR szName[ BLUETOOTH_MAX_NAME_SIZE ];    // Name of the local radio

    ULONG ulClassofDevice;                      // Class of device for the local radio

    USHORT lmpSubversion;                       // lmpSubversion, manufacturer specifc.
    USHORT manufacturer;                        // Manufacturer of the radio, BTH_MFG_Xxx value.  For the most up to date
                                                // list, goto the Bluetooth specification website and get the Bluetooth
                                                // assigned numbers document.
} BLUETOOTH_RADIO_INFO, *PBLUETOOTH_RADIO_INFO;

//
//  Description:
//      Retrieves the information about the radio represented by the handle.
//
//  Parameters:
//      hRadio
//          Handle to a local radio retrieved through BluetoothFindFirstRadio()
//          et al or SetupDiEnumerateDeviceInterfaces()
//
//      pRadioInfo
//          Radio information to be filled in. The dwSize member must match the 
//          size of the structure.
//
//  Return Values:
//      ERROR_SUCCESS
//          The information was retrieved successfully.
//
//      ERROR_INVALID_PARAMETER
//          pRadioInfo or hRadio is NULL.
//
//      ERROR_REVISION_MISMATCH
//          pRadioInfo->dwSize is invalid.
//
//      other Win32 error codes.
//
DWORD
WINAPI
BluetoothGetRadioInfo(
    HANDLE hRadio,
    PBLUETOOTH_RADIO_INFO pRadioInfo
    );

// ***************************************************************************
//
//  Device Information Stuctures
//
// ***************************************************************************

typedef struct _BLUETOOTH_DEVICE_INFO {
    DWORD   dwSize;                             //  size, in bytes, of this structure - must be the sizeof(BLUETOOTH_DEVICE_INFO)

    BLUETOOTH_ADDRESS Address;                  //  Bluetooth address

    ULONG   ulClassofDevice;                    //  Bluetooth "Class of Device"

    BOOL    fConnected;                         //  Device connected/in use
    BOOL    fRemembered;                        //  Device remembered
    BOOL    fAuthenticated;                     //  Device authenticated/paired/bonded

    SYSTEMTIME  stLastSeen;                     //  Last time the device was seen
    SYSTEMTIME  stLastUsed;                     //  Last time the device was used for other than RNR, inquiry, or SDP

    WCHAR   szName[ BLUETOOTH_MAX_NAME_SIZE ];  //  Name of the device

} BLUETOOTH_DEVICE_INFO_STRUCT;

#define BLUETOOTH_DEVICE_INFO BLUETOOTH_DEVICE_INFO_STRUCT

typedef BLUETOOTH_DEVICE_INFO * PBLUETOOTH_DEVICE_INFO;

// ***************************************************************************
//
//  Device Enumeration
//
//  Description:
//      Enumerates the Bluetooth devices. The types of returned device depends
//      on the flags set in the BLUETOOTH_DEVICE_SEARCH_PARAMS (see structure
//      definition for details).
//
//  Sample Usage:
//      HBLUETOOTH_DEVICE_FIND hFind;
//      BLUETOOTH_DEVICE_SEARCH_PARAMS btsp = { sizeof(btsp) };
//      BLUETOOTH_DEVICE_INFO btdi = { sizeof(btdi) };
//
//      btsp.fReturnAuthenticated = TRUE;
//      btsp.fReturnRemembered    = TRUE;
//
//      hFind = BluetoothFindFirstDevice( &btsp, &btdi );
//      if ( NULL != hFind )
//      {
//          do
//          {
//              //
//              //  TODO:   Do something useful with the device info.
//              //
//
//          } while( BluetoothFindNextDevice( hFind, &btdi ) );
//
//          BluetoothFindDeviceClose( hFind );
//      }
//
// ***************************************************************************

typedef struct _BLUETOOTH_DEVICE_SEARCH_PARAMS {
    DWORD   dwSize;                 //  IN  sizeof this structure

    BOOL    fReturnAuthenticated;   //  IN  return authenticated devices
    BOOL    fReturnRemembered;      //  IN  return remembered devices
    BOOL    fReturnUnknown;         //  IN  return unknown devices
    BOOL    fReturnConnected;       //  IN  return connected devices

    BOOL    fIssueInquiry;          //  IN  issue a new inquiry
    UCHAR   cTimeoutMultiplier;     //  IN  timeout for the inquiry

    HANDLE  hRadio;                 //  IN  handle to radio to enumerate - NULL == all radios will be searched

} BLUETOOTH_DEVICE_SEARCH_PARAMS;

typedef HANDLE      HBLUETOOTH_DEVICE_FIND;

//
//  Description:
//      Begins the enumeration of Bluetooth devices.
//
//  Parameters:
//      pbtsp
//          A pointer to a BLUETOOTH_DEVICE_SEARCH_PARAMS structure. This
//          structure contains the flags and inputs used to conduct the search.
//          See BLUETOOTH_DEVICE_SEARCH_PARAMS for details.
//
//      pbtdi
//          A pointer to a BLUETOOTH_DEVICE_INFO structure to return information
//          about the first Bluetooth device found. Note that the dwSize member
//          of the structure must be the sizeof(BLUETOOTH_DEVICE_INFO) before
//          calling because the APIs hast to know the size of the buffer being
//          past in. The dwSize member must also match the exact 
//          sizeof(BLUETOOTH_DEVICE_INFO) or the call will fail.
//
//  Return Values:
//      NULL
//          Error opening radios or not devices found. Use GetLastError for more info.
//
//          ERROR_INVALID_PARAMETER
//              pbtsp parameter or pbtdi parameter is NULL.
//
//          ERROR_REVISION_MISMATCH
//              The pbtfrp structure is not the right length.
//
//          other Win32 errors
//
//      any other value
//          Success. The return handle is valid and pbtdi points to valid data.
//
HBLUETOOTH_DEVICE_FIND
WINAPI
BluetoothFindFirstDevice(
    BLUETOOTH_DEVICE_SEARCH_PARAMS * pbtsp,
    BLUETOOTH_DEVICE_INFO *   pbtdi
    );

//
//  Description:
//      Finds the next Bluetooth device in the enumeration.
//
//  Parameters:
//      hFind
//          The handle returned from BluetoothFindFirstDevice().
//
//      pbtdi
//          A pointer to a BLUETOOTH_DEVICE_INFO structure to return information
//          about the first Bluetooth device found. Note that the dwSize member
//          of the structure must be the sizeof(BLUETOOTH_DEVICE_INFO) before
//          calling because the APIs hast to know the size of the buffer being
//          past in. The dwSize member must also match the exact 
//          sizeof(BLUETOOTH_DEVICE_INFO) or the call will fail.
//
//  Return Values:
//      TRUE
//          Next device succesfully found. pHandleOut points to valid handle.
//
//      FALSE
//          No device found. pHandleOut points to an invalid handle. Call
//          GetLastError() for more details.
//
//          ERROR_INVALID_HANDLE
//              The handle is NULL.
//
//          ERROR_NO_MORE_ITEMS
//              No more radios found.
//
//          ERROR_OUTOFMEMORY
//              Out of memory.
//
//          other Win32 errors
//
BOOL
WINAPI
BluetoothFindNextDevice(
    HBLUETOOTH_DEVICE_FIND  hFind,
    BLUETOOTH_DEVICE_INFO * pbtdi
    );

//
//  Description:
//      Closes the enumeration handle.
//
//  Parameters:
//      hFind
//          The handle returned from BluetoothFindFirstDevice().
//
//  Return Values:
//      TRUE
//          Handle succesfully closed.
//
//      FALSE
//          Failure. Check GetLastError() for details.
//
//          ERROR_INVALID_HANDLE
//              The handle is NULL.
//
BOOL
WINAPI
BluetoothFindDeviceClose(
    HBLUETOOTH_DEVICE_FIND hFind
    );

//
//  Description:
//      Retrieves information about a remote device.
//
//      Fill in the dwSize and the Address members of the pbtdi structure
//      being passed in. On success, the rest of the members will be filled
//      out with the information that the system knows.
//
//  Parameters:
//      hRadio
//          Handle to a local radio retrieved through BluetoothFindFirstRadio()
//          et al or SetupDiEnumerateDeviceInterfaces()
//
//      pbtdi
//          A pointer to a BLUETOOTH_DEVICE_INFO structure to return information
//          about the first Bluetooth device found. The dwSize member of the
//          structure must be the sizeof the structure in bytes. The Address
//          member must be filled out with the Bluetooth address of the remote
//          device.
//
//  Return Values:
//      ERROR_SUCCESS
//          Success. Information returned.
//
//      ERROR_REVISION_MISMATCH
//          The size of the BLUETOOTH_DEVICE_INFO isn't compatible. Check
//          the dwSize member of the BLUETOOTH_DEVICE_INFO structure you
//          passed in.
//
//      ERROR_NOT_FOUND
//          The radio is not known by the system or the Address field of
//          the BLUETOOTH_DEVICE_INFO structure is all zeros.
//
//      ERROR_INVALID_PARAMETER
//          pbtdi is NULL.
//
//      other error codes
//
DWORD
WINAPI
BluetoothGetDeviceInfo(
    HANDLE  hRadio,
    BLUETOOTH_DEVICE_INFO * pbtdi
    );

//
//  Description:
//      Updates the computer local cache about the device. 
//
//  Parameters:
//      pbtdi
//          A pointer to the BLUETOOTH_DEVICE_INFO structure to be updated.
//          The following members must be valid:
//              dwSize
//                  Must match the size of the structure.
//              Address
//                  Must be a previously found radio address.
//              szName
//                  New name to be stored.
//
//  Return Values:
//      ERROR_SUCCESS
//          The device information was updated successfully.
//
//      ERROR_INVALID_PARAMETER
//          pbtdi is NULL.
//
//      ERROR_REVISION_MISMATCH
//          pbtdi->dwSize is invalid.
//
//      other Win32 error codes.
//
DWORD
WINAPI
BluetoothUpdateDeviceRecord(
    BLUETOOTH_DEVICE_INFO * pbtdi
    );

//
//  Description:
//      Delete the authentication (aka "bond") between the computer and the
//      device. Also purges any cached information about the device.
//
//  Return Values:
//      ERROR_SUCCESS
//          The device was removed successfully.
//
//      ERROR_NOT_FOUND
//          The device was not found. If no Bluetooth radio is installed,
//          the devices could not be enumerated or removed.
//
DWORD
WINAPI
BluetoothRemoveDevice(
    BLUETOOTH_ADDRESS * pAddress
    );

// ***************************************************************************
//
//  Device Picker Dialog
//
//  Description:
//      Invokes a common dialog for selecting Bluetooth devices. The list
//      of devices displayed to the user is determined by the flags and
//      settings the caller specifies in the BLUETOOTH_SELECT_DEVICE_PARAMS
//      (see structure definition for more details).
//
//      If BluetoothSelectDevices() returns TRUE, the caller must call
//      BluetoothSelectDevicesFree() or memory will be leaked within the
//      process.
//
//  Sample Usage:
//
//      BLUETOOTH_SELECT_DEVICE_PARAMS btsdp = { sizeof(btsdp) };
//
//      btsdp.hwndParent = hDlg;
//      btsdp.fShowUnknown = TRUE;
//      btsdp.fAddNewDeviceWizard = TRUE;
//
//      BOOL b = BluetoothSelectDevices( &btsdp );
//      if ( b )
//      {
//          BLUETOOTH_DEVICE_INFO * pbtdi = btsdp.pDevices;
//          for ( ULONG cDevice = 0; cDevice < btsdp.cNumDevices; cDevice ++ )
//          {
//              if ( pbtdi->fAuthenticated || pbtdi->fRemembered )
//              {
//                  //
//                  //  TODO:   Do something usefull with the device info
//                  //
//              }
//
//              pbtdi = (BLUETOOTH_DEVICE_INFO *) ((LPBYTE)pbtdi + pbtdi->dwSize);
//          }
//
//          BluetoothSelectDevicesFree( &btsdp );
//      }
//
// ***************************************************************************


typedef struct _BLUETOOTH_COD_PAIRS {
    ULONG   ulCODMask;                          //  ClassOfDevice mask to compare
    LPCWSTR pcszDescription;                    //  Descriptive string of mask

} BLUETOOTH_COD_PAIRS;

typedef BOOL (WINAPI *PFN_DEVICE_CALLBACK)(LPVOID pvParam, PBLUETOOTH_DEVICE_INFO pDevice);

typedef struct _BLUETOOTH_SELECT_DEVICE_PARAMS {
    DWORD   dwSize;                             //  IN  sizeof this structure

    ULONG   cNumOfClasses;                      //  IN  Number in prgClassOfDevice - if ZERO search for all devices
    BLUETOOTH_COD_PAIRS * prgClassOfDevices;    //  IN  Array of CODs to find.

    LPWSTR  pszInfo;                            //  IN  If not NULL, sets the "information" text

    HWND    hwndParent;                         //  IN  parent window - NULL == no parent

    BOOL    fForceAuthentication;               //  IN  If TRUE, authenication will be forced before returning
    BOOL    fShowAuthenticated;                 //  IN  If TRUE, authenticated devices will be shown in the picker
    BOOL    fShowRemembered;                    //  IN  If TRUE, remembered devices will be shown in the picker
    BOOL    fShowUnknown;                       //  IN  If TRUE, unknown devices that are not authenticated or "remember" will be shown.

    BOOL    fAddNewDeviceWizard;                //  IN  If TRUE, invokes the add new device wizard.
    BOOL    fSkipServicesPage;                  //  IN  If TRUE, skips the "Services" page in the wizard.

    PFN_DEVICE_CALLBACK pfnDeviceCallback;      //  IN  If non-NULL, a callback that will be called for each device. If the
                                                //      the callback returns TRUE, the item will be added. If the callback is
                                                //      is FALSE, the item will not be shown.
    LPVOID  pvParam;                            //  IN  Parameter to be passed to pfnDeviceCallback as the pvParam.

    DWORD   cNumDevices;                        //  IN  number calles wants - ZERO == no limit.
                                                //  OUT the number of devices returned.

    PBLUETOOTH_DEVICE_INFO  pDevices;           //  OUT pointer to an array for BLUETOOTH_DEVICE_INFOs.
                                                //      call BluetoothSelectDevicesFree() to free

} BLUETOOTH_SELECT_DEVICE_PARAMS;

//
//  Description:
//      (See header above)
//
//  Return Values:
//      TRUE
//          User selected a device. pbtsdp->pDevices points to valid data.
//          Caller should check the fAuthenticated && fRemembered flags to
//          determine which devices we successfuly authenticated or valid
//          selections by the user.
//
//          Use BluetoothSelectDevicesFree() to free the nessecary data
//          such as pDevices only if this function returns TRUE.
//
//      FALSE
//          No valid data returned. Call GetLastError() for possible details
//          of the failure. If GLE() is:
//
//          ERROR_CANCELLED
//              The user cancelled  the request.
//
//          ERROR_INVALID_PARAMETER
//              The pbtsdp is NULL.
//
//          ERROR_REVISION_MISMATCH
//              The structure passed in as pbtsdp is of an unknown size.
//
//          other WIN32 errors
//
BOOL
WINAPI
BluetoothSelectDevices(
    BLUETOOTH_SELECT_DEVICE_PARAMS * pbtsdp
    );

//
//  Description:
//      This function should only be called if BluetoothSelectDevices() returns
//      TRUE. This function will free any memory and resource returned by the 
//      BluetoothSelectDevices() in the BLUETOOTH_SELECT_DEVICE_PARAMS 
//      structure.
//
//  Return Values:
//      TRUE
//          Success.
//
//      FALSE
//          Nothing to free.
//
BOOL
WINAPI
BluetoothSelectDevicesFree(
    BLUETOOTH_SELECT_DEVICE_PARAMS * pbtsdp
    );

// ***************************************************************************
//
//  Device Property Sheet
//
// ***************************************************************************

//
//  Description:
//      Invokes the CPLs device info property sheet.
//
//  Parameters:
//      hwndParent
//          HWND to parent the property sheet.
//
//      pbtdi
//          A pointer to a BLUETOOTH_DEVICE_INFO structure of the device
//          to be displayed.
//
//  Return Values:
//      TRUE
//          The property page was successfully displayed.
//
//      FALSE
//          Failure. The property page was not displayed. Check GetLastError
//          for more details.
//
BOOL
WINAPI
BluetoothDisplayDeviceProperties(
    HWND hwndParent,
    BLUETOOTH_DEVICE_INFO * pbtdi
    );


// ***************************************************************************
//
//  Radio Authentication
//
// ***************************************************************************

//
//  Description:
//      Sends an authentication request to a remote device. 
//
//      There are two modes of operation. "Wizard mode" and "Blind mode."
//
//      "Wizard mode" is invoked when the pszPasskey is NULL. This will cause
//      the "Bluetooth Connection Wizard" to be invoked. The user will be
//      prompted to enter a passkey during the wizard after which the
//      authentication request will be sent. The user will see the success
//      or failure of the authentication attempt. The user will also be
//      given the oppurtunity to try to fix a failed authentication.
//
//      "Blind mode" is invoked when the pszPasskey is non-NULL. This will
//      cause the computer to send a authentication request to the remote
//      device. No UI is ever displayed. The Bluetooth status code will be
//      mapped to a Win32 Error code.
//
//  Parameters:
//
//      hwndParent
//          The window to parent the authentication wizard. If NULL, the 
//          wizard will be parented off the desktop.
//
//      hRadio
//          A valid local radio handle or NULL. If NULL, then all radios will
//          be tired. If any of the radios succeed, then the call will
//          succeed.
//
//      pbtdi
//          BLUETOOTH_DEVICE_INFO record of the device to be authenticated.
//
//      pszPasskey
//          PIN to be used to authenticate the device.  If NULL, then UI is
//          displayed and the user steps through the authentication process.
//          If not NULL, no UI is shown.  The passkey is NOT NULL terminated.
//
//      ulPasskeyLength
//          Length of szPassKey in bytes. The length must be less than or 
//          equal to BLUETOOTH_MAX_PASSKEY_SIZE * sizeof(WCHAR).
//
//  Return Values:
//
//      ERROR_SUCCESS
//          Success.
//
//      ERROR_CANCELLED
//          User aborted the operation.
//
//      ERROR_INVALID_PARAMETER
//          The device structure in pbtdi is invalid.
//
//      ERROR_NO_MORE_ITEMS
//          The device in pbtdi is already been marked as authenticated.
//
//      other WIN32 error
//          Failure. Return value is the error code.
//
//      For "Blind mode," here is the current mapping of Bluetooth status
//      code to Win32 error codes:
//
//          { BTH_ERROR_SUCCESS,                ERROR_SUCCESS },
//          { BTH_ERROR_NO_CONNECTION,          ERROR_DEVICE_NOT_CONNECTED },
//          { BTH_ERROR_PAGE_TIMEOUT,           WAIT_TIMEOUT },
//          { BTH_ERROR_HARDWARE_FAILURE,       ERROR_GEN_FAILURE },
//          { BTH_ERROR_AUTHENTICATION_FAILURE, ERROR_NOT_AUTHENTICATED },
//          { BTH_ERROR_MEMORY_FULL,            ERROR_NOT_ENOUGH_MEMORY },
//          { BTH_ERROR_CONNECTION_TIMEOUT,     WAIT_TIMEOUT },
//          { BTH_ERROR_LMP_RESPONSE_TIMEOUT,   WAIT_TIMEOUT },
//          { BTH_ERROR_MAX_NUMBER_OF_CONNECTIONS, ERROR_REQ_NOT_ACCEP },
//          { BTH_ERROR_PAIRING_NOT_ALLOWED,    ERROR_ACCESS_DENIED },
//          { BTH_ERROR_UNSPECIFIED_ERROR,      ERROR_NOT_READY },
//          { BTH_ERROR_LOCAL_HOST_TERMINATED_CONNECTION, ERROR_VC_DISCONNECTED },
//
DWORD
WINAPI
BluetoothAuthenticateDevice(
    HWND hwndParent,
    HANDLE hRadio,
    BLUETOOTH_DEVICE_INFO * pbtbi,
    PWCHAR pszPasskey,
    ULONG ulPasskeyLength
    );

//
//  Description:
//      Allows the caller to prompt for multiple devices to be authenticated
//      within a single instance of the "Bluetooth Connection Wizard."
//
//  Parameters:
//
//      hwndParent
//          The window to parent the authentication wizard. If NULL, the 
//          wizard will be parented off the desktop.
//
//      hRadio
//          A valid local radio handle or NULL. If NULL, then all radios will
//          be tired. If any of the radios succeed, then the call will
//          succeed.
//
//      cDevices
//          Number of devices in the rgbtdi array.
//
//      rgbtdi
//          An array BLUETOOTH_DEVICE_INFO records of the devices to be
//          authenticated.
//
//  Return Values:
//
//      ERROR_SUCCESS
//          Success. Check the fAuthenticate flag on each of the devices.
//
//      ERROR_CANCELLED
//          User aborted the operation. Check the fAuthenticate flags on 
//          each device to determine if any of the devices were authenticated
//          before the user cancelled the operation.
//
//      ERROR_INVALID_PARAMETER
//          One of the items in the array of devices is invalid.
//
//      ERROR_NO_MORE_ITEMS
//          All the devices in the array of devices are already been marked as
//          being authenticated.
//
//      other WIN32 error
//          Failure. Return value is the error code.
//
DWORD
WINAPI
BluetoothAuthenticateMultipleDevices(
    HWND hwndParent,
    HANDLE hRadio,
    DWORD cDevices,
    BLUETOOTH_DEVICE_INFO * pbtdi
    );

// ***************************************************************************
//
//  Bluetooth Services
//
// ***************************************************************************

#define BLUETOOTH_SERVICE_DISABLE   0x00
#define BLUETOOTH_SERVICE_ENABLE    0x01
#define BLUETOOTH_SERVICE_MASK      ( BLUETOOTH_ENABLE_SERVICE | BLUETOOTH_DISABLE_SERVICE )

//
//  Description:
//      Enables/disables the services for a particular device.
//
//      The system maintains a mapping of service guids to supported drivers for
//      Bluetooth-enabled devices. Enabling a service installs the corresponding
//      device driver. Disabling a service removes the corresponding device driver.
//
//      If a non-supported service is enabled, a driver will not be installed.
//
//  Parameters
//      hRadio
//          Handle of the local Bluetooth radio device.
//
//      pbtdi
//          Pointer to a BLUETOOTH_DEVICE_INFO record.
//
//      pGuidService
//          The service GUID on the remote device.
//
//      dwServiceFlags
//          Flags to adjust the service.
//              BLUETOOTH_SERVICE_DISABLE   -   disable the service
//              BLUETOOTH_SERVICE_ENABLE    -   enables the service
//
//  Return Values:
//      ERROR_SUCCESS
//          The call was successful.
//
//      ERROR_INVALID_PARAMETER
//          dwServiceFlags are invalid.
//
//      ERROR_SERVICE_DOES_NOT_EXIST
//          The GUID in pGuidService is not supported.
//
//      other WIN32 error
//          The call failed.
//
DWORD
WINAPI
BluetoothSetServiceState(
    HANDLE  hRadio,
    BLUETOOTH_DEVICE_INFO * pbtdi,
    GUID *  pGuidService,
    DWORD   dwServiceFlags
    );

//
//  Description:
//      Enumerates the services guids enabled on a particular device. If hRadio
//      is NULL, all device will be searched for the device and all the services 
//      enabled will be returned.
//
//  Parameters:
//      hRadio
//          Handle of the local Bluetooth radio device. If NULL, it will search
//          all the radios for the address in the pbtdi.
//
//      pbtdi
//          Pointer to a BLUETOOTH_DEVICE_INFO record.
//
//      pcService
//          On input, the number of records pointed to by pGuidServices.
//          On output, the number of valid records return in pGuidServices.
//
//      pGuidServices
//          Pointer to memory that is at least *pcService in length.
//
//  Return Values:
//      ERROR_SUCCESS
//          The call succeeded. pGuidServices is valid.
//
//      ERROR_MORE_DATA
//          The call succeeded. pGuidService contains an incomplete list of
//          enabled service GUIDs.
//
//      other WIN32 errors
//          The call failed.
//
DWORD
WINAPI
BluetoothEnumerateInstalledServices(
    HANDLE  hRadio,
    BLUETOOTH_DEVICE_INFO * pbtdi,
    DWORD * pcServices,
    GUID *  pGuidServices
    );

//
//  Description:
//      Change the discovery state of the local radio(s).
//      If hRadio is NULL, all the radios will be set.
//
//      Use BluetoothIsDiscoverable() to determine the radios current state.
//
//      The system ensures that a discoverable system is connectable, thus
//      the radio must allow incoming connections (see 
//      BluetoothEnableIncomingConnections) prior to making a radio 
//      discoverable. Failure to do so will result in this call failing
//      (returns FALSE).
//
//  Parameters:
//      hRadio
//          If not NULL, changes the state of a specific radio.
//          If NULL, the API will interate through all the radios.
//
//      fEnabled
//          If FALSE, discovery will be disabled.
//
//  Return Values
//      TRUE
//          State was successfully changed. If the caller specified NULL for
//          hRadio, at least of the radios accepted the state change.
//
//      FALSE
//          State was not changed. If the caller specified NULL for hRadio, all
//          of the radios did not accept the state change.
//
BOOL
WINAPI
BluetoothEnableDiscovery(
    HANDLE hRadio,
    BOOL fEnabled
    );

//
//  Description:
//      Determines if the Bluetooth radios are discoverable. If there are 
//      multiple radios, the first one to say it is discoverable will cause 
//      this function to return TRUE.
//
//  Parameters:
//      hRadio
//          Handle of the radio to check. If NULL, it will check all local
//          radios.
//
//  Return Values:
//      TRUE
//          A least one radio is discoverable.
//
//      FALSE
//          No radios are discoverable.
//
BOOL
WINAPI
BluetoothIsDiscoverable(
    HANDLE hRadio
    );

//
//  Description:
//      Enables/disables the state of a radio to accept incoming connections.
//      If hRadio is NULL, all the radios will be set.
//
//      Use BluetoothIsConnectable() to determine the radios current state.
//
//      The system enforces that a radio that is not connectable is not
//      discoverable too. The radio must be made non-discoverable (see 
//      BluetoothEnableDiscovery) prior to making a radio non-connectionable. 
//      Failure to do so will result in this call failing (returns FALSE).
//
//  Parameters:
//      hRadio
//          If not NULL, changes the state of a specific radio.
//          If NULL, the API will interate through all the radios.
//
//      fEnabled
//          If FALSE, incoming connection will be disabled.
//
//  Return Values
//      TRUE
//          State was successfully changed. If the caller specified NULL for
//          hRadio, at least of the radios accepted the state change.
//
//      FALSE
//          State was not changed. If the caller specified NULL for hRadio, all
//          of the radios did not accept the state change.
//
BOOL
WINAPI
BluetoothEnableIncomingConnections(
    HANDLE hRadio,
    BOOL fEnabled
    );

//
//  Description:
//      Determines if the Bluetooth radios are connectable. If there are 
//      multiple radios, the first one to say it is connectable will cause 
//      this function to return TRUE.
//
//  Parameters:
//      hRadio
//          Handle of the radio to check. If NULL, it will check all local
//          radios.
//
//  Return Values:
//      TRUE
//          A least one radio is allowing incoming connections.
//
//      FALSE
//          No radios are allowing incoming connections.
//
BOOL
WINAPI
BluetoothIsConnectable(
    HANDLE hRadio
    );

// ***************************************************************************
//
//  Authentication Registration
//
// ***************************************************************************

typedef HANDLE HBLUETOOTH_AUTHENTICATION_REGISTRATION;

typedef BOOL (*PFN_AUTHENTICATION_CALLBACK)(LPVOID pvParam, PBLUETOOTH_DEVICE_INFO pDevice);

//
//  Description:
//      Registers a callback function to be called when a particular device
//      requests authentication. The request is sent to the last application
//      that requested authentication for a particular device.
//
//  Parameters:
//      pbtdi
//          A pointer to a BLUETOOTH_DEVICE_INFO structure. The Bluetooth
//          address will be used for comparision.
//
//      phRegHandle
//          A pointer to where the registration HANDLE value will be 
//          stored. Call BluetoothUnregisterAuthentication() to close
//          the handle.
//
//      pfnCallback
//          The function that will be called when the authentication event
//          occurs. This function should match PFN_AUTHENTICATION_CALLBACK's
//          prototype.
//
//      pvParam
//          Optional parameter to be past through to the callback function.
//          This can be anything the application was to define.
//
//  Return Values:
//      ERROR_SUCCESS
//          Success. A valid registration handle was returned.
//
//      ERROR_OUTOFMEMORY
//          Out of memory.
//
//      other Win32 error.
//          Failure. The registration handle is invalid.
//
DWORD
WINAPI
BluetoothRegisterForAuthentication(
    BLUETOOTH_DEVICE_INFO * pbtdi,
    HBLUETOOTH_AUTHENTICATION_REGISTRATION * phRegHandle,
    PFN_AUTHENTICATION_CALLBACK pfnCallback,
    PVOID pvParam
    );

//
//  Description:
//      Unregisters an authentication callback and closes the handle. See 
//      BluetoothRegisterForAuthentication() for more information about
//      authentication registration.
//
//  Parameters:
//      hRegHandle
//          Handle returned by BluetoothRegisterForAuthentication().
//
//  Return Value:
//      TRUE
//          The handle was successfully closed.
//
//      FALSE
//          The handle was not successfully closed. Check GetLastError for
//          more details.
//
//          ERROR_INVALID_HANDLE
//              The handle is NULL.
//
//          other Win32 errors.
//
BOOL
WINAPI
BluetoothUnregisterAuthentication(
    HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle
    );

//
//  Description:
//      This function should be called after receiving an authentication request
//      to send the passkey response.
//
//  Parameters:
//
//      hRadio
//          Optional handle to the local radio. If NULL, the function will try
//          each radio until one succeeds.
//
//      pbtdi
//          A pointer to a BLUETOOTH_DEVICE_INFO structure describing the device
//          being authenticated. This can be the same structure passed to the 
//          callback function.
//
//      pszPasskey
//          A pointer to UNICODE zero-terminated string of the passkey response
//          that should be sent back to the authenticating device.
//
//  Return Values:
//      ERROR_SUCESS
//          The device accepted the passkey response. The device is authenticated.
//
//      ERROR_CANCELED
//          The device denied the passkey reponse. This also will returned if there
//          is a communications problem with the local radio.
//
//      E_FAIL
//          The device returned a failure code during authentication.
//
//      other Win32 error codes
//
DWORD
WINAPI
BluetoothSendAuthenticationResponse(
    HANDLE hRadio,
    BLUETOOTH_DEVICE_INFO * pbtdi,
    LPWSTR pszPasskey
    );

// ***************************************************************************
//
//  SDP Parsing Functions
//
// ***************************************************************************

typedef struct _SDP_ELEMENT_DATA {
    //
    // Enumeration of SDP element types.  Generic element types will have a
    // specificType value other then SDP_ST_NONE.  The generic types are:
    // o SDP_TYPE_UINT
    // o SDP_TYPE_INT
    // o SDP_TYPE_UUID
    //
    SDP_TYPE type;

    //
    // Specific types for the generic SDP element types.
    //
    SDP_SPECIFICTYPE specificType;

    //
    // Union of all possible data types.  type and specificType will indicate
    // which field is valid.  For types which do not have a valid specificType,
    // specific type will be SDP_ST_NONE.
    //
    union {
        // type == SDP_TYPE_INT
        SDP_LARGE_INTEGER_16 int128;        // specificType == SDP_ST_INT128
        LONGLONG int64;                     // specificType == SDP_ST_INT64
        LONG int32;                         // specificType == SDP_ST_INT32
        SHORT int16;                        // specificType == SDP_ST_INT16
        CHAR int8;                          // specificType == SDP_ST_INT8

        // type == SDP_TYPE_UINT
        SDP_ULARGE_INTEGER_16 uint128;      // specificType == SDP_ST_UINT128
        ULONGLONG uint64;                   // specificType == SDP_ST_UINT64
        ULONG uint32;                       // specificType == SDP_ST_UINT32
        USHORT uint16;                      // specificType == SDP_ST_UINT16
        UCHAR uint8;                        // specificType == SDP_ST_UINT8

        // type == SDP_TYPE_BOOLEAN
        UCHAR booleanVal;

        // type == SDP_TYPE_UUID
        GUID uuid128;                       // specificType == SDP_ST_UUID128
        ULONG uuid32;                       // specificType == SDP_ST_UUID32
        USHORT uuid16;                      // specificType == SDP_ST_UUID32

        // type == SDP_TYPE_STRING
        struct {
            // raw string buffer, may not be encoded as ANSI, use
            // BluetoothSdpGetString to convert the value if it is described
            // by the base language attribute ID list
            LPBYTE value;

            // raw length of the string, may not be NULL terminuated
            ULONG length;
        } string;

        // type == SDP_TYPE_URL
        struct {
            LPBYTE value;
            ULONG length;
        } url;

        // type == SDP_TYPE_SEQUENCE
        struct {
            // raw sequence, starts at sequence element header
            LPBYTE value;

            // raw sequence length
            ULONG length;
        } sequence;

        // type == SDP_TYPE_ALTERNATIVE
        struct {
            // raw alternative, starts at alternative element header
            LPBYTE value;

            // raw alternative length
            ULONG length;
        } alternative;

    } data;

} SDP_ELEMENT_DATA, *PSDP_ELEMENT_DATA;

//
// Description:
//      Retrieves and parses the element found at pSdpStream
//
// Parameters:
//      IN pSdpStream
//          pointer to valid SDP stream
//
//      IN cbSdpStreamLength
//          length of pSdpStream in bytes
//
//      OUT pData
//          pointer to be filled in with the data of the SDP element at the
//          beginning of pSdpStream
//
// Return Values:
//      ERROR_INVALID_PARAMETER
//          one of required parameters is NULL or the pSdpStream is invalid
//
//      ERROR_SUCCESS
//          the sdp element was parsed correctly
//
DWORD
WINAPI
BluetoothSdpGetElementData(
    LPBYTE pSdpStream,
    ULONG cbSdpStreamLength,
    PSDP_ELEMENT_DATA pData
    );

typedef HANDLE HBLUETOOTH_CONTAINER_ELEMENT;

//
// Description:
//      Iterates over a container stream, returning each elemetn contained with
//      in the container element at the beginning of pContainerStream
//
// Parameters:
//      IN pContainerStream
//          pointer to valid SDP stream whose first element is either a sequence
//          or alternative
//
//      IN cbContainerlength
//          length in bytes of pContainerStream
//
//      IN OUT pElement
//          Value used to keep track of location within the stream.  The first
//          time this function is called for a particular container, *pElement
//          should equal NULL.  Upon subsequent calls, the value should be
//          unmodified.
//
//      OUT pData
//          pointer to be filled in with the data of the SDP element at the
//          current element of pContainerStream
//
//  Return Values:
//      ERROR_SUCCESS
//          The call succeeded, pData contains the data
//
//      ERROR_NO_MORE_ITEMS
//          There are no more items in the list, the caller should cease calling
//          BluetoothSdpGetContainerElementData for this container.
//
//      ERROR_INVALID_PARAMETER
//          A required pointer is NULL or the container is not a valid SDP
//          stream
//
// Usage example:
//
// HBLUETOOTH_CONTAINER_ELEMENT element;
// SDP_ELEMENT_DATA data;
// ULONG result;
//
// element = NULL;
//
// while (TRUE) {
//      result = BluetoothSdpGetContainerElementData(
//          pContainer, ulContainerLength, &element, &data);
//
//      if (result == ERROR_NO_MORE_ITEMS) {
//          // We are done
//          break;
//      }
//      else if (result != ERROR_SUCCESS) {
//          // error
//      }
//
//      // do something with data ...
// }
//
//
DWORD
WINAPI
BluetoothSdpGetContainerElementData(
    LPBYTE pContainerStream,
    ULONG cbContainerLength,
    HBLUETOOTH_CONTAINER_ELEMENT* pElement,
    PSDP_ELEMENT_DATA pData
    );

//
// Description:
//      Retrieves the attribute value for the given attribute ID.  pRecordStream
//      must be an SDP stream that is formatted as an SDP record, a SEQUENCE
//      containing UINT16 + element pairs.
//
// Parameters:
//      IN pRecordStream
//          pointer to a valid SDP stream which is formatted as a singl SDP
//          record
//
//      IN cbRecordlnegh
//          length of pRecordStream in bytes
//
//      IN usAttributeId
//          the attribute ID to search for.  see bthdef.h for SDP_ATTRIB_Xxx
//          values.
//
//      OUT pAttributeData
//          pointer that will contain the attribute ID's value
//
// Return Values:
//      ERRROR_SUCCESS
//          Call succeeded, pAttributeData contains the attribute value
//
//      ERROR_INVALID_PARAMETER
//          One of the required pointers was NULL, pRecordStream was not a valid
//          SDP stream, or pRecordStream was not a properly formatted SDP record
//
//      ERROR_FILE_NOT_FOUND
//          usAttributeId was not found in the record
//
// Usage:
//
// ULONG result;
// SDP_DATA_ELEMENT data;
//
// result = BluetoothSdpGetAttributeValue(
//      pRecordStream, cbRecordLength, SDP_ATTRIB_RECORD_HANDLE, &data);
// if (result == ERROR_SUCCESS) {
//      printf("record handle is 0x%x\n", data.data.uint32);
// }
//
DWORD
WINAPI
BluetoothSdpGetAttributeValue(
    LPBYTE pRecordStream,
    ULONG cbRecordLength,
    USHORT usAttributeId,
    PSDP_ELEMENT_DATA pAttributeData
    );

//
// These three fields correspond one to one with the triplets defined in the
// SDP specification for the language base attribute ID list.
//
typedef struct _SDP_STRING_TYPE_DATA {
    //
    // How the string is encoded according to ISO 639:1988 (E/F): "Code
    // for the representation of names of languages".
    //
    USHORT encoding;

    //
    // MIBE number from IANA database
    //
    USHORT mibeNum;

    //
    // The base attribute where the string is to be found in the record
    //
    USHORT attributeId;

} SDP_STRING_TYPE_DATA, *PSDP_STRING_TYPE_DATA;

//
// Description:
//      Converts a raw string embedded in the SDP record into a UNICODE string
//
// Parameters:
//      IN pRecordStream
//          a valid SDP stream which is formatted as an SDP record
//
//      IN cbRecordLength
//          length of pRecordStream in bytes
//
//      IN pStringData
//          if NULL, then the calling thread's locale will be used to search
//          for a matching string in the SDP record.  If not NUL, the mibeNum
//          and attributeId will be used to find the string to convert.
//
//      IN usStringOffset
//          the SDP string type offset to convert.  usStringOffset is added to
//          the base attribute id of the string.   SDP specification defined
//          offsets are: STRING_NAME_OFFSET, STRING_DESCRIPTION_OFFSET, and
//          STRING_PROVIDER_NAME_OFFSET (found in bthdef.h).
//
//      OUT pszString
//          if NULL, pcchStringLength will be filled in with the required number
//          of characters (not bytes) to retrieve the converted string.
//
//      IN OUT pcchStringLength
//          Upon input, if pszString is not NULL, will contain the length of
//          pszString in characters.  Upon output, it will contain either the
//          number of required characters including NULL if an error is returned
//          or the number of characters written to pszString (including NULL).
//
//  Return Values:
//      ERROR_SUCCES
//          Call was successful and pszString contains the converted string
//
//      ERROR_MORE_DATA
//          pszString was NULL or too small to contain the converted string,
//          pccxhStringLength contains the required length in characters
//
//      ERROR_INVALID_DATA
//          Could not perform the conversion
//
//      ERROR_NO_SYSTEM_RESOURCES
//          Could not allocate memory internally to perform the conversion
//
//      ERROR_INVALID_PARAMETER
//          One of the rquired pointers was NULL, pRecordStream was not a valid
//          SDP stream, pRecordStream was not a properly formatted record, or
//          the desired attribute + offset was not a string.
//
//      Other HRESULTs returned by COM
//
DWORD
WINAPI
BluetoothSdpGetString(
    LPBYTE pRecordStream,
    ULONG cbRecordLength,
    PSDP_STRING_TYPE_DATA pStringData,
    USHORT usStringOffset,
    PWCHAR pszString,
    PULONG pcchStringLength
    );

// ***************************************************************************
//
//  Raw Attribute  Enumeration
//
// ***************************************************************************

typedef BOOL (CALLBACK *PFN_BLUETOOTH_ENUM_ATTRIBUTES_CALLBACK)(
    ULONG   uAttribId,
    LPBYTE  pValueStream,
    ULONG   cbStreamSize,
    LPVOID  pvParam
    );

//
//  Description:
//      Enumerates through the SDP record stream calling the Callback function
//      for each attribute in the record. If the Callback function returns
//      FALSE, the enumeration is stopped.
//
//  Return Values:
//      TRUE
//          Success! Something was enumerated.
//
//      FALSE
//          Failure. GetLastError() could be one of the following:
//
//          ERROR_INVALID_PARAMETER
//              pSDPStream or pfnCallback is NULL.
//
//          ERROR_INVALID_DATA
//              The SDP stream is corrupt.
//
//          other Win32 errors.
//
#define BluetoothEnumAttributes BluetoothSdpEnumAttributes

BOOL
WINAPI
BluetoothSdpEnumAttributes(
    LPBYTE  pSDPStream,
    ULONG   cbStreamSize,
    PFN_BLUETOOTH_ENUM_ATTRIBUTES_CALLBACK pfnCallback,
    LPVOID  pvParam
    );

#ifdef __cplusplus
}
#endif
