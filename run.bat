@echo off
.\setup.bat
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)
.\bin\Windows\Debug\Render-Engine.exe
