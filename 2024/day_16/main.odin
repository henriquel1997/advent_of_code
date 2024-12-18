package day_16

import "core:os"
import "core:fmt"
import "core:strings"
import "core:slice"
import pq "core:container/priority_queue"
import sa "core:container/small_array"

SmallArray :: sa.Small_Array

Vector :: [2]int

Maze :: struct {
    size: Vector,
    start: Vector,
    goal: Vector,
    walls: map[Vector]struct{},
}

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file", filename)
        return
    }
    
    maze := parse_maze(string(input_bytes))

    score, positions := solve_maze(maze.start, maze.goal, maze.walls)

    print_maze(maze, positions)

    fmt.println("Best path score:", score)
    fmt.println("Spot count:", len(positions))
}

print_maze :: proc(maze: Maze, positions: map[Vector]struct{}) {
    for y in 0..<maze.size.y {
        for x in 0..<maze.size.x {
            pos : Vector = {x, y}

            if pos in maze.walls {
                fmt.print('#')
            } else if pos in positions {
                fmt.print('O')
            } else {
                fmt.print('.')
            }
        }
        fmt.println()
    }
}

solve_maze :: proc(start, goal: Vector, walls: map[Vector]struct{}, allocator := context.allocator) -> (score: int = 999999999999, positions: map[Vector]struct{}) {
    Move :: struct {
        position: Vector,
        direction: Vector,
    }

    State :: struct {
        using move: Move,
        cost: int,
    }

    initial_state : State = {
        position = start,
        direction = { 1,  0 },
        cost = 0,
    }

    compare :: proc(a, b: State) -> bool {
        return a.cost < b.cost
    }

    queue: pq.Priority_Queue(State)
    pq.init(&queue, compare, pq.default_swap_proc(State), allocator = allocator)
    defer pq.destroy(&queue)

    pq.push(&queue, initial_state)
    
    state_from_move := make(map[Move]State, allocator = allocator)
    defer delete(state_from_move)

    Marker :: struct {
        best_cost: int,
        previous: SmallArray(4, Move)
    }

    came_from := make(map[Move]Marker, allocator = allocator)
    defer delete(came_from)

    for pq.len(queue) > 0 {
        current_state := pq.pop(&queue)

        if other, exists := state_from_move[current_state]; !exists || current_state.cost < other.cost {
            state_from_move[current_state] = current_state
        }

        if current_state.position == goal {
            score = min(score, current_state.cost)
            continue
        }

        states : [3]State

        states[0] = current_state
        states[0].position += current_state.direction
        states[0].cost += 1

        states[1] = current_state
        states[1].direction = { current_state.direction.y, -current_state.direction.x }
        states[1].cost += 1000

        states[2] = current_state
        states[2].direction = { -current_state.direction.y, current_state.direction.x }
        states[2].cost += 1000

        for state in states {
            if state.position in walls do continue

            if other, exists := state_from_move[state]; exists && state.cost > other.cost {
                continue
            }

            pq.push(&queue, state)

            if marker, exists := came_from[state]; exists {
                if state.cost < marker.best_cost {
                    marker.best_cost = state.cost
                    sa.clear(&marker.previous)
                    sa.push_back(&marker.previous, current_state)
                    came_from[state] = marker
                } else if state.cost == marker.best_cost && !slice.contains(sa.slice(&marker.previous), current_state) {
                    ok := sa.push_back(&marker.previous, current_state)
                    assert(ok)
                    came_from[state] = marker
                }
            } else {
                marker : Marker = {best_cost = state.cost}
                sa.push_back(&marker.previous, current_state)
                came_from[state] = marker
            }
        }

    }

    positions = make(map[Vector]struct{}, allocator = allocator)
    positions[start] = {}
    
    position_queue := make([dynamic]State, allocator = allocator)
    defer delete(position_queue)

    directions : [4]Vector : { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }

    for direction in directions {
        state : State = { position = goal, direction = direction, cost = score }
        if !(state in came_from) do continue
        append(&position_queue, state)
    }

    for len(position_queue) > 0 {
        state := pop_front(&position_queue)

        positions[state.position] = {}

        marker := came_from[state] or_continue

        for previous in sa.slice(&marker.previous) {
            if previous_marker, exists := came_from[previous]; exists {
                score := previous_marker.best_cost

                if score > state.cost do continue

                append(
                    &position_queue, 
                    State { move = previous, cost = score },
                )
            }
        }
    }

    return
}

parse_maze :: proc(input: string) -> (maze: Maze) {
    input := input

    y := 0
    width := 0

    for line in strings.split_lines_iterator(&input) {
        for char, x in line {
            position : Vector = {x, y}

            switch char {
                case 'S': maze.start = position
                case 'E': maze.goal = position
                case '#': maze.walls[position] = {}
            }
        }

        width = max(width, len(line))
        y += 1
    }

    maze.size = {width, y}
    return
}