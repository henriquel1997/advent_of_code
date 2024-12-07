package day_06

import "core:os"
import "core:fmt"
import "core:strings"
import "core:slice"

Vector :: [2]int

main :: proc() {
    filename :: "input.txt"

	input_bytes, ok := os.read_entire_file(filename)

    if !ok {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}

    obstacles : [dynamic]Vector
    guard_position : Vector

    input := string(input_bytes)

    y: = 0
    for line in strings.split_lines_iterator(&input) {
        for char, x in line {
            switch char {
                case '#': append(&obstacles, Vector{ x, y })
                case '^': guard_position = { x, y }
            }
        }
        y += 1
    }

    grid_size := y

    walked_positions, initial_loop := simulate_guard(guard_position, obstacles, grid_size)

    fmt.printfln("walked count: %v", len(walked_positions))
    fmt.printfln("initial loop: %v", initial_loop)

    fmt.println("\ntesting new obstacles for loops (slow)")
    loop_count := 0

    for new_obstacle in walked_positions {

        if guard_position == new_obstacle do continue
        
        append(&obstacles, new_obstacle)
        
        _, loop := simulate_guard(guard_position, obstacles, grid_size, context.temp_allocator)
        free_all(context.temp_allocator)

        if loop {
            loop_count += 1
        }

        pop_safe(&obstacles)
    }

    fmt.printfln("loop count: %v", loop_count)
}

simulate_guard :: proc(guard_position: Vector, obstacles: [dynamic]Vector, grid_size: int, allocator := context.allocator) -> (walked_positions : map[Vector]([dynamic]Vector), loop: bool = false) {
    
    walked_positions = make(map[Vector]([dynamic]Vector), allocator)

    guard_position := guard_position
    guard_direction : Vector = { 0, -1 }

    for {
        if guard_position.x < 0 || guard_position.x >= grid_size || guard_position.y < 0 || guard_position.y >= grid_size {
            break
        }

        directions, exists := walked_positions[guard_position]

        if !exists {
            directions = make([dynamic]Vector, allocator)
        } else {
            if slice.contains(directions[:], guard_direction) {
                loop = true
                return
            }
        }

        append(&directions, guard_direction)
        walked_positions[guard_position] = directions

        next_direction := guard_direction
        for slice.contains(obstacles[:], guard_position + next_direction) {
            next_direction = { -next_direction.y, next_direction.x }
            if next_direction == guard_direction {
                loop = true
                return
            }
        }

        guard_direction = next_direction
        guard_position += guard_direction
    }
    
    return
}