>Web view properties
>===================
>
url
---
>Set or get the currently displayed url in the web view.
html
----
>Set the currently displayed HTML data in the web view.
baseUrl
-------
>Set the base URL used by the web-view when loading relative paths. The value of this URL is used when setting the MAW_WEB_VIEW_URL and MAW_WEB_VIEW_HTML properties. The default value for this property points to the Assets folder in the local file system ("file://pathToLocalFileSystem/Assets/").
softHook
--------
>Set the pattern used to "soft hook" urls, to get notified when pages are being loaded.
hardHook
--------
>Set the pattern used to "hard hook" urls, to get notified and prevent loading of pages.
newurl
------
>Property to get a new url whenever the webview changes the url. See MAW_EVENT_WEB_VIEW_URL_CHANGED.
horizontalScrollBarEnabled
--------------------------
>Sets or gets whether the horizontal scrollbar should be drawn or not. Available only on Android for the moment.
verticalScrollBarEnabled
------------------------
>Sets or gets whether the vertical scrollbar should be drawn or not. The scrollbar is drawn by default. Available only on Android for the moment.
enableZoom
----------
>Enable or disable the zoom controls of the web view.
navigate
--------
>Navigate forward or back the browsing history.
