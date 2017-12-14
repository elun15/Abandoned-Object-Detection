#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <vector>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

///time to detect a stationary object (in seconds)
#define DEFAULT_TIME_TO_STATIC 30

///Flag to show results
#define DEFAULT_RESULTS_SHOW false

///Flag to detect people in every frame
#define DEFAULT_DETECT_ALWAYS false

///Flag to save results (xml/txt files)
#define DEFAULT_RESULTS_SAVE_XML true

///Flag to save results (images)
#define DEFAULT_RESULTS_SAVE_IMG false
#define DEFAULT_RESULTS_SAVE_IMG_FREQ 25

///Flag to filter by min size
#define DEFAULT_FLAG_MINSIZE true

///Flag to filter still people
#define DEFAULT_FLAG_STILLPEOPLE true

///Flag to filter objects near people
#define DEFAULT_FLAG_NEARPEOPLE true

///Flag to filter objects by context masks
#define DEFAULT_FLAG_MASK true

#define DEFAULT_BKG_METHOD  3
#define DEFAULT_SFGD_METHOD 4
#define DEFAULT_PD_METHOD   4
#define DEFAULT_SOC_METHOD  2



class Config
{
public:

    int cols;
    int rows;

    // PROGRAM OPTIONS
    bool ShowResults; // Boolean to show or not the results
    bool DetectPeopleAlways; // Detect people in every frame if true
    bool SaveResults;  // Save XML results file if true
    bool SaveImages; // Save results images if true
    int SaveImages_freq; //frequency to save results in disk

    //FLAGS
    bool flag_stillpeople;
    bool flag_nearpeople;
    bool flag_minsize;
    bool flag_contextmask;

    // DIRECTORIES
    std::string inputPath;  //full path for video input
    std::string inputvideo; //video input filename

    std::string fileResults; // XML file with results (.xml)
    std::string fileTime;    // file with execution times (.time)
    std::string resultsDir;  //
    std::string DirImages;
    std::string fileSettingsPath; //Path for the settings file

    //std::vector<cv::Point> maskPoints;
    cv::Mat AVSS_FGMask;
    cv::Mat contextMask;

    int time_to_static; // Seconds
    int numFrame;
    int totalNumFrames;
    double framerate;

    // METHODS
    int m_bkg;
    int m_sfgd;
    int m_pd;
    int m_soc;
public:
	Config();
	Config(int argc, char *argv[]);
    Config(std::string inputPath, std::string resultsDir, int seconds);
    Config(std::string inputPath, std::string resultsDir, int bkg, int m_sfgd, int pd, int soc,int seconds);

    ~Config();

    void print_usage();
    void print_parsed_data();
    void get_video_info(cv::VideoCapture cap);

    std::string findFilename(std::string filepath);
    void findContextMask();

private:
    void init(int bkg, int m_sfgd, int pd, int soc, std::string inputPath, std::string resultsDir, int seconds);
};

#endif //__SETTINGS_H__
