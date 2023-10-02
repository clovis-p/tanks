//
// Created by clovis on 29/09/23.
//

#ifndef TANKS_ARRAY_H
#define TANKS_ARRAY_H

int** create2DArray(int rows, int cols);
void free2DArray(int** array, int rows);
void createAndUse2DArray(int rows, int cols, int*** my2DArray);

#endif //TANKS_ARRAY_H
