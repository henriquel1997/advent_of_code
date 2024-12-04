package day_04

import "core:fmt"
import "core:os"
import "core:strings"
import "core:unicode/utf8"

main :: proc() {
    filename :: "input.txt"

	input, ok := os.read_entire_file(filename)

    if !ok {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}

    lines, err := strings.split_lines(string(input))

    if err != nil {
        fmt.printfln("Unable to split lines: %v", filename)
		return
    }

    grid : [dynamic][]rune

    for line in lines {
        if len(line) == 0 do continue

        assert(len(line) == (len(lines) - 1), "grid is not square")

        runes : [dynamic]rune

        for char in line {
            append(&runes, char)
        }

        append(&grid, runes[:])
    }

    xmas_count := search_xmas(grid)
    fmt.printfln("xmas count: %v", xmas_count)

    x_count := search_x(grid)
    fmt.printfln("x-mas count: %v", x_count)
}

search_x :: proc(grid : [dynamic][]rune) -> (count: int) {
    size := len(grid)

    for y in 1..<(size -1) {
        for x in 1..<(size -1) {
            if grid[x][y] == 'A' {
                if is_x_at(grid, x, y, 'M', 'S') || is_x_at(grid, x, y, 'S', 'M') {
                    count += 1
                }
            }
        }
    }

    return
}

is_x_at :: proc(grid: [dynamic][]rune, x: int, y: int, first: rune, second: rune) -> bool {
    at_top := grid[x - 1][y + 1] == first && grid[x + 1][y + 1] == second
    at_bottom := grid[x - 1][y - 1] == first && grid[x + 1][y - 1] == second
    
    at_left := grid[x - 1][y + 1] == first && grid[x - 1][y - 1] == second
    at_right := grid[x + 1][y + 1] == first && grid[x + 1][y - 1] == second

    return (at_top && at_bottom) || (at_left && at_right)
}

search_xmas :: proc(grid : [dynamic][]rune) -> (count: int) {
    count += search_word(grid[:])

    diagonal := rotate_45_degrees(grid[:])
    count += search_word(diagonal[:])
    defer delete(diagonal)

    rotated := rotate_90_degrees(grid[:])
    count += search_word(rotated[:])
    defer delete(rotated)

    diagonal = rotate_45_degrees(rotated[:])
    count += search_word(diagonal[:])

    return
}

search_word :: proc(grid : [][]rune) -> (count: int) {
    for line in grid {
        line_str := utf8.runes_to_string(line)
        defer delete(line_str)

        count += strings.count(line_str, "XMAS")
        count += strings.count(line_str, "SAMX")
    }
    return
}

rotate_90_degrees :: proc(grid : [][]rune) -> (res : [dynamic][]rune) {
    size := len(grid)

    for i in 0..<size {
        new_line : [dynamic]rune

        #reverse for line in grid {
            assert(len(line) == size, "grid is not square")
            append(&new_line, line[i])
        }

        append(&res, new_line[:])
    }

    return
}

rotate_45_degrees :: proc(grid : [][]rune) -> (res : [dynamic][]rune) {
    size := len(grid)

    for i in 0..<size {
        new_line : [dynamic]rune

        for j in 0..=i {
            append(&new_line, grid[i - j][j])
        }

        append(&res, new_line[:])
    }

    for i in 1..<size {
        new_line : [dynamic]rune

        x := size - 1
        for y in i..<size {
            append(&new_line, grid[x][y])
            x -= 1
        }

        append(&res, new_line[:])
    }

    return
}

print_grid : :proc(grid : [][]rune) {
    size := len(grid)

    for line in grid {
        padding := (size - len(line)) / 2

        for i in 0..<padding {
            fmt.print(" ")
        }

        for char in line {
            fmt.print(char)
        }

        for i in 0..<padding {
            fmt.print(" ")
        }

        fmt.println()
    }
}