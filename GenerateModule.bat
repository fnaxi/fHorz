@echo off

setlocal
powershell -ExecutionPolicy Bypass -File "%~dp5GenerateModule.ps1" %*
endlocal

pause