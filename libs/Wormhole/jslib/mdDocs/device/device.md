Device
======

> The `device` object describes the device's hardware and software.

Properties
----------

- device.name
- device.phonegap (used for PhoneGap compatibility)
- device.platform
- device.uuid
- device.version

Variable Scope
--------------

Since `device` is assigned to the `window` object, it is implicitly in the global scope.

    // These reference the same `device`
    //
    var phoneName = window.device.name;
    var phoneName = device.name;