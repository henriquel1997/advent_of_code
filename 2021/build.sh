if [ -e part_one.cpp ]
then
    printf "Building Part One\n"
    clang++ -Wno-deprecated-declarations part_one.cpp -O0 -g -o part_one.exe --include-directory ../.
fi

if [ -e part_two.cpp ]
then
    printf "Building Part Two\n"
    clang++ -Wno-deprecated-declarations part_two.cpp -O0 -g -o part_two.exe --include-directory ../.
fi