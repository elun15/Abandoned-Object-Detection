/**
 * \file EventController.h
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 20-12-08
 * \brief Class to define the event detection module (detection and event management)
 * \version 1.0\n
 *			Version history:\n
 *				- 1.0 (20-12-08): Initial Implementation (vfc)
 *				- 2.0 (12-06-16): update  (jcs)
 *
 */

//class description
/**
 * \class EventController
 * \brief Class to define the event detection module (detection and event management)
 */

#ifndef EVENT_CONTROLER_H_INCLUDE
#define EVENT_CONTROLER_H_INCLUDE

// Additional includes
#include <opencv2/opencv.hpp>
#include <vector>
#include "Event.h"
#include "../../../src/BLOBS/ObjectBlob.h"
#include "../../../src/BLOBS/BlobList.h"
//#include "event_buffers.h"
//#include "parameter_structure.h"
#include "EventWriter.h"

// Event detection methods
//#include "EventChecker_Counter.h"
//#include "EventChecker_GetObject.h"
//#include "EventChecker_HandUp.h"
//#include "EventChecker_LeaveObject.h"
//#include "EventChecker_NoPresence.h"
//#include "EventChecker_Presence.h"
//#include "EventChecker_UseObject.h"
//#include "EventChecker_Walking.h"


/**
 *	\def FRAMES_WAIT_STABLE
 *	Default number of frames to wait for stable data (FG masks)
 */
/// Default number of frames to wait for stable data (FG masks)
const int FRAMES_WAIT_STABLE = 25;

/**
 *	\def FRAMES_SHOW_ALARM
 *	Default number of frames to show the alarm for the detected event
 */
/// Default number of frames to show the alarm for the detected event
const int FRAMES_SHOW_ALARM = 50; 


const int MAX_LIFE = 3000;

const double MIN_OVERLAP_SAME_EVENT = 0.2f;//

//class definition
class EventController {

    //private:
public:
    ///Input images (current, background and FG Mask)
    //cv::Mat *_img,*_bkg,*_mask;
    /// List of active events
    vector<Event*> pActiveEvt;
    /// List of past detected events
    vector<Event*> pPastEvt;
    /// List of events detected in the current frame
    vector<Event*> pNewDetectedEvt;
    /// List of final events to write
    vector<Event*> pFinalEvents;
    /// File to write the events
    FILE* pXML;
    /// Current frame number
    int currentFrame;
    /// Number of detected Event
    int numberEvent= 1;
    /// Output Frame with detected events
    cv::Mat outputFrame;
    /// Region of interest (ROI)
    //CvBlob ROI;
    cv::Rect ROI;
    //input size of the image
    int wframe,hframe;
    ///Buffers for blobs and images (last TAM_BUFFER frames)
    //EVENT_BUFFERS pBuf;
    ///	Pointer to log file
    //FILE *pLog;
    /// Verbose flag
    int verbose_mode;
    ///Module to write in disk or send (via IP) the detected events
    EventWriter pwriter;

    double framerate;
    double time2static;

    /// Detection method for Counter event
    //EventChecker_Counter *pEvt_CO;

    /// Detection method for GetObject event
    //EventChecker_GetObject *pEvt_GO;

    /// Detection method for HandUp event
    //EventChecker_HandUp *pEvt_HUP;

    /// Detection method for LeaveObject event
    //EventChecker_LeaveObject *pEvt_LO;

    /// Detection method for NoPresence event
    //EventChecker_NoPresence *pEvt_NPE;

    /// Detection method for Presence event
    //EventChecker_Presence *pEvt_PE;

    /// Detection method for UseObject event
    //EventChecker_UseObject *pEvt_UO;

    /// Detection method for Walking event
    //EventChecker_Walking *pEvt_WA;

public:
    // Standard constructor
    EventController();
    void init(int rows, int cols, char *input_file, char *viper_file, int numframes, double framerate, double frames2static, int verbose_mode = 0);
    // Standard destructor
    virtual ~EventController();

    void checkFinalPastEvents();


    //Method to access the buffers (previous blob's list)
    //BlobList<ObjectBlob*> *getObjList(int numFrame);
    //Method to access the buffers (previous frames)
    //cv::Mat *getFrame(int numFrame);

    //Methods
    //Method to check if the blob is inside the region of interest (ROI)
    //int checkROI(CvBlob *pblob);
    //int checkROI(cv::Rect *pblob);
    // Method to set a ROI of events
    //void setROI(cv::Rect ROI);
    // Method to set the current frame number
    void setCurrentFrame(int currentFrame);
    // Method to increment the current frame number
    void incrCurrentFrame();
    // Method to decrement the current frame number
    void decrCurrentFrame();
    // Method to get the current frame number
    int getCurrentFrame();
    // Method to get the number of event detected
    int getNumberEvent();
    // Method to get the ROI of the events
    //CvBlob getROI();
    //cv::Rect getROI();

    // Method to use the event lists
    // Method to add a event to the list
    int addEvent(Event* event, vector<Event*> *pEventList);
    // Method to clear all events in the list
    int clearEvent(vector<Event*> *pEventList);
    // Method to delete a event by using its position
    int delEvent(int n,vector<Event*> *pEventList);
    // Method to delete a event by using its ID
    int delEventByID(int ID,vector<Event*> *pEventList);
    // Method to get the number of events stored in the list
    int getEventNum(vector<Event*> *pEventList);
    // Method to get a event by using its position
    Event* getEvent(int n,vector<Event*> *pEventList);
    // Method to get a event by using its ID
    Event* getEventByID(int ID,vector<Event*> *pEventList);
    // Method to set a event in a certain position
    int setEvent(int n, Event* newEvent,vector<Event*> *pEventList);
    // Method to set a event in a certain position (?)
    int setEventByID(int ID, Event* newEvent,vector<Event*> *pEventList);
    // Method to set the number of event generated
    void setNumberEvent(int NumberEvent);
    // Method to increment the number of event generated
    void incrNumberEvent();
    // Method to decrement the number of event generated
    void decrNumberEvent();
    // Method to write all current events
    int writeEvents(vector<Event*> *pEventList);
    // Method to check all events in one frame
    //int checkEvents(IplImage *img, IplImage *bkg, IplImage *mask, BlobList<ObjectBlob*> *pObjList);
    int checkEvents(BlobList<ObjectBlob*>* pObjList);
    // Method to print events
    void printEvents();

    //Methods for log file creation
    /*	// Method to get the file handle
        FILE* getFile();
        // Method to set the file handle
        void setFile(char *xmlfile);
        // Method to write XML Viper Header
        void writeViperHeader(char* filename,int numframes,double framerate,int wframe,int hframe);
        // Method to write XML Viper End
        void writeViperEnd();
        // Method to write close events
        int writeEvents();
        // Method to wriet a close event
        int writeEvent(Event* pEvent);
        // Method to write a close event in XML VIPER format
        int writeEventVIPER(Event* pEvent);
    /**/

    // Method to generat the output frame with the events detected
    //IplImage* EventController::getOutputFrame(IplImage* input_frame, BlobList<ObjectBlob*> *pObjList);

    //cv::Mat EventController::getOutputFrame(cv::Mat input_frame, BlobList<ObjectBlob*> *pObjList); //before
    cv::Mat getOutputFrame(cv::Mat input_frame, BlobList<ObjectBlob*> *pObjList);
    // Method to generate a output frame with the legend of the events to detect

    //cv::Mat EventController::getLegendFrame(cv::Mat input_frame, BlobList<ObjectBlob*> *pObjList);

private:
    // Method to update buffer data
    //int updateData(BlobList<ObjectBlob*> *pObjList);

    // Method to detect new events
    int detectNewEvents(BlobList<ObjectBlob*> *pObjList);

    // Method to check dependencies between events (same area, object,...)
    int	checkDependencies();
    /*check if there are new detected events that exist in the event list*/
    void checkDep_NewEventsThatExist();
    /*remove events in active list that have become past events*/
    void checkDep_RemoveOldEvents();
    /*Check inconsistency between detected events (same area and time)*/
    void checkDep_InconsistencyAreaTime();
    //method to write a message (console or file)
    void message(char *string, int NumFrame);
    void message(char *string, double value);
};

#endif
