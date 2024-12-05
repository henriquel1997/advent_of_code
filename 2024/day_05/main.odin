package day_05

import "core:os"
import "core:fmt"
import "core:strings"
import "core:strconv"
import "core:slice"

Rule :: struct {
    before : int,
    after : int,
}

main :: proc() {
    filename :: "input.txt"

	input_bytes, ok := os.read_entire_file(filename)

    if !ok {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}

    input := string(input_bytes)

    rules := parse_rules(&input)
    updates := parse_updates(&input)

    correct_middle_sum := 0
    incorrect_middle_sum := 0

    for update, index in updates {
        if validate_update(update, rules[:]) {
            middle := update[len(update) / 2]
            correct_middle_sum += middle
        } else {
            sorted := sort_update(update, rules[:])
            defer delete(sorted)

            middle := sorted[len(sorted) / 2]
            incorrect_middle_sum += middle
        }
    }

    fmt.printfln("correct middle sum: %v", correct_middle_sum)
    fmt.printfln("incorrect middle sum: %v", incorrect_middle_sum)
}

parse_rules :: proc(input: ^string) -> (rules: [dynamic]Rule) {
    for line in strings.split_lines_iterator(input) {
        numbers := strings.split(line, "|");
        if len(numbers) != 2 do return

        before, ok_before := strconv.parse_int(numbers[0])
        if !ok_before do return

        after, ok_after := strconv.parse_int(numbers[1])
        if !ok_after do return

        rule : Rule = { before, after }
        append(&rules, rule)
    }
    return
}

parse_updates :: proc(input: ^string) -> (updates: [dynamic][]int) {
    for line in strings.split_lines_iterator(input) {
        update : [dynamic]int

        for number in strings.split(line, ",") {
            page, ok := strconv.parse_int(number)
            if !ok do return

            append(&update, page)
        }

        if(len(update) > 0){
            append(&updates, update[:])
        } else {
            delete(update)
            return
        }
    }
    return
}

validate_update :: proc(update: []int, rules: []Rule) -> (valid: bool) {
    for page, index in update {
        for rule in rules {
            if rule.after == page {
                other_index, found := slice.linear_search(update, rule.before)
                if found {
                    if other_index < index {
                        continue
                    } else {
                        return false
                    }
                }
            }
        }
    }
    return true
}

sort_update :: proc(update : []int, rules : []Rule) -> (sorted: [dynamic]int) {
    if len(update) <= 0 do return

    append(&sorted, update[0])

    for page in update[1:] {
        for index in 0..=len(sorted) {
            inject_at(&sorted, index, page)
            
            if validate_update(sorted[:], rules) {
                break
            }

            ordered_remove(&sorted, index)
        }   
    }

    assert(len(sorted) == len(update))
    return
}