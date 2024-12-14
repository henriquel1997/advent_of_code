package day_14

import "core:os"
import "core:fmt"
import "core:strings"
import "core:strconv"
import "core:slice"

import rl "vendor:raylib"
import "core:c"

Vector :: [2]int

Robot :: struct {
    position: Vector,
    velocity: Vector,
}

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file", filename)
        return
    }

    robots := parse_robots(string(input_bytes))
    
    area_size : Vector = {101, 103}

    first_part(robots[:], area_size)
    
    rl.InitWindow(
        c.int(area_size.x * 10), 
        c.int(area_size.y * 10), 
        "Advent of Code 2024 Day 14: Restroom Redoubt",
    )

    seconds := 0
    paused := false
    last_update : f64 = 0

    most_adjacent := 0
    most_adjacent_time := 0

    for !rl.WindowShouldClose() {
        if rl.IsKeyPressed(.SPACE) do paused = !paused

        forwards_button := rl.IsKeyDown(.RIGHT)
        backwards_button := rl.IsKeyDown(.LEFT)
        time_input := forwards_button || backwards_button
        
        adjacent_button := rl.IsKeyPressed(.A)

        time := rl.GetTime()
        update_time_elapsed := (time - last_update) >= 1.0

        if (!paused && update_time_elapsed) || time_input || adjacent_button {
            if adjacent_button {
                for seconds != most_adjacent_time {
                    backwards := seconds > most_adjacent_time
                    simulate_robots(robots[:], area_size, backwards)
                    seconds += backwards ? - 1 : +1
                }
                paused = true
            } else {
                simulate_robots(robots[:], area_size, backwards_button)
                if backwards_button {
                    seconds -= 1
                } else {
                    seconds += 1
                }
                last_update = time

                adjacent_count := 0
                outer: for robot, index in robots {
                    for other in robots[(index + 1):] {
                        diff := robot.position - other.position
                        diff.x = abs(diff.x)
                        diff.y = abs(diff.y)

                        if diff == {1, 0} || diff == {0, 1} {
                            adjacent_count += 1
                            continue outer
                        }
                    }
                }

                if adjacent_count > most_adjacent {
                    most_adjacent = adjacent_count
                    most_adjacent_time = seconds
                }
            }
        }

        rl.BeginDrawing()

        rl.ClearBackground(rl.WHITE)

        color : = rl.GRAY
        color.a = 127

        for robot in robots {
            rl.DrawRectangle(
                c.int(robot.position.x * 10), c.int(robot.position.y * 10), 
                10, 10, 
                color,
            )
        }

        text : string 

        text = fmt.aprint("Seconds:", seconds, "/ Most Adjacent:", most_adjacent, "(", most_adjacent_time, ")", allocator = context.temp_allocator)
        if paused {
            text = fmt.aprint("(Paused)", text, allocator = context.temp_allocator)
        }

        text_c, err := strings.clone_to_cstring(text, allocator = context.temp_allocator)
        if err != nil do text_c = ""

        font_size : c.int = 20
        text_width := rl.MeasureText(text_c, font_size)

        rl.DrawRectangle(0, 0, text_width, font_size, rl.BLACK)
        rl.DrawText(text_c, 0, 0, font_size, rl.RED)

        rl.EndDrawing()

        free_all(context.temp_allocator)
    }

    rl.CloseWindow()
}

first_part :: proc(robots: []Robot, area_size: Vector){
    robots_clone := slice.clone_to_dynamic(robots[:])
    defer delete(robots_clone)

    for _ in 0..<100 {
        simulate_robots(robots_clone[:], area_size)
    }
    fmt.println("Safety Factor (100 seconds):", calculate_safety_factor(robots_clone[:], area_size))
}

simulate_robots :: proc(robots: []Robot, area_size: Vector, backwards: bool = false) {
    for &robot in robots {
        if !backwards {
            robot.position += robot.velocity
        } else {
            robot.position -= robot.velocity
        }

        if robot.position.x < 0 {
            robot.position.x = area_size.x + robot.position.x 
        }
        if robot.position.y < 0 {
            robot.position.y = area_size.y + robot.position.y 
        }

        robot.position %= area_size
    }
}

calculate_safety_factor :: proc(robots: []Robot, area_size: Vector) -> int {
    quadrant_count: [4]int

    quadrant_size := area_size / 2
    area_middle_size := area_size % 2

    quadrant_origins : [4]Vector = {
        {0, 0},
        {quadrant_size.x + area_middle_size.x, 0},
        {0, quadrant_size.y + area_middle_size.y},
        quadrant_size + area_middle_size
    }

    quadrant_ends : [4]Vector = {
        quadrant_origins[0] + quadrant_size,
        quadrant_origins[1] + quadrant_size,
        quadrant_origins[2] + quadrant_size,
        quadrant_origins[3] + quadrant_size
    }

    for robot in robots {
        for quadrant_origin, index in quadrant_origins {
            if robot.position.x >= quadrant_origin.x && robot.position.y >= quadrant_origin.y {
                quadrant_end := quadrant_ends[index]
                if robot.position.x < quadrant_end.x && robot.position.y < quadrant_end.y {
                    quadrant_count[index] += 1
                }
            }
        }
    }

    result := 1
    for count, index in quadrant_count {
        result *= count
    }
    return result
}

parse_robots :: proc(input: string) -> (result: [dynamic]Robot) {

    consume_token :: proc(input: string, token: string) -> (output: string, ok: bool = false) {
        if strings.starts_with(input, token) {
            output = strings.trim_left_space(input[len(token) : ])
            ok = true
        }
        return
    }

    read_vector :: proc(input: string) -> (output: string, result: Vector, valid: bool = false) {
        ok: bool

        comma_index := strings.index_rune(input, ',')
        if comma_index < 0 do return

        result.x, ok = strconv.parse_int(input[:comma_index])
        if !ok do return
        output = strings.trim_left_space(input[(comma_index + 1):])

        space_index := strings.index_proc(output, strings.is_space)
        if space_index < 0 do space_index = len(output)

        result.y, ok = strconv.parse_int(output[:space_index])
        if !ok do return

        output = space_index < len(output) ? strings.trim_left_space(output[(space_index + 1):]) : ""

        valid = true
        return
    }

    input := input
    ok: bool

    for  {
        robot: Robot

        input, ok = consume_token(input, "p=")
        if !ok do break

        input, robot.position, ok = read_vector(input)
        if !ok do break

        input, ok = consume_token(input, "v=")
        if !ok do break

        input, robot.velocity, ok = read_vector(input)
        if !ok do break

        append(&result, robot)
    }

    return
}