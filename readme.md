Myo Mouse Emulator
===================

Description and Usage
----------------------

This project implements a mouse emulator for Windows (including mouse movement and left mouse button control) using the Alpha version of the Thalmic Labs Myo armband.

Controls:

* Make a fist to hold down left mouse button
* Rest your hand to release the mouse button
* Wave-in to perform a quick left click
* Move your arm around to move the mouse cursor

Building and Running
---------------------

* Use `compilemyo.bat` as a starting point for compiling (The actual SDK is not included here)

* Include the relevant DLL files from the SDK to the directory you run the EXE from (ie: `myo32.dll`, `myo64.dll`, `myo-dotnet.dll`)

Notes
------

* This project has only been tested with the Alpha hardware, running firmware 0.7.3, using SDK alpha 6. It might not work with other SDK / firmware / hardware combinations, and likely needs a bit of reworking to add support for the new Beta SDK / firmware / hardware.

* This project is Windows only at the moment, but support for other operating systems can be added by including a file that implements the class found in win-mouse.cpp

* Make sure you first train the Myo using the software provided by the SDK.

