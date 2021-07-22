@echo off
echo Building Part One
call clang -Wno-deprecated-declarations part_one.cpp -O0 -g -o part_one.exe

echo Building Part Two
call clang -Wno-deprecated-declarations part_two.cpp -O0 -g -o part_two.exe