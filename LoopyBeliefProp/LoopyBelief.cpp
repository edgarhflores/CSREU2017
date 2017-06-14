//Program:		LoopyBelief.cpp
//Description:          Single array implementation of loopy belief propagation
//Author:		Edgar Flores
//Started:              6/6/2017
//Revised:		6/12/2017
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
const int WINDOW_SIZE = 5;
const int MAX_OFF_SET = 15;
const int LAMDA = 20;
const int TRUNCATE = 2;
int numOfFiles = 1;
int imageWidthLeft;
int imageWidthRight;
int leftSize;
int rightSize;
int* leftImageArray;
int* rightImageArray;
using namespace std;

//********************************************************************
//Method:	smoothnessCost
//Description:	calculates smoothnessCost
//Parameters:  	
//Returns:     	nothing
//Calls:       	nothing
//Globals:	none
int smoothnessCost(int k, int kPrime){
    int n = abs (k - kPrime);
    int smoothnessCost;
    smoothnessCost = LAMDA * min(n,TRUNCATE);
    return smoothnessCost;
}

//********************************************************************
//Method:	getIndex
//Description:	translates the coordinates of x,y into a single dimensional 
//              coordinate 
//Parameters:  	x - the x coordinate
//              y - the y coordinate
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
//Globals:	none

int dataCost(int x, int y, int label, int leftImage[], int rightImage[]) {
    int sumOfAbsDiff;
    int border = WINDOW_SIZE / 2;
    for (int i = x - border; i <= x + border; i++) {
        for (int j = y - border; j < y + border; j++) {
            sumOfAbsDiff = abs(leftImage[getIndex(i, j, imageWidthLeft)] - rightImage[getIndex(i - label, j, imageWidthRight)]);
        }//inner for loop
    }//outer for loop
    return sumOfAbsDiff;
}

//********************************************************************
//Method:	printArray
//Description:	print array
//Parameters:  	array[] - the array being printed
//              size    - the width * height of an image in a PGM file
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
//Description:	this method calculates the width and height of an image 
//Parameters:  	ifstream& fileObj - this is the current PGM file
//Returns:     	size - the width * height of an image in a PGM file
//Calls:       	nothing
//Globals:	none

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
        imageWidthLeft = imageWidth;
    } else {
        imageWidthRight = imageWidth;
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
        stringstream number(input);
        number >> array[i];
    }
}

//********************************************************************
//Method:	putValuesIntoArray
//Description:	this function asks the user for a PGM file and then calls the
//              appropriate method to build the left and right image array
//Parameters:  	none
//Returns:     	nothing 
//Calls:       	getSize
//              putValuesIntoArray
//Globals:	none

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
                putValuesIntoArray(leftImageArray, fileObj, leftSize);
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
    int x;
    int y;
    int offset = 0;
    cout << "Loopy Belief Propagation: Edgar Flores\n" << endl;
    setFiles();
    while (loop) {
        cout << "1 = Calculate DataCost (default)" << endl;
        cout << "2 = End program" << endl;
        getline(cin, input);
        stringstream(input) >> userAns;
        cout << endl;
        switch (userAns) {
            case 1:
                
//                cout << "Enter value for x: " << endl;
//                getline(cin, input);
//                stringstream(input) >> x;
//                cout << "Enter value for y: " << endl;
//                getline(cin, input);
//                stringstream(input) >> y;
//                cout << "Calculating the dataCost...\n" << endl;
//                int dataCostSum;
//                for (int offset = 0; offset < MAX_OFF_SET; offset++) {
//                    dataCostSum += dataCost(x, y, offset, leftImageArray, rightImageArray);
//                }
//                cout << "Data cost for a window of 5x5: " << dataCostSum << endl;
//                cout << endl;
                break;
            case 2:
                exit(0);
            default:
                cout << "Input not valid. Please enter either 1 or 2.\n" << endl;
        }// end of switch  
    }  
    return 0;
    //Trying out the new git commands
}

class node{
    int grayScaleValue;
    
};