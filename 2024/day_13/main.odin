package day_13

import "core:os"
import "core:fmt"
import "core:strings"
import "core:strconv"

Vector :: [2]int

MachineData :: struct {
    a: Vector,
    b: Vector,
    prize: Vector,
}

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file:", filename)
        return
    }

    data := parse_data(string(input_bytes))

    total_tokens := 0
    for machine, index in data {
        if tokens, possible := test_machine(machine); possible {
            total_tokens += tokens
        }
    }

    fmt.println("(Part 1) Total tokens:", total_tokens)

    total_tokens = 0
    for machine, index in data {
        fixed_machine := machine
        fixed_machine.prize += {10000000000000, 10000000000000}
        if tokens, possible := test_machine(fixed_machine); possible {
            total_tokens += tokens
        }
    }

    fmt.println("(Part 2) Total tokens:", total_tokens)
}

test_machine :: proc(data: MachineData) -> (minimum_tokens: int, possible: bool = false) {
    // prize.x = a_presses * a.x + b_presses * b.x
    // prize.y = a_presses * a.y + b_presses * b.y

    // prize.x = a_presses * a.x + b_presses * b.x ( * b.y)
    // prize.y = a_presses * a.y + b_presses * b.y ( * b.x)

    // prize.x * b.y = a_presses * a.x * b.y + b_presses * b.x * b.y
    // prize.y * b.x = a_presses * a.y * b.x + b_presses * b.y * b.x

    // subtract both equations

    // prize.x * b.y - prize.y * b.x = a_presses * a.x * b.y - a_presses * a.y * b.x
    // prize.x * b.y - prize.y * b.x = a_presses (a.x * b.y - a.y * b.x)

    // a_presses = (prize.x * b.y - prize.y * b.x) / (a.x * b.y - a.y * b.x)

    a_presses := (data.prize.x * data.b.y - data.prize.y * data.b.x) / (data.a.x * data.b.y - data.a.y * data.b.x)

    remaining := data.prize - (data.a * a_presses)
    if remaining % data.b != {0, 0} do return

    b_presses := remaining / data.b
    if b_presses.x != b_presses.y do return

    return a_presses * 3 + b_presses.x, true
}

parse_data :: proc(input: string) -> (result: [dynamic]MachineData) {

    consume_token :: proc(input: string, token: string) -> (output: string, ok: bool = false) {
        if strings.starts_with(input, token) {
            output = strings.trim_left_space(input[len(token) : ])
            ok = true
        }
        return
    }

    read_vector :: proc(input: string, x_token: string, y_token: string) -> (output: string, result: Vector, valid: bool = false) {
        ok: bool

        output, ok = consume_token(input, x_token)
        if !ok do return

        comma_index := strings.index_rune(output, ',')
        if comma_index < 0 do return

        result.x, ok = strconv.parse_int(output[:comma_index])
        if !ok do return
        output = strings.trim_left_space(output[(comma_index + 1):])

        output, ok = consume_token(output, y_token)
        if !ok do return

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
        data: MachineData

        input, ok = consume_token(input, "Button A:")
        if !ok do break

        input, data.a, ok = read_vector(input, "X+", "Y+")
        if !ok do break

        input, ok = consume_token(input, "Button B:")
        if !ok do break

        input, data.b, ok = read_vector(input, "X+", "Y+")
        if !ok do break

        input, ok = consume_token(input, "Prize:")
        if !ok do break

        input, data.prize, ok = read_vector(input, "X=", "Y=")
        if !ok do break

        append(&result, data)
    }

    return
}