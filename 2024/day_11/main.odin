package day_11

import "core:os"
import "core:fmt"
import "core:strings"
import "core:strconv"
import "core:math"

StoneAmountMap :: map[int]int

main :: proc() {
    filename :: "input.txt"

    input_bytes, ok := os.read_entire_file_from_filename(filename)

    if !ok {
        fmt.println("Unable to read file:", filename)
        return
    }

    stones := parse_stones(string(input_bytes))

    for _ in 0..<25 {
        result := blink(stones)
        delete(stones)
        stones = result
    }

    stone_count := 0
    for _, amount in stones {
        stone_count += amount
    }
    fmt.println("Stone count (25 times):", stone_count)

    for _ in 0..<50 {
        result := blink(stones)
        delete(stones)
        stones = result
    }

    stone_count = 0
    for _, amount in stones {
        stone_count += amount
    }
    fmt.println("Stone count (75 times):", stone_count)
}

blink :: proc(stones: StoneAmountMap) -> (result: StoneAmountMap) {
    for stone, amount in stones {
        if stone == 0 {
            result[1] += amount
        } else if digit_count := count_digits(stone); digit_count % 2 == 0 {
            pow := int(math.floor(math.pow_f32(10, f32(digit_count/2))))

            left_stone := stone / pow
            right_stone := stone % pow

            result[left_stone] += amount
            result[right_stone] += amount
        } else {
            result[stone * 2024] += amount
        }
    }
    return
}

count_digits ::proc (n: int) -> int {
    assert(n >= 0)
    if (n < 10) do return 1;
    if (n < 100) do return 2;
    if (n < 1000) do return 3;
    if (n < 10000) do return 4;
    if (n < 100000) do return 5;
    if (n < 1000000) do return 6;
    if (n < 10000000) do return 7;
    if (n < 100000000) do return 8;
    if (n < 1000000000) do return 9;
    if (n < 10000000000) do return 10;
    if (n < 100000000000) do return 11;
    if (n < 1000000000000) do return 12;
    assert(false)
    return -1;
}

@require_results
parse_stones :: proc(input: string) -> (stones: StoneAmountMap) {
    result, err := strings.split(input, " ")
    if err != nil do return

    for number in result {
        stone, ok := strconv.parse_int(strings.trim_space(number))
        if !ok do break
        stones[stone] += 1
    }

    return
}