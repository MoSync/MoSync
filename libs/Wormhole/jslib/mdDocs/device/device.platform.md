device.platform
===============

Get the device's operating system name.

     var string = device.platform;


Quick Example
-------------

    // Depending on the device, a few examples are:
    //   - "Android"
    //   - "iPhone"
    var devicePlatform = device.platform;

Full Example
------------

    <!DOCTYPE html>
    <html>
      <head>
        <title>Device Properties Example</title>

        <script type="text/javascript" charset="utf-8" src="js/wormhole.js"></script>
        <script type="text/javascript" charset="utf-8">

        // Wait for Wormhole to load
        //
        document.addEventListener("deviceready", onDeviceReady, false);

        // Wormhole is ready
        //
        function onDeviceReady() {
            var element = document.getElementById('deviceProperties');
    
            element.innerHTML = 'Device Name: '     + device.name     + '<br />' + 
                                'Device Platform: ' + device.platform + '<br />' + 
                                'Device UUID: '     + device.uuid     + '<br />' + 
                                'Device Version: '  + device.version  + '<br />';
        }

        </script>
      </head>
      <body>
        <p id="deviceProperties">Loading device properties...</p>
      </body>
    </html>
    
