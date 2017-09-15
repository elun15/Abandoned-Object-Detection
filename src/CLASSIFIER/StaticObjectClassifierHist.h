
//file description
/**
* \file StaticObjectClassifierHist.h
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 25-03-08
* \brief Header of StaticObjectClassifierHist class
* \version 1.0\n
*			Version history:\n
*				- 1.0(25-03-08): Initial Implementation (jcs)
*
*/

#ifndef _STATICOBJECTCLASSIFIERHIST_INCLUDED_
#define _STATICOBJECTCLASSIFIERHIST_INCLUDED_

#include "StaticObjectClassifier.h"

//class description
/**
* \class StaticObjectClassifierHist
* \brief Class to determine the state of a stationary blob using colour information (histogram)
*
*/
class StaticObjectClassifierHist : public StaticObjectClassifier
{
public:


	///Number of dims of the histogram
	int nDimsHist;

	///Number of bins of the histogram
	int nBins_hist;

	///hdims
	int *hdims;

    //histogram range
    /*COMENTED
    float range[];
    const float* histRange;*/
  // const float* histRange;

	///Method to compare hist
	int compare_hist_method;

	//Threshold
	double thHist_bkg_change;

	bool need_to_init;
    CvMemStorage *storage; //not used

	///Input image in HSV colour space
    Mat hsvImg;
	///Hue channel of the Input image in HSV colour space
    Mat hueImg;

    Mat histImage ;

    /// Histograma de modelo de background en el area FUERA de la mascara del objeto
    //CvHistogram *bkgHist;
    Mat bkgHist;

    /// Histograma de modelo de background en el area DENTRO de la mascara del objeto
    //CvHistogram *oldObjHist;
    Mat oldObjHist;

	/// Histograma de frame actual en el area DENTRO de la mascara del objeto
    //CvHistogram *newObjHist;
    Mat newObjHist;

public:

	//Standard class Constructor
	StaticObjectClassifierHist(int debug = DEBUG_DEF, int writelog = WRITE_LOG_DEF);

	//Standard class Destructor
	virtual ~StaticObjectClassifierHist();

	//Inizialization method	 
    void init(Mat sampleFrame);

	/// Computation of colour-histogram matching similarity measure for all the objects
    int processFrame(Mat frame, Mat bkgImage, Mat staticObjMask,Mat fgMask, BlobList<ObjectBlob*>* objects);

	/// Method to check each object
    int checkObject(Mat frame, Mat bkgImage, Mat staticObjMask,Mat fgMask, ObjectBlob* object);

	/// Method to compute each histogram
    //int calcularHistograma(Mat img, CvHistogram *hist, Rect roi, Mat objMask, int inside_mask);
    Mat calcularHistograma(Mat img, Rect roi, Mat objMask, int inside_mask);

	/// Method to compute all the histograms
    int calcularHistogramas(Mat frame, Mat bkgImage,Mat staticObjMask, Mat fgMask, Rect roi);

	/// Method to show a histograms
    //void showHistogram(CvHistogram *hist, char *windowName);
    void showHistogram(Mat hist, char *windowName);


	/// Method to convert HSV to RGB
    Scalar hsv2rgb(float hue);
};

#endif
