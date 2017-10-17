//file description
/**
 * \file Event.cpp 
 * \author Victor Fernandez-Carbajales(vfc)
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 20-05-08		
 * \brief Implementation of Event class
 * \version 1.1\n
 *			Version history:\n
 *				- 1.0(?): Initial Implementation (vfc)
 *
 */

#include "Event.h"
#include "time.h"

/**
 *	Standard class Constructor. 
 *
 */
Event::Event() {
	init(-1, EVENT_UNKNOWN, -1, -1, -1.0 , false , 0, 0, false, NULL, NULL, -1);
}


/**
 *	Class Constructor with initial data
 *
 *	\param ID Unique event ID 
 *	\param startFrame Start frame of the event
 *	\param finishFrame End frame of the event
 *	\param score Likelihood of the event
 *	\param decision Indicates if event has been accepted or rejected
 *	\param life 
 *	\param length Duration of the event
 *	\param written 
 *	\param pList List of objects of interest for the event
 *
 */
Event::Event(int ID, EVENT_TYPE IDEvent, int startFrame, int finishFrame, double score , bool decision , int life, int length, bool written,BlobList<ObjectBlob *> *pList, cv::Rect *_location, int _priority)
{
	init(ID, IDEvent, startFrame, finishFrame, score , decision , life, length, written,pList, _location, _priority);
}

/**
 *	Method to init resources
 *
 *	\param ID Unique event ID 
 *	\param startFrame Start frame of the event
 *	\param finishFrame End frame of the event
 *	\param score Likelihood of the event
 *	\param decision Indicates if event has been accepted or rejected
 *	\param life 
 *	\param length Duration of the event
 *	\param written 
 *	\param pList List of objects of interest for the event
 *
 */
void Event::init(int ID, EVENT_TYPE IDEvent, int startFrame, int finishFrame, double score , bool decision , int life, int length, bool written,BlobList<ObjectBlob *> *pList, cv::Rect *_location, int _priority)
{

    this->setID(ID);
	this->setEventType(IDEvent);
	this->setStartFrame(startFrame);
	this->setFinishFrame(finishFrame);
	this->setScore(score);
	if (this->decision)
		this->affDecision();
	else
		this->negDecision();
	this->setLife(life);
	this->setLength(length);
	this->setWritten(written);
	this->setPriority(_priority);

	this->numObjEvt = 0;
    this->numFramesNotDetected = 0;
    this->update = true;
    this->unique = false;
	this->pObjList = new BlobList<ObjectBlob*>;
	if(pList != NULL)
	{
		ObjectBlob *pB=NULL;

		for(int i = 0; i< pList->getBlobNum();i++)
		{
			pB = pList->getBlob(i);
			this->pObjList->addBlob(pB);
			this->numObjEvt++;
		}
	}

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strcpy(this->timestamp, asctime (timeinfo));

	if(_location!=NULL)
		location = *_location;
	else
		location = cvRect(-1,-1,-1,-1);
}

/**
 *	Standard class destructor
 *
 */
Event::~Event() {
	if(this->getBlobList()){
		this->getBlobList()->clear();
		delete this->getBlobList();		
		this->pObjList=NULL;
	}
}
/**
 *	Method to get the event location
 *
 * \return CvRect the event location as a CvRect structure (OpenCV)
 */
cv::Rect *Event::getLocation() 
{
	return &(this->location);
}

/**
 *	Method to set the location of the eventID
 *
 * \param Newlocation New spatial location of the event
 */
void Event::setLocation(cv::Rect *Newlocation) {
	if(Newlocation!=NULL)
		location = *Newlocation;	
}


/**
 *	Method to get the ID
 *
 * \return int event's ID
 */
int Event::getID() {
	return this->ID;
}

/**
 *	Method to get the ID of the event
 *
 * \return int event's ID
 */
EVENT_TYPE Event::getEventType() {
	return this->IDEvent;
}

/**
 *	Method to get the start frame 
 *
 * \return int frame of event
 */
int Event::getStartFrame() {
	return this->startFrame;
}

/**
 * Method to get the finish frame
 *
 * \return int the finish frame
 */
int Event::getFinishFrame() {
	return this->finishFrame;
}

/**
 * Method to get the score of event
 *
 * \return double the score of event
 */
double Event::getScore() {
	return this->score;
}

/**
 * Method to get the decision of event
 *
 * \return bool the decision of event
 */
bool Event::getDecision() {
	return this->decision;
}

/**
 * Method to get the blob's
 *
 * \return int the blob's
 */
BlobList<ObjectBlob *> *Event::getBlobList() {
	return this->pObjList;
}

/**
 * Method to get the flag of actualization
 *
 * \return bool the flag status
 */
bool Event::getAct() {
    return this->update;
}

/**
 * Method to get the life of event
 *
 * \return int the life of event 
 */
int Event::getLife() {
	return this->life;
}

/**
 * Method to get the time interval
 *
 * \return int the time interval
 */
int Event::getLength() {
	return this->length;
}

/**
 * Method to get the written flag
 *
 * \return the written flag
 */
bool Event::getWritten() {
	return this->written;
}

/**
 * Method to get the priority of the event
 *
 * \return the priority
 */
int Event::getPriority() {
	return this->priority;
}

/**
 * Method to get the number of objects involved in the event
 *
 * \return the written flag
 */
int Event::getNumObj() {
	return this->numObjEvt;
}

/**
 * Method to get the timestamp of the event ("Day%week Month Day%num hh:mm:ss Year")
 *
 * \return the string containing the timestamp
 */
char *Event::getTimeStamp()
{
	return this->timestamp;
}
// Method to get the timestamp of the event
	

/**
 * Method to get the priority of the event
 *
 * \param newPriority New value
 */
void Event::setPriority(int newPriority) {
	 this->priority=newPriority;
}

/**
 * Method to set the number of objects involved in the event
 *
 * \param newNumObj New value
 */
void Event::setNumObj(int newNumObj) {
	this->numObjEvt=newNumObj;
}

/**
 *	Method to set the ID of the event
 *
 * \param ID event's ID
 */
void Event::setEventType(EVENT_TYPE IDEvent) {
	this->IDEvent = IDEvent;
}

/**
 *	Method to set the ID
 *
 * \param ID event's ID
 */
void Event::setID(int ID) {
	this->ID = ID;
}
 
/**
 * Method to set the start frame
 *
 * \param startFrame the start frame
 */
void Event::setStartFrame(int startFrame) {
	this->startFrame = startFrame;
}

/**
 * Method to set the finish frame
 *
 * \param finish the finish frame
 */
void Event::setFinishFrame(int finishFrame) {
	this->finishFrame = finishFrame;
}
 
/**
 * Method to set the score of event
 *
 * \param score the score of event
 */
void Event::setScore(double score) {
	this->score = score;
}
 
/**
 * Method to set to TRUE the decision of event
 */
void Event::affDecision() {
	this->decision = true;
}

/**
 * Method to set to FALSE the decision of event
 */
void Event::negDecision() {
	this->decision = false;
}
 
/**
 * Method to set to TRUE the flag
 */
void Event::affAct() {
    this->update = true;
}

/**
 * Method to set to FALSE the flag
 */
void Event::negAct() {
    this->update = false;
}

/**
 * Method to set the life of event
 *
 * \param life the life of event
 */
void Event::setLife(int life) {
	this->life = life;
}	

/**
 * Method to increment the life
 */
void Event::incrLife() {
	this->life++;
}

/**
 * Method to decrement the life
 */
void Event::decrLife() {
	this->life--;
}

/**
 * Method to set the time interval
 *
 * \param length the time interval
 */
void Event::setLength(int length) {
	this->length = length;
}	

/**
 * Method to increment the time interval
 */
void Event::incrLength() {
	this->length++;
}

/**
 * Method to decrement the time interval
 */
void Event::decrLength() {
	this->length--;
}

/**
 * Method to set written flag
 */
void Event::setWritten(bool written) {
	this->written=written;
}

/**
 * Method to add a blob to the event
 */
void Event::addBlob(ObjectBlob *pblob)
{
	ObjectBlob *pB = new ObjectBlob();

	/*pB->setID(pblob->getID());
	pB->setWidth(pblob->getWidth());
	pB->setX(pblob->getX());
	pB->setY(pblob->getY());
	pB->setHeight(pblob->getHeight());
	/**/
	pB->ID = pblob->ID;
	pB->x = pblob->x;
	pB->y = pblob->y;
	pB->w = pblob->w;
	pB->h = pblob->h;
	pB->setStaticCount (pblob->getStaticCount());
	pB->setCount (pblob->getCount());
	pB->results->E_CHA =  pblob->results->E_CHA;
	pB->results->E_CHS =  pblob->results->E_CHS;
	pB->results->E_GHA =  pblob->results->E_GHA;
	pB->results->E_GHS =  pblob->results->E_GHS;
	pB->results->E_GLA =  pblob->results->E_GLA;
	pB->results->E_GLS =  pblob->results->E_GLA;
	pB->results->E_FA =  pblob->results->E_FA;
	pB->results->E_FS =  pblob->results->E_FS;

	pB->setPeopleLikelihood(pblob->getPeopleLikelihood());
	//pB->setFrameStart(pblob->getFrameStart());	
	pB->initialStaticFrame = pblob->initialStaticFrame;

	pObjList->addBlob(pB);
}
