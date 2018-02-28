/************************************************************************************/
/* Archivo de la clase MyCLSSFRSelector:                                            */
/* Esta clase nos permite seleccionar la tecnica a utilizar en la etapa de          */
/* clasificacion robo/abandono del sistema.                                         */
/************************************************************************************/

#include <opencv2/opencv.hpp>
#include "SOCselector.h"
#include "../utils/utils.h"
#include <iostream>   // std::cout
#include <string>     // std::string

using namespace std;
using namespace cv;
/*
 * \brief Default constructor
 */
SOCselector::SOCselector()
{
    cout << "Default parameters:" << endl;
    initialize(SOC_HGR, false, false, NULL, -1);
}


/*
 * \brief Constructor with selected BGS algorithm
 * \param PDid Identifier for the selected algorithm
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
SOCselector::SOCselector(SOC_type SOCid, bool display, bool saveIMG, const char* savePathDir, int saveCounter)
{
    initialize(SOCid, display, saveIMG, savePathDir, saveCounter);
}

/*
 * \brief Default destructor
 */
SOCselector::~SOCselector()
{
    std::cout << "~SOCSelector()" << std::endl;
}

/*
 * \brief Method to initialize variables
 * \param SOCid Identifier for the selected algorithm
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
void SOCselector::initialize(SOC_type SOCid, bool display, bool saveIMG, const char* savePathDir, int saveCounter)
{
    this->_SOCid = SOCid;
    this->_SOCtype_str =  create_SOCtype_str(); //create strings to display the name of each method
    this->_display = display;
    this->_saveIMG = saveIMG;
    this->_savePathDir = savePathDir;
    this->_saveCounter = saveCounter;

    this->_pHGR = NULL;
    this->_pHist = NULL;
    this->_pPCC = NULL;

    cout << "PDSelector(): " << _SOCtype_str[_SOCid] << " selected" << endl;
}

void SOCselector::init()
{
    switch (_SOCid)
    {
    case SOC_HGR:
        this->_pHGR = new StaticObjectClassifierHighGradient();
        break;
    case SOC_HIST:
        this->_pHist = new StaticObjectClassifierHist();
        break;
    case SOC_PCC:
        this->_pPCC = new StaticObjectClassifierPCC();
        break;
    default:
        break;
    }

}

Mat SOCselector::process(cv::Mat frame, cv::Mat bkg, cv::Mat sfgmask, cv::Mat fgmask, BlobList<ObjectBlob*>* objects, int counter)
{
    Mat result, show_hist;
    Mat sfgmaskTemp=sfgmask.clone();

    //structuring element to apply dilation&erosion
    Mat element = getStructuringElement(MORPH_RECT, Size(3,3));

    //remove noise
    erode(sfgmaskTemp, sfgmaskTemp,element);
    dilate(sfgmaskTemp, sfgmaskTemp,element);

    //fill holes
    dilate(sfgmaskTemp, sfgmaskTemp,element);
    dilate(sfgmaskTemp, sfgmaskTemp,element);
    erode(sfgmaskTemp, sfgmaskTemp,element);
    dilate(sfgmaskTemp, sfgmaskTemp,element);

    //binarization
    threshold(sfgmaskTemp, sfgmaskTemp, 250, 255, CV_THRESH_BINARY);

    switch(_SOCid)
    {
    case SOC_HGR:
        _pHGR->processFrame(frame,bkg,sfgmaskTemp,fgmask,objects);
        result = (_pHGR->printBlobs(frame, objects)).clone();
        break;

    case SOC_HIST:
        _pHist->processFrame(frame,bkg,sfgmaskTemp,fgmask,objects);
        result = (_pHist->printBlobs(frame, objects)).clone();


        break;

    case SOC_PCC:
        _pPCC->processFrame(frame,bkg,sfgmaskTemp,fgmask,objects);
        result = (_pPCC->printBlobs(frame, objects)).clone();
        break;
    default:
        break;
    }

    //For display
    string namewindow;
    string str = "StaticClassifier: " + _SOCtype_str[_SOCid];
    namewindow = str.c_str();

    if (_display)
    {
        if (counter>0)
            putText(result,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

        imshow(namewindow,result);
        waitKey(1);
    }


   if (_saveIMG && (counter >= 2000)  )
    {
        if (counter>0){

            //putText(result,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            std::string d1 = _savePathDir + "SOC" + to_string_(counter,5) + ".jpg";
            imwrite(d1,result);
        }
    }
    return result;
}
