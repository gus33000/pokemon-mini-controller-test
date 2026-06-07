@echo off
echo Removing previous output directory...
rmdir /Q /S bin

for /f %%f in ('dir /b /s *.obj') do del %%f

echo.
echo Cloning S1C88 Pokemon Mini toolset...
git clone https://github.com/pokemon-mini/c88-pokemini.git

echo.
echo Installing SC188 Pokemon Mini toolset...
cd c88-pokemini
echo N | powershell -ExecutionPolicy Bypass .\install.ps1
cd ..

set UPDATED_PATH=%PATH%;%CD%\c88-pokemini\c88tools\bin

setx PATH "%UPDATED_PATH%"
setx C88INC "%CD%\c88-pokemini\c88tools\include"
setx C88LIB "%CD%\c88-pokemini\c88tools\lib"

set PATH=%UPDATED_PATH%
set C88INC=%CD%\c88-pokemini\c88tools\include
set C88LIB=%CD%\c88-pokemini\c88tools\lib

echo.
echo Building Pokemon Mini ROM...

%CD%\c88-pokemini\c88tools\bin\as88.exe -e -I%CD%\c88-pokemini\c88tools\..\include -Md -o src\startup.obj src\startup.asm

%CD%\c88-pokemini\c88tools\bin\mk88.exe

echo.
echo Moving built files...
mkdir bin
move *.map bin\
move *.min bin\
move *.sre bin\

echo.
echo Cleaning up...
rmdir /Q /S c88-pokemini