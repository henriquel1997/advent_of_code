package day_15

import "core:os"
import "core:fmt"
import "core:strings"
import "core:slice"

Vector :: [2]int

Entity :: struct {
    id: int,
    position: Vector,
    size: Vector,
}

WarehouseMap :: struct {
    robot: Entity,
    boxes: [dynamic]Entity,
    walls: [dynamic]Entity,
}

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file", filename)
        return
    }

    warehouse_map, moves := parse_map(string(input_bytes))
    wide_warehouse_map := double_map(warehouse_map)

    execute_moves(&warehouse_map, moves)
    fmt.println("Part 1 - Sum of GPS Coordinates:", sum_gps_coordinates(warehouse_map))

    execute_moves(&wide_warehouse_map, moves)
    fmt.println("Part 2 - Sum of GPS Coordinates:", sum_gps_coordinates(wide_warehouse_map))
}

double_map :: proc(warehouse_map: WarehouseMap) -> (result: WarehouseMap) {
    result.robot = warehouse_map.robot
    result.robot.position.x *= 2

    for box in warehouse_map.boxes {
        new_box := box
        new_box.position.x *= 2
        new_box.size.x *= 2
        append(&result.boxes, new_box)
    }

    for wall in warehouse_map.walls {
        new_wall := wall
        new_wall.position.x *= 2
        new_wall.size.x *= 2
        append(&result.walls, new_wall)
    }

    return
}

sum_gps_coordinates :: proc(warehouse_map: WarehouseMap) -> (sum: int) {
    for box in warehouse_map.boxes {
        sum += box.position.y * 100 + box.position.x
    }
    return
}

execute_moves :: proc(warehouse_map: ^WarehouseMap, moves: string) {

    is_colliding :: proc(a, b: Entity) -> bool {
        return a.id != b.id &&
               a.position.x < b.position.x + b.size.x &&
               a.position.x + a.size.x > b.position.x &&
               a.position.y < b.position.y + b.size.y &&
               a.position.y + a.size.y > b.position.y
    }

    context.allocator = context.temp_allocator
    defer free_all(context.temp_allocator)

    box_indices: [dynamic]int
    entity_queue: [dynamic]Entity

    move_loop: for move in moves {
        direction : Vector
        switch move {
            case '^': direction = {  0, -1  }
            case 'v': direction = {  0,  1  }
            case '<': direction = { -1,  0  }
            case '>': direction = {  1,  0  }
            case: continue
        }

        clear(&box_indices)
        clear(&entity_queue)
        append(&entity_queue, warehouse_map.robot)

        for len(entity_queue) > 0 {
            entity := pop_front(&entity_queue)
            entity.position += direction

            for wall in warehouse_map.walls {
                if is_colliding(entity, wall) {
                    continue move_loop
                }
            }

            for box, index in warehouse_map.boxes {
                if is_colliding(entity, box) && !slice.contains(box_indices[:], index) {
                    append(&box_indices, index)
                    append(&entity_queue, box)
                }
            }
        }

        for index in box_indices {
            warehouse_map.boxes[index].position += direction
        }

        warehouse_map.robot.position += direction
    }
}

parse_map :: proc(input: string) -> (warehouse_map: WarehouseMap, moves: string) {
    input := input

    y := 0

    id := 0

    for line in strings.split_lines_iterator(&input) {
        if len(line) == 0 do break

        for char, x in line {
            entity: Entity = { id = id, position = {x, y}, size = {1, 1} }
            id += 1

            switch char {
                case '@': warehouse_map.robot = entity
                case 'O': append(&warehouse_map.boxes, entity)
                case '#': append(&warehouse_map.walls, entity)
                case: id -= 1
            }
        }
        y += 1
    }
    
    moves = input
    return
}