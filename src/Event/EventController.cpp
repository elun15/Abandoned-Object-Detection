/**
 * \file EventController.cpp
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 20-12-08
 * \brief Implementation of EventController class
 * \version 1.1\n
 *			Version history:\n
 *				- 1.0 (05-08-08): Initial Implementation (vfc)
 *				- 1.1 (20-12-08): Use of the EventChecker class (jcs)
 *				- 2.0 (12-06-16): update  (jcs)
 *              - 3.0 (17-09-17): update (elg)
 /

//Additional includes
#include "EventController.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp> //VideoCapture
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include "opencv2/opencv.hpp" //opencv libraries
#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

/**
 *	Standard class Constructor
 *	\param _pLog Point to log file
 *	\param _verbose_mode Flag to indicate verbose mode (=1) or not (=0)
 *
 */

#include "EventController.h"
EventController::EventController()
{

}

void EventController::init(int rows, int cols, char *input_file, char *viper_file, int numframes, double _framerate, double _time2static, int _verbose_mode) {

    //init variables
    this->framerate = _framerate;
    this->time2static = _time2static;
    this->verbose_mode = _verbose_mode;
    //this->pLog = _pLog;
    this->currentFrame = 1;
    this->numberEvent = 0;


    pActiveEvt.clear();
    pPastEvt.clear();
    pNewDetectedEvt.clear();
    pFinalEvents.clear();

    pwriter.initViperFile(viper_file, input_file, numframes,_framerate, cols, rows);
}

/**
 *	Standard class destructor.
 *
 */
EventController::~EventController() {


    writeEvents(&pFinalEvents);


}
/**********************************************************************
                    EVENT DETECTION
**********************************************************************/

int EventController::checkEvents( BlobList<ObjectBlob*>* pObjList)
{
    int ret = -1;
    currentFrame++;

    message("EVENT_CONTROLLER: \n", currentFrame);


    //detect new events
    detectNewEvents(pObjList);


    //check dependencies between the new events detected and the old events
    checkDependencies();


    //print list of active events
    printEvents();

    return 0;
}

int EventController::detectNewEvents(BlobList<ObjectBlob*> *pCurObjList)
{
    double score=0.0;

    message("\tChecking new events...\n", currentFrame);

    //clear list of new detected events
    if (pNewDetectedEvt.size() > 0)
        this->clearEvent(&pNewDetectedEvt);

    //Check object-based events
    for(int i = 0; i < pCurObjList->getBlobNum(); i++)
        //if (checkROI(pCurObjList->getBlob(i)->getRect()) == 1)
    {
        Event *evt = NULL;
        //this->numberEvent++;
        cv::Rect location = cv::Rect(pCurObjList->getBlob(i)->x, pCurObjList->getBlob(i)->y, pCurObjList->getBlob(i)->w, pCurObjList->getBlob(i)->h);

        DECISION dec = pCurObjList->getBlob(i)->results->D_F;
        switch (dec)
        {
        case STATIC_OBJ_TYPE_STOLEN:
            message("\t\tDetected <StolenObject>", score);
            //evt = new Event(this->getNumberEvent(), StolenObject, this->getCurrentFrame() - (this->framerate*this->time2static), this->getCurrentFrame(), 0, true, 0, 1, false, NULL, &location, 3);
            evt = new Event(this->getNumberEvent(), StolenObject, this->getCurrentFrame() , this->getCurrentFrame(), 0, true, 0, 1, false, NULL, &location, 3);

            this->addEvent(evt, &pNewDetectedEvt);
            break;
        case STATIC_OBJ_TYPE_ABANDONED:
            message("\t\tDetected <AbandonedObject>", score);

            //evt = new Event(this->getNumberEvent(), AbandonedObject, this->getCurrentFrame() - (this->framerate*this->time2static), this->getCurrentFrame(), 0, true, 0, 1, false, NULL, &location, 3);
            evt = new Event(this->getNumberEvent(), AbandonedObject, this->getCurrentFrame(), this->getCurrentFrame(), 0, true, 0, 1, false, NULL, &location, 3);

            this->addEvent(evt, &pNewDetectedEvt);
            break;
        default:
            message("\t\tDetected <Unknown>", score);
            break;


        }
    }
    return 1;
}

int EventController::checkDependencies()
{
    message("\tChecking dependencies between events...\n", currentFrame);

    /*check if there are new detected events that exist in the event list*/
    checkDep_NewEventsThatExist();

    /*Check inconsistency between detected events (same area and time)*/
    checkDep_InconsistencyAreaTime();

    /*remove events in active list that have become past events*/
    checkDep_RemoveOldEvents();

    return 1;
}

void EventController::checkDep_NewEventsThatExist()
{	
    Event *evtN,*evt,*newActEvt,*pastEvt;


    if( ((int)pActiveEvt.size()) ==  0 ) //No active events
    {
        for (int i = 0; i< (int)pNewDetectedEvt.size(); i++) // Add all the new detected events to active list
        {
            evtN = this->getEvent(i,&pNewDetectedEvt);
            newActEvt = new Event(this->getNumberEvent(),evtN->getEventType(), evtN->getStartFrame(), evtN->getFinishFrame(), evtN->getScore(),
                                  evtN->getDecision(), evtN->getLife(), evtN->getLength(), evtN->getWritten(), NULL, evtN->getLocation() );

            this->addEvent(newActEvt, &pActiveEvt);

            this->incrNumberEvent();
        }
    }
    else // Active events list is not empty
    {

        // Set all active events as non-updated
        for(int i=0;i<(int)pActiveEvt.size();i++)
        {
            evt = this->getEvent(i,&pActiveEvt);
            evt->update = false;

        }

        // Search new detections in the active events

        for (int i = 0; i< (int)pNewDetectedEvt.size(); i++)
        {
            evtN = this->getEvent(i,&pNewDetectedEvt);
            bool overlapping = false;

            for (int j = 0; j< (int)pActiveEvt.size(); j++)
            {
                evt = this->getEvent(j,&pActiveEvt);

                //search new event detected in event list (type and blob)

                cv::Rect A = *(evtN->getLocation());
                cv::Rect B = *(evt->getLocation());
                cv::Rect inter = A & B;

                double op1 = 2*inter.area() ;
                double op2 = (A.area() + B.area());
                double solap;


                solap = op1 / op2;

                if (solap > MIN_OVERLAP_SAME_EVENT)
                {
                    ////update event info (only blob position)

                    overlapping = true;
                    //If they are the same event, update the finish frame

                    evt->setFinishFrame(evtN->getFinishFrame());


                    //Only update new location if the blob is increasing in order to keep the maximum size of the event
                    if (A.area() > B.area())
                    {
                        evt->setLocation(evtN->getLocation());
                    }

                    evt->update = true;

                    //Update counters
                    if (evtN->getEventType() == AbandonedObject )
                    {
                        evt->abandonedCounter++;
                    }
                    else
                    {
                        evt->stolenCounter++;
                    }

                    //Update type
                    if ( evt->abandonedCounter >= evt->stolenCounter )
                    {
                        evt->setEventType(AbandonedObject);
                    }
                    else
                    {
                        evt->setEventType(StolenObject);
                    }

                    if (verbose_mode == 1)
                        fprintf(stderr, "\t\tUpdating event! (TYPE %d) (ID=%d)\n", evt->getEventType(), evt->getID());

                }

            }

            if(overlapping == false)
            {

                if(verbose_mode==1)
                    fprintf(stderr, "\t\tEvent Added to active list! (TYPE %d) (ID=%d)\n", evtN->getEventType(), evtN->getID());

                newActEvt = new Event(this->getNumberEvent(),evtN->getEventType(), evtN->getStartFrame(), evtN->getFinishFrame(), evtN->getScore(),
                                      evtN->getDecision(), evtN->getLife(), evtN->getLength(), evtN->getWritten(), NULL, evtN->getLocation() );


                this->addEvent(newActEvt, &pActiveEvt);
                this->incrNumberEvent();
            }


        }

        // Check updates in active list
        for(int i=0;i<(int)pActiveEvt.size();i++)
        {
            evt = this->getEvent(i,&pActiveEvt);

            if (evt->update == false)
            {
                evt->numFramesNotDetected++;

            }

            if (evt->numFramesNotDetected > 1000)
            {
                if (evt->getLife() > 50) //al menos 100 frames
                {
                    pastEvt = new Event(evt->getID(),evt->getEventType(), evt->getStartFrame(), evt->getFinishFrame(), evt->getScore(),
                                        evt->getDecision(), evt->getLife(), evt->getLength(), evt->getWritten(), NULL, evt->getLocation());
                    addEvent(pastEvt, &pPastEvt);

                    delEventByID(evt->getID(), &pActiveEvt);
                }
                else
                {
                    delEventByID(evt->getID(), &pActiveEvt);
                }

            }

        }

    }


}


void EventController::checkFinalPastEvents()
{
    //All actives to past events
    for(int i=0;i<(int)pActiveEvt.size();i++)
    {
        Event *event,*pastevent;
        event = getEvent(i,&pActiveEvt);
        pastevent = new Event(event->getID(),event->getEventType(), event->getStartFrame(), event->getFinishFrame(), event->getScore(),
                              event->getDecision(), event->getLife(), event->getLength(), event->getWritten(), NULL, event->getLocation());
        addEvent(pastevent, &pPastEvt);

    }

    //All new detected to past
    for(int i=0;i<(int)pNewDetectedEvt.size();i++)
    {
        Event *event,*pastevent;
        event = getEvent(i,&pNewDetectedEvt);
        pastevent = new Event(event->getID(),event->getEventType(), event->getStartFrame(), event->getFinishFrame(), event->getScore(),
                              event->getDecision(), event->getLife(), event->getLength(), event->getWritten(), NULL, event->getLocation());
        addEvent(pastevent, &pPastEvt);

    }



    //CHECK past events
    for(int i=0;i<(int)pPastEvt.size();i++)
    {
        Event *e1;
        e1 = getEvent(i,&pPastEvt);
        int flag_write = 1;

        if(((int)pFinalEvents.size()) == 0)
        {
            addEvent(e1, &pFinalEvents);
        }


        else{
            //compare each past event with the other past events
            for(int j=0;j<(int)pFinalEvents.size();j++)
            {

                Event *e2;
                e2 = getEvent(j,&pFinalEvents);

                //check spatial overlap
                cv::Rect A = *(e1->getLocation());
                cv::Rect B = *(e2->getLocation());
                cv::Rect inter = A & B;


                EVENT_TYPE type1 = e1->getEventType();
                EVENT_TYPE type2 = e2->getEventType();


                double op1 = 2*inter.area() ;
                double op2 = (A.area() + B.area());
                double solap  ;

                solap = op1 / op2;

                if (flag_write == 1)
                {
                    // If are not the same
                    if ((solap < 0.3f )|| (solap > 0.3f && (type1 != type2)) || (e1->getLife() > 50))
                    {
                        flag_write = 1;

                    }
                    else //If are the same
                    {
                        flag_write = 0;

                    }
                }


            }

            if (flag_write == 1)
            {

                addEvent(e1, &pFinalEvents);
            }

        }

    }



}

void EventController::checkDep_RemoveOldEvents()
{
    //NOTA:
    /*remove events in active list that have become past events*/
    Event *pastEvt=NULL, *evt=NULL;

    for(int j=0;j<(int)pActiveEvt.size();j++)
    {
        evt = this->getEvent(j,&pActiveEvt);

        if (evt->getLife() == MAX_LIFE)
        {
            if(verbose_mode==1)
                fprintf(stderr, "\t\tEvent passed to inactive (ID=%d TYPE %d)\n", evt->getID(),evt->getEventType());

            pastEvt = new Event(evt->getID(),evt->getEventType(), evt->getStartFrame(), evt->getFinishFrame(), evt->getScore(),
                                evt->getDecision(), evt->getLife(), evt->getLength(), evt->getWritten(), NULL, evt->getLocation());
            addEvent(pastEvt, &pPastEvt);

            delEventByID(evt->getID(), &pActiveEvt);
        }
        else
            evt->incrLife();
    }
}

void EventController::checkDep_InconsistencyAreaTime()
{

    Event *evt1,*evt2;
    cv::Rect r1, r2, intersection;
    double intArea, minArea;
    int stFrame1, stFrame2, fnFrame1,fnFrame2;
    bool duplicated = false;

    //remove detected events in the same area and time
    for(int i=0;i< (int)this->pActiveEvt.size();i++)
        for(int j=0;j< (int)this->pActiveEvt.size();j++)
            if(i!=j)
            {
                duplicated = false;

                evt1 = this->getEvent(i,&pActiveEvt);
                evt2 = this->getEvent(j,&pActiveEvt);

                if(evt1 != NULL && evt2 != NULL)
                {

                    r1 = *(evt1->getLocation());
                    r2 = *(evt2->getLocation());


                    {
                        intersection = r1 & r2;
                        intArea=intersection.width*intersection.height;
                        (r1.width*r1.height > r2.width*r2.height)?minArea=r2.width*r2.height:minArea=r1.width*r1.height;

                        if((double)intArea/(double)minArea > 0.7)
                        {
                            //check the overlap time
                            stFrame1 = evt1->getStartFrame();
                            fnFrame1 = evt1->getFinishFrame();
                            stFrame2 = evt2->getStartFrame();
                            fnFrame2 = evt2->getFinishFrame();

                            int var1 = max(stFrame1,stFrame2);
                            int var2 = min(fnFrame1,fnFrame2);

                            if(var1 <= var2)
                            {
                                //si hay solapamiento
                                int l1 = fnFrame1 - stFrame1+1;
                                int l2 = fnFrame2 - stFrame2+1;
                                int loverlap = var2 - var1+1;
                                double overlap=0.0;
                                (l1 > l2)?overlap=(double)loverlap/(double)l2:overlap=(double)loverlap/(double)l1;

                                if(overlap > 0.001)
                                    duplicated = true;

                            }
                        }
                    }

                    if(duplicated == true)
                    {
                        if(verbose_mode==1)
                            fprintf(stderr, "\t\tDeleted duplicated event...(ID1=%d ID2=%d)", evt1->getID(),evt2->getID());

                        //get the event happened most times (5 times)
                        if(evt1->getLife() >= FRAMES_WAIT_STABLE)
                        {
                            if(verbose_mode==1)
                                fprintf(stderr, "(del ID=%d)\n", evt2->getID());
                            this->delEventByID(evt2->getID(), &pActiveEvt);
                        }
                        else
                        {
                            if(evt2->getLife() >= FRAMES_WAIT_STABLE)
                            {
                                if(verbose_mode==1)
                                    fprintf(stderr, "(del ID=%d)\n", evt1->getID());
                                this->delEventByID(evt1->getID(), &pActiveEvt);
                            }
                            else
                            {
                                //delete the event with less probability
                                if(evt1->getScore() > evt2->getScore())
                                {
                                    if(verbose_mode==1)
                                        fprintf(stderr, "(del ID=%d)\n", evt2->getID());
                                    this->delEventByID(evt2->getID(), &pActiveEvt);
                                }
                                else
                                {
                                    if(verbose_mode==1)
                                        fprintf(stderr, "(del ID=%d)\n", evt1->getID());
                                    this->delEventByID(evt1->getID(), &pActiveEvt);
                                }
                            }
                        }
                        this->decrNumberEvent();
                    }
                }
            }

}

/**********************************************************************
                        PARAMETERS GET/SET
**********************************************************************/

/**
 *	Method to get the current frame
 *
 * \return int the current frame
 */
int EventController::getCurrentFrame() {
    return this->currentFrame;
}

/**
 *	Method to set the current frame
 *
 * \return currentFrame the current frame
 */
void EventController::setCurrentFrame(int currentFrame) {
    this->currentFrame = currentFrame;
}

/**
 *	Method to increment the current frame
 *
 */
void EventController::incrCurrentFrame() {
    this->currentFrame++;
}

/**
 *	Method to decrement the current frame
 *
 */
void EventController::decrCurrentFrame() {
    this->currentFrame--;
}

/**********************************************************************
                        EVENT LIST
**********************************************************************/
/**
 *	Method to get the number of event generated
 *
 * \return int the number of event generated
 */
int EventController::getNumberEvent() {
    return this->numberEvent;
}

/**
 *	Method to set the number of event generated
 *
 * \return numberEvent the number of event generated
 */
void EventController::setNumberEvent(int numberEvent) {
    this->numberEvent = numberEvent;
}

/**
 *	Method to increment the number of event generated
 *
 */
void EventController::incrNumberEvent() {
    this->numberEvent++;
}

/**
 *	Method to decrement the number of event generated
 */
void EventController::decrNumberEvent() {
    this->numberEvent--;
}


/**
 *	Method to add a event in the event's list
 *
 * \param Event Event to add.
 *
 * \return Returns operation code
 */
int EventController::addEvent(Event *evt, vector<Event *> *pEventList)
{

    if (pEventList == NULL)
        pEventList = &pActiveEvt;

    pEventList->push_back(evt);

    return 0;
}

/**
 *	Method to clear the event's list
 *
 * \return Returns operation code
 */
int EventController::clearEvent(vector<Event *> *pEventList) {

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    for(int i = 0; i < (int)pEventList->size(); i++) {
        delete this->getEvent(i, pEventList);
    }
    pEventList->clear();

    return 0;
}

/**
 *	Method to delete a event in the event's list
 *
 * \param n Position of the event to delete.
 *
 * \return Returns operation code
 */
int EventController::delEvent(int n, vector<Event *> *pEventList)
{
    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    delete this->getEvent(n,pEventList);
    vector<Event*>::iterator iter;
    iter = pEventList->begin();
    iter = iter+n;
    pEventList->erase(iter);
    return 0;
}

/**
 *	Method to delete a event in the event's list by using its ID
 *
 * \param ID Id of event to delete.
 *
 * \return Returns operation code
 */
int EventController::delEventByID(int ID, vector<Event *> *pEventList) {

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    vector<Event*>::iterator iter;
    int i = 0;
    iter = pEventList->begin();

    for (i = 0; i < (int)pEventList->size(); i++) {
        if (pEventList->at(i)->getID() == ID) {
            delete this->getEvent(i,pEventList);
            iter = iter + i;
            pEventList->erase(iter);
            return 0;
        }
    }
    return -1;
}

/**
 *	Method to get the number of Event in the event's list
 *
 * \return Returns the number of Event
 */
int EventController::getEventNum(vector<Event *> *pEventList) {
    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    return (int)pEventList->size();
}

/**
 *	Method to get a event in the event's list
 *
 * \param n Position of the event to get.
 *
 * \return Returns the event in this position
 */
Event* EventController::getEvent(int n, vector<Event *> *pEventList) {

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    if (this->getEventNum(pEventList) > n)
        return pEventList->at(n);
    return NULL;
}

/**
 *	Method to get a event in the event's list by using this ID
 *
 * \param ID Id of the event to get.
 *
 * \return Returns the event with this ID
 */
Event* EventController::getEventByID(int ID, vector<Event *> *pEventList) {
    int i = 0;

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    for(i = 0; i < (int)pEventList->size(); i++) {
        if (pEventList->at(i)->getID() == ID) {
            return pEventList->at(i);
        }
    }
    return NULL;
}

/**
 *	Method to set a event in the event's list
 *
 * \param n Position of the new event to set.
 * \param newEvent the new event to set
 *
 * \return Returns operation code
 */
int EventController::setEvent(int n, Event* newEvent, vector<Event *> *pEventList) {

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    if (n > (int)pEventList->size())
        return -1;
    vector<Event*>::iterator iter = pEventList->begin();
    iter = iter+n;
    delete pEventList->at(n);
    pEventList->insert(iter, newEvent);
    return 0;
}

/**
 *	Method to set a event in the input event's list by using this ID
 *
 * \param ID Id of the old event to set.
 * \param newEvent the new event to set
 *
 * \return Returns operation code
 */
int EventController::setEventByID(int ID, Event* newEvent, vector<Event *> *pEventList) {
    int i = 0;

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    vector<Event*>::iterator iter = pEventList->begin();

    for(i = 0; i < (int)pEventList->size(); i++) {
        if (pEventList->at(i)->getID() == ID) {
            delete pEventList->at(i);
            pEventList->insert(iter, newEvent);
            return 0;
        }
        iter++;
    }
    return -1;
}

/**
 *	Method to close the event list and write them in the log file (if their life is more than MAX_LIFE)
 *
 *
 * \return Returns operation code
 */
int EventController::writeEvents(vector<Event *> *pEventList) {

    Event* evento = NULL;

    if(pEventList == NULL)
        pEventList = &pActiveEvt;

    for(int i = 0; i < (int)pEventList->size(); i++)
    {

        evento = this->getEvent(i,pEventList);
        if (evento != NULL)
        {
            if(evento->getWritten()==false)
            {//los eventos EVENT_OBJECTPUT solo se escriben una vez
                pwriter.writeEventVIPER(evento,false);

                evento->setWritten(true);
            }
        }
    }

    return 0;
}



void EventController::printEvents()
{
    Event *evt;

    /*print events*/
    //if(verbose_mode==1)
    fprintf(stderr,"\tCurrent event List (%d events) (past %d)\n", pActiveEvt.size(), pPastEvt.size());

    //if (verbose_mode > 1)

    for(int i=0;i< (int)pActiveEvt.size();i++)
    {
        evt = this->getEvent(i,&pActiveEvt);

        switch(evt->getEventType())
        {

        case AbandonedObject:
            if(verbose_mode==1)
                fprintf(stderr,"\t\t Event ID = %d (AbandonedObject)(life=%d)(score=%.2f)\n", evt->getID(), evt->getLife(), evt->getScore());
            break;
        case StolenObject:
            if(verbose_mode==1)
                fprintf(stderr,"\t\t Event ID = %d (StolenObject)(life=%d)(score=%.2f)\n", evt->getID(), evt->getLife(), evt->getScore());
            break;

        }
    }
    /**/
}

/**/
// Method to generat the output frame with the events detected
cv::Mat EventController::getOutputFrame(cv::Mat input_frame, BlobList<ObjectBlob*> *pObjList)
{
    cv::Scalar colour;
    cv::Point p1, p2;

    //draw active events
    for (int i = 0; i < this->getEventNum(&pActiveEvt); i++) {
        Event *evt = this->getEvent(i, &pActiveEvt);
        ObjectBlob *pB= new ObjectBlob();
        CvRect location = *(evt->getLocation());
        pB->x=location.x;
        pB->y=location.y;
        pB->w=location.width;
        pB->h=location.height;

        switch(evt->getEventType()) {


        case AbandonedObject: // GREEN
            //if (evt->getLife() < 2*MAX_LIFE)
        {
            colour.val[1] = 255;
            colour.val[0] = colour.val[2] = colour.val[3] = 0;
            p1.x = (int)pB->x + 4;
            p1.y = (int)pB->y + 4;
            p2.x = (int)(pB->x + pB->w) + 4;
            p2.y = (int)(pB->y + pB->h) + 4;
        }
            break;
        case StolenObject: // ??
            //if (evt->getLife() < 2*MAX_LIFE)
        {
            colour.val[1] = 255;
            colour.val[2] = 255;
            colour.val[0] = colour.val[3] = 0;
            p1.x = (int)pB->x + 4;
            p1.y = (int)pB->y + 4;
            p2.x = (int)(pB->x + pB->w) + 4;
            p2.y = (int)(pB->y + pB->h) + 4;
        }
            break;



        default:
            colour.val[0]=colour.val[1]=colour.val[2]=colour.val[3]=0;
        }
        cv::rectangle(input_frame, p1, p2, colour, 2);
    }
    return input_frame;
}

void EventController::message(char *string, int NumFrame)
{
    if(verbose_mode==1)
        fprintf(stderr, "%s", string);
}

void EventController::message(char *string, double value)
{
    if(verbose_mode==1)
        fprintf(stderr, "%s (%.2f)\n", string, value);
}
