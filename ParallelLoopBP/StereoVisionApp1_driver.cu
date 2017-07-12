#ifndef __STEREOVISIONAPP_DRIVER_CU
#define __STEREOVISIONAPP_DRIVER_CU

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>
#include "./tests/stereoVisionAppData.h"
#include "../codegenInput/UpdateApp.cuh"
#include "../codegenInput/EnergyApp.cuh"
#define ifL "/home/research/e.flores/StereoVision/tutorial-tsukuba-imL.pgm"
#define ifR "/home/research/e.flores/StereoVision/tutorial-tsukuba-imR.pgm"
#define of "/home/research/e.flores/StereoVision/tskuba-output.pgm"
#define convergence -0.001
#define numLabels 16
#define windowSize 5
#define lambda 20
#define truncate 2

int width, height, numPixels;

void importPGM(string filePath, unsigned char** pixels);
void setDC(int x, int y, int label, float val, float** DCArray);
float getDC(int x, int y, int label, float** DCArray);

void run_stereoVisionApp()
{
	unsigned char* leftPixels, * rightPixels;

	// import the images
	importPGM(ifL, &leftPixels);
	importPGM(ifR, &rightPixels);

	// calculate borders
	int leftBorder = (numLabels - 1) + (windowSize / 2);
	int rightBorder = width - (windowSize / 2) - 1;
	int topBorder = windowSize / 2;
	int bottomBorder = height - (windowSize / 2) - 1;

	// allocate memory
	float* currMsg;
	cudaMallocManaged(&currMsg, 4*numLabels*numPixels*sizeof(float));
	currMsg = new float[4*numLabels*numPixels];

	float* prevMsg;
	cudaMallocManaged(&prevMsg, 4*numLabels*numPixels*sizeof(float));
	prevMsg = new float[4*numLabels*numPixels];

	float* belief;
	cudaMallocManaged(&belief, numLabels*numPixels*sizeof(float));
	belief = new float[numLabels*numPixels];

	float* DC;
	cudaMallocManaged(&DC, numLabels*numPixels*sizeof(float));
	DC = new float[numLabels*numPixels];

	// memoize data costs
	for (int x = leftBorder; x <= rightBorder; x++) {
		for (int y = topBorder; y <= bottomBorder; y++) {
			for (int k = 0; k < numLabels; k++) {
				int sum = 0;
				int absDiff;
				for (int i = x - windowSize / 2; i <= x + windowSize / 2; i++) {
					for (int j = y - windowSize / 2; j <= y + windowSize / 2; j++) {
						absDiff = abs((int) leftPixels[j*width+i]
							- (int) rightPixels[(j-k)*width+i]);
						sum += absDiff;
					}
				}
				setDC(x, y, k, (float) sum, &DC);
			}
		}
	}

    // print out all DC vectors for debugging
	for (int x = leftBorder; x <= rightBorder; x++) {
		for (int y = topBorder; y <= bottomBorder; y++) {
			if (x%100==0 && y%100==0) {
				std::cout << "(" << x << "," << y << ")\t[";
				for (int k = 0; k < numLabels; k++) {
					std::cout << getDC(x, y, k, &DC) << ", ";
				}//End of for
				std::cout << "]\n";
			}//End of if
		}//End of for
	}//End of for

	// set up buffers
	int* inBufferData;

	//Would I need to create another inBufferData variable? 
	//Would I need to create another InputBuffer?
	//If how would I deal with therons calculations (updated belief) output to use for my input of my calcualtions (energy calcuations)

	cudaMallocManaged(&inBufferData, numPixels*sizeof(int));

	Mercator::InputBuffer<int>* inBuffer = new Mercator::InputBuffer<int>(inBufferData, numPixels);

	Mercator::OutputBuffer<int>* outBuffer = new Mercator::OutputBuffer<int>(numPixels);

	Mercator::OutputBuffer<int>∗ outBuffer2 = new Mercator::OutputBuffer<int >(OUT_BUFFER_CAPACITY1); //How to know how much of a out buffer capacity I need?
	//do I say inBufferData?

	for (int x = leftBorder; x <= rightBorder; x++) {
		for (int y = topBorder; y <= bottomBorder; y++) {
			inBuffer->add(y*width+x);
		}
	}

	// create app object
	UpdateApp* updateApp = new UpdateApp();

	//create energyApp object
	EnergyApp* energyApp = new EnergyApp();

	// set app-level data
	StereoVisionAppData* appData = new StereoVisionAppData(
		currMsg, prevMsg,
		belief,
		DC,
		numLabels,
		lambda, truncate,
		width, height
		);
	updateApp->set_userData(appData);
	energyApp-> set_userData(energyApp);

	// associate buffers with nodes

	updateApp->sourceNode->set_inBuffer(inBuffer);
	updateApp->sinkNode->set_outBuffer(outBuffer);

	updateApp->run();
	energyApp->run();

	std::cout << "App run completed" << endl;

//   synthApp0->run();

//   std::cout << "SynthApp0 finished.\n" ;

//   // print contents of output buffer
// #if PRINT_OUTPUT_BUFFERS
//   std::cout << " Output buffers: \n" ;

//   int* outData1 = outBuffer1->get_data();
//   printf("SynthApp0, OutBuffer1 (%p):\n", outBuffer1);
//   for(int i=0; i < outBuffer1->size(); ++i)
//   	printf("[%d]: %d\n", i, outData1[i]);

//   int* outData2 = outBuffer2->get_data();
//   printf("SynthApp0, OutBuffer2 (%p):\n", outBuffer2);
//   for(int i=0; i < outBuffer2->size(); ++i)
//   	printf("[%d]: %d\n", i, outData2[i]);

//   int* outData3 = outBuffer3->get_data();
//   printf("SynthApp0, OutBuffer3 (%p):\n", outBuffer3);
//   for(int i=0; i < outBuffer3->size(); ++i)
//   	printf("[%d]: %d\n", i, outData3[i]);
// #endif   // print contents of output buffer

//   // cleanup
//   cudaFree(inBufferData);
//   cudaFree(synthApp0);

}

void importPGM(string filePath, unsigned char** pixels) {
	string line;
	ifstream file(filePath.c_str());
	if (file.is_open()) {
		getline(file, line);
		line = line.substr(0, 2);
		if (line.compare("P2") != 0) {
			std::cout << "Incorrect file type" << endl;
			exit(EXIT_FAILURE);
		}
		getline(file, line);
		stringstream ss(line);
		char c;
		int ifWidth, ifHeight;
		ss >> c;
        if (c == '#') { // ignore this line of the file
        	getline(file, line);
        	ss.str(line);
        	ss >> ifWidth >> ifHeight;
        }
        else { // that char is actually the width
        	ifWidth = (int) c;
        	ss >> ifHeight;
        }
        width = ifWidth;
        height = ifHeight;
        std::cout << "Width: " << width << " Height: " << height << endl;
        numPixels = width * height;
        std::cout << "numPixels: " << numPixels << endl;
        *pixels = new unsigned char[numPixels];
        getline(file, line); // read maximum gray value
        int pixel;
        int count = 0;
        while (!file.eof()) {
        	getline(file, line);
        	ss.clear();
        	ss.str(line);
        	while (ss >> pixel) {
        		(*pixels)[count] = pixel;
        		count++;
        	}
        }
        std::cout << "Import successful" << endl << endl;
    }
    else {
    	std::cout << "Import failed" << endl;
    	exit(EXIT_FAILURE);
    }
}
void setDC(int x, int y, int label, float val, float** DCArray) {
	(*DCArray)[(numPixels*label) + (y*width+x)] = val;
}
float getDC(int x, int y, int label, float** DCArray) {
	return (*DCArray)[(numPixels*label) + (y*width+x)];
}
#endif