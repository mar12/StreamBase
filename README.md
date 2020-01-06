# StreamBase
A client/server application to manage data on a Windows system

## Introduction

This document provides only general information about the application. Some specific information on a particular code can be found in the comments at the beginning of each header file and among the code. However, as the code is written to be self-explanatory, there are not many comments among it.


## General Information

The application itself comprises of three executable files:
* `StreamBaseServer.exe` - server application 
* `StreamBaseClient.exe` - client application
* `UnitTests.exe` - unit tests application

There are no third-party dependencies in the application, only the standard C++ library features and Windows API are used.


## Communication protocol

A communication protocol is developed specifically for this application. For communication, the Windows API message-type Named Pipes are used.
A request message to the server has the following format:

| Action ID | Class ID | Object ID | Member ID | Data |
|-----------|----------|-----------|-----------|------|

A response message from the server has the following format:

| Result ID | Data |
|-----------|------|

`Action ID` and `Result ID` fields are of 32-bit integral type whereas other fields are variable-length arrays preceded by the string length 32-bit integer. However, the type of `Class ID`, `Object ID` and `Member ID` fields can be easily replaced to the integral type, as the string type was chosen because of the simplicity of the interactive client.
Depending on the `Action ID` and `Result ID`, not all fields are used in the request or response. These unused fields consume only 4 bytes per each, so it should not be a problem.


## The Server

The server application registers a class and waits for connections of the clients. Many clients can be connected to one server. The default pipe name `\.\pipe\streambase` can be changed with a command-line argument (type `StreamBaseServer /?` for more details).
To make use of a custom C++ class, the class should be registered with `ClassRegistry::ClassesPool`, and its members (attributes and methods) supposed to be used from the client, should be registered with `ClassRegistry::Class<T>` returned on class registration. The registration of classes and their members must be done before starting the communication to prevent race conditions.
As many clients can be connected simultaneously, object creation, removal and retrieving are protected by a mutex. However, the protection of the registered class data members, if needed, should be implemented inside that class. This is to avoid unnecessary mutex handling delays when other means are used for preventing race conditions.


## The Client

The client application has an interactive command-line interface. The interactive interface was chosen for easier usage compared to the command-line arguments interface. However, the default pipe name `\\.\pipe\streambase` can be changed with a command-line argument (type `StreamBaseClient /?` for more details).
After the client application starts, the user is expected to type commands. For the list of supported commands, type `help` and press ENTER. With `mode` command, the user can switch mode to synchronous (the default one) and asynchronous. In asynchronous mode, the command processing and communication are performed asynchronously to the main thread. For that reason, the processing result can interfere with the command-line interface (this could have been protected by a mutex, but it was considered as non-important as the output to `stdout` is just a demo feature; also, it is good proof that operations are done asynchronously).
Despite the asynchronous mode, the request and response are protected by a mutex to prevent interfering with other requests and responses, as they can be mixed among receiving concurrently-executed threads. For asynchronous mode, the I/O with `OVERLAPPED` structure isn't used, as that has no benefits compared to running I/O in separate thread implemented.

## Tests

The unit tests provided are not supposed to provide full code coverage. They were written just to simplify the debugging at the development stage. However, to check the application, manual testing was performed. Tests were performed on the following configuration:
* Windows 7 Ultimate SP1 build 7601
* Build with Visual Studio 2019 Community


