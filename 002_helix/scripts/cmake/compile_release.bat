@echo off

set d=%CD%
if not exist "%d%\build" (
   mkdir %d%\build
)

if not exist "%d%\..\..\..\bin" (
   mkdir "%d\..\..\..\bin"
)

if not exist "%d%\..\..\..\bin\data" (
   mkdir "%d\..\..\..\bin\data"
)

cd %d%\build
cmake -G "Visual Studio 10" ..\
cmake --build . --target install --config Release

cd %d%