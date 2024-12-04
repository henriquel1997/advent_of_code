package day_03

import "core:fmt"
import "core:os"
import "core:strings"
import "core:unicode"
import "core:strconv"

main :: proc() {
    filename :: "input.txt"

	commands, ok := os.read_entire_file(filename)

    if !ok {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}

    total_sum := 0
    enabled_sum := 0

    mul_enabled := true
    
    initial_index := strings.starts_with(string(commands), "mul(") ? 0 : 1

    for str in strings.split(string(commands), "mul(")[initial_index : ] {
        defer {
            check_do_and_dont(str, &mul_enabled)
        }

        inputs, err := strings.split_n(str, ",", 2)

        if err != nil do continue
        if len(inputs) != 2 do continue

        first_digits := count_digits(inputs[0])
        if(first_digits < 1 || first_digits > 3) do continue

        first_number, ok_first := strconv.parse_int(inputs[0])
        if !ok_first do continue

        second_digits := count_digits(inputs[1])
        if(second_digits < 1 || second_digits > 3) do continue

        second_number, ok_second := strconv.parse_int(inputs[1][:second_digits])
        if !ok_second do continue

        if !strings.starts_with(inputs[1][second_digits : ], ")") {
            continue
        }

        result := first_number * second_number
        fmt.printf("mul(%v,%v) = %v", first_number, second_number, result)

        total_sum += result
        if(mul_enabled) {
            enabled_sum += result
        } else {
            fmt.print(" (SKIPPED)")
        }

        fmt.println()
    }

    fmt.println()
    fmt.printfln("enabled sum: %v", enabled_sum)
    fmt.printfln("total sum: %v", total_sum)
}

count_digits :: proc(str : string) -> int {
    count := 0
    for char in str {
        if unicode.is_digit(char) {
            count += 1
        } else {
            break
        }
    }
    return count
}

check_do_and_dont :: proc(str : string, mul_enabled : ^bool)  {
    for i in 0..<len(str) {
        if strings.starts_with(str[i:], "do()") {
            mul_enabled^ = true
        } else if strings.starts_with(str[i:], "don't()") {
            mul_enabled^ = false
        }
    }
}