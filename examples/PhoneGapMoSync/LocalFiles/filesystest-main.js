
function PrintObject(obj, indent)
{
    if (undefined === indent)
    {
        indent = "";
    }

    console.log(indent + "@@ PrintObject");

    for (var field in obj)
    {
        if (typeof obj[field] != "function")
        {
            console.log("  " + indent + "[" + field + ": " + obj[field] + "]");
            if ((null != obj[field]) && (typeof obj[field] == "object"))
            {
                PrintObject(obj[field], indent + "  ");
            }
        }
    }
}

var FileSys = function()
{
    var self = {};

    /**
     * Make an error handling function that calls fun.
     */
    function error(fun)
    {
        return function(result)
        {
            console.log("@@@ FileSys error: " + result.code);
            fun(false, null);
        };
    }

    self.writeText = function(path, data, fun)
    {
        window.requestFileSystem(
            LocalFileSystem.PERSISTENT,
            0,
            function (fileSystem)
            {
                fileSystem.root.getFile(
                    path,
                    { create: true },
                    function(fileEntry)
                    {
                        var writer = new FileWriter(fileEntry);
                        writer.onwrite = function(obj)
                        {
                            fun(true);
                        };
                        writer.error = function(obj)
                        {
                            fun(false);
                        };
                        writer.write(data);
                    },
                    error(fun));
            },
            error(fun));
    };

    self.readText = function(path, fun)
    {
        window.requestFileSystem(
            LocalFileSystem.PERSISTENT,
            0,
            function (fileSystem)
            {
                fileSystem.root.getFile(
                    path,
                    { create: true },
                    function(fileEntry)
                    {
                        var reader = new FileReader();
                        reader.onload = function(obj)
                        {
                            fun(true, obj.target.result);
                        };
                        reader.onerror = function(obj)
                        {
                             fun(false, null);
                        };
                        reader.readAsText(fileEntry);
                    },
                    error(fun));
            },
            error(fun));
    };

    return self;
}();

function testFileSystem()
{
    function fileWritten(success)
    {
        console.log("@@@@@ FileWritten result: " + success);
        if (success)
        {
            FileSys.readText("hello2.txt", fileRead);
        }
    }

    function fileRead(success, data)
    {
        console.log("@@@@@ FileRead result: " + success);
        console.log("@@@@@ FileRead data: " + data);
    }

    FileSys.writeText("hello2.txt", "Hello Wonderful World!", fileWritten);
}

function foo()
{
    function bar()
    {
        return 44;
    }

    return bar();
}

function XtestFileSystem() {
    console.log("@@ Calling window.requestFileSystem");
    window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, gotFileSystem, filefail);
    //window.requestFileSystem(LocalFileSystem.TEMPORARY, 0, gotFileSystem, filefail);
}

function gotFileSystem(fileSystem)
{
    console.log("@@ gotFileSystem");
    PrintObject(fileSystem);
    console.log("@@ Calling fileSystem.root.getFile");
    fileSystem.root.getFile("helloworld.txt", {create: true}, gotOpenFileEntry, filefail);
}

function gotOpenFileEntry(fileEntry) {
    gFileEntry = fileEntry;
    console.log("@@ gotOpenFileEntry");
    console.log("@@ Calling fileEntry.createWriter");
    fileEntry.createWriter(gotFileWriter, filefail);
}

function gotFileWriter(writer) {
    console.log("@@ gotFileWriter");
    writer.onwrite = function(obj) {
        console.log("@@ writer.onwrite obj: " + obj);
        PrintObject(obj);
        readFileEntry(gFileEntry);
    };
    writer.error = function(obj) {
        console.log("@@ writer.error");
        PrintObject(obj);
    };
    writer.write("Hello World");
    console.log("@@ writer.write called");
}

function readFileEntry(fileEntry)
{
    console.log("@@ readFileEntry");
    console.log("@@ Calling fileEntry.file");
    fileEntry.file(function(file)
    {
        readAsText(file);
    },
    filefail);
}

function readAsText(file) {
    console.log("@@ readAsText");
    var reader = new FileReader();
    reader.onload = function(obj)
    {
        console.log("@@ reader.onload");
        PrintObject(obj);
    };
    reader.onerror = function(obj)
    {
        console.log("@@ reader.onerror");
        PrintObject(obj);
    };
    console.log("@@ Calling reader.readAsText");
    reader.readAsText(file);
}

function readFile(file){
    console.log("@@ readFile");
    readDataUrl(file);
    readAsText(file);
}

function readDataUrl(file) {
    console.log("@@ readDataUrl");
    var reader = new FileReader();
    reader.onloadend = function(evt) {
        console.log("Read as data URL");
        console.log(evt.target.result);
    };
    reader.readAsDataURL(file);
}


function filefail(error) {
    console.log("@@ filefail code: " + error.code);
}

// ---------------------------------------------------------------

var deviceInfo = function() {
    document.getElementById("platform").innerHTML = device.platform;
    document.getElementById("version").innerHTML = device.version;
    document.getElementById("uuid").innerHTML = device.uuid;
    document.getElementById("name").innerHTML = device.name;
    document.getElementById("width").innerHTML = screen.width;
    document.getElementById("height").innerHTML = screen.height;
    document.getElementById("colorDepth").innerHTML = screen.colorDepth;
};

var getLocation = function() {
    var suc = function(p) {
        alert(p.coords.latitude + " " + p.coords.longitude);
    };
    var locFail = function() {
    };
    navigator.geolocation.watchPosition(suc, locFail);
};

var beep = function() {
    navigator.notification.beep(2);
};

var vibrate = function() {
    navigator.notification.vibrate(1000);
};

function roundNumber(num) {
    var dec = 3;
    var result = Math.round(num * Math.pow(10, dec)) / Math.pow(10, dec);
    return result;
}

var accelerationWatch = null;

function updateAcceleration(a) {
    document.getElementById('x').innerHTML = roundNumber(a.x);
    document.getElementById('y').innerHTML = roundNumber(a.y);
    document.getElementById('z').innerHTML = roundNumber(a.z);
}

var toggleAccel = function() {
    if (accelerationWatch !== null) {
        navigator.accelerometer.clearWatch(accelerationWatch);
        updateAcceleration({
            x : "",
            y : "",
            z : ""
        });
        accelerationWatch = null;
    } else {
        var options = {};
        options.frequency = 1000;
        accelerationWatch = navigator.accelerometer.watchAcceleration(
                updateAcceleration, function(ex) {
                    alert("accel fail (" + ex.name + ": " + ex.message + ")");
                }, options);
    }
};

var preventBehavior = function(e) {
    e.preventDefault();
};

function dump_pic(data) {
    var viewport = document.getElementById('viewport');
    console.log(data);
    viewport.style.display = "";
    viewport.style.position = "absolute";
    viewport.style.top = "10px";
    viewport.style.left = "10px";
    document.getElementById("test_img").src = "data:image/jpeg;base64," + data;
}

function fail(msg) {
    alert(msg);
}

function show_pic() {
    navigator.camera.getPicture(dump_pic, fail, {
        quality : 50
    });
}

function close() {
    var viewport = document.getElementById('viewport');
    viewport.style.position = "relative";
    viewport.style.display = "none";
}

function contacts_success(contacts) {
    alert(contacts.length
            + ' contacts returned.'
            + (contacts[2] && contacts[2].name ? (' Third contact is ' + contacts[2].name.formatted)
                    : ''));
}

function get_contacts() {
    var obj = new ContactFindOptions();
    obj.filter = "";
    obj.multiple = true;
    navigator.contacts.find(
            [ "displayName", "name" ], contacts_success,
            fail, obj);
}

function check_network() {
    var networkState = navigator.network.connection.type;

    var states = {};
    states[Connection.UNKNOWN]  = 'Unknown connection';
    states[Connection.ETHERNET] = 'Ethernet connection';
    states[Connection.WIFI]     = 'WiFi connection';
    states[Connection.CELL_2G]  = 'Cell 2G connection';
    states[Connection.CELL_3G]  = 'Cell 3G connection';
    states[Connection.CELL_4G]  = 'Cell 4G connection';
    states[Connection.NONE]     = 'No network connection';

    confirm('Connection type:\n ' + states[networkState]);
}

function init() {
    // the next line makes it impossible to see Contacts on the HTC Evo since it
    // doesn't have a scroll button
    // document.addEventListener("touchmove", preventBehavior, false);
    document.addEventListener("deviceready", deviceInfo, true);
}
