package day_01

import "core:fmt"
import "core:os"
import "core:encoding/csv"
import "core:strconv"
import "core:slice"

main :: proc() {
    reader: csv.Reader
    reader.comma = ' '
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

    arrays: [2][dynamic]int

	for record, row, err in csv.iterator_next(&reader) {
		if err != nil {
           fmt.println("Error iterating through csv")
           return
        }
		for value, column in record {
			//fmt.printf("row %v, column %v: %q", row, column, value)
            value_int, ok := strconv.parse_int(value)
            if (ok) {
                append(&arrays[column], value_int)
            } else {
                fmt.printfln("Error parsing row %v, column %v (%q)", row, column, value)
                return
            }
		}
	}

    length := len(arrays[0])
    if(length != len(arrays[1])){
        fmt.println("Arrays are not the same size")
        return
    }

    slice.sort(arrays[0][:])
    slice.sort(arrays[1][:])

    total_distance : int = 0

    for index in 0..<length {
        distance := abs(arrays[0][index] - arrays[1][index])
        fmt.printfln("row %v, distance: %v", index, distance)
        total_distance += distance
    }

    fmt.printfln("total distance: %v", total_distance)

    total_similarity : int = 0
    for value in arrays[0] {
        similarity := value * slice.count(arrays[1][:], value)
        total_similarity += similarity
    }

    fmt.printfln("similarity: %v", total_similarity)
}

