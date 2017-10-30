/**
 * \file EventWriter.h 
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 20-10-09
 * \brief Class to define an interface to write (ViPER format) or send (IP) the events detected
 * \version 1.1\n
 *			Version history:\n
 *				- 1.0 (1-06-08): Initial Implementation (jcs)
 *				- 1.1 (20-10-09): Modification to support IP EventWriters (jcs)
 *
 */

//class description
/**
 * \class EventWriter
 * \brief Class to define an interface to write (ViPER format) or send (IP) the events detected
 */

#ifndef EventWriter_H_INCLUDE
#define EventWriter_H_INCLUDE


#include "Event.h"
#include "blobs/ObjectBlob.h"
#include "blobs/BlobList.h"


//draw settings
const int SANGRIA_OUTPUT = 10;
const int INTERLINE_OUTPUT = 15;
const int ANCHURALEYENDA = 110;
const int ANCHURAMEDIANA = 20;
//const int TIME_INTERVAL_MIN = 10;

//class definition
class EventWriter {
private:

	/// File to write the events
	char* pXML;
	FILE *pf;

	bool writeViper;

	int wframe, hframe;

public:
	// Default constructor
	EventWriter();

	// Standar destructor
	~EventWriter();

	//method to write the detected events (in disk or network)
    int writeEvents(vector<Event*> *pActiveEvt, bool resize);

	int initViperFile(char *filenameXML, char* source_filename,int numframes,double framerate,int wframe,int hframe);	
	void writeViperHeader(char* filename,int numframes,double framerate,int wframe,int hframe);
    int writeEventVIPER(Event* pEvent, bool resize);
	int writeViperEnd();
};

#endif
