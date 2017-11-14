/**
* \file BGSSelector.h
* \author Elena Luna Garcia (elg)
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 23-10-17
* \brief Module to select a foreground detection algorithm based on background subtraction
* \version 1.0\n
*			Version history:\n
*				- 1.0 (10-09-17): Initial Implementation (elg)
*				- 1.1 (23-10-17): Added typedef types, initialization methods and new variables for behavior configuration (jcs)
*
*/
#ifndef BGS_SELECTOR_H
#define BGS_SELECTOR_H

#include <map>//std::map

#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp> //algorithms BGS_MOG2, BGS_KNN

#include "../../lib/bgslibrary/package_bgs/bgslibrary.h" // BGS_LOBSTER,BGS_PAWCS,BGS_Multimodal,BGS_SUBSENSE

///algorithms implemented by BkgSubtractionSelector
typedef enum {
    BGS_NONE=-1,
    BGS_LOBSTER     =1,
    BGS_PAWCS       =2,
    BGS_MOG2        =3,
    BGS_KNN         =4,
    BGS_Multimodal  =5,
    BGS_SUBSENSE    =6,
    BGS_KDE         =7,
} BGS_type;

/*! \class BkgSubtractionSelector
 *  \brief This class implements a selector for a particular background subtraction method
 *
 */
class BGSselector
{
public:
    // Constructors
	BGSselector();
	BGSselector(BGS_type BGSid,bool display=false, bool saveIMG=false, const char* savePathDir=NULL,int saveCounter=-1);  // BGSid: technique ID

    //Destructor
    ~BGSselector();

    BGS_type _BGSid; //selected BGS algorithm
    std::map<int,std::string> _BGStype_str;//string for each selected algorithm

    // Pointer to algorithms available in BGSlibrary
    bgslibrary::algorithms::IBGS *_bgs;
     bgslibrary::algorithms::IBGS *_bgs_L;

    //For MOG2 CASE
    Ptr<BackgroundSubtractor> _pMOG2; //MOG2 Background subtractor
    Ptr<BackgroundSubtractor> _pMOG2_L; //MOG2 Background subtractor
    double _learningRate;
    double _learningRate_L;

    cv::Mat _img_input; //input frame (color or gray scale image)
    cv::Mat _img_fg;//output frame (gray scale image)
    cv::Mat _img_fg_L;//output frame (gray scale image) for Dual
    cv::Mat _img_bgmodel; //background frame (color or gray scale image)
    cv::Mat _img_bgmodel_L; //background frame (color or gray scale image)

    //display settings
    bool _display;

    //output settings
    bool _saveIMG;
    int _saveCounter;
    std::string _savePathDir;

public:
    void init(cv::Mat frame, double learningRate=-1, double learningRate2 =-1, int method_sfgd = 0);
    void process(cv::Mat frame, cv::Mat contextMask=Mat(), int counter=-1, int method_sfgd = 0);
    std::vector<Mat> GetFGmask(int method_sfgd);
    cv::Mat GetBGmodel();

private:
    //internal init
    void initialize(BGS_type BGSid, bool display, bool saveIMG, const char* savePathDir, int saveCounter);

    //list of strings for display purposes
    static std::map<int,std::string> create_BGStype_str() {

           std::map<int,std::string> m;
           m.insert(std::make_pair(BGS_LOBSTER  , "BGS_LOBSTER"));
           m.insert(std::make_pair(BGS_PAWCS    , "BGS_PAWCS"));
           m.insert(std::make_pair(BGS_MOG2     , "BGS_MOG2"));
           m.insert(std::make_pair(BGS_KNN      , "BGS_KNN"));
           m.insert(std::make_pair(BGS_Multimodal, "BGS_Multimodal"));
           m.insert(std::make_pair(BGS_SUBSENSE , "BGS_SUBSENSE"));
           return m;
       }
};
#endif // BGS_SELECTOR_H
