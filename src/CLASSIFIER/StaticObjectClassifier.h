
//file description
/**
* \file StaticObjectClassifier.h
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 25-03-08
* \brief Header of StaticObjectClassifier class
* \version 1.0\n
*			Version history:\n
*				- 1.0(23/09/08): Initial Implementation (jcs)
*
*/

//class description
/**
* \class StaticObjectClassifier
* \brief Class to determine the state of a stationary blob
*
*/

#ifndef _STATICOBJECTCLASSIFIER_INCLUDED_
#define _STATICOBJECTCLASSIFIER_INCLUDED_

// Additional includes 
#include <opencv2/opencv.hpp>

#include "../../../src/BLOBS/BlobList.h"
#include "../../../src/BLOBS/ObjectBlob.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;


// States of an static object (defined in ObjectBlob.h)
/*typedef enum {
NONE=-1,
STATIC_OBJ_TYPE_UNKNOWN=0,
STATIC_OBJ_TYPE_STOLEN=1,
STATIC_OBJ_TYPE_ABANDONED=2 ,
STATIC_OBJ_TYPE_LIGHT_CHANGE=3
} DECISION;/**/

///Debug flag (write log data)
const int WRITE_LOG_DEF = 0; // for no log-writting
//const int WRITE_LOG_DEF = 1; // for log-writting


///Debug flag (output data)
const int DEBUG_DEF = 0; // data on screen
//const int DEBUG_DEF = 0; // no data on screen



class StaticObjectClassifier
{
protected:

	///write log flag
	int writeLog;

	///Debug data flag
	int debug;

	///Zoom factor to perform the analysis
	double incFactorBB;

	/// Image with blobs printed
    Mat resultImage;

public:
	/// Bounding box to check
	CvRect checkBB;

public:
	/**
	* Constructor
	*/
	StaticObjectClassifier(int debug = DEBUG_DEF, int writeLog = WRITE_LOG_DEF);

	/**
	* Destructor
	*/
	virtual ~StaticObjectClassifier();

	/**
	* Classifies all static objects in a list
	* \param frame current frame
	* \param bkgImage background model image
	* \param staticObjMask static objects mask
	* \param fgMask foreground/background mask
	* \param objects static object list
	* \param nObj number of static objects in the list
	* \return number of objects classified as uncovered or covered background
	*/
    virtual int processFrame(Mat frame,Mat bkgImage, Mat staticObjMask, Mat fgMask, BlobList<ObjectBlob*>* objects, int _nObj);

    /////
   virtual int classifyBlobStationary(Mat frame, Mat bkg, Mat sfgmask);
    /////

	/**
	* Classifies a static object
	* \param frame current frame
	* \param bkgImage background model image
	* \param staticObjMask static objects mask
	* \param fgMask foreground/background mask
	* \param object static object to check
	* \return Returns a DECISION about the object analyzed
	*/
    virtual int checkObject(Mat frame, Mat bkgImage, Mat staticObjMask, Mat fgMask, ObjectBlob* object);

	/**
	* Extract a region (indicated by the roi) from the input image
	* \param src Input frame
	* \param roi Region of interest
	*
	* \return Region extracted as an IplImage
	*/
    Mat extractImage(Mat src, CvRect roi);


	/**
	* Method to increase the size of the bounding box
	* \param box Initial region of interest
	* \param factorAmpliacion Increase factor
	* \param img Input image (for checking procedures)
	*
	* \return CvRect structure with the increased region of interest
	*/
    CvRect ampliarBB(CvRect box, double factorAmpliacion, Mat img);

	/**
	* Method to compute the S_GH in the x and y dimmensions. Currently, Sobel operator is applied in the two dims.
	* \param src Color or gray image to process
	* \param order Order of the S_GH operation
	*
	* \return S_GH image as IplImage
	*/
    Mat GradientXYSobel(Mat src, int order);

	/**
	* Method to show an image .
	* \param img Image to show
	* \param windowName Name of the window
	*
	* \return S_GH image as IplImage
	*/
    void showImage(char *windowName, Mat img);

	/**
	* Method to print the status of the blobs in an image.
	* \param img Image to use
	* \param pObjList List of blobs to print
	*
	* \return A copy of the input image with the status of the blobs printed
	*/
    Mat printBlobs(Mat frame, BlobList<ObjectBlob *> *pObjList);





};

#endif
