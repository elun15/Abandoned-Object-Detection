
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

#include "../../src/soc/StaticObjectClassifier.h"

//class description
/**
* \class StaticObjectClassifierHist
* \brief Class to determine the state of a stationary blob using colour inforcv::Mation (histogram)
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

   	///Method to compare hist
	int compare_hist_method;

	//Threshold
	double thHist_bkg_change;

	bool need_to_init;

	///Input image in HSV colour space
    cv::Mat hsvImg;
	///Hue channel of the Input image in HSV colour space
    cv::Mat hueImg;

    cv::Mat histImage;

    cv::Mat bkgHist; 	// Histograma de modelo de background en el area FUERA de la mascara del objeto
    cv::Mat oldObjHist; // Histograma de modelo de background en el area DENTRO de la mascara del objeto
    cv::Mat newObjHist; // Histograma de frame actual en el area DENTRO de la mascara del objeto

public:

	//Standard class Constructor
	StaticObjectClassifierHist(int debug = DEBUG_DEF, int writelog = WRITE_LOG_DEF);

	//Standard class Destructor
	~StaticObjectClassifierHist();

	//Inizialization method	 
    void init(cv::Mat sampleFrame);

    /// Classifies a static object between abandoned and stolen
    int checkObject(cv::Mat frame, cv::Mat bkgImage, cv::Mat staticObjMask,cv::Mat fgMask, ObjectBlob* object);

private:
	/// Method to compute each histogram
    //int calcularHistograma(cv::Mat img, CvHistogram *hist, Rect roi, cv::Mat objMask, int inside_mask);
    cv::Mat calcularHistograma(cv::Mat img, cv::Rect roi, cv::Mat objMask, int inside_mask);

	/// Method to compute all the histograms
    int calcularHistogramas(cv::Mat frame, cv::Mat bkgImage,cv::Mat staticObjMask, cv::Mat fgMask, cv::Rect roi);

	/// Method to show a histograms
    //void showHistogram(CvHistogram *hist, char *windowName);
    void showHistogram(cv::Mat hist, char *windowName);

	/// Method to convert HSV to RGB
    cv::Scalar hsv2rgb(float hue);
};

#endif
