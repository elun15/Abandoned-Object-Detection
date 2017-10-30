#include <opencv2/opencv.hpp> //opencv libraries
#include <sfgd/SFGDselector.h> // SFGD class

#include "Config.h" // Structure with program settings
#include "soc/SOCselector.h" // BGS class
#include "blobs/BlobExtractor.h" //Blob Extractor class
#include "blobs/DefineObjectBlobList.h"

using namespace cv;
using namespace std;

#define _SAVE_IMAGES_FREQ_ 20

/*********************************/
/*** MAIN PROGRAM FUNCTION     ***/
/*********************************/
// To check command usage see Config::print_usage()
int main(int argc, char *argv[])
{
	// Initialize all settings to configure the AOD system
	Config cfg(argc,argv);
	cfg.ShowResults = true;
	cfg.m_soc = SOC_HGR;
	//cfg.m_soc = SOC_HIST;
	//cfg.m_soc = SOC_PCC;

    // SOC selector
	BlobExtractor be;
	SOCselector mysoc((SOC_type)cfg.m_soc,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),_SAVE_IMAGES_FREQ_);
	mysoc.init();

	//test case 1
	cv::Mat frame = cv::imread("../datasets/SOC_test/avss-frame02720.png");
	cv::Mat mask = cv::imread("../datasets/SOC_test/avss-mask02720.png",IMREAD_GRAYSCALE);
	cv::Mat bkg = cv::imread("../datasets/SOC_test/avss-bkg02720.png");/**/

	/*//test case 2
	cv::Mat frame = cv::imread("../datasets/SOC_test/cantata-frame00355.png");
	cv::Mat mask = cv::imread("../datasets/SOC_test/cantata-mask00355.png",IMREAD_GRAYSCALE);
	cv::Mat bkg = cv::imread("../datasets/SOC_test/cantata-bkg00355.png");/**/

	//test case 3
	/*cv::Mat frame = cv::imread("../datasets/SOC_test/EPFL_frame01005.png");
	cv::Mat mask = cv::imread("../datasets/SOC_test/EPFL_mask01005.png",IMREAD_GRAYSCALE);
	cv::Mat bkg = cv::imread("../datasets/SOC_test/EPFL_bkg01005.png");/**/

	//test case 4
	/*cv::Mat frame = cv::imread("../datasets/SOC_test/PETS_frame02140.png");
	cv::Mat mask = cv::imread("../datasets/SOC_test/PETS_mask02140.png",IMREAD_GRAYSCALE);
	cv::Mat bkg = cv::imread("../datasets/SOC_test/PETS_bkg02140.png");	/**/

	cv::imshow("frame",frame);
	cv::imshow("mask",mask);
	cv::imshow("bkg",bkg);

	double maxv,minv;
	cv::minMaxLoc(mask, &minv, &maxv);

	be.extractBlobs(mask);
	imshow("BLOBS",be.getBlobBoundingBoxMask(frame));

	std::vector<cv::Rect> found;
	BlobList<ObjectBlob*> pObjectList;
	DefineObjectBlobList( be.getBlobList(),found, &pObjectList);
	cout << "Static objects to process = " << pObjectList.getBlobNum() << std::endl;

	mysoc.process(frame,bkg,mask,mask,&pObjectList);
	cv::waitKey(0);

    return 1;
}
