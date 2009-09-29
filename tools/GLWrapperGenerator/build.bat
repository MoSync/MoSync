ruby GLWrapperGen.rb
mkdir ..\..\runtimes\java\shared\generated
copy opengl_invoke_generated.jpp ..\..\runtimes\java\shared\generated
copy magl_functions.h ..\..\libs\MAStd\GLES
copy opengl_generated.idl ..\idl2
