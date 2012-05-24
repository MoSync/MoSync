
var net = require('net');

// File upload server test code.

var server = net.createServer(function(socket)
{
	console.log('Socket connected');

	socket.on('end', function()
	{
		console.log('Socket disconnected');
	});

	socket.on('data', function(data)
	{
		console.log('server data:\n' + data.toString());
		socket.write('HTTP/1.1 200 OK\r\n');
		socket.write('Content-Type: text/plain\r\n');
		socket.write('\r\n');
		socket.write('Hello World');
		socket.end();
	});
});

/**
 * Opens a mock TCP connection to find out this server's IP,
 * supplied to the callback
 */
function getNetworkIP(callback)
{
	var socket = net.createConnection(80, "www.google.com");
	socket.on('connect', function()
	{
		//callback(undefined, socket.address().address);
		callback(socket.address().address);
		socket.end();
	});
	socket.on('error', function(e)
	{
		//callback(e, 'error');
		callback(undefined);
	});
}

server.listen(4042, function()
{
	console.log('Upload server started');
	var address = server.address();
	getNetworkIP(function(address)
	{
		if (address != undefined)
		{
			console.log('Server address: ' + address + ':4042');
		}
	});
});
