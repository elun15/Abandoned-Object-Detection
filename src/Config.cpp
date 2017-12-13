#include <iostream>   // std::cout
#include <string>     // std::string

#include <opencv2/opencv.hpp>

#include "Config.h"
#include "utils/utils.h" // to_string_

using namespace std;
using namespace cv;

Config::Config()
{
    init(DEFAULT_BKG_METHOD,DEFAULT_SFGD_METHOD,DEFAULT_PD_METHOD,DEFAULT_SOC_METHOD, "./datasets/VISOR_test/visor_Video00.avi","./results", DEFAULT_TIME_TO_STATIC);
}

Config::Config(int argc, char *argv[])
{
    if (argc == 8)
        init(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5],argv[6],atoi(argv[7]));
    else {
        cout << "Wrong number of parameters" << endl;
        print_usage();
        exit(EXIT_FAILURE);
    }
}

Config::Config(std::string inputPath, std::string resultsDir, int seconds)
{
    init(DEFAULT_BKG_METHOD,DEFAULT_SFGD_METHOD,DEFAULT_PD_METHOD,DEFAULT_SOC_METHOD, inputPath,resultsDir,seconds);
}

Config::Config(std::string inputPath, std::string resultsDir, int bkg, int sfgd, int pd, int soc, int seconds)
{

    init(bkg,sfgd,pd,soc,inputPath,resultsDir,seconds);
}

void Config::init(int bkg, int sfgd, int pd, int soc, std::string inputPath, std::string resultsDir,int seconds)
{
    this->m_bkg     = bkg;
    this->m_sfgd    = sfgd;
    this->m_pd      = pd;
    this->m_soc 	= soc;
    this->inputPath = inputPath;
    this->resultsDir= resultsDir;

    //output for images
    this->DirImages = this->resultsDir + "/images/";
    //Path for settings file
    //in QT
    //this->fileSettingsPath = this->resultsDir + "config" + to_string_(this->m_bkg) +  to_string_( this->m_sfgd)+ to_string_( this->m_pd)+ to_string_( this->m_soc  )+"/parameters.settings";
    //Terminal:
    this->fileSettingsPath = "./results/config" + to_string_(this->m_bkg) +  to_string_( this->m_sfgd)+ to_string_( this->m_pd)+ to_string_( this->m_soc  )+"/parameters.settings";


    //default display/output Config
    this->ShowResults = DEFAULT_RESULTS_SHOW;
    this->SaveResults = DEFAULT_RESULTS_SAVE_XML;
    this->SaveImages = DEFAULT_RESULTS_SAVE_IMG;
    this->SaveImages_freq = DEFAULT_RESULTS_SAVE_IMG_FREQ;

    //default flags
    this->flag_minsize = DEFAULT_FLAG_MINSIZE;
    this->flag_nearpeople = DEFAULT_FLAG_NEARPEOPLE;
    this->flag_stillpeople = DEFAULT_FLAG_STILLPEOPLE;
    this->flag_contextmask = DEFAULT_FLAG_MASK;


    //other Config
    this->time_to_static = seconds;
    this->DetectPeopleAlways = DEFAULT_DETECT_ALWAYS;

    //find filename for the full video path
    this->inputvideo = findFilename(this->inputPath);

    //output files
    if (this->SaveResults == true){
        string baseoutfile = resultsDir + inputvideo + "_"+ to_string_(this->m_bkg) + "_" + to_string_( this->m_sfgd) + "_"+ to_string_( this->m_pd )+ "_"+to_string_( this->m_soc ) ;
        this->fileResults = baseoutfile + "_"+ currentDateTime() + ".xml";    // XML file with results (.xml)
        this->fileTime = baseoutfile + "_"+ currentDateTime() + ".time";      // file with execution times (.time)
    }

    //print the current settings
    print_parsed_data();
}

void Config::print_parsed_data()
{
    //display parameters
    cout << " Selected bkg method: " << this->m_bkg << endl;
    cout << " Selected sfgd method: " << this->m_sfgd << endl;
    cout << " Selected people detector method:" << this->m_pd << endl;
    cout << " Selected static object classifier method: " << this->m_soc << endl;
    cout << " Input video dir: " << this->inputPath << endl;
    cout << " Output dir: " << this->resultsDir  << endl<< endl;

    cout << " Time (seconds) to static: " << this->time_to_static << endl;
    cout << " Show results: " << this->ShowResults << endl;
    if (this->SaveImages == true)
        cout << " Save results (images): " << this->SaveImages << "(saved in " <<  this->DirImages << ")" << endl;
    else
        cout << " Save results (images): " << this->SaveImages << endl;

    cout << " Save results (xml): " << this->SaveResults << endl;
    if (this->SaveResults == true){
        cout << " - Results will be saved in (XML): " << this->fileResults << endl;
        cout << " - Results will be saved in (Time): " << this->fileTime << endl;
    }

    findContextMask();//find contextual masks for processing

}

Config::~Config()
{

}

void Config::get_video_info(cv::VideoCapture cap)
{
    //open video file & check if it has been correctly opened
    if (!cap.isOpened())
    {
        cout << "Could not open the video file" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        // Read video properties
        this->framerate = cap.get(CAP_PROP_FPS);
        this->totalNumFrames = cap.get(CAP_PROP_FRAME_COUNT);
        this->rows = cap.get(CAP_PROP_FRAME_HEIGHT);
        this->cols = cap.get(CAP_PROP_FRAME_WIDTH);
        cout << "Video successfully opened";
        cout << this->framerate << " fps. Size: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    }
}

string Config::findFilename(string filepath)
{
    // Compute videoName variable, used for saving the results
    string basename  = filepath.substr(filepath.find_last_of("/")+1);
    string::size_type const point_position(basename.find_last_of('.'));
    return basename.substr(0,point_position);
}


void Config::findContextMask()
{


    if (flag_contextmask == true)
    {
        //READ CONTEXT MASK FOR EVERY VIDEO


       // string mask_folder  = inputPath.substr(3,inputPath.find_last_of("/")-2);
        string mask_folder  = inputPath.substr(0,inputPath.find_last_of("/")+1);
        string mask_path = mask_folder + this->inputvideo + "_mask.jpg";
        this->contextMask = imread(mask_path,CV_LOAD_IMAGE_GRAYSCALE);
        if (this->contextMask.empty())
        {
            cout << "Could not open mask image:"<< mask_path << endl;
            exit(EXIT_FAILURE);
        }
        else
        {
           // imshow("Context mask", this->contextMask);
           // waitKey();
        }

    }
    else
    {
        this->contextMask = Mat();

    }

    //ONLY FOR AVSS, TO REMOVE BOUNDARY ERRORS
    if ( this->inputvideo.find("AVSS") != string::npos)
    {
        this->AVSS_FGMask = imread("./datasets/AVSS2007/AVSS_Mask_1.jpg",CV_LOAD_IMAGE_GRAYSCALE); //BORDES
        if (this->AVSS_FGMask.empty())
        {
            cout << "Could not open mask image." << endl;
            exit(EXIT_FAILURE);
        }
        bitwise_not(this->AVSS_FGMask,this->AVSS_FGMask);
    }
    else
    {
        this->AVSS_FGMask = Mat();
    }


}


void Config::print_usage()
{
    cout << "Command usage:"<<endl;
    cout << " <System_Evaluation> <bgs_method> <sfgd_method> <people_detector_method> <static_classifier_method><videofilepath> <output_dir>" << endl;

    cout << "/*****************************************************/" << endl;
    cout << "/*** IMPLEMENTED ALGORITHMS **************************/" << endl;
    cout << "/*****************************************************/" << endl;
    cout << " - BGS          	--> 1:LOBSTER; 2:PAWCS; 3:MOG2; 4:KNN; 5:Multimodal; 6:SUBSENSE" << endl;
    cout << " - SBGS         	--> 1:Subsampling; 2:Acc Mask" << endl;
    cout << " - PEOPLE DETECTOR --> 1:HOG; 2:DPM; 3:ACF" << endl;
    cout << " - CLASSIFIER   	--> 1:High Gradient; 2:Histogram" << endl;
}
