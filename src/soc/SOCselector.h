#ifndef __SOCSELECTOR_H__
#define __SOCSELECTOR_H__

#include <map> //std::map
#include <string> //std:string
#include <opencv2/opencv.hpp> //OpenCV library

#include "StaticObjectClassifier.h"
#include "StaticObjectClassifierHighGradient.h"
#include "StaticObjectClassifierHist.h"
#include "StaticObjectClassifierPCC.h"

/*#include "StaticObjectClassifierPixelColourContrast.h"
#include "StaticObjectClassifierGradient.h"
#include "StaticObjectClassifierGradientH.h"
#include "StaticObjectClassifierGradientL.h"
*/

///algorithms implemented by SOCSelector
typedef enum {
	SOC_NONE		=-1,
	SOC_HGR			=1,
	SOC_HIST	    =2,
	SOC_PCC			=3
} SOC_type;

/*! \class SOCselector
 *  \brief This class implements a selector for methods to classify stationary objects (abandoned, stolen or errors)
 */
class SOCselector
{
public:
	SOC_type _SOCid;
	std::map<int,std::string> _SOCtype_str;//string for each selected algorithm

	// Available algorithms
	StaticObjectClassifierHist *_pHist;
	StaticObjectClassifierHighGradient *_pHGR;
	StaticObjectClassifierPCC *_pPCC;
	//StaticObjectClassifierGradientH *pSOCGradientH;
	//StaticObjectClassifierGradientL *pSOCGradientL;

    //display settings
   bool _display;

   //output settings
   bool _saveIMG;
   int _saveCounter;
   std::string _savePathDir;

public:

	SOCselector();
	SOCselector(SOC_type SOCid,bool display=false, bool saveIMG=false, const char* savePathDir=NULL,int saveCounter=-1);  // SOCid: technique ID
	~SOCselector();

	void init();
	cv::Mat process(cv::Mat frame, cv::Mat bkgImage, cv::Mat StaticFGImage, cv::Mat ForegroundImage, BlobList<ObjectBlob*>* objects,int counter=-1);

private:
	//internal init
	void initialize(SOC_type SOCid, bool display, bool saveIMG, const char* savePathDir, int saveCounter);
	
	//list of strings for display purposes
	static std::map<int,std::string> create_SOCtype_str() {

	   std::map<int,std::string> m;
	   m.insert(std::make_pair(SOC_HGR    , "SOC_HGR"));
	   m.insert(std::make_pair(SOC_HIST   , "SOC_HIST"));
	   m.insert(std::make_pair(SOC_PCC   , "SOC_PCC"));
	   return m;
   }
};

#endif // __SOCSELECTOR_H__
