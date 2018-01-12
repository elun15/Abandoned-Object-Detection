#ifndef __PDSELECTOR_H__
#define __PDSELECTOR_H__

#include <vector> //std::vector
#include <map> //std::map
#include <opencv2/opencv.hpp> //OpenCV library
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

//include path to 'lib' must be included in the project settings
#include "../../lib/pdlibrary/dpm/dpm.hpp" //DMP people detector
#include "../../lib/pdlibrary/acf/acf.hpp" //ACF people detector

///algorithms implemented by PDSelector
typedef enum {
    PD_NONE		  =-1,
    PD_HOG		  =1,
    PD_DPM	      =2,
    PD_ACF		  =3,
    PD_HAAR_FULL  =4,
    PD_HAAR_UPPER =5,
} PD_type;


#define QT_PATH_PERSON_MODEL_DPM "../models/people/INRIA_DPM.xml"
#define QT_PATH_PERSON_MODEL_ACF "../models/people/INRIA_ACF.xml"
#define QT_PATH_PERSON_MODEL_HAAR_FULL "../models/people/haarcascade_fullbody.xml"
#define QT_PATH_PERSON_MODEL_HAAR_UPPER "../models/people/haarcascade_upperbody.xml"

#define PATH_PERSON_MODEL_DPM "./models/people/INRIA_DPM.xml"
#define PATH_PERSON_MODEL_ACF "./models/people/INRIA_ACF.xml"
#define PATH_PERSON_MODEL_HAAR_FULL "./models/people/haarcascade_fullbody.xml"
#define PATH_PERSON_MODEL_HAAR_UPPER "./models/people/haarcascade_upperbody.xml"



/*! \class PDSelector
 *  \brief This class implements a selector for methods to detect people in images
 */
class PDselector
{
private:
    PD_type _PDid;
    std::map<int,std::string> _PDtype_str;//string for each selected algorithm

    cv::Mat _img_input; //input frame (color or gray scale image)
    cv::Mat _label_image;
    std::vector<cv::Rect> _found;

    //HOG detector
    cv::HOGDescriptor _pHOG;

    //Haar
    cv::CascadeClassifier pUpperBodyCascade;
    cv::CascadeClassifier pFullBodyCascade;


    //ACF detector
    ACFDetector _pACF;
    NonMaximumSuppression NMS;
    DetectionList Dl;
    DetectionList Dl_NMS;

    //DPM detector
    cv::Ptr<cv::dpm::DPMDetector> _pDPM;
    std::vector<cv::dpm::DPMDetector::ObjectDetection> _DPMBbox; // DPM

    //display settings
    bool _display;

    //output settings
    bool _saveIMG;
    int _saveCounter;
    std::string _savePathDir;

    bool _QT_execution;

public:

    PDselector();
    PDselector(PD_type PDid,bool display=false, bool saveIMG=false, const char* savePathDir=NULL,int saveCounter=-1,bool QT_execution= true);  // PDid: technique ID

    ~PDselector();

    void init();

    std::vector<cv::Rect> process(cv::Mat frame,int counter=-1);
    void non_max_suppresion(const std::vector<cv::Rect> &srcRects, std::vector<cv::Rect> &resRects, float thresh);

    std::vector<cv::Rect> getDetections();
    cv::Mat &getDetectionsBoundingBoxMask(cv::Mat &img);

private:
    //internal init
    void initialize(PD_type BGSid, bool display, bool saveIMG, const char* savePathDir, int saveCounter,bool QT_execution);

    //list of strings for display purposes
    static std::map<int,std::string> create_PDtype_str() {

        std::map<int,std::string> m;
        m.insert(std::make_pair(PD_HOG    , "PD_HOG"));
        m.insert(std::make_pair(PD_DPM    , "PD_DPM"));
        m.insert(std::make_pair(PD_ACF    , "PD_ACF"));
        return m;
    }
};

#endif // __PDSELECTOR_H__
