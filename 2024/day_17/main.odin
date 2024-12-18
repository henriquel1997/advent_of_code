package day_17

import "core:os"
import "core:fmt"
import "core:strings"
import "core:reflect"
import "core:strconv"
import pq "core:container/priority_queue"

RegisterName :: enum {
    A, B, C
}

Opcode :: enum {
    adv = 0,
    bxl = 1,
    bst = 2,
    jnz = 3,
    bxc = 4,
    out = 5,
    bdv = 6,
    cdv = 7,
}

integer :: uint

Instruction :: struct {
    opcode: Opcode,
    operand: integer
}

ComputerState :: struct {
    registers: [RegisterName]integer,
    program: [dynamic]Instruction,
    instruction_pointer: integer,
}

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file", filename)
        return
    }

    state, valid := parse_computer(string(input_bytes))
    if !valid do return

    fmt.println("Register A", state.registers[.A])
    fmt.println("Register B", state.registers[.B])
    fmt.println("Register C", state.registers[.C])
    fmt.println("Program", state.program)
    fmt.println()

    fmt.print("Part 1 Solution: ")
    execute_program(state)

    if a, found := search(state); found {
        fmt.println("Part 2 Solution:", a)

        state.registers[.A] = a
        execute_program(state)
    } else {
        fmt.println("Part 2 not found")
    }
}

search :: proc(state: ComputerState) -> (a_register : integer = 9999999999999999999, found : bool = false) {
    
    PossibleValue :: struct {
        a_register: integer,
        index: integer,
        matches: integer,
    }

    compare :: proc(a, b: PossibleValue) -> bool {
        return a.matches > b.matches
    }

    queue: pq.Priority_Queue(PossibleValue)
    pq.init(&queue, compare, pq.default_swap_proc(PossibleValue))
    defer pq.destroy(&queue)

    pq.push(&queue, PossibleValue {a_register = 0, index = 1})

    max_matches : integer = 0

    program_len := integer(len(state.program)) * 2

    seen: map[integer]struct{}
    defer delete(seen)
    
    for pq.len(queue) > 0 {
        current := pq.pop(&queue)

        if max_matches == program_len {
            if current.a_register >= a_register {
                continue
            }
        }

        outer: for x in 0..<512 {
            state := state

            index := current.index

            reg := current.a_register

            mask : integer = (1 << ((index - 1) * 3)) - 1
            reg &= mask

            reg += integer(x) << ((index - 1) * 3)

            if reg in seen do continue
            seen[reg] = {}

            if max_matches == program_len && reg > a_register do break

            state.registers[.A] = reg

            current_index: integer = 0
            for out in execute_program_until_output(&state) {
                instruction := state.program[current_index / 2]
                if current_index % 2 == 0 {
                    if instruction.opcode != Opcode(out) {
                        if current_index < index {
                            continue outer
                        } else {
                            break
                        }
                    }
                } else {
                    if instruction.operand != out {
                        if current_index < index {
                            continue outer
                        } else {
                            break
                        }
                    }
                }

                current_index += 1

                if current_index >= program_len {
                    break
                } 
            }

            if current_index == program_len {
                if _, executing := execute_program_until_output(&state); !executing {
                    if reg < a_register {
                        a_register = reg
                        found = true
                        break outer
                    }
                } else {
                    continue
                }
            }

            bitpos : integer = 0
            r := reg
            for r != 0 {
                r >>= 1
                bitpos += 1
            }

            next_index := max(bitpos / 3, index) + 1
            next_index = min(next_index, integer(len(state.program)) * 2)

            pq.push(&queue, PossibleValue {a_register = reg, index = next_index, matches = current_index})
        }
    }

    return
}

execute_program :: proc(state: ComputerState) {
    state := state

    first, executing := execute_program_until_output(&state)
    if !executing do return
    fmt.print(first)

    for output in execute_program_until_output(&state) {
        fmt.print(",", output, sep = "")
    }
    fmt.println()
}

execute_program_until_output :: proc(state: ^ComputerState) -> (output: integer, executing: bool = true) {

    get_combo_operand :: proc(state: ComputerState, operand: integer) -> (result: integer, valid: bool = true) {
        switch operand {
            case 0..=3: result = operand
            case 4..=6: result = state.registers[RegisterName(operand - 4)]
            case: valid = false
        }
        return
    }

    divide :: proc(state: ComputerState, instruction: Instruction) -> (result: integer, ok: bool = true) {
        x := get_combo_operand(state, instruction.operand) or_return
        result = state.registers[.A] >> x
        return
    }

    state := state

    for state.instruction_pointer < integer(len(state.program)) {
        instruction := state.program[state.instruction_pointer]

        switch instruction.opcode {
            case .adv: {
                state.registers[.A] = divide(state^, instruction) or_return
            }
            case .bxl: {
                state.registers[.B] ~= instruction.operand
            }
            case .bst: {
                combo := get_combo_operand(state^, instruction.operand) or_return
                state.registers[.B] = combo % 8
            }
            case .jnz: {
                if state.registers[.A] != 0 {
                    state.instruction_pointer = instruction.operand / 2
                    continue
                }
            }
            case .bxc: {
                state.registers[.B] ~= state.registers[.C]
            }
            case .out: {
                combo := get_combo_operand(state^, instruction.operand) or_return
                output = combo % 8

                state.instruction_pointer += 1
                return
            }
            case .bdv: {
                state.registers[.B] = divide(state^, instruction) or_return
            }
            case .cdv: {
                state.registers[.C] = divide(state^, instruction) or_return
            }
        }

        state.instruction_pointer += 1
    }

    executing = false
    return
}

parse_computer :: proc(input: string) -> (state: ComputerState, ok: bool = false) {
    consume_token :: proc(input: string, token: string) -> (output: string, ok: bool = false) {
        if strings.starts_with(input, token) {
            output = strings.trim_left_space(input[len(token) : ])
            ok = true
        }
        return
    }

    read_register :: proc(input: string, state: ^ComputerState) -> (output: string, valid: bool = false) {

        output = consume_token(input, "Register") or_return

        separator_index := strings.index_rune(output, ':')
        if separator_index < 0 do return

        register := reflect.enum_from_name(RegisterName, strings.trim_left_space(output[:separator_index])) or_return

        output = strings.trim_left_space(output[(separator_index + 1):])

        space_index := strings.index_proc(output, strings.is_space)
        if space_index < 0 do space_index = len(output)

        state.registers[register] = strconv.parse_uint(output[:space_index]) or_return
        
        output = strings.trim_left_space(output[space_index:])

        valid = true
        return
    }

    output := read_register(input, &state) or_return
    output = read_register(output, &state) or_return
    output = read_register(output, &state) or_return

    output = consume_token(output, "Program:") or_return

    output = strings.trim_space(output)

    for op_code in strings.split_iterator(&output, sep = ",") {
        instruction: Instruction

        opcode_value := strconv.parse_int(op_code) or_return
        if opcode_value < 0 || opcode_value >= 8 do return
        if opcode_value >= len(Opcode) do return

        instruction.opcode = Opcode(opcode_value)

        operand := strings.split_iterator(&output, sep = ",") or_return
        instruction.operand = strconv.parse_uint(operand) or_return
        if instruction.operand < 0 || instruction.operand >= 8 do return

        append(&state.program, instruction)
    }

    ok = true
    return
}