@echo off


if exist part_one.cpp (
echo Building Part One
call clang -Wno-deprecated-declarations part_one.cpp -O0 -g -o part_one.exe
)

if exist part_two.cpp (
echo Building Part Two
call clang -Wno-deprecated-declarations part_two.cpp -O0 -g -o part_two.exe
)