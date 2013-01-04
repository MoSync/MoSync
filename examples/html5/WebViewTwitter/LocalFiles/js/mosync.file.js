/**
 * @file mosync.file.js
 * @author Mikael Kindborg
 *
 * Very simple library for file access. Extends the
 * mosync object with new functions.
 */

/**
 * The file submodule.
 */
mosync.file = {};

/**
 * Get the path to the local storage area on the device.
 * The path is returned asynchronously to the callback function.
 */
mosync.file.getLocalPath = function(callbackFun)
{
	mosync.bridge.send(
		["Custom", "File", "mosync.file.getLocalPath"],
		callbackFun);
};

/**
 * Read the contents of a text file asynchronously.
 */
mosync.file.read = function(filePath, callbackFun)
{
	mosync.bridge.send(
		["Custom", "File", "mosync.file.read", filePath],
		callbackFun);
};

/**
 * Write the contents of a text file asynchronously.
 */
mosync.file.write = function(filePath, data, callbackFun)
{
	mosync.bridge.send(
		["Custom", "File", "mosync.file.write", filePath, data],
		callbackFun);
};

/**
 * Add function for logging to the top-level
 * of the mosync object.
 */
mosync.log = function(message)
{
	mosync.bridge.send(
		["Custom", "File", "mosync.log", message],
		null);
};
