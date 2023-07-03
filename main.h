//
// Created by 21bade1bif on 19.06.23.
//

#ifndef SUDOKUABGABE_MAIN_H
#define SUDOKUABGABE_MAIN_H

#include <vector>
#include <unordered_set>

int** makeSudoku(int size);
int** makeDiagonal(int** sudoku);
int** makeBorder(int** sudoku);
std::unordered_set<int> getBorderRestrictions(int row, int col, int** sudoku);
std::vector<int> getAllowed(std::unordered_set<int> restrictions);
int contains(std::vector<int> array,int test);
int getRandomInt(int vectorSize);
void sudokuOut(int** sudoku);
void timeOut(std::string message);

#endif //SUDOKUABGABE_MAIN_H