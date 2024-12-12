package day_12

import "core:os"
import "core:fmt"
import "core:strings"
import sa "core:container/small_array"
import "core:slice"

SmallArray :: sa.Small_Array

Vector :: [2]int
Region :: [dynamic]Vector
Garden :: map[rune]([dynamic]Region)

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file:", filename)
        return
    }

    garden := parse_garden(string(input_bytes))

    total_price_perimeter := 0
    total_price_side := 0
    for char, regions in garden {
        for region in regions {
            perimeter_price, side_price := region_price(region)
            total_price_perimeter += perimeter_price
            total_price_side += side_price
        }
    }

    fmt.println("Total price (Perimeter):", total_price_perimeter)
    fmt.println("Total price (Side):", total_price_side)
}

region_price :: proc(region: Region) -> (perimeter_price: int, side_price: int) {
    Edge :: struct {
        start: Vector,
        end: Vector,
        side: Vector,
    }

    edges := make([dynamic]Edge, allocator = context.temp_allocator)
    defer free_all(context.temp_allocator)

    perimeter := 0
    for plot in region {

        plot_sides := SmallArray(4, Vector) {
            data = {
                { 0,  1},
                { 0, -1},
                { 1,  0},
                {-1,  0},
            },
            len = 4
        }

        for other in region {
            if index, found := slice.linear_search(sa.slice(&plot_sides), other - plot); found {
                sa.unordered_remove(&plot_sides, index)
                if plot_sides.len == 0 do break
            }
        }

        perimeter += plot_sides.len

        for side in sa.slice(&plot_sides) {
            plot_edge := plot + side
            start := plot_edge
            end := plot_edge
            for index := len(edges) - 1; index >= 0; index -= 1 {
                edge := edges[index];
                if side == edge.side && edge.start.x == plot_edge.x {
                    if edge.start.y - plot_edge.y == 1 || edge.end.y - plot_edge.y == -1 {
                        start.y = min(start.y, edge.start.y)
                        end.y = max(end.y, edge.end.y)
                        unordered_remove(&edges, index)
                    }
                } else if side == edge.side && edge.start.y == plot_edge.y {
                    if edge.start.x - plot_edge.x == 1 || edge.end.x - plot_edge.x == -1 {
                        start.x = min(start.x, edge.start.x)
                        end.x = max(end.x, edge.end.x)
                        unordered_remove(&edges, index)
                    }
                }
            }
            append(&edges, Edge{ start, end, side })
        }
    }

    area := len(region)
    perimeter_price = area * perimeter

    sides := len(edges)
    side_price = area * sides
    return 
}

is_next_to :: proc(a: Vector, b: Vector) -> bool {
    dist : Vector = { abs(a.x - b.x), abs(a.y - b.y) }
    return (dist.x == 1 && dist.y == 0) || (dist.x == 0 && dist.y == 1)
}

parse_garden :: proc(input: string) -> (garden: Garden) {
    input := input

    y := 0
    for line in strings.split_lines_iterator(&input) {
        for char, x in line {
            if char == '.' do continue

            position : Vector = {x, y}

            region_index : int = -1

            if regions, ok := &garden[char]; ok {
                regions_loop: for &region, index in regions {
                    for plot in region {
                        if is_next_to(plot, position) {
                            append(&region, position)
                            region_index = index
                            break regions_loop
                        }
                    }
                }
            }

            if region_index >= 0 {
                regions := &garden[char]
                main_region := &regions[region_index]
                for index := len(regions) - 1; index > region_index; index -= 1 {
                    region := &regions[index]
                    for plot in region {
                        if is_next_to(plot, position) {
                            append(main_region, ..region[:])
                            delete(region^)
                            unordered_remove(regions, index)
                            break
                        }
                    }
                }
            } else {
                region := make(Region)
                append(&region, position)

                regions := garden[char] or_else make([dynamic]Region)
                append(&regions, Region { position })

                garden[char] = regions
            }
        }
        y += 1
    }

    return
}