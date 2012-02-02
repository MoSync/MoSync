MediaFile.getFormatData
=======================

> Retrieves format information about the media capture file.

    mediaFile.getFormatData( 
        MediaFileDataSuccessCB successCallback, 
        [MediaFileDataErrorCB errorCallback]
    );

Description
-----------

This function asynchronously attempts to retrieve the format information for the media file.  If successful, it invokes the MediaFileDataSuccessCB callback with a MediaFileData object.  If the attempt fails, this function will invoke the MediaFileDataErrorCB callback.

Supported Platforms
-------------------

- Android
- iOS

Android Quirks
--------------
The API for retrieving media file format information is limited.  Therefore, not all MediaFileData properties are supported.  See MediaFileData documentation.

iOS Quirks
----------
The API for retrieving media file format information is limited.  Therefore, not all MediaFileData properties are supported.  See MediaFileData documentation.