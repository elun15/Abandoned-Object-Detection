#include <opencv2/opencv.hpp> //opencv libraries

#include "../src/aod.h" // AOD system class
#include "../src/Config.h" // Structure with program settings
#include "../src/blobs/DefineObjectBlobList.h"

using namespace cv;
using namespace std;


/*********************************/
/*** MAIN PROGRAM FUNCTION     ***/
/*********************************/

// To check command usage see Config::print_usage()
int main(int argc, char *argv[])
{

 AOD system;
    // Initialize all settings to configure the AOD system
    Config cfg(argc,argv);
    cfg.ShowResults = true;
    cfg.m_soc = SOC_HGR;
    //cfg.m_soc = SOC_HIST;
    //cfg.m_soc = SOC_PCC;

    cv::Mat frame = cv::imread("../datasets/SOC_test/avss-frame02720.png");
    cv::Mat sfg = cv::imread("../datasets/SOC_test/avss-mask02720.png",IMREAD_GRAYSCALE);
    cv::Mat bkg = cv::imread("../datasets/SOC_test/avss-bkg02720.png");/**/

    BlobExtractor be;
    be.extractBlobs(sfg);
    imshow("BLOBS",be.getBlobBoundingBoxMask(frame));

    std::vector<cv::Rect> found;
    BlobList<ObjectBlob*> pObjectList;
    DefineObjectBlobList( be.getBlobList(),found, &pObjectList);
    cout << "Static objects to process = " << pObjectList.getBlobNum() << std::endl;

   // sel_soc = new SOCselector((SOC_type)cfg.m_soc,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),cfg.SaveImages_freq);
    string dir = "";

    system._sel_soc = new SOCselector(SOC_HGR,true,false);
    system._sel_soc->init();
    system._sel_soc->process(frame, bkg, sfg, sfg,&pObjectList,1);





    return 1;



}
