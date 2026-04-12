@echo off

set ROOT=.
call :create_folder "%ROOT%"
call :create_folder "%ROOT%\bin"

call glslangValidator -V -o bin/basic.vert.spirv basic.vert.glsl
call glslangValidator -V -o bin/basic.frag.spirv basic.frag.glsl
pause

:create_folder
if not exist "%~1" (
    mkdir "%~1"
)
