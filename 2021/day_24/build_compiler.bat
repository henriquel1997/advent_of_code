@echo off
if exist *.obj del /q /f *.obj
if exist *.pdb del /q /f *.pdb
if exist *.ilk del /q /f *.ilk
if exist *.exe del /q /f *.exe

if exist alu_to_c_compiler.cpp (
echo Building Compiler
call clang -Wno-deprecated-declarations alu_to_c_compiler.cpp -O0 -g -o compiler.exe --include-directory ../.
)

echo Running Compiler
compiler.exe

if exist part_one.cpp (
echo Building Part One
call clang -Wno-deprecated-declarations part_one.cpp -O3 -g -o part_one.exe --include-directory ../.
)

REM if exist part_two.cpp (
REM echo Building Part Two
REM call clang -Wno-deprecated-declarations part_two.cpp -O3 -g -o part_two.exe --include-directory ../.
REM )