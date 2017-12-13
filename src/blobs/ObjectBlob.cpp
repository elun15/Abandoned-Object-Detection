//file description
/**
* \file ObjectBlob.cpp
* \author Juan Carlos San Miguel (jcs)
* \date 30-03-09
* \brief Implementation of ObjectBlob class
* \version 1.2\n
*			Version history:\n
*				- 1.0(01-06-08): Initial Implementation (jcs)
*				- 1.1(05-06-08): Added support for people detection(jcs)
*				- 1.2(30-03-09): Added support for velocity(jcs)
*
*/

// Addicional includes
#include "ObjectBlob.h"

/**
*	Standard class Constructor.
*
*/
ObjectBlob::ObjectBlob()
{

	init(0, NULL, 0, 0, 0, 0, 0.0, false, 0, NULL);
}

/**
*	Class Constructor with specific parameters
*
*/
ObjectBlob::ObjectBlob(int id, cvBlob *pBlob, int count, int StaticCount, int StartFrame, int LastFrame, double peopleLikelihood, bool _act, int _numberOfCheckingSteps, DETECTION_RESULTS *_results)
{
	init(id, pBlob, count, StaticCount, StartFrame, LastFrame, peopleLikelihood, _act, _numberOfCheckingSteps, _results);
}

/**
*	Method to initialize resources.
*
*/
void ObjectBlob::init(int id, cvBlob *pBlob, int count, int StaticCount, int StartFrame, int LastFrame, double peopleLikelihood, bool _act, int _numberOfCheckingSteps, DETECTION_RESULTS *_results)
{
	this->results = new DETECTION_RESULTS();

	this->v = this->vPrev = cvPoint(0, 0);

	this->initialStaticFrame = 0;

	this->results->nFrame = -1;
	this->results->numCalc = 0;

	//scores obtained with each detector
	results->S_GRD = results->S_GH = results->S_GL = results->S_CONB = results->S_SHP = results->S_CON = results->S_CH = results->fusion = results->S_CH_dO = results->S_CH_dN = results->S_CH_dON = 0.0;
	//Evidences of being unattended or stolen
	results->E_GRDA = results->E_GRDS = results->E_CONBA = results->E_CONBS = results->E_SHPA = results->E_SHPS = results->E_GLA = results->E_GLS = results->E_GHA = results->E_GHS = results->E_CHA = results->E_CHS = results->E_CONA = results->E_CONS = results->E_FS = results->E_FA = results->E_F1S = results->E_F1A = results->E_F2S = results->E_F2A = results->E_F3S = results->E_F3A = results->E_F4S = results->E_F4A = results->E_F5S = results->E_F5A = 0.0;
	//final decissions of each detector
	results->D_GRD = results->D_CONB = results->D_SHP = results->D_CH = results->D_CON = results->D_CH = results->D_ED = results->D_GH = results->D_GL = results->D_F = results->D_GT = results->D_F1 = results->D_F2 = results->D_F3 = results->D_F4 = results->D_F5 = NONE;


	//if(_results != NULL)
	//	*results = *_results;

	//this->setID(id);
	/*if (pBlob != NULL)
		this->setBlob(pBlob);*/
	//this->setID(id);
	/*if (pBlob != NULL)
	this->setBlob(pBlob);*/
	this->setCount(count);
	this->setStaticCount(StaticCount);
	this->setLastFrame(LastFrame);
	//this->setFrameStart(StartFrame);
	//this->setFrameStart(StartFrame);
	this->setPeopleLikelihood(peopleLikelihood);
	this->act = _act;
	this->numberOfCheckingSteps = _numberOfCheckingSteps;
	
	//Aniadido por jorge gomez vicente
	if (pBlob != NULL) {
		this->x = pBlob->x;
		this->h = pBlob->h;
		this->y = pBlob->y;
		this->w = pBlob->w;
		this->peopleLikelihood = pBlob->PeopleLikelihood;
	}

    this->attended = false;
}

/**
*	Standard class destructor
*
*/
ObjectBlob::~ObjectBlob()
{
	if (results != NULL)
		delete results;
}

/**
*	Returns the life count of the blob
*
* \return int The count life of the blob
*
*/
int ObjectBlob::getCount() {
	return this->count;
}

/**
*	Returns the life count of the blob
*
* \return int The count life of the blob
*/
bool ObjectBlob::getAct() {
	return this->act;
}

/**
*	Returns if the people likelihood of the blob has been computed.
*
* \return bool Boolean value
*/
bool ObjectBlob::getPeopleLikelihoodComputed() {
	return this->peopleLikelihoodComputed;
}

/**
*	Method to set that the people likelihood has been computed
*
* \param bool Boolean value indicating the likelihood computed
*/
void ObjectBlob::setPeopleLikelihoodComputed(bool peopleLikelihoodComputed) {
	this->peopleLikelihoodComputed = peopleLikelihoodComputed;
}

/**
*	Method to set the life count of the blob.
*
* \param count the new value of the life count
*
*/
void ObjectBlob::setCount(int count) {
	this->count = count;
}

/**
*	Method to increase the life count by one
*
*/
void ObjectBlob::incrCount() {
	this->count++;
}

/**
*	Method to decrease the life count by one
*
*/
void ObjectBlob::decrCount() {
	this->count--;
}

/**
*	Method to put in false state the blob
*
*/
void ObjectBlob::negAct() {
	this->act = false;
}

/**
*	Method to put in true state the blob
*
*/
void ObjectBlob::affAct() {
	this->act = true;
}


/**
*	Method to set the detection results for all the detectors
*
*/
void ObjectBlob::setDetectionResults(DETECTION_RESULTS *pdetection)
{
	*results = *pdetection;
}

/**
*	Method to get the detection results of the blob
*
*/
DETECTION_RESULTS *ObjectBlob::getDetectionResults()
{
	return results;
}

// Method to set the Static Life count of the blob
void ObjectBlob::setStaticCount(int value)
{

	this->staticCount = value;
}
// Method to set the Last frame of the static Life count of the blob
void ObjectBlob::setLastFrame(int value)
{
	this->lastFrame = value;
}
// Method to set the Likelihood of being people
void ObjectBlob::setPeopleLikelihood(double value)
{
	this->peopleLikelihood = value;
}

// Method to increase the static life count
void ObjectBlob::incrStaticCount()
{
	this->staticCount++;
}
// Method to decrease the static life count
void ObjectBlob::decrStaticCount()
{
	this->staticCount--;
}

// Method to get the Static Life count of the blob
int ObjectBlob::getStaticCount()
{
	return this->staticCount;
}
// Method to get the Last frame of the static Life count of the blob
int ObjectBlob::getLastFrame()
{
	return this->lastFrame;
}
// Method to get the Likelihood of being people
double ObjectBlob::getPeopleLikelihood()
{
	return this->peopleLikelihood;
}

// Method to get the number of checking steps that has been performed previously
int ObjectBlob::getNumOfCheckingSteps()
{
	return this->numberOfCheckingSteps;
}

// Method to increment number of checking steps that has been performed previously
void ObjectBlob::IncrNumOfCheckingSteps()
{
	this->numberOfCheckingSteps++;
}
// Method to decrement number of checking steps that has been performed previously
void ObjectBlob::DecrNumOfCheckingSteps()
{
	this->numberOfCheckingSteps--;
}

// Method to set the number of checking steps that has been performed previously
void ObjectBlob::setNumOfCheckingSteps(int value)
{
	this->numberOfCheckingSteps = value;
}

cv::Rect ObjectBlob::getRect() {
	return cv::Rect(this->x - this->w / 2, this->y - this->h / 2, this->w, this->h);
}


/**
*	Method to set the attended state
*
*/
void ObjectBlob::setAttended(bool newAttState) {
    this->attended = newAttState;
}

/**
*	Method to get the attended state
*
*/
bool ObjectBlob::getAttended() {
    return this->attended;
}
