package day_02

import "core:fmt"
import "core:os"
import "core:encoding/csv"
import "core:strconv"
import "core:slice"
import "core:strings"

main :: proc() {
    reader: csv.Reader
    reader.comma = ' '
    reader.comment = '#'
    reader.fields_per_record   = -1
	reader.trim_leading_space  = true
	reader.reuse_record        = true
	reader.reuse_record_buffer = true
	defer csv.reader_destroy(&reader)
    
    filename :: "input.txt"

	csv_data, ok := os.read_entire_file(filename)
	if ok {
		csv.reader_init_with_string(&reader, string(csv_data))
	} else {
		fmt.printfln("Unable to open file: %v", filename)
		return
	}
	defer delete(csv_data)

    reports: [dynamic][dynamic]int

	for record, row, err in csv.iterator_next(&reader) {
		if err != nil {
           fmt.println("Error iterating through csv")
           return
        }

        assign_at(&reports, row, make([dynamic]int, 0, 8))

		for value, column in record {
            value_int, ok := strconv.parse_int(value)
            if (ok) {
                append(&reports[row], value_int)
            } else {
                fmt.printfln("Error parsing row %v, column %v (%q)", row, column, value)
                return
            }
		}
	}

    safe_count : int = 0
    dampened_count : int = 0

    for report, index in reports {
        fmt.printf("report %v: ", index)
        for item in report {
            fmt.printf("%v ", item)
        }

        safe, dampened := is_report_safe(report[:])
        if safe {
            if !dampened {
                safe_count += 1
                fmt.print("SAFE")
            } else {
                dampened_count += 1
                fmt.print("SAFE (DAMPENED)")
            }
        } else {
            fmt.print("UNSAFE")
        }

        fmt.println()
    }

    fmt.printfln("safe count: %v", safe_count)
    fmt.printfln("dampened count: %v", dampened_count)
    fmt.printfln("total: %v", safe_count + dampened_count)
}

is_report_safe :: proc(report: []int, enable_dampening : bool = true) -> (safe : bool = true, dampened : bool = false) {
    previous_diff_sign : int = 0
    
    for index in 1..<len(report) {
        safe, previous_diff_sign = is_safe(report[index - 1], report[index], previous_diff_sign)
        if !safe {
           break
        }
    }

    if !safe && enable_dampening {
        dampened = true

        report_copy: [dynamic] int
        defer delete(report_copy)

        for i in 0..<len(report) {
            clear(&report_copy)

            for item, j in report {
                if i == j {
                    continue
                }
                append(&report_copy, item)
            }

            safe, _ = is_report_safe(report_copy[:], false)

            if safe {
                break
            }
        }
    }

    return
}

is_safe :: proc(x1: int, x2: int, previous_sign: int) -> (safe : bool, diff_sign : int) {
    diff := x1 - x2
    diff_sign = sign(diff)
    safe = (diff != 0 && abs(diff) <= 3) && (previous_sign == 0 || diff_sign == previous_sign)
    return
}

sign :: #force_inline proc (x: int) -> int {
    return int(x > 0) - int(x < 0)
}

