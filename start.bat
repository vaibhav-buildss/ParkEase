@echo off
cd /d "%~dp0"

if not exist "backend\api.exe" (
    echo api.exe not found.
    echo Please run compile.bat first.
    pause
    exit /b 1
)

cd frontend

echo Starting ParkEase...
echo Open http://localhost:3000 in your browser.
echo.
node server.js

pause
