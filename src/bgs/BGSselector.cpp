/************************************************************************************/
/* Source file of BGSselector class                                      */
/* This class allows us to choose a background subtraction method                   */
/*                                       */
/************************************************************************************/

#include <iostream>   // std::cout
#include <string>     // std::string

#include <opencv2/opencv.hpp>	//OpenCV library
#include <opencv2/video/background_segm.hpp> //support for BGS algorithms

//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

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
void BGSselector::init(Mat frame, double learningRate, double learningRate2,double learningRate3, int method_sfgd)
{
    this->_img_fg      = Mat::zeros(frame.rows, frame.cols, 0);
    this->_img_fg_L      = Mat::zeros(frame.rows, frame.cols, 0);
      this->_img_fg_M      = Mat::zeros(frame.rows, frame.cols, 0);
    this->_img_bgmodel = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
    this->_img_bgmodel_L = Mat::zeros(frame.rows, frame.cols, CV_8UC3);


    _learningRate = learningRate; //-1 if default SHORT-TERM
    _learningRate_L = learningRate2; //-1 if default LONG-TERM
    _learningRate_M = learningRate3;

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

        this->_pMOG2 = createBackgroundSubtractorMOG2(); // //OPENCV VERSIONMOG2 approach

        if(method_sfgd == 4)//DUAL MOG2
        {
            this->_pMOG2_L = createBackgroundSubtractorMOG2(); //MOG2 approach
        }
        if(method_sfgd == 5)//TRIPLE MOG2
        {
            this->_pMOG2_M = createBackgroundSubtractorMOG2(); //MOG2 approach
            this->_pMOG2_L = createBackgroundSubtractorMOG2(); //MOG2 approach
        }

        break;

    case BGS_KNN:
        this->_pKNN = createBackgroundSubtractorKNN();

        if(method_sfgd == 4)//DUAL KNN
        {
            this->_pKNN_L = createBackgroundSubtractorKNN(); //KNN approach
        }
        if(method_sfgd == 5)//TRIPLE KNN
        {
            this->_pKNN_L = createBackgroundSubtractorKNN(); //KNN approach
            this->_pKNN_M = createBackgroundSubtractorKNN(); //KNN approach
        }
        break;

    case BGS_Multimodal:
        this->_bgs = new IndependentMultimodal;
        break;

    case BGS_SUBSENSE:
        if(method_sfgd != 4)
        {
            this->_bgs = new SuBSENSE(100); //default
        }

        else //DUAL
        {
            this->_bgs = new SuBSENSE(_learningRate);
            this->_bgs_L = new SuBSENSE(_learningRate_L);
        }
        break;

    case BGS_KDE:

        if(method_sfgd != 4)
        {
            this->_bgs = new KDE(500); //framesToLearn = 10 (default)

        }
        else    //DUAL
        {
            this->_bgs = new KDE(300); //framesToLearn = 10 (default)
            this->_bgs_L = new KDE(5000);
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
void BGSselector::process(Mat frame, Mat BoundaryMask, int counter, int method_sfgd, int nframe)
{
    _img_input = frame.clone();
    _img_input2 = frame.clone();

    if (_BGSid == 3) // MOG2
    {
        if (method_sfgd == 4)
        {
            this->_pMOG2->apply(frame, _img_fg);
            this->_pMOG2_L->apply(_img_input, _img_fg_L,_learningRate_L);
            this->_pMOG2_L->getBackgroundImage(_img_bgmodel);

        }
        else if(method_sfgd == 5)
        {
            this->_pMOG2->apply(frame, _img_fg);
            this->_pMOG2_L->apply(_img_input, _img_fg_L,_learningRate_L);
            this->_pMOG2_M->apply(_img_input2, _img_fg_M,_learningRate_M);
            this->_pMOG2_L->getBackgroundImage(_img_bgmodel);


        }

        else
        {
            this->_pMOG2->apply(frame, _img_fg,0.00005);
            this->_pMOG2->getBackgroundImage(_img_bgmodel);
        }

        ;

    }

    else if (_BGSid == 4) // KNN
    {
        if (method_sfgd == 4)
        {
            this->_pKNN->apply(_img_input, _img_fg);

            if (nframe < 400)
            {
                this->_pKNN_L->apply(_img_input2, _img_fg_L);
            }
            else
            {
                this->_pKNN_L->apply(_img_input2, _img_fg_L,_learningRate_L);

            }
             this->_pKNN_L->getBackgroundImage(_img_bgmodel);

        }
        else if(method_sfgd == 5)
        {
            this->_pKNN->apply(frame, _img_fg);

            if (nframe < 400)
            {
                this->_pKNN_L->apply(_img_input, _img_fg_L);
                this->_pKNN_M->apply(_img_input2, _img_fg_M);

            }
            else
            {
                this->_pKNN_L->apply(_img_input, _img_fg_L,_learningRate_L);
                this->_pKNN_M->apply(_img_input2, _img_fg_M,_learningRate_M);

            }
           this->_pKNN_L->getBackgroundImage(_img_bgmodel);

        }

        else //NO DUAL
        {
            if (nframe < 400)
                this->_pKNN->apply(_img_input, _img_fg);
            else
                this->_pKNN->apply(_img_input, _img_fg,0.0001);

             this->_pKNN->getBackgroundImage(_img_bgmodel);
        }



    }
    else //OTHERS
    {
        this->_bgs->process(_img_input,_img_fg,_img_bgmodel);

        if (method_sfgd == 4)
        {
            //DUAL BG OTHERS
            this->_bgs_L->process(_img_input2,_img_fg_L,_img_bgmodel_L);

        }

    }

    if (!BoundaryMask.empty())  //ONLY FOR AVSS
    {
        bitwise_and(_img_fg,BoundaryMask,_img_fg);
        bitwise_and(_img_fg_L,BoundaryMask,_img_fg_L);
        bitwise_and(_img_fg_M,BoundaryMask,_img_fg_M);


    }

    if (_display)
    {
        string str = "Frame: " + _BGStype_str[_BGSid];
        putText(frame.clone(),to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        imshow(str.c_str(),frame.clone());

        string str1 = "Background image: " + _BGStype_str[_BGSid];
        putText(_img_bgmodel,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        imshow(str1.c_str(),_img_bgmodel);

        string str2 = "FG: " + _BGStype_str[_BGSid];
        putText(_img_fg,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        imshow(str2.c_str(),_img_fg);
        waitKey(1);

        if (method_sfgd == 4)
        {
            string str2 = "FG Long: " + _BGStype_str[_BGSid];
            putText(_img_fg_L,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow(str2.c_str(),_img_fg_L);
            waitKey(1);


        }
        if (method_sfgd == 5)
        {
            string str2 = "FG Long: " + _BGStype_str[_BGSid];
            putText(_img_fg_L,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow(str2.c_str(),_img_fg_L);
            waitKey(1);
            string str3 = "FG Medium: " + _BGStype_str[_BGSid];
            putText(_img_fg_M,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow(str3.c_str(),_img_fg_M);
            waitKey(1);




        }
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
std::vector<cv::Mat> BGSselector::GetFGmask(int method_sfgd)
{
    std::vector<cv::Mat> vector;
    vector.push_back(this->_img_fg);

    if (method_sfgd == 4) //DUAL BG
    {
        vector.push_back(this->_img_fg_L);
    }
    if (method_sfgd == 5) //TRIPLE
    {
       vector.push_back(this->_img_fg_M);
       vector.push_back(this->_img_fg_L);
    }


    return vector;
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
