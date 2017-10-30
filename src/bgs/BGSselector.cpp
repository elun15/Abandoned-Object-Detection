/************************************************************************************/
/* Source file of BGSselector class                                      */
/* This class allows us to choose a background subtraction method                   */
/*                                       */
/************************************************************************************/

#include <iostream>   // std::cout
#include <string>     // std::string

#include <opencv2/opencv.hpp>	//OpenCV library
#include <opencv2/video/background_segm.hpp> //support for BGS algorithms

#include "BGSselector.h"
#include "../utils/utils.h" // to_string_

using namespace std;
using namespace cv;
using namespace bgslibrary::algorithms;

/*
 * \brief Default constructor
 */
BGSselector::BGSselector()
{
	cout << "Default parameters:" << endl;
	initialize(BGS_MOG2, false, false, NULL, -1);
}

/*
 * \brief Constructor with selected BGS algorithm
 * \param BGSid Identifier for the selected algorithm
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
BGSselector::BGSselector(BGS_type BGSid, bool display, bool saveIMG, const char* savePathDir, int saveCounter)
{
	initialize(BGSid, display, saveIMG, savePathDir, saveCounter);
}

/*
 * \brief Default destructor
 */
BGSselector::~BGSselector()
{
    std::cout << "~BGSselector()" << std::endl;
}

/*
 * \brief Method to initialize variables
 * \param BGSid Identifier for the selected algorithm
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
void BGSselector::initialize(BGS_type BGSid, bool display, bool saveIMG, const char* savePathDir, int saveCounter)
{
	this->_BGSid = BGSid;
	this->_BGStype_str =  create_BGStype_str(); //create strings to display the name of each method
	this->_display = display;
	this->_saveIMG = saveIMG;
	this->_savePathDir = savePathDir;
	this->_saveCounter = saveCounter;
	cout << "BGSselector(): " << _BGStype_str[_BGSid] << " selected" << endl;
}

/*
 * \brief Method to initialize the BGS selector
 *
 *	Method to initialize the BGS selector. Determines the variables size and algorithms
 *	according to the video sequence resolution and creates the bgs algorithms.
 *
 * \param frame first image of the video sequence in cv::Mat
 */
void BGSselector::init(Mat frame, double learningRate)
{
    this->_img_fg      = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    this->_img_bgmodel = Mat::zeros(frame.rows, frame.cols, CV_8UC3);

    _learningRate = learningRate; //-1 if default

    //initialization of BGS modules is performed here
    switch (_BGSid)
    {
        case BGS_LOBSTER:
            this->_bgs = new LOBSTER;
            break;
        case BGS_PAWCS:
            this->_bgs = new PAWCS;
            break;
        case BGS_MOG2:
            this->_bgs = new MixtureOfGaussianV2;
            if(_learningRate != -1)
            {
            	//set new learningrate
            }
            break;
        case BGS_KNN:
            this->_bgs = new KNN;
            break;
        case BGS_Multimodal:
            this->_bgs = new IndependentMultimodal;
            break;
        case BGS_SUBSENSE:
            this->_bgs = new SuBSENSE;
            if(_learningRate != -1)
            {
            	//set new learning rate
            }
            break;
        default:
            break;
    }
    cout << "BGSselector(): " << _BGStype_str[_BGSid] << " method initialized." << endl;
}


/*
 * \brief Method to process each frame by the selected BGS algorithm
 *
 * \param frame video frame in cv::Mat format
 * \param contextMask Optional parameter to apply a binary mask to the result
 * \param counter frame counter for display purposes
 */
void BGSselector::process(Mat frame, Mat contextMask,int counter)
{
    _img_input = frame.clone();
    _img_fg = Mat::zeros(frame.rows,frame.cols,frame.type());

    this->_bgs->process(frame,_img_fg,_img_bgmodel);

    if (!contextMask.empty())
        bitwise_and(_img_fg,contextMask,_img_fg);

    if (_display)
    {
    	string str1 = "Background image: " + _BGStype_str[_BGSid];
        putText(_img_bgmodel,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        imshow(str1.c_str(),_img_bgmodel);

        string str2 = "Foreground image: " + _BGStype_str[_BGSid];
        putText(_img_fg,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        imshow(str2.c_str(),_img_fg);
        waitKey(1);
    }

    if (_saveIMG && (counter% _saveCounter == 0))
    {
        putText(_img_input,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        std::string d1 = _savePathDir + "frame" + to_string_(counter,5) + ".jpg";
        imwrite(d1,_img_input);

        putText(_img_fg,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        std::string d2 = _savePathDir + "fg" + to_string_(counter,5) + ".jpg";
        imwrite(d2,_img_fg);
    }
}

/*
 * \brief Method to return the foreground mask
 *
 * Method to return the foreground mask after processing with the
 * selected BGS algorithm
 * \return cv::Mat with the foreground mask
 */
Mat BGSselector::GetFGmask()
{
    //vector<Mat> vector;
    //vector.push_back(this->img_output);
    //return vector;
    return this->_img_fg;
}

/*
 * \brief Method to return the Background image
 *
 * Method to return the Background image after processing
 * with the selected BGS algorithm
 * \return cv::Mat with the Background image
 */
Mat BGSselector::GetBGmodel()
{
    return this->_img_bgmodel;
}
