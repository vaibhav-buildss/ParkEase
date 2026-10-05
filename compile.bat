@echo off
cd /d "%~dp0backend"

echo Compiling ParkEase...

gcc main.c -o main.exe
if errorlevel 1 (
    echo Error compiling main.c
    pause
    exit /b 1
)

gcc api.c -o api.exe
if errorlevel 1 (
    echo Error compiling api.c
    pause
    exit /b 1
)

echo.
echo Compilation successful.
echo.
echo main.exe = C terminal version
echo api.exe  = used by the simple website
echo.
pause
