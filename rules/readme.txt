Quick guide to writing Workfiles

First off, there are many examples throughout MoSync, so I won't give any here.

Workfiles are actually Ruby programs.
They can, but are not required to, use some of the Work library to build their stuff.
They are canonically called 'workfile.rb', but that is only required if you use the method Work.invoke_subdir.
The Work library files are canonically stored in the directory 'rules', but again, this is not required.

Normally, a Workfile will define an object of one of the Work classes, appropriate for the type of work that will be done, and then use instance_eval to set variables in the object, that contain the specific details of the work.

Useful Work classes include ExeWork(exe.rb), DllWork(dll.rb) and NativeLibWork(native_lib.rb)
For building MoSync internals, variants of these three defined are in native_mosync.rb.
For building MoSync libraries, use the MoSyncLib module, which works a bit differently, because it can build both pipe and native versions. See mosync_lib.rb for details.
For building MoSync applications, use PipeExeWork(mosync_exe.rb).

Common variables that one will want to set include:
@NAME, the name of the work.
@SOURCES, a list of directories containing source files.

Other variables may be used, depending on the type of Work. See defaults.rb for a list of variables and their default values.

When the variables are set, call invoke on your object. This builds all needed files.


# Options
Workfiles can take command-line arguments. If they're on the form "KEY=value", then they're an option, which are by default saved as constants in your workfile.


# Targets
Command-line arguments on the form "key" are Targets. They have to match targets defined in the Workfile, or an error is generated. Not all Workfiles use the Target system; those that don't simply ignore any command-line targets. Targets are defined using rake-like syntax:

target :name => preqs do
	...
end

Where preqs is either a single symbol or an array of symbols.
Each symbol specifies another target, or the name of a Work defined previously in the Workfile.

If no targets are specified on the command line, the :default target will be invoked. It's usually a good idea to specify a :default target in every workfile that uses targets, unless you want to force the user to select one.

Finish by calling Targets.invoke.


# Subdirectories
To invoke workfiles in other directories, use Work.invoke_subdirs. Options will be passed on, and you can specify new targets, or go with the default target.
