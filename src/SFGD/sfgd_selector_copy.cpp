/************************************************************************************/
/* Source file of SFGDSelector class                                                */
/* This class allows us to choose a static foreground detection method              */
/*                                                                                  */
/************************************************************************************/
#include "../../../src/SFGD/SFGDSelector.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

SFDG_Selector::SFDG_Selector(char *SFGDid, double framerate, int time_to_static)
{
    cout << "MySFGDSelector()" << std::endl;
    this->int_SFGDid = atoi(SFGDid);
    this->framerate = framerate;
    this->time2static = time_to_static;
}

SFDG_Selector::~SFDG_Selector()
{
    std::cout << "~MySFGDSelector()" << std::endl;
}

void SFDG_Selector::init(cv::Mat frame, MyBGSSelector bgs)
{
    //IplImage *Ipl_frame = cvCloneImage(&(IplImage)frame);
    //CHANGED
    // IplImage *Ipl_frame;
    //Ipl_frame=cvCreateImage(cvSize(frame.cols,frame.rows),8,3);
    //IplImage ipltemp=frame;
    //cvCopy(&ipltemp,Ipl_frame);


    switch (this->int_SFGDid)
    {
    case 1:
        this->psubsamplingSFGD = new StaticMaskExtractor_subsampling(frame, NULL, this->framerate, this->time2static, SFGD_SUBSAMPLING_NUMSTAGES);
        break;
    case 2:
        this->pAccMaskSFGD = new StaticMaskExtractor_AccMask(frame, NULL, this->framerate, this->time2static, 2.0f, 250);
        break;
    case 3:
        this->pVPUSFGD = new StaticMaskExtractor_HistoryImagesVPU_doh(frame, NULL, this->framerate, this->time2static, 2.0f, 20);
        break;

    case 4:
        this->pDBMSFGD = new StaticMaskExtractor_DualBkg(frame, NULL, this->framerate, (double)this->time2static, &bgs, this->alpha);
        break;

    default:
        break;
    }
}

void SFDG_Selector::process(cv::Mat frame, std::vector<cv::Mat> foreground_img, cv::Mat bgmodel_img)
{
    //IplImage *Ipl_frame = cvCloneImage(&(IplImage)frame);


    //IplImage *Ipl_foreground = cvCloneImage(&(IplImage)foreground_img);
    Mat foreground;

    //IplImage *Ipl_foregroundShort = cvCloneImage(&(IplImage)foreground_img[0]);

    Mat foregroundShort = foreground_img[0];

    Mat foregroundLong;

    if (foreground_img.size() == 2)
    {

        foregroundLong = foreground_img[1];



    }

    // ERROR 3 channels copiar a 1 channel
    if (foreground_img[0].channels() == 1){

        foreground = foreground_img[0];

    }
    else{

        Mat tmp;
        cvtColor(foreground_img[0], tmp, CV_BGR2GRAY);
        foreground = tmp;


    }
    //IplImage *Ipl_bgmodel = cvCloneImage(&(IplImage)bgmodel_img);


    switch (this->int_SFGDid)
    {
    case 1:
        this->psubsamplingSFGD->processFrame(frame, bgmodel_img);
        cvShowImage("STATIC MOTION MASK SUBSAMPLING", this->psubsamplingSFGD->getStaticMask());
        break;
    case 2:
        this->pAccMaskSFGD->processFrame(frame, foreground);
        cvShowImage("STATIC MOTION MASK ACC", this->pAccMaskSFGD->getStaticMask());
        break;
    case 3:
        this->pVPUSFGD->processFrame(frame, foreground, bgmodel_img, 20);
        cvShowImage("STATIC MOTION MASK VPUDOH", this->pVPUSFGD->getStaticMask());
        break;

    case 4:
        this->pDBMSFGD->processFrame(frame, foregroundLong, foregroundShort);
        cvShowImage("STATIC MOTION MASK DBM", this->pDBMSFGD->getStaticMask());
        break;

    default:
        break;
    }
    /*cvReleaseImage(&Ipl_frame);
    cvReleaseImage(&Ipl_foreground);
    cvReleaseImage(&Ipl_bgmodel);
    cvReleaseImage(&Ipl_foregroundLong);
    cvReleaseImage(&Ipl_foregroundShort);*/

    frame.release();
    foreground.release();
    bgmodel_img.release();
    foregroundLong.release();
    foregroundShort.release();

}

cv::Mat SFDG_Selector::GetStaticForeground(){
    switch (this->int_SFGDid)
    {
    case 1:
        return cv::cvarrToMat(this->psubsamplingSFGD->getStaticMask());
        break;
    case 2:
        return cv::cvarrToMat(this->pAccMaskSFGD->getStaticMask());
        break;
    case 3:
        return cv::cvarrToMat(this->pVPUSFGD->getStaticMask());
        break;
    case 4:
        return cv::cvarrToMat(this->pDBMSFGD->getStaticMask());
        break;
    default:
        break;
    }
}

void SFDG_Selector::finish()
{

}
