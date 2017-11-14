/**
 * \file Event.h 
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 20-09-08		
 * \brief Class to define an event
 * \version 1.1\n
 *			Version history:\n
 *				- 1.0 (1-06-08): Initial Implementation (jcs)
 *				- 1.1 (20-09-08): Modification to support new types of events (jcs)
 *
 */

//class description
/**
 * \class Event
 * \brief Class to define an event
 */

#ifndef EVENT_H_INCLUDE
#define EVENT_H_INCLUDE

#include "../blobs/ObjectBlob.h"
#include "../blobs/BlobList.h"

/**
 *	\var EVENT_TYPE
 *	Event defined to detect by the semantic system
 */
typedef enum {
	///Unknown event
	EVENT_UNKNOWN		= 0,
	///Presence event
	//EVENT_PRESENCE		= 1,
	///No Presence event
	//EVENT_NOPRESENCE	= 2,
	///Get Object event
	//EVENT_GETOBJECT		= 3,
	///Use Object event
	//EVENT_USEOBJECT		= 4,
	///Leave Object event
	//EVENT_LEAVEOBJECT	= 5,
	///People Counter event
	//EVENT_COUNTER		= 6,
	///People walking event
	//EVENT_WALKING		= 7,
	///Hand Up event
	//EVENT_HANDUP		= 8
	PutObject		= 2,
	AbandonedObject = 3,
	StolenObject	= 4,
	AbandonedPerson = 5,
	StolenPerson	= 6,

} EVENT_TYPE;

//class definition
class Event {
private:
	/// Event type
	EVENT_TYPE IDEvent;
	/// Event unique descriptor
	int ID;
	// Started frame of event
	int startFrame;
	// Finished frame of event
	int finishFrame;
	// Probability of event
	double score;
	// Decison of event
	bool decision;
	// List of Blob that perform the event
	BlobList<ObjectBlob *> *pObjList;	

	// life of event
	int life;
	// time interval
	int length;

	//event written flag
	bool written;

	//priority of the event
	int priority;

	//Number of objects of the event
	int numObjEvt;

	//Timestampo of the event
	char timestamp[256];

	//CvRect location;

	cv::Rect location;

public:
    // flag to actualization
    bool update;
    int numFramesNotDetected;

    int stolenCounter=0;
    int abandonedCounter=0;

    bool unique;

	// Default constructor
	Event();
	// Complet constructor
	//Event(int ID, int startFrame, BlobList<ObjectBlob *> *pBlobList=NULL, double score = 0.0, bool decision = false, int finishFrame = -1, bool written=false);
	Event(int ID, EVENT_TYPE IDEvent, int startFrame, int finishFrame, double score, bool decision, int life, int length, bool written=false, BlobList<ObjectBlob *> *pList=NULL, cv::Rect *location=NULL, int _priority=1);
	// Standar destructor
	virtual ~Event();

	// Method to get the id of event
	EVENT_TYPE getEventType();
	// Method to get the id's
	int getID();
	//int getEventType();
	// Method to get the start frame 
	int getStartFrame();
	// Method to get the finish frame
	int getFinishFrame();
	// Method to get the score of event
	double getScore();
	// Method to get the decision of event
	bool getDecision();
	// Method to get the blob's
	BlobList<ObjectBlob *> *getBlobList();
	// Method to get the flag of actualization
	bool getAct();
	// Method to get the life of event
	int getLife();
	// Method to get the time interval of event
	int getLength();
	// Method to get the written flag
	bool getWritten();
	// Method to get the priority of the event
	int getPriority();
	// Method to get the number of objects involved in the event
	int getNumObj();
	// Method to get the timestamp of the event
	char *getTimeStamp();
	// Method to get the location of the event
	cv::Rect *getLocation();

	// Method to set the location of the event	
	void setLocation(cv::Rect *newlocation);
	// Method to set the id of event
	void setEventType(EVENT_TYPE IDEvent);
	// Method to set the id
	void setID(int ID);
	// Method to set the start frame 
	void setStartFrame(int startFrame);
	// Method to set the finish frame
	void setFinishFrame(int finishFrame);
	// Method to set the score of event
	void setScore(double score);
	// Method to set to TRUE the decision of event
	void affDecision();
	// Method to set to FALSE the decision of event
	void negDecision();
	// Method to set the blob's id
	void setBlobList(BlobList<ObjectBlob *> *pList);
	// Method to set the priority of the event
	void setPriority(int newPriority);
	// Method to set the number of objects involved in the event
	void setNumObj(int newNum);

	// Method to set the blob's id
	void addBlob(ObjectBlob *pblob);
	
	// Method to set to TRUE the flag
	void affAct();
	// Method to set to FALSE the flag
	void negAct();
	// Method to set the life of event
	void setLife(int life);
	// Method to increment the life 
	void incrLife();
	// Method to decrement the life
	void decrLife();
	// Method to set the time interval
	void setLength(int length);
	// Method to increment the time interval
	void incrLength();
	// Method to decrement the time interval
	void decrLength();

	// Method to set written flag
	void setWritten(bool written);

private:
	void init(int ID, EVENT_TYPE IDEvent, int startFrame, int finishFrame, double score , bool decision , int life, int length, bool written,BlobList<ObjectBlob *> *pList, cv::Rect *location, int _priority);
};

#endif
