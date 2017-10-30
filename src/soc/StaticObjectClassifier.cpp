
//file description
/**
* \file StaticObjectClassifier.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 25-03-08
* \brief Implementation of StaticObjectClassifier class
* \version 1.0\n
*			Version history:\n
*				- 1.0(23/09/08): Initial Implementation (jcs)
*
*/

#include "StaticObjectClassifier.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
* Standard class Constructor
*
* \param _debug Flag to activate debug mode
* \param _writelog Flag to write log file
*/
StaticObjectClassifier::StaticObjectClassifier(int debug, int writelog)
{
    _debug = debug;
    _writeLog = writelog;
    _incFactorBB = 1.1;

    _resultImage.release();
}

/**
* Destructor
*/
StaticObjectClassifier::~StaticObjectClassifier()
{
    if (!_resultImage.empty())
        _resultImage.release();
}

/**
* Method to increase the size of the bounding box
* \param box Initial region of interest
* \param factorAmpliacion Increase factor
* \param img Input image (for checking procedures)
*
* \return CvRect structure with the increased region of interest
*/
CvRect StaticObjectClassifier::ampliarBB(CvRect box, double factorAmpliacion, Mat img)
{

    //	CvRect newBox;
    int margen_x = (int)floor(((box.width*factorAmpliacion) - box.width) / 2);
    int margen_y = (int)floor(((box.height*factorAmpliacion) - box.height) / 2);

    int bb_x = box.x - margen_x;
    if (bb_x < 1)
        bb_x = 1;


    int bb_width = (int)floor(box.width * factorAmpliacion);
    if (bb_x + bb_width > img.cols)
        bb_width = img.cols - bb_x;

    int bb_y = box.y - margen_y;
    if (bb_y < 1)
        bb_y = 1;

    int bb_height = (int)floor(box.height * factorAmpliacion);
    if (bb_y + bb_height > img.rows)
        bb_height = img.rows - bb_y;

    //	CvRect checkBB = cvRect(bb_x1, bb_y1, floor(bb_width*incFactorBB), floor(bb_height*incFactorBB));
    CvRect newBB = cvRect(bb_x, bb_y, bb_width, bb_height);

    return newBB;

}

/**
* Extract a region (indicated by the roi) from the input image
* \param src Input frame
* \param roi Region of interest
*
* \return Region extracted as an IplImage
*/
Mat StaticObjectClassifier::extractImage(Mat src, cv::Rect roi)
{
    Mat objImage = Mat(roi.height,roi.width,src.depth(),src.channels());

    if (roi.x < 1 || roi.y < 1 || ((roi.x + roi.width) > src.cols) || ((roi.y + roi.height) > src.rows))
        objImage.release();
    else
        objImage=src(roi);

    return objImage;
}
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
int StaticObjectClassifier::processFrame(Mat frame, Mat bkgImage, Mat staticObjMask, Mat fgMask, BlobList<ObjectBlob*>* objectList)
{
	for (int j = 0; j <  objectList->getBlobNum(); j++) {

		//get the object to analyze
		ObjectBlob *object = objectList->getBlob(j);

		//process object
		checkObject(frame,bkgImage,staticObjMask,fgMask, object);
	}
	return 1;
}

/**
* Method to show an image .
* \param img Image to show
* \param windowName Name of the window
*
* \return S_GH image as IplImage
*/
void StaticObjectClassifier::showImage(char *windowName, Mat img)
{
    if (!img.empty())
    {
        imshow(windowName,img);
        waitKey(1);
    }
}

/**
* Method to print the status of the blobs in an image.
* \param img Image to use
* \param pObjList List of blobs to print
*
* \return A copy of the input image with the status of the blobs printed
*/
Mat StaticObjectClassifier::printBlobs(Mat frame, BlobList<ObjectBlob *> *pObjList)
{
    if (!_resultImage.empty())
        _resultImage.release();

    if (!frame.empty())
        this->_resultImage = Mat(frame.rows, frame.cols,frame.depth(),frame.channels());

    //gray scale

    //cvCopy(frame, _resultImage);

    frame.copyTo(_resultImage);

    CvScalar color = cvScalar(255, 255, 255, 0);
    char string[15] = "";
    //CvFont* display_font = new CvFont();
    //cvInitFont(display_font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);

    for (int i = 0; i<pObjList->getBlobNum(); i++)
    {
        ObjectBlob *pB = pObjList->getBlob(i);

        //print blob and state in the image
        switch (pB->results->D_F)
        {
        case NONE:
            color = Scalar(255, 255, 255, 0);
            sprintf(string, "None");
            break;
        case STATIC_OBJ_TYPE_UNKNOWN:
            color = Scalar(127, 127, 127, 0);
            sprintf(string, "Unknown");
            putText(_resultImage,string,Point((pB->x)-5,(pB->y)-5),CV_FONT_HERSHEY_SIMPLEX,0.5,color,2,8);

            //cvPutText(_resultImage, strNumFrame, cvPoint(0, frame.rows - 35), display_font, color);
            break;
        case STATIC_OBJ_TYPE_STOLEN:
            color = Scalar(255, 0, 0, 0);
            sprintf(string, "Stolen");
            putText(_resultImage,string,Point((pB->x)-5,(pB->y)-5),CV_FONT_HERSHEY_SIMPLEX,0.5,color,2,8);
            break;
        case STATIC_OBJ_TYPE_ABANDONED:
            color = Scalar(0, 255, 0, 0);
            sprintf(string, "Abandoned");
            putText(_resultImage,string,Point((pB->x)-5,(pB->y)-5),CV_FONT_HERSHEY_SIMPLEX,0.5,color,2,8);
            break;
        case STATIC_OBJ_TYPE_LIGHT_CHANGE:
            color = Scalar(0, 0, 255, 0);
            sprintf(string, "Light Change");
            putText(_resultImage,string,Point(frame.rows - 50),CV_FONT_HERSHEY_SIMPLEX,0.5,color,1,8);
            break;
        }

        //cvRectangle(_resultImage, cvPoint((int)pB->getBlob()->x, (int)pB->getBlob()->y), cvPoint((int)(pB->getBlob()->x + pB->getBlob()->w), (int)(pB->getBlob()->y + pB->getBlob()->h)), color, 1, 8, 0);
        //cvRectangle(_resultImage, cvPoint((int)pB->x, (int)pB->y), cvPoint(((int)(pB->x + pB->w)), (int)(pB->y + pB->h)), color, 1, 8, 0);


        rectangle(_resultImage, Point((int)pB->x,(int)pB->y), Point((int)(pB->x + pB->w),(int)(pB->y + pB->h)),color,2,8,0);
    }

    //delete display_font;

    return _resultImage;
}
