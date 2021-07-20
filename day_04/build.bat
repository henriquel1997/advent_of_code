@echo off
echo Building Part One
call clang part_one.cpp -O0 -g -gcodeview -gno-column-info -o part_one.exe

echo Building Part Two
call clang part_two.cpp -O0 -g -gcodeview -gno-column-info -o part_two.exe