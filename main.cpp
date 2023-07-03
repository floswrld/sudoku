#include <iostream>
#include "main.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <random>
#include <ctime>
#include <unordered_set>

int size = 0;
int minSize = 0;


int main() {
    timeOut("Start time:");
    int **sudoku = makeSudoku(16);
    sudokuOut(sudoku);

    // Deallocate memory for the 2D array
    for (int i = 0; i < size; i++) {
        delete[] sudoku[i];
    }
    delete sudoku;
    timeOut("End time: ");

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
    std::vector<std::unordered_set<int>> borderArray((size*size)-minSize);

    std::vector<int> borderOptions;
    borderOptions.reserve(size);
    std::unordered_set<int> restrictions;
    restrictions.reserve(size);

    int bCounter=0;
    int boxI=1;
    int boxJ=0;
    bool locIndex=true;
    int count = 1;
    int temp=0;
    while(true){
        while(boxI<minSize&&boxJ<minSize){
            for (int i = 0; i < minSize; i++) {
                for (int j = 0; j < minSize; j++) {
                    int iT =  i+ boxI * minSize;
                    int jT = j + boxJ * minSize;
                    restrictions=getBorderRestrictions(iT, jT, sudoku);
                    borderOptions=getAllowed(restrictions);

                    if (borderOptions.size() != 0) {
                        int ran = getRandomInt(borderOptions.size());
                        temp=borderOptions.at(ran);
                        sudoku[iT][jT] = temp;
                        restrictions.insert(restrictions.end(),temp);
                        borderArray[bCounter].insert(restrictions.begin(),restrictions.end());
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
                            borderOptions= getAllowed(borderArray[bCounter]);
                            if(borderOptions.size()!=0){
                                int ran = getRandomInt(borderOptions.size());
                                temp = borderOptions.at(ran);
                                sudoku[iT][jT] = temp;
                                borderArray[bCounter].insert(temp);
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
std::unordered_set<int> getBorderRestrictions(int row, int col, int **sudoku) {
    //std::cout<<"Row: "<<row<<" Col: "<<col<<std::endl;
    std::unordered_set<int> restrictions;
    restrictions.reserve(size);
    int boxRow=row/minSize;
    int boxCol=col/minSize;
    int temp;
    if(row>col){
        for(int i =boxRow*minSize-1;i>=0;i--){
            temp=sudoku[i][col];
            if(temp==0)break;
            restrictions.insert(temp);
        }
        for(int i =boxCol*minSize+3;i<size;i++){
            temp=sudoku[row][i];
            if(temp==0)break;
                restrictions.insert(temp);
        }
    }else{
        for(int i =boxRow*minSize+3;i<size;i++){
            temp=sudoku[i][col];
            if(temp==0)break;
            restrictions.insert(temp);
        }
        for(int i =boxCol*minSize-1;i>=0;i--){
            temp=sudoku[row][i];
            if(temp==0)break;
            restrictions.insert(temp);
        }
    }
    bool breakBool=false;
    for (int i = 0 + minSize * boxRow; i < minSize + minSize * boxRow; i++) {
        for (int j = 0 + minSize * boxCol; j < minSize + minSize * boxCol; j++) {
            temp=sudoku[i][j];
            if(temp==0) {
                breakBool=true;
                break;
            }
            restrictions.insert(temp);
        }
        if(breakBool)break;
    }
    return restrictions;
}

std::vector<int> getAllowed(std::unordered_set<int> restrictions){
    std::vector<int> allowed;
    allowed.reserve(size);
    for (int i = 1; i <= size; i++) {
        if (restrictions.find(i)==restrictions.end()) {
            allowed.push_back(i);
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


void timeOut(std::string message){
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the time to a string format
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    // Print the current time
    std::cout << message << buffer << std::endl;
}

