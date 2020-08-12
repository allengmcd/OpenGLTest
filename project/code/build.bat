@echo off

 
set CommonCompilerFlags=-WL -Od -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 -GS- -Gs9999999

set CommonCompilerFlags= %CommonCompilerFlags%

set glm_inc="E:/ProgramFiles/GLM"

set IncludeFlags= -I%glm_inc%

set CompilerFlags= %IncludeFlags% -EHsc -nologo -std:c++17 -FC -Zi 

set CommonLinkerFlags= -STACK:0x100000,0x100000 -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib kernel32.lib opengl32.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM 64-bit build

cl /MD %CompilerFlags% ..\code\OpenGLTest.cpp user32.lib Gdi32.lib opengl32.lib 

popd

