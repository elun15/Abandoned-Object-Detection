//file description
/**
* \file StaticObjectClassifierHist.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 25-03-08
* \brief Implementation of StaticObjectClassifierHist class
* \version 1.0\n
*			Version history:\n
*				- 1.0(25-03-08): Initial Implementation (jcs)
* * \version 2.0\n
*			Version history:\n
*				- 2.0(13/09/17): Adapted to openCV 3.2 (elg)
*
*
*/

#include "../../../src/CLASSIFIER/StaticObjectClassifierHist.h"
#include "../../../src/CLASSIFIER/StaticObjectClassifier.h"
#include <opencv2/opencv.hpp>
//#include "highgui.h"
using namespace std;
using namespace cv;


/**
* Standard class Constructor
*
* \param _debug Flag to activate debug mode
* \param _writelog Flag to write log file
*/
StaticObjectClassifierHist::StaticObjectClassifierHist(int _debug, int _writelog)
{
    // Asignar valor a prametros de funcionamiento

    incFactorBB = 1.5;

    //nDimsHist = 3; // Histogramas en 3 dimensiones (RGB?)
    nDimsHist = 1; // Histogramas en 1 dimension (HUE?)
    nBins_hist = 256;


    //compare_hist_method = CV_COMP_CORREL;
    //compare_hist_method = CV_COMP_CHISQR;
    //compare_hist_method = CV_COMP_INTERSECT;

    compare_hist_method = CV_COMP_BHATTACHARYYA;
    thHist_bkg_change = 0.250;

    need_to_init = true;

    writeLog = WRITE_LOG_DEF;
    if (writeLog == 1)
    {
        FILE *pf = fopen("Results_Color.txt", "w+");
        fprintf(pf, "FRAME\tID\tSCORE\n");
        fclose(pf);
    }
}

/**
* Standard class destructor
*
*/
StaticObjectClassifierHist::~StaticObjectClassifierHist()
{
    if (need_to_init == false)
    {

        hsvImg.release();
        hueImg.release();

        bkgHist.release();
        oldObjHist.release();
        newObjHist.release();

        free(hdims);

    }

}

/**
* Initialization resources
*
* /param sampleFrame Image to get properties of the data
*/
void StaticObjectClassifierHist::init(Mat sampleFrame)
{

    hsvImg= sampleFrame.clone();

    hueImg = Mat(sampleFrame.rows,sampleFrame.cols,CV_8UC1);

    // Histogram initialization

    hdims = (int*)malloc(nDimsHist * sizeof(int));
    for (int i = 0; i < nDimsHist; i++){
        hdims[i] = nBins_hist;
    }

    need_to_init = false;
}

/**
* Classifies all static objects in the list
* \param frame current frame
* \param bkgImage background model image
* \param staticObjMask static objects mask
* \param fgMask foreground/background mask
* \param objects static object list
* \param nObj number of static objects in the list
* \return number of objects classified as uncovered or covered background
*/
int StaticObjectClassifierHist::processFrame(Mat frame, Mat bkgImage2, Mat staticObjMask, Mat fgMask, BlobList<ObjectBlob*>* objectList)
{

  Mat bkgImage = bkgImage2.clone();
    if (need_to_init == true){

        init(frame);
    }



    for (int i = 0; i < objectList->getBlobNum(); i++){

        ObjectBlob *obj;
        obj = objectList->getBlob(i);
        checkObject(frame, bkgImage, staticObjMask, fgMask,obj);
    }


    return 0;
}

/**
* Classifies a static object
* \param frame current frame
* \param bkgImage background model image
* \param staticObjMask static objects mask
* \param fgMask foreground/background mask
* \param object static object to check
* \return Returns a DECISION about the object analyzed
*/
int StaticObjectClassifierHist::checkObject(Mat frame, Mat bkgImage, Mat staticObjMask, Mat fgMask, ObjectBlob *_object)
{

    CvRect Bbox = cvRect((int)_object->x, (int)_object->y, (int)_object->w, (int)_object->h);

    // Ampliar la BB

    checkBB = ampliarBB(Bbox, incFactorBB, frame);

    // Calcular Histogramas
    calcularHistogramas(frame, bkgImage, staticObjMask, fgMask, checkBB);

    double distOldObj = abs(compareHist(bkgHist, oldObjHist, CV_COMP_BHATTACHARYYA));
    double distNewObj = abs(compareHist(bkgHist, newObjHist, CV_COMP_BHATTACHARYYA));
    double distOldNew = abs(compareHist(oldObjHist, newObjHist, CV_COMP_BHATTACHARYYA));


    int decision = STATIC_OBJ_TYPE_UNKNOWN;



    if (distOldNew < thHist_bkg_change)
    {
        _object->results->D_CH = STATIC_OBJ_TYPE_LIGHT_CHANGE; //Color Hist decision
        decision = STATIC_OBJ_TYPE_LIGHT_CHANGE;
        if (writeLog == 1){
            printf("bkg_changed");
        }

    }
    else
        if (distOldObj < distNewObj)
        {
            _object->results->D_CH = STATIC_OBJ_TYPE_ABANDONED;
            decision = STATIC_OBJ_TYPE_ABANDONED;
            //cout << "Decision: Abandoned" << endl;
            if (writeLog == 1){
                printf("bkg_covered(aband.)");
            }

        }
        else
        {
            _object->results->D_CH = STATIC_OBJ_TYPE_STOLEN;
            decision = STATIC_OBJ_TYPE_STOLEN;
            //cout << "Decision: Stolen" << endl;
            if (writeLog == 1){
                printf("bkg_uncovered(robo)");

            }

        }



    _object->results->S_CH = distOldObj - distNewObj; //Score
    _object->results->S_CH_dN = distNewObj; //Distances
    _object->results->S_CH_dO = distOldObj;
    _object->results->S_CH_dON = distOldNew;


    _object->results->D_F = _object->results->D_CH; //Final decision

    return decision;
}

Mat StaticObjectClassifierHist::calcularHistograma(Mat img, Rect roi, Mat objMask, int inside_mask)
{
    Mat hist;
    int histSize = 256;    // bin size
    float range[] = { 0, 256 };
    const float *ranges[] = { range };


    img.copyTo(hsvImg);

    // Si la imagen es grayscale, usarla tal cual
    if (hsvImg.channels() == 1)
    {
            hsvImg.copyTo(hueImg);
    }

    // Si la imagen esta en color, pasarla a HSV y quedarse con el canal hue
    else
    {
        cvtColor(hsvImg, hsvImg, CV_BGR2HSV);
        Mat channels[3];
        split(hsvImg, channels);
        hueImg = channels[2];

   }

    Mat hueImg_ROI = hueImg(roi);
    Mat objMask_ROI = objMask(roi);


    if (inside_mask){

        calcHist( &hueImg_ROI, 1, 0,objMask_ROI, hist, 1, &histSize, ranges, true, false );

    }

    else
    {

        calcHist( &hueImg_ROI, 1, 0,~objMask_ROI, hist, 1, &histSize, ranges, true, false );

    }


    histImage = Mat::ones(hist.rows, hist.cols, CV_8U)*255;
    normalize(hist,hist,0, histImage.rows, NORM_MINMAX, -1);

    return hist;
}


int StaticObjectClassifierHist::calcularHistogramas(Mat frame, Mat bkgImage, Mat staticObjMask, Mat fgMask, Rect roi) //, IplImage *objMask)
{
    //bkgHist: Hist. of the area inside the blob not corresponding to the object in the bkg image
    bkgHist = calcularHistograma(bkgImage, roi, staticObjMask, 0);
    //newObjHist: Hist. of the area inside the blob corresponding to the object in the frame
    newObjHist=  calcularHistograma(frame, roi, staticObjMask, 1);
    //newObjHist: Hist. of the area inside the blob corresponding to the object in the bkg image
    oldObjHist = calcularHistograma(bkgImage , roi, staticObjMask, 1);

    return 0;
}

void StaticObjectClassifierHist::showHistogram(Mat hist, char *windowName)
{

    histImage = Scalar::all(255);

    imshow(windowName,histImage);
    waitKey();

    histImage.release();

}

Scalar StaticObjectClassifierHist::hsv2rgb(float hue)
{
    int rgb[3], p, sector;
    static const int sector_data[][3] =
    { { 0, 2, 1 }, { 1, 2, 0 }, { 1, 0, 2 }, { 2, 0, 1 }, { 2, 1, 0 }, { 0, 1, 2 } };
    hue *= 0.033333333333333333333333333333333f;
    sector = floor(hue);
    p = round(255 * (hue - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return Scalar(rgb[2], rgb[1], rgb[0], 0);
}
