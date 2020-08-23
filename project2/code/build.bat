@echo off

 
set CommonCompilerFlags=-WL -Od -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 -GS- -Gs9999999

set CommonCompilerFlags= %CommonCompilerFlags%

set glew_inc="E:\ProgramFiles\GLEW\glew-2.1.0\include"
set glfw_inc="E:\ProgramFiles\GLFW\glfw-3.3.2.bin.WIN64\include"
set glm_inc="E:/ProgramFiles/GLM"

set IncludeFlags= -I%glew_inc% -I%glfw_inc% -I%glm_inc%

set glew_link="E:\ProgramFiles\GLEW\glew-2.1.0\lib\Release\x64"
set glfw_link="E:\ProgramFiles\GLFW\glfw-3.3.2.bin.WIN64\lib-vc2019"

set LinkerFlags= /LIBPATH:%glew_link% /LIBPATH:%glfw_link%

set CompilerFlags= %IncludeFlags% -EHsc -nologo -std:c++17 -FC -Zi 

set CommonLinkerFlags=  %LinkerFlags% glfw3.lib glew32.lib user32.lib Gdi32.lib opengl32.lib kernel32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib

set Sources1= ../*.cpp 

IF NOT EXIST build mkdir build
pushd build

REM 64-bit build

cl /MD %CompilerFlags% %Sources1% /Fe:OpenGLTest /link %CommonLinkerFlags%

popd

