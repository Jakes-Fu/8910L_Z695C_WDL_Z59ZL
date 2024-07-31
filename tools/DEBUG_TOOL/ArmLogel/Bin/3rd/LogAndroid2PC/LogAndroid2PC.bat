@echo off
set VERSION=3.0.6
title Export Log From UE - V%VERSION%

"%~dp0Python27\python.exe" "%~dp0main.py"

if exist "%~dp0logs" (
    start /max ""  "%~dp0logs"
)

taskkill /FI "IMAGENAME eq adb.exe" /T /F  1>nul 2>nul

exit