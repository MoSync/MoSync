//
// This function sends a message to MoSync C/C++ code. The message
// is posted as a widget event to the MoSync event queue.
// The function takes a variable number of arguments.
//
// Format is:
//   SendMoSyncMessage(<messageName>, <param1>, <value1>, ...);
// Example:
//
//   SendMoSyncMessage('SendSMS', 'phoneNo', '0707602631', 'Hi Micke :)');
//
function SendMoSyncMessage(messageName)
{
  // Create first part of message url.
  var message = 'mosync://' + messageName;

  // If there are at least one parameter,
  // add the '?' delimiter character.
  if (arguments.length > 2)
  {
    message = message + '?';
  }

  // Iterate over parameters.
  for (var i = 1; i < arguments.length; i = i + 2)
  {
    // Parmeter name and value are "percent encoded".
    message =
      message
      + escape(arguments[i])
      + '='
      + escape(arguments[i + 1]);

    // If there are more parameters, add the '&' delimiter.
    if (arguments.length > i + 2)
    {
      message = message + '&';
    }
  }

  // Setting the document location "sends" the message.
  document.location = message;
}


/* THIS IS A SKETCH OF A CALLBACK BASED MESSAGE SENDER
var MessageHandler = function()
{
  var self = {};
  var funTable = {};
  var funId = 0;

  self.send = function(message, callback)
  {
    funId = funId + 1;
    funTable[funId] = callback;
    document.location = 'mosync://' + funId + '/' + escape(message);
  }

  callJS("MessageHandler.reply(45, 1)");

  self.reply = function(id)
  {
    var callback = funTable[id];
    if (undefined != callback)
    {
      -- Remove the first param, the function id.
      var args = Array.prototype.slice.call(arguments);
      args.shift()
      -- Call the function.
      callback.apply(null, args);
    }
  }

  return self;
}()


MessageHandler.send('ReadFile/"Dir/Data.txt"', function(data) {
  if (null != data) {
    alert(data);
  }
});

MessageHandler.send('WriteFile//dir/Data.txt', 'HelloWorld', function(result) {
  if (null != result) {
    alert(result);
  }
});

function SendSMS(phoneNo, message, callback)
{
  MessageHandler.send(
    'SendSMS/' + id + '/' + phoneNo + '/' + message,
    callback);
}
*/
