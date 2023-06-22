#include <iostream>
#include "main.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <random>

int size = 0;
int minSize = 0;

int main() {
    int **sudoku = makeSudoku(9);
    sudokuOut(sudoku);

    // Deallocate memory for the 2D array
    for (int i = 0; i < size; i++) {
        delete[] sudoku[i];
    }
    delete sudoku;
    return 0;
}

/*
 * Method to generate a Sudoku grid with a random but possible solution
 */
int **makeSudoku(int pSize) {
    size = pSize;
    minSize = sqrt(size);
    int **sudoku = new int *[size];
    for (int i = 0; i < size; i++) {
        sudoku[i] = new int[size];
    }

    for (int j = 0; j < size; j++) {
        for (int k = 0; k < size; k++) {
            sudoku[j][k] = 0;
        }
    }

    sudoku = makeCorners(sudoku);

    //sudoku=makeDiagonal(sudoku);
    /*

    makeBorder(sudoku);
    std::vector<int> borderOptions = getBorderOptions(0, 3, sudoku);
    for(int i = 0;i<borderOptions.size();i++){
        std::cout<<borderOptions.at(i)<<std::endl;

    }

    std::cout<<std::endl;

*/
    return sudoku;
}


/*
 * Method to fill a main diagonal with random integers without a duplicate within a 3x3 box
 */
int **makeDiagonal(int **sudoku) {
    for (int c = 0; c < minSize; c++) {
        std::vector<int> optionVector;
        for (int i = 1; i <= size; i++) {
            optionVector.push_back(i);
        }
        for (int i = 0 + (c * minSize); i < minSize + (c * minSize); i++) {
            for (int j = 0 + (c * minSize); j < minSize + (c * minSize); j++) {
                int temp = getRandomInt(optionVector.size());
                sudoku[i][j] = optionVector.at(temp);
                optionVector.erase(optionVector.begin() + temp);
            }
        }
    }
    return sudoku;
}

/*
 * Method to fill upper-mid, mid-left, mid-right and mid-lower box with random integers, without duplicates
 * within the box, row and column
 *
 * YET TO IMPLEMENT
 */
int **makeBorder(int **sudoku) {
    int ***borderArray = new int **[size];
    for (int i = 0; i < size; i++) {
        borderArray[i] = new int *[size];
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            borderArray[i][j] = new int[size - minSize];
        }
    }

    std::vector<int> borderOptions;
    int bCounter = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i / minSize == j / minSize)continue;
            if (i / minSize == minSize - 1 && j / minSize == 0
                || j / minSize == minSize - 1 && i / minSize == 0)
                continue;

            /*
            borderOptions=getBorderOptions(i,j,sudoku);
            for(int k = 0; k < borderOptions.size(); k++){
                std::cout << borderOptions[k] << " ";
            }
            std::cout << std::endl;*/

            if (sizeof(borderOptions) / sizeof(int) != 0) {
                int ran = rand() % (sizeof(borderOptions) / sizeof(int));
                sudoku[i][j] = borderOptions[ran];
                borderOptions[ran] = 0;
            } else {
                borderArray[i][j][0] = sudoku[i][j];

            }


        }
    }
}

/*
 * Method to get possible integers for a slot in a border-box
 */
std::vector<int> getBorderOptions(int row, int col, int **sudoku) {
    std::vector<int> restrictions;
    //adds the already used numbers from the row to the restrictions array
    for (int i = 0; i < size; i++) {
        if (!contains(restrictions, sudoku[row][i]) && sudoku[row][i] != 0) {
            restrictions.insert(restrictions.end(), sudoku[row][i]);
        }
    }
    //adds the already used numbers from the col to the restrictions array
    for (int i = 0; i < size; i++) {
        if (!contains(restrictions, sudoku[i][col]) && sudoku[i][col] != 0) {
            restrictions.insert(restrictions.cend(), sudoku[i][col]);
        }
    }
    //adds the already used numbers of the same box to the restictions array
    int blockRow = row / minSize;
    int blockCol = col / minSize;

    for (int i = 0 + minSize * blockRow; i < minSize + minSize * blockRow; i++) {
        for (int j = 0 + minSize * blockCol; j < minSize + minSize * blockCol; j++) {
            if (!contains(restrictions, sudoku[i][j]) && sudoku[i][j] != 0) {
                restrictions.insert(restrictions.end(), sudoku[i][j]);
            }
        }
    }

    std::vector<int> allowed;
    for (int i = 1; i <= size; i++) {
        if (!contains(restrictions, i)) {
            allowed.insert(allowed.end(), i);
        }
    }
    return allowed;
}

/*
 * Method to get a boolean depending on a comparison between array value and test-integer
 */
bool contains(std::vector<int> array, int test) {
    for (int i = 0; i < array.size(); i++) {
        if (array.at(i) == test)return true;
    }
    return false;
}

/*
 * Method to generate random integer between 1 and size
 *
 * Ansatz fue True Random anhand von Hardware
 * (davor Problem immer selbes Sudoku)
 */
int getRandomInt(int vectorSize) {
    std::random_device rd;// Obtain a random seed from the hardware
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> distribution(0, vectorSize - 1);
    int randomInt = distribution(engine);

    return randomInt;
}

/*
 * function to fill the missing corners
 */
int **makeCorners(int **sudoku) {
    for (int num = 1; num <= size; num++) {
        int x, y;
        for (int i = 0; i < minSize; i++) {
            bool contains = false;
            for (int j = 0; j < size - minSize; j++) {
                if (sudoku[i][j] == num) {
                    contains = true;
                    break;
                }
            }
            if (contains == false) {
                x = i;
                break;
            }
        }
        for (int i = size - minSize; i < size; i++) {
            bool contains = false;
            for (int j = minSize; j < size; j++) {
                if (sudoku[j][i] == num) {
                    contains = true;
                    break;
                }
            }
            if (contains == false) {
                y = i;
                break;
            }
        }
        sudoku[x][y] = num;
    }

    for (int num = 1; num <= size; num++) {
        std::cout << num << std::endl;
        int x, y;
        for (int i = 0; i < minSize; i++) {
            bool contains = false;
            for (int j = 0; j < size - minSize; j++) {
                if (sudoku[j][i] == num) {
                    contains = true;
                    break;
                }
            }
            if (contains == false) {
                std::cout << "x=" << i << std::endl;
                x = i;
                break;
            }
        }
        for (int i = size - minSize; i < size; i++) {
            bool contains = false;
            for (int j = minSize; j < size; j++) {
                if (sudoku[i][j] == num) {
                    contains = true;
                    break;
                }
            }
            if (contains == false) {
                std::cout << "y=" << i << std::endl;
                y = i;
                break;
            }
        }

        sudoku[y][x] = num;
    }
    return sudoku;
}

/*
 * Method to print Sudoku grid onto the console
 */
void sudokuOut(int **sudoku) {
    for (int i = 0; i < size; i++) {
        if (i % (int) sqrt(size) == 0 && i != 0) {

            std::cout << "----------+-----------+---------" << std::endl;
        }
        for (int j = 0; j < size; j++) {
            if (j % (int) sqrt(size) == 0 && j != 0) {

                std::cout << " | ";
            }
            std::cout << " " << sudoku[i][j] << " ";


        }

        std::cout << std::endl;

    }
}