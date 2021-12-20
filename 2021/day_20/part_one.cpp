#include "common.h"

struct LitPixel{
    int64 x;
    int64 y;
};

#define MAX_PIXELS 10000
#define ALGORITHM_ITERATIONS 2
#define ALGORITHM_SIZE 512

int main(){
    String text = readFile("input.txt");
	
    char* position = text.data;

    uint64 algorithm_size = 0;
    bool* algorithm = (bool*) calloc(ALGORITHM_SIZE, sizeof(bool));

    while(!isEndOfLine(*position)){
        if(*position == '#') algorithm[algorithm_size] = true;
        algorithm_size++;
        position++;
        if(isEndOfLine(*position)) position++;
    }
    position++;

    uint64 num_pixels = 0;
    LitPixel* pixels = (LitPixel*) malloc(MAX_PIXELS * sizeof(pixels));
    
    int64 x = 0;
    int64 y = 0;
    int64 min_x = 0;
    int64 min_y = 0;
    int64 max_x = 0;
    int64 max_y = 0;
    while((position - text.data) < text.size){
        if(*position == '#'){
            LitPixel pixel;
            pixel.x = x;
            pixel.y = y;
            pixels[num_pixels++] = pixel;
        }

        position++;

        if(isEndOfLine(*position)){
            max_x = x;
            x = 0;
            y++;
            position++;
        }else{
            x++;
        }
    }
    max_y = y;

    uint64 num_pixels_next = 0;
    LitPixel* next_pixels = (LitPixel*) malloc(MAX_PIXELS * sizeof(pixels));

    bool outside_pixel_state = false;
    for(int iteration = 0; iteration < ALGORITHM_ITERATIONS; iteration++){
        printf("Iteration %i\n", iteration);
        int64 start_x = min_x - 1;
        int64 start_y = min_y - 1;
        int64 limit_x = max_x + 1;
        int64 limit_y = max_y + 1;
        
        max_x = LLONG_MIN;
        max_y = LLONG_MIN;
        min_x = LLONG_MAX;
        min_y = LLONG_MAX;

        for(int64 y = start_y; y <= limit_y; y++){
            for(int64 x = start_x; x <= limit_x; x++){
                uint64 filter_pixels_size = 0;
                LitPixel filter_pixels[9];

                uint64 algorithm_index = 0;
                #define set_alg_index(other_x, other_y) algorithm_index |= 256ULL >> ((((other_y) - y + 1) * 3) + ((other_x) - x + 1))
                
                for(int64 filter_x = x - 1; filter_x <= x + 1; filter_x++){
                    for(int64 filter_y = y - 1; filter_y <= y + 1; filter_y++){
                        if(filter_x > start_x && filter_y > start_y && filter_x < limit_x && filter_y < limit_y){
                            filter_pixels[filter_pixels_size++] = (LitPixel){filter_x, filter_y};
                        }else if(outside_pixel_state){
                            set_alg_index(filter_x, filter_y);
                        }
                    }
                }

                for(uint64 i = 0; i < num_pixels && filter_pixels_size > 0; i++){
                    LitPixel pixel = pixels[i];

                    for(uint64 j = 0; j < filter_pixels_size; j++){
                        LitPixel filter_pixel = filter_pixels[j];
                        if(pixel.x == filter_pixel.x && pixel.y == filter_pixel.y){
                            set_alg_index(filter_pixel.x, filter_pixel.y);
                            for(uint64 k = j; k < (filter_pixels_size - 1); k++){
                                filter_pixels[k] = filter_pixels[k + 1];
                            }
                            filter_pixels_size--;
                            break;
                        }
                    }
                }
                
                if(algorithm[algorithm_index]){
                    next_pixels[num_pixels_next++] = (LitPixel){x, y};
                    if(x < min_x) min_x = x;
                    if(x > max_x) max_x = x;

                    if(y < min_y) min_y = y;
                    if(y > max_y) max_y = y;
                }
            }
        }

        LitPixel* aux = pixels;
        pixels = next_pixels;
        next_pixels = aux;

        num_pixels = num_pixels_next;
        num_pixels_next = 0;

        outside_pixel_state = algorithm[outside_pixel_state * (algorithm_size - 1)];
    }

    for(int64 y = min_y - 1; y <= max_y + 1; y++){
        for(int64 x = min_x - 1; x <= max_x + 1; x++){
            bool lit = false;
            if(x >= min_x && y >= min_y && x <= max_x && y <= max_y){
                for(uint64 i = 0; i < num_pixels; i++){
                    LitPixel pixel = pixels[i];
                    if(pixel.x == x && pixel.y == y){
                        lit = true;
                        break;
                    }
                }
            }else{
                lit = outside_pixel_state;
            }
            printf(lit ? "#" : ".");
        }
        printf("\n");
    }

    printf("Number of lit pixels: %llu\n", num_pixels);
    return 0;
}