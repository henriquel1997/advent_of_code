package day_08

import "core:os"
import "core:fmt"
import "core:strings"

Vector :: [2]int
AntennaMap :: map[rune][dynamic]Vector

main :: proc() {
    filename :: "input.txt"

	input_bytes, ok := os.read_entire_file(filename)

    if !ok {
		fmt.println("Unable to open file: ", filename)
		return
	}

    antenna_map, bounds := parse_antennas(string(input_bytes))

    fmt.println("Unique Antinodes:", count_antinodes(antenna_map, bounds))
    fmt.println("Unique Antinodes With Harmonics:", count_antinodes(antenna_map, bounds, enable_harmonics = true))
}

count_antinodes :: proc(antenna_map: AntennaMap, bounds: Vector, enable_harmonics := false) -> int {
    antinodes_set : map[Vector]struct{}
    defer delete(antinodes_set)

    is_inside_bounds :: proc(pos: Vector, bounds: Vector) -> bool {
        return pos.x >= 0 && pos.x < bounds.x && pos.y >= 0 && pos.y < bounds.y
    }

    for _, positions in antenna_map {
        for p0, index in positions {
            for p1 in positions[(index + 1):] {
                distance := p0 - p1

                for antinode := p0 + distance; is_inside_bounds(antinode, bounds); antinode += distance {
                    antinodes_set[antinode] = {}

                    if !enable_harmonics do break
                }
                
                for antinode := p1 - distance; is_inside_bounds(antinode, bounds); antinode -= distance {
                    antinodes_set[antinode] = {}

                    if !enable_harmonics do break
                }

                if enable_harmonics {
                    antinodes_set[p0] = {}
                    antinodes_set[p1] = {}
                }
            }
        }
    }

    return len(antinodes_set)
}

@require_results
parse_antennas :: proc(input: string) -> (antenna_map: AntennaMap, bounds: Vector) {
    input := input

    y := 0
    for line in strings.split_lines_iterator(&input) {
        defer y += 1

        for frequency, x in line {
            if frequency == '.' do continue

            positions, exists := antenna_map[frequency]

            if !exists do positions = make([dynamic]Vector)

            append(&positions, Vector{ x, y })
            antenna_map[frequency] = positions
        }

        bounds.x = max(bounds.x, len(line))
    }

    bounds.y = y

    return
}