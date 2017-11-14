#include <iostream>   // std::cout
#include <string>     // std::string

#include <opencv2/opencv.hpp>

#include "Config.h"
#include "utils/utils.h" // to_string_

using namespace std;
using namespace cv;

Config::Config()
{
	init(DEFAULT_BKG_METHOD,DEFAULT_SFGD_METHOD,DEFAULT_PD_METHOD,DEFAULT_SOC_METHOD, "./datasets/VISOR_test/visor_Video00.avi","./results");
}

Config::Config(int argc, char *argv[])
{
	if (argc == 7)
		init(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5],argv[6]);
	else {
		cout << "Wrong number of parameters" << endl;
		print_usage();
		exit(EXIT_FAILURE);
	}
}

Config::Config(std::string inputPath, std::string resultsDir)
{
	init(DEFAULT_BKG_METHOD,DEFAULT_SFGD_METHOD,DEFAULT_PD_METHOD,DEFAULT_SOC_METHOD, inputPath,resultsDir);
}

Config::Config(std::string inputPath, std::string resultsDir, int bkg, int sfgd, int pd, int soc)
{

	init(bkg,sfgd,pd,soc,inputPath,resultsDir);
}

void Config::init(int bkg, int sfgd, int pd, int soc, std::string inputPath, std::string resultsDir)
{
	this->m_bkg     = bkg;
	this->m_sfgd    = sfgd;
	this->m_pd      = pd;
	this->m_soc 	= soc;
	this->inputPath = inputPath;
	this->resultsDir= resultsDir;

	 //output for images
	this->DirImages = this->resultsDir + "/images/";

	//default display/output Config
	this->ShowResults = DEFAULT_RESULTS_SHOW;
	this->SaveResults = DEFAULT_RESULTS_SAVE_XML;
	this->SaveImages = DEFAULT_RESULTS_SAVE_IMG;
	this->SaveImages_freq = DEFAULT_RESULTS_SAVE_IMG_FREQ;


	//other Config
	this->time_to_static = DEFAULT_TIME_TO_STATIC;
    this->DetectPeopleAlways = DEFAULT_DETECT_ALWAYS;

	//find filename for the full video path
	this->inputvideo = findFilename(this->inputPath);

	//output files
	if (this->SaveResults == true){
		string baseoutfile = resultsDir + inputvideo + "_"+ to_string_(this->m_bkg) + "_" + to_string_( this->m_sfgd) + "_"+ to_string_( this->m_soc ) + "_" + to_string_( this->m_pd );
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

//TODO: fix the path for the contextual masks
void Config::findContextMask()
{
    // Check if it is required to apply context mask
    if ( this->inputvideo.find("AVSS") != string::npos)
    {
        this->contextMask1 = imread("../datasets/AVSS_test/AVSS_Mask_1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if (this->contextMask1.empty())
        {
            cout << "Could not open mask image." << endl;
            exit(EXIT_FAILURE);
        }
        bitwise_not(this->contextMask1,this->contextMask1);

        this->contextMask = imread("../datasets/AVSS_test/AVSS_Mask_2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if (this->contextMask.empty())
        {
            cout << "Could not open mask image." << endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
    	this->contextMask1 = Mat();
    	this->contextMask = Mat();
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
