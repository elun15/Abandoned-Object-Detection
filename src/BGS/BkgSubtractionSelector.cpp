/************************************************************************************/
/* Source file of BkgSubtractionSelector class                                      */
/* This class allows us to choose a background subtraction method                   */
/*                                       */
/************************************************************************************/
#include "BkgSubtractionSelector.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "../../../lib/bgslibrary/package_bgs/bgslibrary.h"
#include <opencv2/video/background_segm.hpp>

#include "../../../src/video.h"


using namespace std;
using namespace cv;
using namespace bgslibrary::algorithms;


//Constructor: Identify selected subtraction technique from the main programa and initialize the needed variables

BkgSubtractionSelector::BkgSubtractionSelector(int BGSid)
{

    this->int_BGSid = BGSid;
    this->learningRateOverride = 0;
    this->learningRate = 16;
    cout << "BkgSubtractionSelector()" << endl;
    cout << "Method: " << BGSid << endl;


}

// Destructor.
BkgSubtractionSelector::~BkgSubtractionSelector()
{
    std::cout << "~BkgSubtractionSelector()" << std::endl;
}


//Init function: Determines the variables size in order to suit them to the video sequence resolution
//and create the subtraction technique object
void BkgSubtractionSelector::init(Mat frame)
{

    this->img_output = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
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

        cout << "KNN Method selected." << endl;
        this->bgs = new KNN;
        break;

    case 5:

        cout << "Independent Multimodal Method selected." << endl;
        this->bgs = new IndependentMultimodal;
        break;


    case 6:

        cout << "SUSBSENSE Method selected." << endl;
        this->bgs = new SuBSENSE;
        break;


    default:
        break;

    }


}




//Funcion process: realiza el procesamiento de la secuencia de video a analizar segun la tecnica de substraccion de fondo
//seleccionada desde el programa principal y muestra los resultados si asi se desea.
void BkgSubtractionSelector::process(Mat frame,video Video)
{
    img_input = frame.clone();
    img_output = Mat::zeros(frame.rows,frame.cols,frame.type());

    this->bgs->process(frame,this->img_output,this->img_bgmodel);


    if (!Video.contextMask1.empty())
    {
        bitwise_and(this->img_output,Video.contextMask1,this->img_output);
    }

    if (Video.ShowResults)
    {
        imshow("Background image",this->img_bgmodel );
        waitKey(1);
        imshow("Foregroung image",this->img_output );
        waitKey(1);

    }


    if( Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
    {

        putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
        imwrite(d,img_input);

    }



}


//Funcion GetForegroundImage: Captura y devuelve el foreground procesado del frame actual bajo analisis.
Mat BkgSubtractionSelector::GetForegroundImage()
{
    vector<Mat> vector;
    vector.push_back(this->img_output);
    //return vector;
    return this->img_output;
}


//Funcion GetBGModel: Captura y devuelve el modelo de fondo procesado del frame actual bajo analisis.
Mat BkgSubtractionSelector::GetBGModel()
{
    return this->img_bgmodel;
}
