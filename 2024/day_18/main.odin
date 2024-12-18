package day_18

import "core:os"
import "core:fmt"
import "core:encoding/csv"
import "core:strconv"
import pq "core:container/priority_queue"
import "core:slice"

Vector :: [2]int

MemoryMap :: struct {
    size: Vector,
    falling_bytes: [dynamic]Vector
}

Walls :: map[Vector]struct{}

main :: proc() {
    reader: csv.Reader
    reader.comma = ','
    reader.fields_per_record   = -1
	reader.trim_leading_space  = true
	reader.reuse_record        = true
	reader.reuse_record_buffer = true
	defer csv.reader_destroy(&reader)
    
    filename :: "input.txt"

	data, ok := os.read_entire_file(filename)
	if !ok {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}

    memory_map := parse_map(string(data))
    memory_map.size = {70, 70}

    walls: Walls
    for pos, index in memory_map.falling_bytes {
        if index >= 1024 do break
        walls[pos] = {}
    }

    steps, found := find_exit(walls, memory_map.size)

    if !found {
        fmt.printfln("Unable to find a solution for part 1")
		return
    }

    fmt.println("(Part 1) - Minimum number of steps:", steps)

    for pos in memory_map.falling_bytes[1024:] {
        walls[pos] = {}

        if _, found := find_exit(walls, memory_map.size); !found {
            fmt.printfln("(Part 2) - First blocking byte: %v,%v", pos.x, pos.y)
            return
        }
    }

    fmt.println("(Part 2) - No blocking byte found")
}

find_exit :: proc(walls: Walls, size: Vector, allocator := context.allocator) -> (steps: int, found: bool = false) {
    Move :: struct {
        position: Vector,
    }

    State :: struct {
        using move: Move,
        cost: int,
        heuristic: int,
    }

    heuristic :: proc(a: State, goal: Vector) -> int {
        return abs(a.position.x - goal.x) + abs(a.position.y - goal.y)
    }

    goal := size

    initial_state : State = {
        position = {0, 0},
        cost = 0,
    }
    initial_state.heuristic = heuristic(initial_state, goal)

    compare :: proc(a, b: State) -> bool {
        return (a.cost + a.heuristic) < (b.cost + b.heuristic)
    }

    queue: pq.Priority_Queue(State)
    pq.init(&queue, compare, pq.default_swap_proc(State), allocator = allocator)
    defer pq.destroy(&queue)

    pq.push(&queue, initial_state)
    
    seen := make(map[Move]struct{}, allocator = allocator)
    defer delete(seen)

    directions : [4]Vector : { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }

    for pq.len(queue) > 0 {
        current_state := pq.pop(&queue)

        if current_state.position == goal {
            steps = current_state.cost
            found = true
            return
        }

        seen[current_state] = {}

        for direction, index in directions {
            state := current_state
            state.position += direction
            state.cost += 1
            state.heuristic = heuristic(state, goal)

            if state.position.x < 0 || state.position.x > size.x do continue
            if state.position.y < 0 || state.position.y > size.y do continue

            if state.position in walls do continue
            if state in seen do continue

            pq.push(&queue, state)
        }

    }

    return
}

parse_map :: proc(data: string) -> (memory_map: MemoryMap) {
    reader: csv.Reader
    reader.comma = ','
    reader.fields_per_record   = -1
	reader.trim_leading_space  = true
	reader.reuse_record        = true
	reader.reuse_record_buffer = true
	defer csv.reader_destroy(&reader)
    
	csv.reader_init_with_string(&reader, data)

	for record, row, err in csv.iterator_next(&reader) {
		if err != nil {
           fmt.println("Error iterating through csv")
           return
        }

        byte: Vector

		for value, column in record {
            assert(column < len(byte))
            value_int, ok := strconv.parse_int(value)
            if (ok) {
                byte[column] = value_int
            } else {
                fmt.printfln("Error parsing row %v, column %v (%q)", row, column, value)
                return
            }
		}

        append(&memory_map.falling_bytes, byte)
	}

    return
}