//Program:		LoopyBelief.cpp
//Description:          Single array implementation of loopy belief propagation
//Author:		Edgar Flores
//Started:              6/6/2017
//Revised:		6/16/2017
//Language:		C++
//IDE:			NetBeans 8.2
//Notes:		none
//******************************************************************************
//******************************************************************************
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include "node.h"
//constants
const int WINDOW_SIZE = 5;
const int MAX_OFF_SET = 15;
const int LAMDA = 20;
const int TRUNCATE = 2;
//
int numOfFiles = 1;
//widths
int leftImageWidth;
int rigthImageWidth;
//heights
int leftImageHeight;
//int rightImageHeight;
//sizes
int leftSize; //does this need to be a global?
int rightSize; //does this need to be a global?
//image arrays
int* leftImageArray;
int* rightImageArray;
//node arrays
node* tempData;
node* prevData;
node* currData;

using namespace std;

inline void wait_on_enter() {
    string dummy;
    cout << "Enter to continue..." << std::endl;
    getline(cin, dummy);
}

//********************************************************************
//Method:	smoothnessCost
//Description:	calculates smoothnessCost
//Parameters:  	k - the current node
//              kPrime - the neighbor node
//Returns:     	smoothnessCost - the penalization of adjacent labels that are 
//                               different
//Calls:       	nothing
//Globals:	LAMDA
//              TRUNCATE

int smoothnessCost(int k, int kPrime) {
    int n = abs(k - kPrime);
    int smoothnessCost;
    smoothnessCost = LAMDA * min(n, TRUNCATE);
    return smoothnessCost;
}

//********************************************************************
//Method:	getIndex
//Description:	translates the coordinates of x,y into a single dimensional 
//              coordinate 
//Parameters:  	x - the x coordinate
//              y - the y coordinate
//     imageWidth -  the width of an image
//Returns:     	index - the coordinates of single dimensional index
//Calls:       	nothing
//Globals:	none

int getIndex(int x, int y, int imageWidth) {
    int index;
    return index = (imageWidth * y) + x;
}

//********************************************************************
//Method:	dataCost
//Description:	calculates the sum of absolute difference for every pixel in 
//              one window
//Parameters:  	x - the x coordinate
//              y - the y coordinate
//              offset - the offset for the right image
//              leftImage[] - the array of the left image
//              rightImage[] - the array of the right image
//Returns:     	sumOfAbsDiff - the sum of absolute difference for every pixel in 
//              one window
//Calls:       	nothing
//Globals:	WINDOW_SIZE
//              imageWidthLeft
//              imageWidthRight

int dataCost(int x, int y, int kPrime) {

    int sumOfAbsDiff;
    int border = WINDOW_SIZE / 2;
    for (int i = x - border; i <= x + border; i++) {
        for (int j = y - border; j < y + border; j++) {
            sumOfAbsDiff += abs(leftImageArray[getIndex(i, j, leftImageWidth)] - rightImageArray[getIndex(i - kPrime, j, rigthImageWidth)]);
        }//inner for loop
    }//outer for loop
    return sumOfAbsDiff;
}

//********************************************************************
//Method:	printArray
//Description:	print array
//Parameters:  	array[] - the array being printed
//                 size - the width * height of an image in a PGM file
//Returns:     	nothing
//Calls:       	nothing
//Globals:	none

void printArray(int array[], int size) {
    for (int index = 0; index < size; index++) {
        cout << array[index] << endl;
    }
}

//********************************************************************
//Method:	getSize
//Description:	this method calculates the width and height of an image and sets
//              the imageWidthLeft and imageWidthRight
//Parameters:  	ifstream& fileObj - this is the current PGM file
//Returns:     	size - the width * height of an image in a PGM file
//Calls:       	nothing
//Globals:	imageWidthLeft
//              imageWidthRight
//              numOfFiles

int getSize(ifstream& fileObj) {
    string input;
    int size;
    getline(fileObj, input);
    getline(fileObj, input);
    getline(fileObj, input);
    int imageHeight;
    int imageWidth;
    stringstream nums(input);
    nums >> imageWidth >> imageHeight;
    size = imageWidth * imageHeight;
    getline(fileObj, input); // this is the maximum gray scale value
    if (numOfFiles == 0) {
        leftImageHeight = imageHeight;
        leftImageWidth = imageWidth;
    } else {
        rigthImageWidth = imageWidth;
    }
    return size;
}

//********************************************************************
//Method:	putValuesIntoArray
//Description:	this function puts the PGM text file into a single array 
//Parameters:  	array[] - the array that will store the values in the PGM file
//              fileObj - the PGM file
//              size - the width * height of an image in a PGM file
//Returns:     	nothing 
//Calls:       	nothing
//Globals:	none

void putValuesIntoArray(int *array, ifstream& fileObj, int size) {
    string input;
    for (int i = 0; i < size; i++) {
        getline(fileObj, input);
        stringstream value(input);
        value >> array[i];
    }// End of for loop
}// End of putValuesIntoArray function

void createPGMImageArray(ifstream& PGM_File, string fileName) {
    cout << "Creating image array of " << fileName << "...\n" << endl;
    if (numOfFiles == 1) {
        leftSize = getSize(PGM_File);
        cout << "The size of left size is: " << leftSize << endl;
        leftImageArray = new int[leftSize];
        putValuesIntoArray(leftImageArray, PGM_File, leftSize);
        cout << "The left image array was created\n" << endl;
    } else {
        rightSize = getSize(PGM_File);
        cout << "The size of right size is: " << rightSize << endl;
        rightImageArray = new int[rightSize];
        putValuesIntoArray(rightImageArray, PGM_File, rightSize);
        cout << "The right image array was created\n" << endl;
    }//End of else
    numOfFiles--;
}

void openFile(string fileName) {
    ifstream PGM_File;
    cout << "Opening " << fileName << "..." << endl;
    PGM_File.open(fileName.c_str());
    if (PGM_File.is_open()) {
        cout << "FILE OPENED.\n" << endl;
        createPGMImageArray(PGM_File, fileName);
        PGM_File.close();
    } else {
        cout << "FILE DID NOT OPEN.\n" << endl;
    }//End of else
}// End of openFile

void setFiles() {
    openFile("bowlLeft.pgm");
    openFile("bowlRight.pgm");
    //    openFile("left.pgm");
    //    openFile("left.pgm");
}//End of setFiles

void updateMessage(int x, int y) {

    double minVal;

    int top = getIndex(x, y - 1, leftImageWidth);
    int bot = getIndex(x, y + 1, leftImageWidth);
    int left = getIndex(x - 1, y, leftImageWidth);
    int right = getIndex(x + 1, y, leftImageWidth);

    //update top message
    cout << "Updating top message vector on offset of 0 - 15..." << endl;
    cout << endl;
    for (int k = 0; k < MAX_OFF_SET; k++) {
        cout << endl;
        cout << "Offset: " << k << endl;
        cout << endl;
        minVal = numeric_limits<int>::max();
        for (int kPrime = 0; kPrime < MAX_OFF_SET; kPrime++) {
            cout << "The sum of : " << endl;
            cout << dataCost(x, y, kPrime) << " (dataCost)"  << " + " << endl;
            cout << smoothnessCost(k, kPrime)<<  " (smoothnessCost)"  << " + " << endl;
            cout <<  prevData[bot].getMsgTop(kPrime)  << " (MsgBot)"  << " + " << endl;
            cout << prevData[left].getMsgRight(kPrime)<< " (MsgLeft) "  << " + " << endl;
            cout << prevData[right].getMsgLeft(kPrime) <<   " (MsgRight) " << " + " << endl;
            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[left].getMsgRight(kPrime) +
                    prevData[right].getMsgLeft(kPrime);
            if (tempMsg < minVal) {
                minVal = tempMsg;
            }//End of if statement  
        }//End of inner loop

        cout << "min value is: " << minVal << endl;

        currData[getIndex(x, y, leftImageWidth)].setMsgTop(k, minVal);
        cout << " The message at currData [" << getIndex(x, y, leftImageWidth) << "] for k =" << k << " is : " << currData[getIndex(x, y, leftImageWidth)].getMsgTop(k) << endl;
    }//End of outer loop
    cout << "Updating top message vector 0 - 15 complete" << endl;

    //    //update bottom message
    //    for (int k = 0; k < MAX_OFF_SET; k++) {
    //        minVal = numeric_limits<int>::max();
    //        for (int kPrime = 0; kPrime < 15; kPrime++) {
    //            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
    //                    prevData[top].getMsgBottom(kPrime) +
    //                    prevData[left].getMsgRight(kPrime) +
    //                    prevData[right].getMsgLeft(kPrime);
    //            if (tempMsg < minVal) {
    //                minVal = tempMsg;
    //            }//End of if statement      
    //        }//End of inner loop
    //        currData[getIndex(x, y, leftImageWidth)].setMsgBottom(k, minVal);
    //    }//End of outer loop
    //    cout << "Updating top message vector 0 - 15 complete" << endl;

    //        //update left message
    //        for (int k = 0; k < MAX_OFF_SET; k++) {
    //            minVal = numeric_limits<int>::max();
    //            for (int kPrime = 0; kPrime < 15; kPrime++) {
    //                int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
    //                        prevData[top].getMsgBottom(kPrime) +
    //                        prevData[bot].getMsgTop(kPrime) +
    //                        prevData[right].getMsgLeft(kPrime);
    //                if (tempMsg < minVal) {
    //                    minVal = tempMsg;
    //                }//End of if statement     
    //            }//End of inner loop
    //            currData[getIndex(x, y, leftImageWidth)].setMsgLeft(k, minVal);
    //        }//End of outer loop
    //    
    //        //update right message
    //        for (int k = 0; k < MAX_OFF_SET; k++) {
    //            minVal = numeric_limits<int>::max();
    //            for (int kPrime = 0; kPrime < 15; kPrime++) {
    //                int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
    //                        prevData[top].getMsgBottom(kPrime) +
    //                        prevData[bot].getMsgTop(kPrime) +
    //                        prevData[left].getMsgRight(kPrime);
    //                if (tempMsg < minVal) {
    //                    minVal = tempMsg;
    //                }//End of if statement     
    //            }//End of inner loop
    //            currData[getIndex(x, y, leftImageWidth)].setMsgRight(k, minVal);
    //        }//End of outer loop

}// End of updateMessage

void updateBelief(int x, int y) {
    double belief;

    int top = getIndex(x, y - 1, leftImageWidth);
    int bot = getIndex(x, y + 1, leftImageWidth);
    int left = getIndex(x - 1, y, leftImageWidth);
    int right = getIndex(x + 1, y, leftImageWidth);

    for (int k = 0; k < MAX_OFF_SET; k++) {
        belief = dataCost(x, y, k) + prevData[top].getMsgBottom(k) +
                prevData[bot].getMsgTop(k) +
                prevData[left].getMsgRight(k) + prevData[right].getMsgLeft(k);
        currData[getIndex(x, y, leftImageWidth)].setBelief(k, belief);
    }
}

void loopyBP() {
    cout << "RUNNING loopyBp..." << endl;

    int iterations = 1;

    currData = new node[leftSize];
    prevData = new node[leftSize];
    tempData = new node[leftSize];

    int border = (WINDOW_SIZE / 2) + MAX_OFF_SET;

    cout << "Border = " << border << endl;
    cout << endl;
    cout << "Working on " << iterations << " iterations... " << endl;
    cout << endl;
    for (int i = 0; i <= iterations; i++) {
        //need to loop through x and y values 
        cout << "Working on pixels for iteration " << i << endl;

        for (int x = border; x < leftImageWidth - border; x++) {
            for (int y = border; y < leftImageHeight - border; y++) {
                //swap pointers
                tempData = prevData;
                prevData = currData;
                currData = tempData;
                //update data
                if (x == 36 && y == 89) {
                    cout << "working on update message for pixel (" << x << ", " << y << ")..." << endl;
                    updateMessage(x, y);
                    cout << "Update message for pixel (" << x << ", " << y << ") COMPLETE. " << endl;
                    cout << endl;
                }
                //updateBelief(x,y);
            }// End of for loop for y
        }// End of for loop for x
    }//End of for loop for iterations
    cout << "loopyBp COMPLETE." << endl;
}// End of loopyBP function
//********************************************************************
//Method:	main
//Description:	This main method opens two files and stores the contents of both
//              files into a single array
//Parameters:  	none
//Returns:     	nothing
//Calls:       	printArray
//              getSize
//              putValuesIntoArray
//Globals:	none

int main() {
    cout << "Loopy Belief Propagation: Edgar Flores\n" << endl;
    setFiles();
    loopyBP();
    return 0;
}