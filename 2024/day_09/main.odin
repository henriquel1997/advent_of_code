package day_09

import "core:os"
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:odin/tokenizer"

File :: struct {
    id: int,
    quantity: int,
    free_space: int,
}

main :: proc() {
    filename :: "input.txt"

	input_bytes, ok := os.read_entire_file(filename)

    if !ok {
		fmt.println("Unable to open file: ", filename)
		return
	}

    files := parse_files(string(input_bytes))
    first_compacted, err := slice.clone_to_dynamic(files[:])
    if err != nil {
        fmt.println("Unable to clone file array: ", err)
        return
    }
    
    compact_fragmented(&first_compacted)
    fmt.println("Checksum Fragmented: ", calculate_checksum(first_compacted))

    compact_whole_files(&files)
    fmt.println("Checksum Whole Files: ", calculate_checksum(files))
}

compact_fragmented :: proc(files: ^[dynamic]File) {
    for {
        last := pop(files)

        for &file, index in files {
            if last.quantity <= 0 do break

            if file.free_space > 0 {
                new_quantity := min(file.free_space, last.quantity) 
                new_file: File = {
                    id = last.id,
                    quantity = new_quantity,
                    free_space = file.free_space - new_quantity,
                }

                file.free_space = 0
                last.quantity -= new_file.quantity

                inject_at(files, index + 1, new_file)
            }
        }

        if last.quantity > 0 {
            if files[len(files) - 1].id == last.id {
                files[len(files) - 1].quantity += last.quantity
            } else {
                append(files, last)
            }
            break
        }
    }
}

compact_whole_files :: proc(files: ^[dynamic]File) {
    for id := len(files) - 1; id > 0; id -= 1 {

        id_index := -1
        for file, index in files {
            if file.id == id {
                id_index = index
                break
            }
        }
        assert(id_index > 0)

        file := files[id_index]

        for i in 0..<id_index {
            other := &files[i]
            if other.free_space >= file.quantity {
                files[id_index - 1].free_space += file.quantity + file.free_space

                file.free_space = other.free_space - file.quantity
                other.free_space = 0

                ordered_remove(files, id_index)
                inject_at(files, i + 1, file)
                break
            }
        }
    }
}

calculate_checksum :: proc(files: [dynamic]File) -> (checksum: int) {
    position := 0
    for file, index in files {
        for i in 0..<file.quantity {
            checksum += (i + position) * file.id
        }
        position += file.quantity + file.free_space
    }
    return
}

@require_results
parse_files :: proc(input: string) -> (files: [dynamic]File){
    input := input
    ok : bool
    input, ok = strings.split_lines_iterator(&input)
    assert(ok)

    for i := 0; i < len(input); i += 2 {
        file: File
        file.id = i / 2
        file.quantity = tokenizer.digit_val(rune(input[i]))
        file.free_space = (i + 1 < len(input)) ? tokenizer.digit_val(rune(input[i + 1])) : 0
        
        append(&files, file)
    }

    return
}