#include <iostream>
#include "main.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <random>

int size = 0;
int minSize = 0;

int main() {
    int **sudoku = makeSudoku(25);
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
    sudoku=makeDiagonal(sudoku);

    //sudoku = fillManually16(sudoku);
    //sudoku = makeCorners(sudoku);


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
        for (int i = size-minSize; i < size; i++) {
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
 * Method to print Sudoku grid onto the console
 */
void sudokuOut(int **sudoku) {
    for (int i = 0; i < size; i++) {
        if (i % (int) sqrt(size) == 0 && i != 0) {
            if(size==9)std::cout << "----------+-----------+---------" << std::endl;
            else if(size==16)std::cout << "-----------------+------------------+------------------+-----------------" << std::endl;
            else std::cout << "---------------------+----------------------+----------------------+----------------------+----------------------" << std::endl;
        }
        for (int j = 0; j < size; j++) {
            if (j % (int) sqrt(size) == 0 && j != 0) {

                std::cout << " | ";
            }
            if(sudoku[i][j]<10)std::cout << "  " << sudoku[i][j] << " ";
            else std::cout << " " << sudoku[i][j] << " ";


        }

        std::cout << std::endl;

    }
}

/**
 * Debug
 */
int **fillManually9(int **sudoku) {
    //TOP LEFT
    sudoku[0][0] = 8;
    sudoku[0][1] = 2;
    sudoku[0][2] = 7;
    sudoku[1][0] = 9;
    sudoku[1][1] = 6;
    sudoku[1][2] = 5;
    sudoku[2][0] = 3;
    sudoku[2][1] = 4;
    sudoku[2][2] = 1;
    //Top MIDDLE
    sudoku[0][3] = 1;
    sudoku[0][4] = 5;
    sudoku[0][5] = 4;
    sudoku[1][3] = 3;
    sudoku[1][4] = 2;
    sudoku[1][5] = 7;
    sudoku[2][3] = 6;
    sudoku[2][4] = 8;
    sudoku[2][5] = 9;

    //MIDDLE LEFT
    sudoku[3][0] = 5;
    sudoku[3][1] = 9;
    sudoku[3][2] = 3;
    sudoku[4][0] = 4;
    sudoku[4][1] = 7;
    sudoku[4][2] = 2;
    sudoku[5][0] = 6;
    sudoku[5][1] = 1;
    sudoku[5][2] = 8;
    //MIDDLE MIDDLE
    sudoku[3][3] = 4;
    sudoku[3][4] = 6;
    sudoku[3][5] = 8;
    sudoku[4][3] = 5;
    sudoku[4][4] = 1;
    sudoku[4][5] = 3;
    sudoku[5][3] = 9;
    sudoku[5][4] = 7;
    sudoku[5][5] = 2;
    //MIDDLE RIGHT
    sudoku[3][6] = 2;
    sudoku[3][7] = 7;
    sudoku[3][8] = 1;
    sudoku[4][6] = 6;
    sudoku[4][7] = 8;
    sudoku[4][8] = 9;
    sudoku[5][6] = 4;
    sudoku[5][7] = 3;
    sudoku[5][8] = 5;

    //BOTTOM MIDDLE
    sudoku[6][3] = 2;
    sudoku[6][4] = 3;
    sudoku[6][5] = 5;
    sudoku[7][3] = 7;
    sudoku[7][4] = 9;
    sudoku[7][5] = 6;
    sudoku[8][3] = 8;
    sudoku[8][4] = 4;
    sudoku[8][5] = 1;
    //BOTTOM RIGHT
    sudoku[6][6] = 9;
    sudoku[6][7] = 1;
    sudoku[6][8] = 4;
    sudoku[7][6] = 8;
    sudoku[7][7] = 2;
    sudoku[7][8] = 3;
    sudoku[8][6] = 5;
    sudoku[8][7] = 6;
    sudoku[8][8] = 7;
    return sudoku;
}

int **fillManually16(int **sudoku) {
    //TOPEST ROW
    //TOPEST
    sudoku[0][0]=4;
    sudoku[0][1]=10;
    sudoku[0][2]=9;
    sudoku[0][3]=15;
    sudoku[0][4]=1;
    sudoku[0][5]=7;
    sudoku[0][6]=13;
    sudoku[0][7]=8;
    sudoku[0][8]=6;
    sudoku[0][9]=14;
    sudoku[0][10]=2;
    sudoku[0][11]=12;
    //TOP
    sudoku[1][0]=2;
    sudoku[1][1]=5;
    sudoku[1][2]=3;
    sudoku[1][3]=1;
    sudoku[1][4]=15;
    sudoku[1][5]=4;
    sudoku[1][6]=11;
    sudoku[1][7]=16;
    sudoku[1][8]=13;
    sudoku[1][9]=9;
    sudoku[1][10]=8;
    sudoku[1][11]=7;
    //LOW
    sudoku[2][0]=14;
    sudoku[2][1]=6;
    sudoku[2][2]=13;
    sudoku[2][3]=12;
    sudoku[2][4]=3;
    sudoku[2][5]=10;
    sudoku[2][6]=5;
    sudoku[2][7]=2;
    sudoku[2][8]=16;
    sudoku[2][9]=11;
    sudoku[2][10]=1;
    sudoku[2][11]=4;
    //LOWEST
    sudoku[3][0]=11;
    sudoku[3][1]=7;
    sudoku[3][2]=16;
    sudoku[3][3]=8;
    sudoku[3][4]=6;
    sudoku[3][5]=14;
    sudoku[3][6]=9;
    sudoku[3][7]=12;
    sudoku[3][8]=5;
    sudoku[3][9]=3;
    sudoku[3][10]=10;
    sudoku[3][11]=15;

    //TOP ROW
    //TOPEST
    sudoku[4][0]=8;
    sudoku[4][1]=16;
    sudoku[4][2]=11;
    sudoku[4][3]=4;
    sudoku[4][4]=13;
    sudoku[4][5]=15;
    sudoku[4][6]=14;
    sudoku[4][7]=9;
    sudoku[4][8]=2;
    sudoku[4][9]=5;
    sudoku[4][10]=7;
    sudoku[4][11]=3;
    sudoku[4][12]=12;
    sudoku[4][13]=1;
    sudoku[4][14]=10;
    sudoku[4][15]=6;
    //TOP
    sudoku[5][0]=1;
    sudoku[5][1]=14;
    sudoku[5][2]=6;
    sudoku[5][3]=13;
    sudoku[5][4]=12;
    sudoku[5][5]=8;
    sudoku[5][6]=4;
    sudoku[5][7]=5;
    sudoku[5][8]=10;
    sudoku[5][9]=16;
    sudoku[5][10]=9;
    sudoku[5][11]=11;
    sudoku[5][12]=2;
    sudoku[5][13]=3;
    sudoku[5][14]=7;
    sudoku[5][15]=15;
    //LOW
    sudoku[6][0]=10;
    sudoku[6][1]=15;
    sudoku[6][2]=5;
    sudoku[6][3]=3;
    sudoku[6][4]=2;
    sudoku[6][5]=1;
    sudoku[6][6]=6;
    sudoku[6][7]=7;
    sudoku[6][8]=4;
    sudoku[6][9]=12;
    sudoku[6][10]=14;
    sudoku[6][11]=8;
    sudoku[6][12]=9;
    sudoku[6][13]=11;
    sudoku[6][14]=16;
    sudoku[6][15]=13;
    //LOWEST
    sudoku[7][0]=12;
    sudoku[7][1]=2;
    sudoku[7][2]=7;
    sudoku[7][3]=9;
    sudoku[7][4]=11;
    sudoku[7][5]=3;
    sudoku[7][6]=16;
    sudoku[7][7]=10;
    sudoku[7][8]=15;
    sudoku[7][9]=13;
    sudoku[7][10]=6;
    sudoku[7][11]=1;
    sudoku[7][12]=4;
    sudoku[7][13]=8;
    sudoku[7][14]=14;
    sudoku[7][15]=5;

    //LOW ROW
    //TOPEST
    sudoku[8][0]=9;
    sudoku[8][1]=4;
    sudoku[8][2]=1;
    sudoku[8][3]=10;
    sudoku[8][4]=14;
    sudoku[8][5]=2;
    sudoku[8][6]=3;
    sudoku[8][7]=13;
    sudoku[8][8]=11;
    sudoku[8][9]=15;
    sudoku[8][10]=12;
    sudoku[8][11]=6;
    sudoku[8][12]=7;
    sudoku[8][13]=16;
    sudoku[8][14]=5;
    sudoku[8][15]=8;
    //TOP
    sudoku[9][0]=5;
    sudoku[9][1]=8;
    sudoku[9][2]=14;
    sudoku[9][3]=16;
    sudoku[9][4]=7;
    sudoku[9][5]=9;
    sudoku[9][6]=1;
    sudoku[9][7]=6;
    sudoku[9][8]=3;
    sudoku[9][9]=4;
    sudoku[9][10]=13;
    sudoku[9][11]=10;
    sudoku[9][12]=11;
    sudoku[9][13]=12;
    sudoku[9][14]=15;
    sudoku[9][15]=2;
    //LOW
    sudoku[10][0]=7;
    sudoku[10][1]=3;
    sudoku[10][2]=15;
    sudoku[10][3]=6;
    sudoku[10][4]=16;
    sudoku[10][5]=11;
    sudoku[10][6]=12;
    sudoku[10][7]=4;
    sudoku[10][8]=8;
    sudoku[10][9]=2;
    sudoku[10][10]=5;
    sudoku[10][11]=9;
    sudoku[10][12]=14;
    sudoku[10][13]=13;
    sudoku[10][14]=1;
    sudoku[10][15]=10;
    //LOWEST
    sudoku[11][0]=13;
    sudoku[11][1]=12;
    sudoku[11][2]=2;
    sudoku[11][3]=11;
    sudoku[11][4]=10;
    sudoku[11][5]=5;
    sudoku[11][6]=8;
    sudoku[11][7]=15;
    sudoku[11][8]=7;
    sudoku[11][9]=1;
    sudoku[11][10]=16;
    sudoku[11][11]=14;
    sudoku[11][12]=3;
    sudoku[11][13]=6;
    sudoku[11][14]=4;
    sudoku[11][15]=9;

    //LOWEST ROW
    //TOPEST
    sudoku[12][4]=4;
    sudoku[12][5]=12;
    sudoku[12][6]=7;
    sudoku[12][7]=3;
    sudoku[12][8]=1;
    sudoku[12][9]=10;
    sudoku[12][10]=11;
    sudoku[12][11]=13;
    sudoku[12][12]=5;
    sudoku[12][13]=14;
    sudoku[12][14]=6;
    sudoku[12][15]=16;
    //TOP
    sudoku[13][4]=9;
    sudoku[13][5]=16;
    sudoku[13][6]=15;
    sudoku[13][7]=1;
    sudoku[13][8]=14;
    sudoku[13][9]=8;
    sudoku[13][10]=4;
    sudoku[13][11]=2;
    sudoku[13][12]=10;
    sudoku[13][13]=7;
    sudoku[13][14]=11;
    sudoku[13][15]=3;
    //LOW
    sudoku[14][4]=8;
    sudoku[14][5]=13;
    sudoku[14][6]=10;
    sudoku[14][7]=14;
    sudoku[14][8]=12;
    sudoku[14][9]=6;
    sudoku[14][10]=3;
    sudoku[14][11]=5;
    sudoku[14][12]=15;
    sudoku[14][13]=9;
    sudoku[14][14]=2;
    sudoku[14][15]=1;
    //LOWEST
    sudoku[15][4]=5;
    sudoku[15][5]=6;
    sudoku[15][6]=2;
    sudoku[15][7]=11;
    sudoku[15][8]=9;
    sudoku[15][9]=7;
    sudoku[15][10]=15;
    sudoku[15][11]=16;
    sudoku[15][12]=13;
    sudoku[15][13]=4;
    sudoku[15][14]=8;
    sudoku[15][15]=12;
    return sudoku;
}
