
/**
* \file StaticMaskExtractor_HistoryImages.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Implementation of the common interface for the static region detection algorithms
* \version 2.0\n
*			Version history:\n
*				- 1.0 (01-09-09): Initial Implementation (jcs)
*				- 2.0 (16-12-10): Modification for being a common interface (jcs)
*				- 3.0 (07-12-12): El algoritmo lo he cambiado pues el anterior no funcionaba bien,
*								  ahora la imagen S se incrementa de 1 en 1  si es FG y se decrementa
*                                de 2 en 2 si es BG. Hay un umbral, que en este caso esta a 240, que
*                                cuando se supera significa que el pixel es estatico y la mascara
*                                estatica se pone a 1. (HistoryImages)
* \version 3.0\n
*			Version history:\n
*				- 3.0(13/09/17): reimplemented and adapted to openCV 3.2 (elg)
*
*/
#include "../../../src/SFGD/StaticMaskExtractor.h"
#include "../../../src/SFGD/StaticMaskExtractor_subsampling.h"
#include "../../../src/SFGD/StaticMaskExtractor_HistoryImages.h"

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

#define C_COST2 1 //increment cost for stationary detection
#define D_COST2 15 //penalization cost for stationary detection

Mat funSSIM( const Mat& I1, const Mat& I2);
Mat temporalMedian(vector<Mat> vector_images);

/**
*	Class Constructor with initial data
*
*	\param sampleFrame Sample of images to process in the future
*	\param log Pointer to log file
*	\param _framerate Framerate of the input video frame
*	\param _wd Weight decay for penalizing the no-motion time intervals (default 2.0)
*	\param _conf_th Confidence threshold for detecting a stationary object (default 250)
*
*/
StaticMaskExtractor_HistoryImages::StaticMaskExtractor_HistoryImages(Mat sampleFrame, double _framerate, double time_to_static, double _wd, int _conf_th) :StaticMaskExtractor(sampleFrame, _framerate, time_to_static)
{
    this->wd = _wd; //Weight to penalty non-motion detection
    this->confThreshold = _conf_th;	//Threshold to consider as stationary mask (range 0---1 is mapped to 0...255)
    this->Acc_Cont = 1;
    this->Acc_Dec= 250;
    this->static_th = 0.7 * 250;
    this->adapt = (255 / (framerate*secs2static));
    this->FG_Dec = 3 * this->adapt;

    this->fgmask_counter= Mat::zeros(sampleFrame.rows,sampleFrame.cols,CV_32FC1);
    this->ssim_counter = cv::Mat::zeros(sampleFrame.rows,sampleFrame.cols,CV_32FC1);
    this->motion_counter = cv::Mat::zeros(sampleFrame.rows,sampleFrame.cols,CV_32FC1);
    this->windowSize = 11;
    //this->StaticFGmat = cv::Mat::zeros(aaa.rows,aaa.cols,CV_32FC1);
}


/**
*	Default destructor
*
*/
StaticMaskExtractor_HistoryImages::~StaticMaskExtractor_HistoryImages()
{

    if (!fgmask_counter.empty())
        fgmask_counter.release();

}


/**
*	Method to accumulate FG masks (currently is based on a simple counter).
*
*	\param frame Current image (Color or gray)
*	\param fg_mask Mask indicating the presence of moving objects (Gray)
*
*/
void StaticMaskExtractor_HistoryImages::processFrame(Mat fgmask, Mat srcMat, Mat bkgMat, double framerate, int time_to_static)
{
    this->counter++;
    //num frames to static
    int numframes2static = (int)(framerate * time_to_static);

    Mat srcGrayMat;
    cvtColor(srcMat, srcGrayMat, CV_BGR2GRAY);

    Mat R = cv::imread("chan_dest_red.png", 0);
    Mat G = cv::imread("chan_dest_green.png", 0);
    Mat B = cv::imread("chan_dest_blue.png", 0);

    //vector<cv::Mat> array_to_merge;

    //array_to_merge.push_back(srcGrayMat);

    /****************** Foreground History Image ********************/

    Mat bgmask;
    bitwise_not(fgmask, bgmask);

    //operate with fgmask to update fgmask_counter
    double a = C_COST2;//*adapt;
    cv::add(this->fgmask_counter, (float)a, this->fgmask_counter, fgmask); // Add C_COST if FG pixel
    //cv::imshow( "Counter", this->fgmask_counter);
    //cvWaitKey(1);
    double b = 15;//*adapt;
    subtract(this->fgmask_counter, (float)b, this->fgmask_counter, bgmask);

    fgmask_counter.setTo(0, this->fgmask_counter < 0);

    /**************************************************************/

    /***************** Structural History Image *******************/
    //SSIM
    Mat ssimMat =	funSSIM(srcMat, bkgMat); //SSIM (3D)

    vector<Mat> data;
    data.push_back( Mat(ssimMat.rows, ssimMat.cols, CV_8UC1));
    data.push_back( Mat(ssimMat.rows, ssimMat.cols, CV_8UC1));
    data.push_back( Mat(ssimMat.rows, ssimMat.cols, CV_8UC1));
    data.push_back( Mat(ssimMat.rows, ssimMat.cols, CV_8UC1));
    split(ssimMat,data);//SSIM in vector of Mats

    // Compute mean of each SSIM channel
    Mat meanSSIM = (data[0] + data[1] + data[2])/data.size();
    //cv::imshow( "SSIM", meanSSIM);
    //cvWaitKey(1);
    //Average SSIM in local patches (RSSIM)
    Mat RSSIM;
    int kernel_size = 5;
    Mat kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    filter2D(meanSSIM, RSSIM, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
    //cv::imshow( "RSSIM", RSSIM);
    //cvWaitKey(1);

    //Get SSIM FG by thresholding
    Mat FG_ssim = Mat::zeros(RSSIM.rows,RSSIM.cols,CV_8UC1);
    FG_ssim.setTo((float)255, RSSIM <= 0.5);
    FG_ssim.setTo((float)0, RSSIM > 0.5);

    //Compute Structural History Image
    cv::add(this->ssim_counter, (float)a, this->ssim_counter, FG_ssim); // Add C_COST if FG pixel
    this->ssim_counter.setTo(0, FG_ssim == 0);

    /**************************************************************/

    /***************** Motion History Image ***********************/
    Mat diff = Mat::zeros(srcGrayMat.rows,srcGrayMat.cols,CV_32FC1);
    vector<Mat> Median1;
    vector<Mat> Median2;

    //Compute median images of last this->windowSize frames and following this->windowSize frames
    if (this->counter < (this->windowSize*2))
    {
        this->Frames4median.push_back(srcGrayMat);
        if (this->counter>1)
            diff = (this->Frames4median.back()) - (this->Frames4median.at(this->Frames4median.size()-2));
    }
    else
    {
        this->Frames4median.erase(this->Frames4median.begin());
        this->Frames4median.push_back(srcGrayMat);

        for(int kk=0;kk<this->Frames4median.size();kk++)
        {
            if(kk<=this->windowSize-2)
                Median1.push_back(this->Frames4median.at(kk));
            else if(kk==this->windowSize-1)
            {
                Median1.push_back(this->Frames4median.at(kk));
                Median2.push_back(this->Frames4median.at(kk));
            }
            else
                Median2.push_back(this->Frames4median.at(kk));
        }

        Mat medianImage1 = temporalMedian(Median1);//Median of preceeding frames
        Mat medianImage2 = temporalMedian(Median2);//Median of upcoming frames
        absdiff(medianImage1, medianImage2, diff);//Substraction

    }

    Mat FG_mot = Mat::zeros(diff.rows,diff.cols,CV_8UC1);
    FG_mot.setTo((float)255, diff < 20);//BG based motion
    FG_mot.setTo((float)0, diff >= 20);//BG based motion
    //Using 20 instead of Kapur threshols, as 20 is accepted for
    //frame difference comparisons (see evaluation of http://scenebackgroundmodeling.net/)

    //cv::imshow( "Med1", medianImage1);
    //cvWaitKey(1);
    //cv::imshow( "Med2", medianImage2);
    //cvWaitKey(1);
    //cv::imshow( "Diff", diff);
    //cvWaitKey(1);

    //Compute Motion History Image
    cv::add(this->motion_counter, (float)a, this->motion_counter, FG_mot); // Add C_COST if FG pixel
    this->motion_counter.setTo(0, FG_mot == 0);

    /**************************************************************/

    //Normalization
    Mat HI_F, HI_M, HI_S;
    //this->fgmask_counter.copyTo(HI_F);
    divide(this->fgmask_counter,(float)numframes2static,HI_F);
    HI_F.setTo(1,HI_F>1);
    divide(this->motion_counter,(float)numframes2static,HI_M);
    HI_M.setTo(1,HI_M>1);
    divide(this->ssim_counter,(float)numframes2static,HI_S);
    HI_S.setTo(1,HI_S>1);

    //cv::imshow( "HI_F", HI_F);
    //cvWaitKey(1);

    //cv::imshow( "HI_M", HI_M);
    //cvWaitKey(1);

    //cv::imshow( "HI_S", HI_S);
    //cvWaitKey(1);/**/

    //Combination
    Mat mask = Mat::zeros(diff.rows,diff.cols,CV_8UC1);
    mask.setTo(1,this->staticMask==1);//Values to 1 in frame t-1
    Mat SHI = Mat::zeros(diff.rows,diff.cols,CV_32FC1);
    SHI = (HI_F + HI_M + HI_S)/3;

    //Occlusion handling
    Mat maskMov = Mat::zeros(diff.rows,diff.cols,CV_8UC1);
    maskMov.setTo(1,HI_M<1 & HI_F==1 & HI_S==1);//Values to 1 in frame t-1

    //cv::imshow( "Combined HI", SHI);
    //cvWaitKey(1);

    this->staticMask = Mat::zeros(this->staticMask.rows,this->staticMask.cols,CV_8UC1);
    this->staticMask.setTo(255,maskMov==1 & SHI>=0.7);
    this->staticMask.setTo(255,maskMov==0 & SHI==1);
    this->staticMask.setTo(255,mask==1 & HI_F>=0.8 & HI_M<1 & HI_S==1);

    //cv::imshow( "My static mask", this->staticMask);
    //cvWaitKey(1);

}


/**
*	Method to set the weight decay (penalty for no-motion)
*
*	\params _wd The new value
*
*/
void StaticMaskExtractor_HistoryImages::setWeightDecay(double _wd)
{
    this->wd = _wd;
}

/**
*	Method to get the weight decay (penalty for no-motion)
*
*	\returns the current value
*
*/
double StaticMaskExtractor_HistoryImages::getWeightDecay()
{
    return this->wd;
}

/**
*	Method to set the confidence threshold
*
*	\params the new value
*
*/
void StaticMaskExtractor_HistoryImages::setConfidenceThreshold(int _th)
{
    this->confThreshold = _th;
}

/**
*	Method to get the confidence threshold
*
*	\returns the current value
*
*/
int StaticMaskExtractor_HistoryImages::getConfidenceThreshold()
{
    return confThreshold;
}

Mat StaticMaskExtractor_HistoryImages::getConfidenceImage()
{
    return fgmask_counter;
}

Mat funSSIM( const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;

    cv::Mat I1, I2;
    i1.convertTo(I1, d);            // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    cv::Mat I2_2   = I2.mul(I2);        // I2^2
    cv::Mat I1_2   = I1.mul(I1);        // I1^2
    cv::Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    cv::Mat mu1, mu2;                   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1_2   =   mu1.mul(mu1);
    cv::Mat mu2_2   =   mu2.mul(mu2);
    cv::Mat mu1_mu2 =   mu1.mul(mu2);

    cv::Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    cv::Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    cv::Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

    //Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
    return ssim_map;
}

Mat temporalMedian(vector<Mat> vector_images){

    int rows = vector_images[0].rows;
    int cols = vector_images[0].cols;
    int num_elem=vector_images.size();

    int median=0;
    vector<uchar> scores;
    Mat out = Mat::zeros(rows,cols,CV_8UC1);
    for(int i=0;i<rows;i++)//Recorro para los FTS+1 frames, el mismo pixel
    {
        for(int j=0;j<cols;j++)
        {
            for(int k=0; k < num_elem; k++)//Recorro los pixeles del frame de cada canal
            {
                uchar pixelValue = vector_images.at(k).at<uchar>(i,j);//Acces a pixel
                //Almaceno cada valor de pixel
                scores.push_back(pixelValue);
            }
            size_t size = (scores.size())/2;
            size_t size2 = (scores.size()+1)/2;
            nth_element(scores.begin(), scores.begin() + size, scores.end());
            median = scores[size2-1];
            out.at<uchar>(i,j) = median;
            scores.clear();
        }
    }

    return out;

}
