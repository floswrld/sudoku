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
    sudoku=makeDiagonal(sudoku);

    sudoku = makeBorder(sudoku);
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



int **makeBorder(int **sudoku) {
    std::vector<std::vector<int>> borderArray((size*size)-minSize);

    std::vector<int> borderOptions;
    int bCounter=0;
    int boxI=1;
    int boxJ=0;
    bool locIndex=true;
    int count = 1;
    while(true){
        while(boxI<minSize&&boxJ<minSize){

            for (int i = 0; i < minSize; i++) {
                for (int j = 0; j < minSize; j++) {
                    int iT =  i+ boxI * minSize;
                    int jT = j + boxJ * minSize;
                    borderOptions=getBorderOptions(iT,jT,sudoku);

                    if (borderOptions.size() != 0) {
                        int ran = getRandomInt(borderOptions.size());
                        sudoku[iT][jT] = borderOptions.at(ran);
                        borderArray[bCounter].insert(borderArray[bCounter].end(),borderOptions.at(ran));
                        bCounter++;
                    } else {
                        while (boxI>=0 || boxJ>=0 || i>=0||j>=0){
                            if(i==0&&j==0){
                                if(locIndex){
                                    if(boxJ==0){
                                        count--;
                                        locIndex= false;
                                        boxJ=minSize-1;
                                        boxI=minSize-1-count;
                                    }else{
                                        boxJ--;
                                        boxI--;
                                    }
                                }else{
                                    if(boxI==0){
                                        locIndex=true;
                                        boxJ=minSize-1-count;
                                        boxI=minSize-1;
                                    } else{
                                        boxJ--;
                                        boxI--;
                                    }
                                }
                                j=minSize-1;
                                i=minSize-1;
                            }else if(j==0){
                                i--;
                                j=minSize-1;
                            }else{
                                j--;
                            }
                            bCounter--;
                            iT=i+boxI*minSize;
                            jT=j+boxJ*minSize;
                            sudoku[iT][jT]=0;
                            borderOptions=getBorderOptions(iT,jT,sudoku);
                            borderOptions= vectorXwithoutVectorY(borderOptions,borderArray[bCounter]);
                            if(borderOptions.size()!=0){
                                int ran = getRandomInt(borderOptions.size());
                                sudoku[iT][jT] = borderOptions.at(ran);
                                borderArray[bCounter].insert(borderArray[bCounter].end(),borderOptions.at(ran));
                                bCounter++;
                                break;
                            }
                            borderArray[bCounter].clear();
                        }
                    }
                }
            }
            boxI++;
            boxJ++;
        }
        if(locIndex){
            locIndex=false;
            boxI=0;
            boxJ=count;
        } else{
            count++;
            locIndex=true;
            boxI=count;
            boxJ=0;
        }
        if(count==minSize)break;
    }
    return sudoku;
}

/*
 * Method to get possible integers for a slot in a border-box
 */
std::vector<int> getBorderOptions(int row, int col, int **sudoku) {
    std::vector<int> restrictions;
    int boxRow,boxCol;
    boxRow=row/minSize;
    boxCol=col/minSize;
    //adds the already used numbers from the row to the restrictions array
    for (int i = 0; i < size; i++) {
        if(i/minSize==boxRow)continue;
        if (contains(restrictions, sudoku[row][i])==-1 && sudoku[row][i] != 0) {
            restrictions.insert(restrictions.end(), sudoku[row][i]);
        }
    }
    //adds the already used numbers from the col to the restrictions array
    for (int i = 0; i < size; i++) {
        if(i/minSize==boxCol)continue;
        if (contains(restrictions, sudoku[i][col])==-1 && sudoku[i][col] != 0) {
            restrictions.insert(restrictions.cend(), sudoku[i][col]);
        }
    }
    //adds the already used numbers of the same box to the restictions array

    for (int i = 0 + minSize * boxRow; i < minSize + minSize * boxRow; i++) {
        for (int j = 0 + minSize * boxCol; j < minSize + minSize * boxCol; j++) {
            if (contains(restrictions, sudoku[i][j])==-1 && sudoku[i][j] != 0) {
                restrictions.insert(restrictions.end(), sudoku[i][j]);
            }
        }
    }

    std::vector<int> allowed;
    for (int i = 1; i <= size; i++) {
        if (contains(restrictions, i)==-1) {
            allowed.insert(allowed.end(), i);
        }
    }
    return allowed;
}


/*
 * Method to get a boolean depending on a comparison between array value and test-integer
 * returns the position and -1 if not found
 */
int contains(std::vector<int> array, int test) {
    for (int i = 0; i < array.size(); i++) {
        if (array.at(i) == test)return i;
    }
    return -1;
}

/*
 * Returns a vector which consists only of the elements which aren't in y
 */
std::vector<int> vectorXwithoutVectorY(std::vector<int> x, std::vector<int> y){
    for(int i = 0;i<y.size();i++){
        int c = contains(x,y.at(i));
        if(c!=-1){
            x.erase(x.begin()+c);
        }
    }
    return x;
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
            if(size==9)std::cout << "-------------+--------------+------------" << std::endl;
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

