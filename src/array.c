//
// Created by clovis on 29/09/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "array.h"

int** create2DArray(int rows, int cols) {
    // Allocate memory for the rows.
    int **array = (int **)malloc(rows * sizeof(int *));
    if (array == NULL) {
        // Handle memory allocation failure.
        perror("Failed to allocate memory for rows.");
        exit(1);
    }

    // Allocate memory for each row's columns.
    for (int i = 0; i < rows; i++) {
        array[i] = (int *)malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            // Handle memory allocation failure.
            perror("Failed to allocate memory for columns.");
            exit(1);
        }
    }

    return array;
}

void free2DArray(int** array, int rows) {
    for (int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

void createAndUse2DArray(int rows, int cols, int*** my2DArray) {
    *my2DArray = create2DArray(rows, cols);

    // Use the 2D array (e.g., populate).
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            (*my2DArray)[i][j] = i * j;
        }
    }
}