@echo off
WHERE cl >nul 2>nul
IF NOT %ERRORLEVEL% == 0 call vcvarsall x64

if not exist build mkdir build
cd build
del *.exe > NUL 2> NUL
del *.pdb > NUL 2> NUL
del *.ilk > NUL 2> NUL
del *.png > NUL 2> NUL
cd ..

copy .\bin\raylib.dll .\build > NUL 2> NUL

copy .\spritesheet.png .\build > NUL 2> NUL

set ExecutableName=build\calc

cl /nologo /Fe%ExecutableName%  /Iinclude /I..\. /Z7 raylibdll.lib alu_calculator.cpp /link /LIBPATH:"bin"

del *.obj > NUL 2> NUL

if exist "%ExecutableName%.exe" %ExecutableName%.exe