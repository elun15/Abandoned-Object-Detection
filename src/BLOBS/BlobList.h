/**
* \class BlobList
* \brief Class to describe a list of blobs
*
*
*/

#pragma once
//#ifndef BLOBLIST_H_INCLUDE
#define BLOBLIST_H_INCLUDE

// Incluimos las librerias necesarias
#include <vector>

using namespace std;

template <class T>
class BlobList {
private:
	vector<T> *pBlob;

public:
	// Class Constructor
	BlobList();

	// Extended Class constructor (with copy)
	BlobList(vector<T> *pBlob);

	//Class destructor
	virtual ~BlobList();

	// Method to add a blob to the list
	int addBlob(T blob);

	// Method to clear all blobs in the list
	int clear();

	// Method to delete a blob by using its position
	int delBlob(int n);

	// Method to delete a blob by using its ID
	int delBlobByID(int ID);

	// Method to get the number of blobs stored in the list
	int getBlobNum();

	// Method to get a blob by using its position
	T getBlob(int n);

	// Method to get a blob by using its ID
	T getBlobByID(int ID);

	// Method to set a blob in a certain position
	int setBlob(int n, T Blob);

	// Method to set a blob in a certain position (?)
	int setBlobByID(int ID, T Blob);

	// Method to get the vector
	vector<T>* getPBlob();

	//Prints some features of all the blobs in a file
	int write(char *filename);
};

#include "BlobList.cpp"

//#endif
