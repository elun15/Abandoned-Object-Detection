/**
* \file SFGDSelector.h
* \author Elena Luna Garcia (elg)
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 23-10-17
* \brief Module to select a stationary foreground detection algorithm
* \version 1.0\n
*			Version history:\n
*				- 1.0 (10-09-17): Initial Implementation (elg)
*				- 1.1 (23-10-17): Added typedef types, initialization methods and new variables for behavior configuration (jcs)
*
*/
#ifndef SFGDSELECTOR_H
#define SFGDSELECTOR_H

#include <opencv2/opencv.hpp>

#include "StaticMaskExtractor.h"
#include "StaticMaskExtractor_subsampling.h"
#include "StaticMaskExtractor_AccMask.h"
//#include "StaticMaskExtractor_histimg.h"
#include "StaticMaskExtractor_DualBkg.h"
#include "StaticMaskExtractor_HistoryImages.h"

///	Default Default framerate of the input video
const double SFGD_FRAMERATE_DEFAULT = 25;

///	Default Default time to detect an object as stationary
const double SFGD_TIME_STATIC_DEFAULT = 10;

///algorithms implemented by SFDGSelector
typedef enum {
	SFGD_NONE=-1,
	SFGD_SUBSAMPLING	=1,
	SFGD_ACC	        =2,
	SFGD_HISTIMG		=3,
	SFGD_DBM			=4
} SFGD_type;

/*! \class SFDGSelector
 *  \brief This class implements a selector for method to extract stationary foreground
 */
class SFDGselector
{
public:
	SFDGselector();
	SFDGselector(SFGD_type SFGDid, double framerate, double time_to_static,bool display=false, bool saveIMG=false, const char* savePathDir=NULL,int saveCounter=-1);
	~SFDGselector();

	SFGD_type _SFGDid; //selected SFGD algorithm
	std::map<int,std::string> _SFGDtype_str;//string for each selected algorithm

	// Pointer to algorithms available for SFGD
	StaticMaskExtractor_subsampling *psubsamplingSFGD;
	StaticMaskExtractor_AccMask *pAccMaskSFGD;
    StaticMaskExtractor_DualBkg *pDBMSFGD;
    StaticMaskExtractor_HistoryImages *pHistoryImagesSFGD;

	//specific parameters for stationary detection
	double _framerate; //measured in fps
	double _time_to_static; //measured in seconds


	cv::Mat _img_input; //input frame (color or gray scale image)
	cv::Mat _img_sfgd;//output frame (gray scale image)
	cv::Mat _img_bgmodel; //background frame (color or gray scale image)

	//display settings
	bool _display;

	//output settings
	bool _saveIMG;
	int _saveCounter;
	std::string _savePathDir;

    void init(cv::Mat frame);
    void process(cv::Mat frame, std::vector<cv::Mat>, cv::Mat bgmodel_img, int counter=-1);
    cv::Mat GetSFGmask();

private:
    // internal init
	void initialize(SFGD_type SFGDid, double framerate, double time_to_static,bool display, bool saveIMG, const char* savePathDir,int saveCounter);

	//list of strings for display purposes
	static std::map<int,std::string> create_SFGDtype_str() {

		   std::map<int,std::string> m;
		   m.insert(std::make_pair(SFGD_NONE     	, "SFGD_NONE"));
		   m.insert(std::make_pair(SFGD_SUBSAMPLING , "SFGD_SUBSAMPLING"));
		   m.insert(std::make_pair(SFGD_ACC    		, "SFGD_ACC"));
		   m.insert(std::make_pair(SFGD_HISTIMG		, "SFGD_HISTIMG"));
		   return m;
	   }
};

#endif // SFGD_SELECTOR_H
