package day_10

import "core:os"
import "core:fmt"
import "core:strings"
import "core:odin/tokenizer"

Vector :: [2]int

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file: ", filename)
        return
    }

    trail_map := parse_trail_map(string(input_bytes))

    total_score := 0
    total_rating := 0
    for y in 0..<len(trail_map) {
        for x in 0..<len(trail_map[y]) {
            if trail_map[y][x] == 0 {
                top_map: map[Vector]int
                defer delete(top_map)

                find_paths({x, y}, trail_map, &top_map)

                score := len(top_map)
                total_score += score

                rating := 0
                for _, value in top_map {
                    rating += value
                }
                total_rating += rating

                fmt.printfln("(%v, %v) - Score: %v - Rating: %v", x, y, score, rating)
            }
        }
    }

    fmt.println("Total Score:", total_score)
    fmt.println("Total Rating:", total_rating)
}

find_paths :: proc(position: Vector, trail_map: [dynamic][dynamic]int, top_map: ^map[Vector]int) {
    height := trail_map[position.y][position.x]

    if height == 9 {
        top_map[position] += 1
        return
    }

    next := height + 1

    if position.y > 0 && trail_map[position.y - 1][position.x] == next {
        find_paths(position - {0, 1}, trail_map, top_map)
    }

    if position.y < (len(trail_map) - 1) && trail_map[position.y + 1][position.x] == next {
        find_paths(position + {0, 1}, trail_map, top_map)
    }

    if position.x > 0 && trail_map[position.y][position.x - 1] == next {
        find_paths(position - {1, 0}, trail_map, top_map)
    }

    if position.x < (len(trail_map[position.y]) - 1) && trail_map[position.y][position.x + 1] == next {
        find_paths(position + {1, 0}, trail_map, top_map)
    }

    return
}

parse_trail_map :: proc(input: string) -> (trail_map: [dynamic][dynamic]int) {
    input := input

    for line in strings.split_lines_iterator(&input) {
        line_array: [dynamic]int

        for char in line {
            append(&line_array, tokenizer.digit_val(char))
        }

        append(&trail_map, line_array)
    }

    return
}