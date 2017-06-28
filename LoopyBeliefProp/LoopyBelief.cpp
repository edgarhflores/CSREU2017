//Program:		LoopyBelief.cpp
//Description:          Single array implementation of loopy belief propagation
//Author:		Edgar Flores
//Started:              6/6/2017
//Revised:		6/28/2017
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
//sizes
int leftSize; //does this need to be a global?
int rightSize; //does this need to be a global?
//image arrays
int* leftImageArray;
int* rightImageArray;
int* finalImageArray;
//node arrays
node* tempData;
node* prevData;
node* currData;

using namespace std;

//inline void wait_on_enter() {
//    string dummy;
//    cout << "Enter to continue..." << std::endl;
//    getline(cin, dummy);
//}

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
//              kPrime - the offset for the right image
//Returns:     	sumOfAbsDiff - the sum of absolute difference for every pixel in 
//              one window
//Calls:       	nothing
//Globals:	WINDOW_SIZE
//              imageWidthLeft
//              imageWidthRight
int dataCost(int x, int y, int kPrime) {
    int sumOfAbsDiff = 0;
    int border = WINDOW_SIZE / 2;
    for (int i = x - border; i <= x + border; i++) {
        for (int j = y - border; j <= y + border; j++) {
            //cout << "(" <<x<< ", " << y << ")" << endl;
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
//Parameters:  	fileObj - this is the current PGM file
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
//Parameters:  	*array - the array that will store the values in the PGM file
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

//********************************************************************
//Method:	createPGMImageArray
//Description:	this function creates a 1-D array of the gray scale values in a
//              PGM file
//Parameters:  	PGM_File - the PGM file object
//              fileName - the name of the PGM file
//Returns:     	nothing 
//Calls:       	putValuesIntoArray
//Globals:	left size
//              leftImageArray
//              rightImageArray
void createPGMImageArray(ifstream& PGM_File, string fileName) {
    cout << "Creating image array of " << fileName << "..." << endl;
    if (numOfFiles == 1) {
        leftSize = getSize(PGM_File);
        leftImageArray = new int[leftSize];
        putValuesIntoArray(leftImageArray, PGM_File, leftSize);
        cout << "The left image array was created\n" << endl;
    } else {
        rightSize = getSize(PGM_File);
        rightImageArray = new int[rightSize];
        putValuesIntoArray(rightImageArray, PGM_File, rightSize);
        cout << "The right image array was created\n" << endl;
    }//End of else
    numOfFiles--;
}

//********************************************************************
//Method:	openFile
//Description:	this function verifies whether or not a PGM file opens
//Parameters:  	fileName - the name of the PGM file
//Returns:     	nothing 
//Calls:        createPGMImageArray
//Globals:	none
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

//********************************************************************
//Method:	setFiles
//Description:	this function allows for the specification of the two PGM files
//              that will be used in this program
//Parameters:  	none
//Returns:     	nothing 
//Calls:        openFile
//Globals:	none
void setFiles() {
    openFile("tutorial-tsukuba-imL.pgm");
    openFile("tutorial-tsukuba-imR.pgm");
}//End of setFiles

//********************************************************************
//Method:	updateMessage
//Description:	this function updates the top, bottom, left, and right  messages
//              of each node.
//Parameters:  	x - the x coordinate of a pixel
//              y - the y coordinate of a pixel
//Returns:     	nothing 
//Calls:        getIndex
//              dataCost
//              smoothnessCost
//Globals:	leftImageWidth
//              MAX_OFF_SET
//              prevData
//              currData
void updateMessage(int x, int y) {

    double minVal;

    int top = getIndex(x, y - 1, leftImageWidth);
    int bot = getIndex(x, y + 1, leftImageWidth);
    int left = getIndex(x - 1, y, leftImageWidth);
    int right = getIndex(x + 1, y, leftImageWidth);
    //used used for debugging
    int tempKPrime;
    int tempDataCost;
    int tempSmoothnessCost;
    float botMsg;

    if (x == 36 && y == 89) {
        cout << "UPDATE for pixel (" << x << ", " << y << "): " << endl;
    }
    //update top message
    for (int k = 0; k <= MAX_OFF_SET; k++) {

        minVal = numeric_limits<int>::max();

        for (int kPrime = 0; kPrime <= MAX_OFF_SET; kPrime++) {

            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[left].getMsgRight(kPrime) +
                    prevData[right].getMsgLeft(kPrime);

            if (tempMsg < minVal) {
                minVal = tempMsg;
                tempKPrime = kPrime; // used for debugging
                tempDataCost = dataCost(x, y, kPrime); // used for debugging
            }//End of if statement

        }//End of inner loop

        currData[getIndex(x, y, leftImageWidth)].setMsgTop(k, minVal);

        if (x == 36 && y == 89) {
            cout << "MsgTop[" << k << "] = " << currData[getIndex(x, y, leftImageWidth)].getMsgTop(k)<<endl;
//            cout << " [Calculation : ";
//            //cout << dataCost(x, y, tempKPrime) << " + ";
//            cout << tempDataCost << " (dataCost)" << " + ";
//            cout << smoothnessCost(k, tempKPrime) << " (smoothnessCost)" << " + ";
//            cout << prevData[bot].getMsgTop(tempKPrime) << " (MsgBot)" << " + ";
//            cout << prevData[left].getMsgRight(tempKPrime) << " (MsgLeft) " << " + ";
//            cout << prevData[right].getMsgLeft(tempKPrime) << " (MsgRight)";
//            cout << " = " << minVal << "(minVal)]" << endl;
        }//End of if statement
        if (x == 36 && y == 89 && k == 15) {
            cout << endl;
        }// End of if statement

    }//End of outer loop

    //update bottom message
    for (int k = 0; k <= MAX_OFF_SET; k++) {

        minVal = numeric_limits<int>::max();

        for (int kPrime = 0; kPrime <= MAX_OFF_SET; kPrime++) {

            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[top].getMsgBottom(kPrime) +
                    prevData[left].getMsgRight(kPrime) +
                    prevData[right].getMsgLeft(kPrime);

            if (tempMsg < minVal) {
                minVal = tempMsg;
                tempKPrime = kPrime;
                tempDataCost = dataCost(x, y, kPrime);
            }//End of if statement

        }//End of inner loop

        currData[getIndex(x, y, leftImageWidth)].setMsgBottom(k, minVal);

        if (x == 36 && y == 89) {
            cout << "MsgBottom[" << k << "] = " << currData[getIndex(x, y, leftImageWidth)].getMsgBottom(k)<<endl;
//            cout << " [Calculation : ";
//            //cout << dataCost(x, y, tempKPrime) << " + ";
//            cout << tempDataCost << " (dataCost)" << " + ";
//            cout << smoothnessCost(k, tempKPrime) << " (smoothnessCost)" << " + ";
//            cout << prevData[top].getMsgBottom(tempKPrime) << " (MsgTop)" << " + ";
//            cout << prevData[left].getMsgRight(tempKPrime) << " (MsgLeft) " << " + ";
//            cout << prevData[right].getMsgLeft(tempKPrime) << " (MsgRight)";
//            cout << " = " << minVal << "(minVal)]" << endl;
        }//End of if statement
        if (x == 36 && y == 89 && k == 15) {
            cout << endl;
        }// end of if statement

    }//End of outer loop

    //update left message
    for (int k = 0; k <= MAX_OFF_SET; k++) {

        minVal = numeric_limits<int>::max();

        for (int kPrime = 0; kPrime <= MAX_OFF_SET; kPrime++) {

            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[top].getMsgBottom(kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[right].getMsgLeft(kPrime);

            if (tempMsg < minVal) {
                minVal = tempMsg;
                tempKPrime = kPrime;
                tempDataCost = dataCost(x, y, kPrime);
            }//End of if statement 

        }//End of inner loop
        currData[getIndex(x, y, leftImageWidth)].setMsgLeft(k, minVal);
        if (x == 36 && y == 89) {
            cout << "MsgLeft[" << k << "] = " << currData[getIndex(x, y, leftImageWidth)].getMsgLeft(k)<<endl;
//            cout << " [Calculation : ";
//            //cout << dataCost(x, y, tempKPrime) << " + ";
//            cout << tempDataCost << " (dataCost)" << " + ";
//            cout << smoothnessCost(k, tempKPrime) << " (smoothnessCost)" << " + ";
//            cout << prevData[top].getMsgBottom(tempKPrime) << " (MsgTop)" << " + ";
//            cout << prevData[bot].getMsgTop(tempKPrime) << " (MsgBottom) " << " + ";
//            cout << prevData[right].getMsgLeft(tempKPrime) << " (MsgRight)";
//            cout << " = " << minVal << "(minVal)]" << endl;
        }
        if (x == 36 && y == 89 && k == 15) {
            cout << endl;
        }
    }//End of outer loop

    //update right message
    for (int k = 0; k <= MAX_OFF_SET; k++) {

        minVal = numeric_limits<int>::max();

        for (int kPrime = 0; kPrime <= MAX_OFF_SET; kPrime++) {

            int tempMsg = dataCost(x, y, kPrime) + smoothnessCost(k, kPrime) +
                    prevData[top].getMsgBottom(kPrime) +
                    prevData[bot].getMsgTop(kPrime) +
                    prevData[left].getMsgRight(kPrime);

            if (tempMsg < minVal) {
                minVal = tempMsg;
                tempKPrime = kPrime;
                tempDataCost = dataCost(x, y, kPrime);
            }//End of if statement  

        }//End of inner loop
        currData[getIndex(x, y, leftImageWidth)].setMsgRight(k, minVal);

        if (x == 36 && y == 89) {
            cout << "MsgRight[" << k << "] = " << currData[getIndex(x, y, leftImageWidth)].getMsgRight(k)<<endl;
//            cout << " [Calculation : ";
//            //cout << dataCost(x, y, tempKPrime) << " + ";
//            cout << tempDataCost << " (dataCost)" << " + ";
//            cout << smoothnessCost(k, tempKPrime) << " (smoothnessCost)" << " + ";
//            cout << prevData[top].getMsgBottom(tempKPrime) << " (MsgTop)" << " + ";
//            cout << prevData[left].getMsgRight(tempKPrime) << " (MsgLeft) " << " + ";
//            cout << prevData[bot].getMsgTop(tempKPrime) << " (MsgBottom) ";
//            cout << " = " << minVal << "(minVal)]" << endl;
        }//End of if statement
        if (x == 36 && y == 89 && k == 15) {
            cout << endl;
        }//End of if statement
    }//End of outer loop

}// End of updateMessage

//********************************************************************
//Method:	updateBelief
//Description:	this function updates the belief of each of set for every node.
//Parameters:  	x - the x coordinate of a pixel
//              y - the y coordinate of a pixel
//Returns:     	nothing 
//Calls:        getIndex
//              dataCost
//Globals:	leftImageWidth
//              MAX_OFF_SET
//              prevData
//              currData
void updateBelief(int x, int y) {
    float belief;

    int top = getIndex(x, y - 1, leftImageWidth);
    int bot = getIndex(x, y + 1, leftImageWidth);
    int left = getIndex(x - 1, y, leftImageWidth);
    int right = getIndex(x + 1, y, leftImageWidth);

    for (int k = 0; k <= MAX_OFF_SET; k++) {
        belief = dataCost(x, y, k) + prevData[top].getMsgBottom(k) +
                prevData[bot].getMsgTop(k) +
                prevData[left].getMsgRight(k) + prevData[right].getMsgLeft(k);
        currData[getIndex(x, y, leftImageWidth)].setBelief(k, belief);
        if (x == 36 && y == 89) {
            cout << "Belief[" << k << "] = ";
            cout << currData[getIndex(x, y, leftImageWidth)].getBelief(k)<<endl;
            cout << " (Calculation: ";
            cout << dataCost(x, y, k) << "(data cost) + ";
            cout << prevData[top].getMsgBottom(k) << " (MsgTop) + ";
            cout << prevData[bot].getMsgTop(k) << " (MsgBot) + ";
            cout << prevData[left].getMsgRight(k) << " (MsgLeft) + ";
            cout << prevData[right].getMsgLeft(k) << " (MsgRight))" << endl;
        }// End of if statement
    }//End of for loop
    if (x == 36 && y == 89) {
        cout << endl;
        cout << "Waiting on other pixels for current iteration..." << endl;
        cout << endl;
    }
}

//********************************************************************
//Method:	loopyBP
//Description:	this function updates the data at every node for a fixed number 
//              of iterations
//Parameters:  	none
//Returns:     	nothing 
//Calls:        updateMessage
//              updateBelief
//Globals:	leftImageWidth
//              MAX_OFF_SET
//              prevData
//              currData
//              tempData
void loopyBP() {
    cout << "RUNNING Loopy Belief Propagation..." << endl;

    int iterations = 1;

    currData = new node[leftSize];
    prevData = new node[leftSize];
    tempData = new node[leftSize];

    int border = (WINDOW_SIZE / 2) + MAX_OFF_SET;

    cout << endl;
    cout << "Total iterations : " << iterations << endl;

    for (int i = 0; i < iterations; i++) {
        //need to loop through x and y values 
        cout << endl;
        cout << "Updating data for iteration " << i << "... " << endl;
        cout << endl;
        //swap pointers
        tempData = prevData;
        prevData = currData;
        currData = tempData;
        for (int x = border; x <= leftImageWidth - border; x++) {
            for (int y = border; y <= leftImageHeight - border; y++) {
                //update data
                updateMessage(x, y);
                updateBelief(x, y);
            }// End of for loop for y
        }// End of for loop for x
        cout << "Iteration " << i << " COMPLETE" << endl;
    }//End of for loop for iterations

    cout << endl;
    cout << "All iterations are COMPLETE." << endl;
    cout << endl;
    cout << "Loopy Belief Propagation COMPLETE.\n" << endl;
}// End of loopyBP function

//********************************************************************
//Method:	getK
//Description:	this function gets the offset with the lowest associated cost
//Parameters:  	pixel - the current pixel in the picture
//Returns:     	k - the index of the offset 
//Calls:        none
//Globals:	currData
int getK(int pixel) {
    //if (pixel == getIndex(36,89, leftImageWidth)){
        //cout << "getting k..." << endl;
    //}//End of if statement debugging    
    int belief = numeric_limits<int>::max();
    int currentBelief;
    int k;
    for (int kIndex = 0; kIndex <= MAX_OFF_SET; kIndex++) {
        currentBelief = currData[pixel].getBelief(kIndex);
        if (currentBelief < belief) {
            k = kIndex;
            belief = currentBelief;
            //if (pixel == getIndex(36,89, leftImageWidth)){
            //cout << "K = " << k << endl;
            //cout << "belief = " << belief << endl;                
            //}//End of if statement debugging              
        }//End of if statement
    }//End of k for loop
    return k;
}//End of calculateBeliefs

void calculateOutputPixels() {
    cout << "Calculating output pixels..." << endl;
    finalImageArray = new int [leftSize];
    int outputPixel;
    int k;
    for (int x = 0; x < leftImageWidth; x++) {
        for (int y = 0; y < leftImageHeight; y++) {
            int pixel = getIndex(x, y, leftImageWidth);
            //cout << "Pixel("<<x<<","<<y<<")"<<endl;
            k = getK(pixel);
            outputPixel = (255 * k) / MAX_OFF_SET;
            //cout<< "Output = " << outputPixel << endl;         
            finalImageArray[pixel] = outputPixel;
        }//End of y for loop
    }//End of x for loop
    cout << "Calculating output pixels COMPLETE " << endl;
}// end of calculateOutputPixels

void writeFinalDepthMapImage() {
    cout << "Writing Final Depth-Map Image..." << endl;
    ofstream outFile;
    outFile.open("FinalDepthMapImage.pgm");
    if (outFile.is_open()) {
        outFile << "P2" << endl;
        outFile << "# output from LoopBelief Program" << endl;
        outFile << leftImageWidth << " " << leftImageHeight << endl;
        outFile << "255" << endl;
        int i = 0;
        while (i < leftSize) {
            for (int j = 0; j < 17; j++) {
                outFile << (int) finalImageArray[i] << " ";
                i++;
                if (i >= leftSize) break;
            }//End of for loop
            outFile << "\r\n";
        }//End of while loop
        cout << "Output SUCCESSFUL" << endl;
    }else{
        cout << "Output FAILED" << endl;
    }// End of else statement
}// End of writeFinalDepthMapImage

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
    calculateOutputPixels();
    cout << "FinalImageArray: " << endl;
    //wait_on_enter();
    //printArray(finalImageArray, leftSize);
    writeFinalDepthMapImage();
    return 0;
}