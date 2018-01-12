/************************************************************************************/
/* Source file of SFGDSelector class                                                */
/* This class allows us to choose a static foreground detection method              */
/*                                                                                  */
/************************************************************************************/
#include <string>
#include <opencv2/opencv.hpp>

#include "SFGDselector.h"
#include "../utils/utils.h" // to_string_


using namespace std;
using namespace cv;

/*
 * \brief Default constructor
 */
SFDGselector::SFDGselector()
{
    cout << "SFGD with default parameters: " << endl;
    initialize(SFGD_ACC, SFGD_FRAMERATE_DEFAULT, SFGD_TIME_STATIC_DEFAULT,false, false, NULL,-1);
}


/*
 * \brief Constructor with selected SFGD algorithm
 *
 * \param SFGDid Identifier for the selected algorithm
 * \param framerate Framerate of the video sequences (in fps)
 * \param time_to_static Time (in seconds) to determine a blob/object as stationary
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
SFDGselector::SFDGselector(SFGD_type SFGDid, double framerate, double time_to_static,bool display, bool saveIMG, const char* savePathDir,int saveCounter)
{
    initialize(SFGDid, framerate, time_to_static,display, saveIMG, savePathDir,saveCounter);
}

/*
 * \brief Default destructor
 */
SFDGselector::~SFDGselector()
{
    cout << "~SFGDSelector()" << endl;
}

/*
 * \brief Method to initialize variables
 *
 * \param SFGDid Identifier for the selected algorithm
 * \param framerate Framerate of the video sequences (in fps)
 * \param time_to_static Time (in seconds) to determine a blob/object as stationary
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
void SFDGselector::initialize(SFGD_type SFGDid, double framerate, double time_to_static,bool display, bool saveIMG, const char* savePathDir,int saveCounter)
{
    this->_SFGDid = SFGDid;
    this->_SFGDtype_str =  create_SFGDtype_str(); //create strings to display the name of each method
    this->_display = display;
    this->_saveIMG = saveIMG;
    this->_savePathDir = savePathDir;
    this->_saveCounter = saveCounter;
    this->_time_to_static = time_to_static;
    this->_framerate = framerate;

    this->psubsamplingSFGD  = NULL;
    this->psubsamplingSFGD = NULL;

    cout << "SFDGselector(): " << _SFGDtype_str[_SFGDid] << " selected time2static=" << _time_to_static << "secs framerate=" << _framerate << "fps" << endl;
}

/*
 * \brief Method to initialize the SFGD selector
 *
 *	Method to initialize the SFGD selector. Determines the variables size and algorithms
 *	according to the video sequence resolution and creates the SFGD algorithms.
 *
 * \param frame first image of the video sequence in cv::Mat
 */
void SFDGselector::init(Mat frame)
{
    switch (_SFGDid)
    {
    case SFGD_SUBSAMPLING:
        this->psubsamplingSFGD = new StaticMaskExtractor_subsampling(frame,_framerate, _time_to_static);
        break;
    case SFGD_ACC:
        this->pAccMaskSFGD = new StaticMaskExtractor_AccMask(frame, _framerate, _time_to_static, 2.0f, 250);
        break;
    case SFGD_HISTIMG:
        this->pHistoryImagesSFGD = new StaticMaskExtractor_HistoryImages(frame, _framerate, _time_to_static, 2.0f, 250);
        break;
    case SFGD_DBM:
        this->pDBMSFGD = new StaticMaskExtractor_DualBkg(frame, _framerate, _time_to_static);
        break;
    case SFGD_TBM:
        this->pTBMSFGD = new StaticMaskExtractor_TripleBkg(frame, _framerate, _time_to_static);
        break;

    default:
        break;
    }
}

/*
 * \brief Method to process each frame by the selected BGS algorithm
 *
 * \param frame video frame in cv::Mat format
 * \param fgmask foreground mask corresponding to the frame
 * \param bgmodel background model corresponding to the frame
 * \param counter frame counter for display purposes
 */
void SFDGselector::process(Mat frame, std::vector<cv::Mat> foreground_img, Mat bgmodel_img,int counter)
{
    switch (_SFGDid)
    {
    case SFGD_SUBSAMPLING:
        this->psubsamplingSFGD->processFrame(frame, foreground_img[0]);
        this->_img_sfgd = this->psubsamplingSFGD->getStaticMask();
        break;
    case SFGD_ACC:
        this->pAccMaskSFGD->processFrame(foreground_img[0],_framerate, _time_to_static);
        this->_img_sfgd = this->pAccMaskSFGD->getStaticMask();
        break;
    case SFGD_HISTIMG:
        this->pHistoryImagesSFGD->processFrame(foreground_img[0], frame, bgmodel_img, _framerate, _time_to_static);
        this->_img_sfgd = this->pHistoryImagesSFGD->getStaticMask();
        break;
    case SFGD_DBM:
        this->pDBMSFGD->processFrame(foreground_img[1],foreground_img[0]); // ( L, S)
        this->_img_sfgd= this->pDBMSFGD->getStaticMask();
        break;
   case SFGD_TBM:
        this->pTBMSFGD->processFrame(foreground_img[2],foreground_img[1],foreground_img[0]); // (L, M , S)
        this->_img_sfgd = this->pTBMSFGD->getStaticMask();
        break;
    default:
        break;
    }


    if (_display)
    {
        string str = "Static Foreground: " + _SFGDtype_str[_SFGDid];
        imshow(str.c_str(),_img_sfgd);
    }


    if (_saveIMG && (counter% _saveCounter == 0))
    {
        putText(_img_input,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        std::string d = _savePathDir + "sfgd" + to_string_(counter,5) + ".jpg";
        imwrite(d,_img_input);
    }
}

/*
 * \brief Method to return the static foreground mask
 *
 * Method to return the static foreground mask after processing with the
 * selected SFGD algorithm
 * \return cv::Mat with the static foreground mask
 */
Mat SFDGselector::GetSFGmask()
{
    return this->_img_sfgd;
}
