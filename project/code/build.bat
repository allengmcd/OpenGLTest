@echo off

 
set CommonCompilerFlags=-WL -Od -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 -GS- -Gs9999999
set CommonCompilerFlags=-DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 %CommonCompilerFlags%
set CommonLinkerFlags= -STACK:0x100000,0x100000 -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib kernel32.lib opengl32.lib

IF NOT EXIST build mkdir build
pushd build
cl -FC -Zi ../SplineTest.cpp  user32.lib Gdi32.lib opengl32.lib 
popd

