@echo off
if exist *.obj del /q /f *.obj
if exist *.pdb del /q /f *.pdb
if exist *.ilk del /q /f *.ilk
if exist *.exe del /q /f *.exe

if exist part_one.cpp (
echo Building Part One
call clang -Wno-deprecated-declarations part_one.cpp -O0 -g -o part_one.exe --include-directory ../.
)

if exist part_two.cpp (
echo Building Part Two
call clang -Wno-deprecated-declarations part_two.cpp -O0 -g -o part_two.exe --include-directory ../.
)