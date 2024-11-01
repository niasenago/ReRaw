#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define MAP_SIZE 256

typedef struct {
    char *content;   
    size_t length;   
} FileData;


int read_file(const char *filepath, FileData *file_data);

int main(int argc, char **argv){
    if (argc < 2) {
        fprintf(stderr, "USAGE: main.out <filename>\n");
        exit(1);
    }

    FileData file_content = {0};
    if(!read_file(argv[1], &file_content)) return 1;

    printf("File length: %zu bytes\n", file_content.length);

    if(file_content.length == 0) {
        fprintf(stderr, "Error: file %s is empty\n", argv[1]);
        return 1;
        free(file_content.content);
    }

    size_t map[MAP_SIZE][MAP_SIZE] = {0};
    uint32_t pixels[MAP_SIZE][MAP_SIZE] = {0};
    
    for(size_t i = 0; i < file_content.length - 1; ++i){
        uint8_t x = file_content.content[i];
        uint8_t y = file_content.content[i + 1];
        map[x][y] ++;
    }

    size_t max_count = 0;
    for(size_t x = 0; x < MAP_SIZE; ++x){
        for(size_t y = 0; y < MAP_SIZE; ++y){
            if(map[x][y] > max_count) {
                max_count = map[x][y];
            }
        }
    }

    float max = logf((float)max_count);

    for(size_t x = 0; x < MAP_SIZE; ++x){
        for(size_t y = 0; y < MAP_SIZE; ++y){
            float t = 0;
            if (map[x][y] > 0) {
                t = logf((float)map[x][y]) / max;
            }
            uint32_t brightness = (uint32_t)(t * 255);
            pixels[x][y] = 0xFF000000 | (brightness << 16) | (brightness << 8) | brightness;
        }
    }

    char output_file_path[256];
    sprintf(output_file_path, "%s.output.png", argv[1]);
    if(!stbi_write_png(output_file_path, MAP_SIZE, MAP_SIZE, 4, pixels, MAP_SIZE * sizeof(uint32_t))) {
        fprintf(stderr, "Error: Could not save the output image %s\n", output_file_path);
        return 1;
    }
    printf("Succesifully generated image %s\n",output_file_path );

    free(file_content.content);
    return 0;
}

/*
    returns 1 if success 0 if fails
    reads binary file data into file_data variable
*/
int read_file(const char *filepath, FileData *file_data) {
    FILE *fp = fopen(filepath, "rb");  // Open in binary mode
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filepath);
        return 0;
    }

    // Seek to the end to determine file size
    fseek(fp, 0, SEEK_END);
    file_data->length = ftell(fp);
    rewind(fp);

    file_data->content = (char *)malloc(file_data->length + 1);
    if (file_data->content == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        fclose(fp);
        return 0;
    }

    size_t read_size = fread(file_data->content, 1, file_data->length, fp);
    file_data->content[read_size] = '\0';  

    fclose(fp);

    if (read_size != file_data->length) {
        fprintf(stderr, "Error: could not read entire file\n");
        free(file_data->content);
        file_data->content = NULL;
        file_data->length = 0;
        return 0;
    }

    return 1;
}