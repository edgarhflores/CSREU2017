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

//void printNodeArray(node array[], int size) {
//    for (int index = 0; index < size; index++) {
//        node currentNode;
//        currentNode = node(array[index]);
//        cout << currentNode.getGrayScaleValue() << endl;
//    }
//}

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

//void createLeftNodeArray(int *array, int size, node *nodeArray) {
//    int value;
//    for (int i = 0; i < size; i++) {
//        value = array[i];
//        node newNode(value);
//        nodeArray[i] = newNode;
//    }// End of for loop
//}// End of createLeftNodeArray function

//********************************************************************
//Method:	putValuesIntoArray
//Description:	this function asks the user for a PGM file and then calls the
//              appropriate method to build the left and right image array
//Parameters:  	none
//Returns:     	nothing 
//Calls:       	getSize
//              putValuesIntoArray
//Globals:	numOfFiles
//              leftSize
//              rightSize
//              leftImageArray
//              rightImageArray
//              currentRoundData
//              previousRoundData
//              tempRoundData

void setFiles() {
    char filename[50];
    ifstream fileObj;
    string input;
    int userAns;
    string lrImage;
    while (numOfFiles >= 0) {
        switch (numOfFiles) {
            case 1:
                lrImage = "LEFT";
                break;
            case 0:
                lrImage = "Right";
                break;
            default:
                lrImage = "MISTAKE";
        }
        cout << "Specify the PGM file for the " << lrImage << " image to be read:" << endl;
        cin.getline(filename, 50);
        fileObj.open(filename);
        cout << endl;
        if (!fileObj.is_open()) {
            cout << "FILE DID NOT OPEN.\n" << endl;
            cout << "Press ENTER to open another file (1 = exit program )" << endl;
            getline(cin, input);
            stringstream(input) >> userAns;
            cout << endl;
            switch (userAns) {
                case 1:
                    exit(0);
                default:
                    break;
            }// end of switch       
        } else {
            cout << "Creating image array of PGM file...\n" << endl;
            if (numOfFiles == 1) {
                leftSize = getSize(fileObj);
                leftImageArray = new int[leftSize];
                currData = new node[leftSize];
                prevData = new node[leftSize];
                tempData = new node[leftSize];
                putValuesIntoArray(leftImageArray, fileObj, leftSize);
                //createLeftNodeArray(leftImageArray, leftSize, currentRoundData);
                cout << "The left image array was created\n" << endl;
            } else {
                rightSize = getSize(fileObj);
                rightImageArray = new int[rightSize];
                putValuesIntoArray(rightImageArray, fileObj, rightSize);
                cout << "The right image array was created\n" << endl;
            }
            fileObj.close();
            numOfFiles--;
        }// end of else
    }//end of while loop
}
void updateMessage(int x, int y) {
    double minVal;
    int top = getIndex(x, y - 1, leftImageWidth);
    int bot = getIndex(x, y + 1, leftImageWidth);
    int left = getIndex(x - 1, y, leftImageWidth);
    int right = getIndex(x + 1, y, leftImageWidth);
    //update top message
    for (int k = 0; k < MAX_OFF_SET; k++) {
        minVal = numeric_limits<int>::max();
        for (int kPrime = 0; kPrime < 15; kPrime++) {
            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[left].getMsgRight(kPrime) +
                    prevData[right].getMsgLeft(kPrime);
            if (tempMsg < minVal) {
                minVal = tempMsg;
            }//End of if statement     
        }//End of inner loop
        currData[getIndex(x, y, leftImageWidth)].setMsgTop(k, minVal);
    }//End of outer loop

    //update bottom message
    for (int k = 0; k < MAX_OFF_SET; k++) {
        minVal = numeric_limits<int>::max();
        for (int kPrime = 0; kPrime < 15; kPrime++) {
            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[top].getMsgBottom(kPrime) +
                    prevData[left].getMsgRight(kPrime) +
                    prevData[right].getMsgLeft(kPrime);
            if (tempMsg < minVal) {
                minVal = tempMsg;
            }//End of if statement  

        }//End of inner loop
        currData[getIndex(x, y, leftImageWidth)].setMsgBottom(k, minVal);
    }//End of outer loop

    //update left message
    for (int k = 0; k < MAX_OFF_SET; k++) {
        minVal = numeric_limits<int>::max();
        for (int kPrime = 0; kPrime < 15; kPrime++) {
            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[top].getMsgBottom(kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[right].getMsgLeft(kPrime);
            if (tempMsg < minVal) {
                minVal = tempMsg;
            }//End of if statement     
        }//End of inner loop
        currData[getIndex(x, y, leftImageWidth)].setMsgLeft(k, minVal);
    }//End of outer loop

    //update right message
    for (int k = 0; k < MAX_OFF_SET; k++) {
        minVal = numeric_limits<int>::max();
        for (int kPrime = 0; kPrime < 15; kPrime++) {
            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[top].getMsgBottom(kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[left].getMsgRight(kPrime);
            if (tempMsg < minVal) {
                minVal = tempMsg;
            }//End of if statement     
        }//End of inner loop
        currData[getIndex(x, y, leftImageWidth)].setMsgRight(k, minVal);
    }//End of outer loop
    
}// End of updateMessage

void updateBelief(int x, int y) {
    double belief;
    int top = getIndex(x, y - 1, leftImageWidth);
    int bot = getIndex(x, y + 1, leftImageWidth);
    int left = getIndex(x - 1, y, leftImageWidth);
    int right = getIndex(x + 1, y, leftImageWidth);
    
    for (int k = 0; k < MAX_OFF_SET ; k++) {
        belief = dataCost(x,y,k) + prevData[top].getMsgBottom(k) +
                    prevData[bot].getMsgTop(k) +
                    prevData[left].getMsgRight(k) + prevData[right].getMsgLeft(k);
        currData[getIndex(x, y, leftImageWidth)].setBelief(k, belief);
    }
}

void loopyBP() {

    for (int k = 0; k <= 15; k++) {
        //need to loop through x and y values
        for (int x = 0; x < leftImageWidth; x++) {
            for (int y = 0; y < leftImageHeight; y++) {
                //swap pointers
                tempData = prevData;
                prevData = currData;
                currData = tempData;
                //update data
                updateMessage(x,y); // need to pass in x and y values
                //updateBelief();
            }// End of for loop for y
        }// End of for loop for x
    }//End of for loop for iterations
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
    string input;
    int userAns;
    bool loop = true;
    int x = 3;
    int y = 3;
    node currentNode;
    cout << "Loopy Belief Propagation: Edgar Flores\n" << endl;
    setFiles();
    cout << "setting pixel (3,3)'s (in currentRoundData) MsgBottom vector at position 2' value to 23... " << endl;
    currData[getIndex(x, y, leftImageWidth)].setMsgBottom(2, 23);
    int value;
    value = int (currData[getIndex(x, y, leftImageWidth)].getMsgBottom(2));

    cout << "The pixel (3,3)'s MsgBottom vector at position 2' value is: " << value << endl;

    //    while (loop) {
    //        cout << "1 = Calculate DataCost (default)" << endl;
    //        cout << "2 = End program" << endl;
    //        getline(cin, input);
    //        stringstream(input) >> userAns;
    //        cout << endl;
    //        switch (userAns) {
    //            case 1:
    //                break;
    //            case 2:
    //                exit(0);
    //            default:
    //                cout << "Input not valid. Please enter either 1 or 2.\n" << endl;
    //        }// end of switch  
    //    }//End of while loop 
    return 0;
}