//
// Created by 21bade1bif on 19.06.23.
//

#ifndef SUDOKUABGABE_MAIN_H
#define SUDOKUABGABE_MAIN_H

#include <vector>

int** makeSudoku(int size);
int** makeDiagonal(int** sudoku);
int** makeBorder(int** sudoku);
std::vector<int> getBorderOptions(int row,int col,int** sudoku);
bool contains(std::vector<int> array,int test);
int getRandomInt(int vectorSize);
void sudokuOut(int** sudoku);

#endif //SUDOKUABGABE_MAIN_H