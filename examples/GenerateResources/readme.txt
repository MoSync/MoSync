This example shows one way to generate resources at build-time for MoSync projects.
It also shows a way to integrate that generation with command-line building of installable packages.

The core of this system is the workfile.
It is a Ruby script that contains the definitions of the resources used in the program,
as well as build instructions for the program itself.

The workfile contains three sections.
1. Sub-systems, useful variables, and helper functions.
2. The resources themselves.
3. Build instructions.

You'll probably be most interested in the middle section. To understand it, you should need only basic programming skills.

There are three ways to run the system from a command line.
* You can invoke the workfile directly. This will build the resources and the program, but not package it.
	"ruby workfile.rb"
** Or, to just build the resources:
	"ruby workfile.rb lst"
* You can build and package the program using Eclipse's headless mode:
	"ruby build-one.rb MoSync/Emulator"
	Replace "MoSync/Emulator" with the device profile of your choice.
* You can build and package the program for many devices at once:
	"ruby build-all.rb"
	The list of target devices are inside the build-all file. Edit them as you please.

There are two files you'll need to create: config.rb and profile.rb.
config.rb contains the path to the Eclipse workspace to which you've imported the project.
profile.rb contains the name of the device profile used for manual invocations of the workfile.

End of Line
