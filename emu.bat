@echo off
IF "%~1"=="" GOTO end
qemu-system-i386 -d guest_errors -drive format=raw,file=%~1
:end