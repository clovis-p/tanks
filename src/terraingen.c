//
// Created by clovis on 28/09/23.
//

/**
 * Generates a random terrain and returns a pointer, pointing to a 2D array. Said array is as big as the resolution of the window.
 * The contents of the array will then be rendered to the screen, pixel per pixel.
 */

#include "main.h"
#include "terraingen.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int** generateTerrain(int width, int height) {
    // Allocate memory for the rows.
    int **terrain = (int **)malloc(width * sizeof(int *));
    if (terrain == NULL) {
        // Handle memory allocation failure.
        perror("Failed to allocate memory for terrain.");
        exit(1);
    }

    // Allocate memory for each row's columns.
    for (int i = 0; i < width; i++) {
        terrain[i] = (int *)malloc(height * sizeof(int));
        if (terrain[i] == NULL) {
            // Handle memory allocation failure.
            perror("Failed to allocate memory for terrain.");
            exit(1);
        }
    }
    /*
    srand(time(NULL));

    for (int i = 0; i < width; i++)
    {
        int groundLevel = height - rand() % 100;
        terrain[i][groundLevel] = 1;
        printf("%d %d\n", i, groundLevel);
    }
    */

    // Set the frequency and amplitude for the sine wave terrain.
    double frequency = 2 * M_PI / width;
    double amplitude = height / 4;

    for (int x = 0; x < width; x++) {
        int h = (int)(height/2 + amplitude * sin(x * frequency));

        for (int y = 0; y < height; y++) {
            if (y < h) {
                terrain[x][y] = 0; // air
            } else {
                terrain[x][y] = 1; // solid ground
            }
        }
    }
    return terrain;
}

void freeTerrain(int** terrain, int width) {
    for (int i = 0; i < width; i++) {
        free(terrain[i]);
    }
    free(terrain);
}