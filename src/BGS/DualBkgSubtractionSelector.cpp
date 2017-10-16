/************************************************************************************/
/* Source file of BkgSubtractionSelector class                                      */
/* This class allows us to choose a background subtraction method                   */
/*                                       */
/************************************************************************************/
#include "DualBkgSubtractionSelector.h"
#include "BkgSubtractionSelector.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "../../../lib/bgslibrary/package_bgs/bgslibrary.h"
#include <opencv2/video/background_segm.hpp>

#include "../../../src/settings.h"


using namespace std;
using namespace cv;
using namespace bgslibrary::algorithms;


//Constructor: Identify selected subtraction technique from the main program and initialize the needed variables

DualBkgSubtractionSelector::DualBkgSubtractionSelector(int BGSid)
    : BkgSubtractionSelector ( BGSid)
{

    this->int_BGSid = BGSid;
    this->learningRateOverride = 0;
    this->learningRate = 16;
    cout << "DualBkgSubtractionSelector()" << endl;
    cout << "Method: " << BGSid << endl;


}

// Destructor.
DualBkgSubtractionSelector::~DualBkgSubtractionSelector()
{
    std::cout << "~DualBkgSubtractionSelector()" << std::endl;
}


//Init function: Determines the variables size in order to suit them to the video sequence resolution
//and create the subtraction technique object
void DualBkgSubtractionSelector::init(Mat frame)
{

    this->img_outputLong = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    this->img_outputshort = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    this->img_bgmodel = Mat::zeros(frame.rows, frame.cols, CV_8UC3);


    switch (this->int_BGSid)
    {


    case 1:
        cout << "LOBSTER Method selected." << endl;

        this->bgs = new LOBSTER;
        break;

    case 2:
        cout << "PAWCS Method selected." << endl;

        this->bgs = new PAWCS;
        break;


    case 3:

        cout << "MOG2 Method selected." << endl;

        this->bgs = new MixtureOfGaussianV2;

        break;

    case 4:

        cout << " KNN Method selected." << endl;

        this->bgs = new KNN;
        break;

    case 5:

        cout << "Independent Multimodal Method selected." << endl;

        this->bgs = new IndependentMultimodal;
        break;


    case 6:

        cout << "Dual SUSBSENSE Method selected." << endl;
        this->bgsLong = new SuBSENSE(600);
        this->bgsShort = new SuBSENSE(100);
        break;


    default:
        break;

    }

}




//Funcion process: realiza el procesamiento de la secuencia de video a analizar segun la tecnica de substraccion de fondo
//seleccionada desde el programa principal y muestra los resultados si asi se desea.
void DualBkgSubtractionSelector::process(Mat frame, settings Video)
{
      img_input = frame.clone();
    if (Video.sfgd_method == 4) //DBM
    {
        this->img_outputLong = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
        this->img_outputshort = Mat::zeros(frame.rows, frame.cols, CV_8UC1);

        this->bgsShort->process(frame,this->img_outputshort,this->img_bgmodel);
        this->bgsLong->process(frame,this->img_outputLong,this->img_bgmodelLong);


        if (!Video.contextMask1.empty())
        {
            bitwise_and(this->img_outputLong,Video.contextMask1,this->img_outputLong);
            bitwise_and(this->img_outputshort,Video.contextMask1,this->img_outputshort);
        }


        if (Video.ShowResults)

        {
            imshow("Frame image",frame );
            waitKey(1);
            imshow("Short Background image",this->img_bgmodel );
            waitKey(1);
            imshow("Long Background image",this->img_bgmodelLong );
            waitKey(1);
            imshow("Short Foregroung image",this->img_outputshort);
            waitKey(1);
            imshow("Long Foregroung image",this->img_outputLong);
            waitKey(1);



        }


    }
    else
    {
        this->img_output = Mat::zeros(frame.rows, frame.cols, CV_8UC1);

        this->bgs->process(frame,this->img_output,this->img_bgmodel);

        if (!Video.contextMask1.empty())
        {
            bitwise_and(this->img_output,Video.contextMask1,this->img_output);
        }

        if (Video.ShowResults)
        {
            imshow("Background image",this->img_bgmodel );
            waitKey(1);
            imshow("Foregroung image",this->img_output);
            waitKey(1);

        }

    }


    if( Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
    {

        putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
        imwrite(d,img_input);

    }



}


std::vector<cv::Mat> DualBkgSubtractionSelector::GetForegroundImage()
{

    std::vector<cv::Mat> vector;
    vector.push_back(this->img_outputshort);
    vector.push_back(this->img_outputLong);
    return vector;
}

//Funcion GetBGModel: Captura y devuelve el modelo de fondo procesado del frame actual bajo analisis.
