/** \file api_connections.h
* \brief Overview of the MoSync Connection API
* \author Fredrik Eldh
*/

/** \page connApiOverview MoSync Connection API Overview

The MoSync Connection API provides support for communicating with remote devices
through protocols such as HTTP, TCP and Bluetooth.

The API is comprised of all functions whose names begin with \a maConn,
a few structs and #EVENT_TYPE_CONN.

The different aspects of each symbol is explained in their respective documentation,
but there are a few common features that will be explained here.

MoSync Connections are asynchronous. This means that when you call an \a maConn function,
it will return immediately, but it will keep working in the background. When it's done,
it will send an event, which must be retrieved by maGetEvent().

There are different types of connection operations.
For each connection, only one of each type of operation may be active at the same time.
The operation types are defined as \link #CONNOP_READ CONNOP \endlink constants.

For example, you can run maConnRead() and maConnWrite() simultaneously on
a single connection, but two Writes or a combination of maConnRead() and maConnReadToData()
will cause a Panic.

Write operations are especially hazardous in this respect.
They don't actually wait for the data to reach the remote end.
Thus, they can complete very quickly; practically instantly on modern PCs with a broadband connection.
Add to this the fact that you don't actually need to retrieve the completion event
from the event queue before calling maConnWrite*() again.
This means that it's possible to write a program that calls maConnWrite()
multiple times without waiting for it to complete, and have it work without error
on MoRE or even on WiFi-equipped phones, and then have it fail on slower connections such as 3G.

*/
