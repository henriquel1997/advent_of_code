package day_19

import "core:os"
import "core:fmt"
import "core:strings"

Towels :: struct {
    max_size: int,
    set: map[string]struct{},
}

main :: proc() {
    filename :: "input.txt"
    
    input_bytes, ok := os.read_entire_file_from_filename(filename)
    if !ok {
        fmt.println("Unable to read file", filename)
        return
    }

    towels, patterns := parse_data(string(input_bytes))

    possible_patterns := 0
    total_combinations := 0

    cache : map[string]int

    for pattern in patterns {
        combinations := combinations(pattern, towels,&cache)

        if combinations > 0 {
            possible_patterns += 1
            total_combinations += combinations
        }
    }

    fmt.println("(Part 1) - Possible patterns:", possible_patterns)
    fmt.println("(Part 2) - Total combinations:", total_combinations)
}

parse_data :: proc(input: string) -> (towels: Towels, patterns: [dynamic]string) {
    input := input
    
    towels_string, ok := strings.split_lines_iterator(&input)
    if !ok do return

    for t in strings.split_iterator(&towels_string, ",") {
        towel := strings.trim_space(t)
        towels.set[towel] = {}
        towels.max_size = max(towels.max_size, len(towel))
    }

    input = strings.trim_space(input)

    for pattern in strings.split_lines_iterator(&input) {
        append(&patterns, strings.trim_space(pattern))
    }
    return
}

combinations :: proc(pattern: string, towels: Towels, cache : ^map[string]int) -> (count: int = 0) {
    size := len(pattern)
    if size <= 0 do return 0

    if cached_result, exists := cache[pattern]; exists {
        return cached_result
    }

    for towel_size := min(size, towels.max_size); towel_size > 0; towel_size -= 1 {
        towel := pattern[:towel_size]
        if towel in towels.set {
            remaining := pattern[len(towel):]
            if len(remaining) == 0 {
                count += 1
                if size == 1 do break
            } else {
                count += combinations(remaining, towels, cache)
            }
        }
    }
    
    cache[pattern] = count
    return
}