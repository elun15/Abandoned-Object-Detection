
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

#include "../../src/blobs/BlobList.h"
#include "../../src/blobs/ObjectBlob.h"

// States of an static object (defined in ObjectBlob.h)
//typedef enum {
//NONE=-1,
//STATIC_OBJ_TYPE_UNKNOWN=0,
//STATIC_OBJ_TYPE_STOLEN=1,
//STATIC_OBJ_TYPE_ABANDONED=2 ,
//STATIC_OBJ_TYPE_LIGHT_CHANGE=3
//} DECISION;

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
	int _writeLog;

	///Debug data flag
	int _debug;

	///Zoom factor to perform the analysis
	double _incFactorBB;

	/// Image with blobs printed
    cv::Mat _resultImage;
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
	int processFrame(cv::Mat frame, cv::Mat bkgImage, cv::Mat staticObjMask, cv::Mat fgMask, BlobList<ObjectBlob*>* objects);

	/**
	* Classifies a static object
	* \param frame current frame
	* \param bkgImage background model image
	* \param staticObjMask static objects mask
	* \param fgMask foreground/background mask
	* \param object static object to check
	* \return Returns a DECISION about the object analyzed
	*/
    virtual int checkObject(cv::Mat frame, cv::Mat bkgImage, cv::Mat staticObjMask, cv::Mat fgMask, ObjectBlob* object){return -1;};

	/**
	* Extract a region (indicated by the roi) from the input image
	* \param src Input frame
	* \param roi Region of interest
	*
	* \return Region extracted as an IplImage
	*/
    cv::Mat extractImage(cv::Mat src, cv::Rect roi);

	/**
	* Method to increase the size of the bounding box
	* \param box Initial region of interest
	* \param factorAmpliacion Increase factor
	* \param img Input image (for checking procedures)
	*
	* \return CvRect structure with the increased region of interest
	*/
    CvRect ampliarBB(CvRect box, double factorAmpliacion, cv::Mat img);

   /**
	* Method to show an image .
	* \param img Image to show
	* \param windowName Name of the window
	*
	* \return S_GH image as IplImage
	*/
    void showImage(char *windowName, cv::Mat img);

	/**
	* Method to print the status of the blobs in an image.
	* \param img Image to use
	* \param pObjList List of blobs to print
	*
	* \return A copy of the input image with the status of the blobs printed
	*/
    cv::Mat printBlobs(cv::Mat frame, BlobList<ObjectBlob *> *pObjList);
};

#endif
