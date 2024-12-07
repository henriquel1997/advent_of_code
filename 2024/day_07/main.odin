package day_07

import "core:os"
import "core:fmt"
import "core:strings"
import "core:strconv"
import sa "core:container/small_array"

SmallArray :: sa.Small_Array

Equation :: struct {
    result: int,
    values: SmallArray(20, int)
}

main :: proc() {
    filename :: "input.txt"

	input_bytes, ok := os.read_entire_file(filename)

    if !ok {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}

    equations := parse_equations(string(input_bytes))

    first_sum := 0
    for &equation in equations {
        if is_valid(equation) {
            first_sum += equation.result
        }
    }

    fmt.printfln("first sum: %v", first_sum)

    second_sum := 0
    for &equation in equations {
        if is_valid(equation, enable_concat = true) {
            second_sum += equation.result
        }
    }

    fmt.printfln("second sum: %v", second_sum)
}

is_valid :: proc(equation: Equation, previous_result: int = 0, index: int = 0, enable_concat: bool = false) -> bool {
    if index >= equation.values.len {
        return previous_result == equation.result
    }

    value := sa.get(equation.values, index)
    
    if is_valid(equation, previous_result + value, index + 1, enable_concat) {
        return true
    } else if index > 0 {
        if is_valid(equation, previous_result * value, index + 1, enable_concat) {
            return true
        } else if enable_concat {
            pow := 10
            for value >= pow do pow *= 10
            return is_valid(equation, (previous_result * pow) + value, index + 1, enable_concat)
        }
    }

    return false
}

@require_results
parse_equations :: proc(input: string) -> (equations: [dynamic]Equation) {
    input := input

    for line in strings.split_lines_iterator(&input) {
        equation : Equation

        res, err_res := strings.split_n(line, ":", 2)
        
        if err_res != nil || len(res) != 2 {
            fmt.printfln("Unable to split line : %v", line)
		    return
        }

        result, ok_result := strconv.parse_int(strings.trim_space(res[0]))
        if !ok_result {
            fmt.printfln("Unable to parse result from line : %v", line)
		    return
        }

        equation.result = result

        values, err_values := strings.split(strings.trim_space(res[1]), " ")

        if err_values != nil {
            fmt.printfln("Unable to split values from line : %v", line)
		    return
        }

        assert(len(values) <= len(equation.values.data))

        for value_str, index in values {
            value, ok := strconv.parse_int(strings.trim_space(value_str))
            if !ok {
                fmt.printfln("Unable to parse value %v from line : %v", index, line)
		        return
            }

            sa.append(&equation.values, value)
        }

        append(&equations, equation)
    }

    return
}