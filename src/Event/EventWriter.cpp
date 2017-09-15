/**
 * \file EventWriter.cpp
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 20-10-09
 * \brief Implementation of the interface to write (ViPER format) or send (IP) the events detected
 * \version 1.1\n
 *			Version history:\n
 *				- 1.0 (1-06-08): Initial Implementation (jcs)
 *				- 1.1 (20-10-09): Modification to support IP EventWriters (jcs)
 * \version 2.0\n
 *			Version history:\n
 *				- 2.0(13/09/17): Adapted to openCV 3.2 (elg)
 *
 */

#include "EventWriter.h"
//#include <winsock2.h>


/**
 *	Default Constructor
 *
 */
EventWriter::EventWriter()
{
    this->writeViper = false;
    this->pf = NULL;
    this->pXML = NULL;
}

/**
 *	Default destructor
 *
 */
EventWriter::~EventWriter()
{

    if(this->writeViper == true)
    {
        writeViperEnd();
        fclose(pf);
    }
}


int EventWriter::initViperFile(char *filenameXML, char* source_filename, int numframes, double framerate, int wframe, int hframe)
{
    if (filenameXML != NULL)
    {
        this->writeViper = true;
        this->pXML = (char *)malloc(strlen(filenameXML) + 1);
        strcpy(pXML, filenameXML);
    }
    else
        pXML = NULL;

    pf = fopen(pXML, "w+");

    cout << "Init viper file" << endl;
    if (this->pf == NULL)
        cout << "WARNING: Viper log file can't be create" << endl;
    else
        writeViperHeader(source_filename, numframes, framerate, wframe, hframe);

    return 1;
}


/**
 *	Method to process all the active events and write them (to disk or network)
 *
 *	\param pActiveEvt List active events
 *
 *	\return operation code
 */
int EventWriter::writeEvents(std::vector<Event*> *pEvt, bool resize)
{
    Event* evt = NULL;

    for(int i = 0; i < (int)pEvt->size(); i++) {
        evt = pEvt->at(i);

        if(evt->getWritten() == false)
        {
            //this->writeEvent(evento);
            //if(this->writeViper)
            this->writeEventVIPER(evt,resize);

            evt->setWritten(true);
        }
    }
    return 1;
}

void EventWriter::writeViperHeader(char* filename,int numframes,double framerate,int _wframe,int _hframe)
{

    if(this->pf !=NULL)
    {
        fprintf(this->pf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
        fprintf(this->pf,"<viper xmlns=\"http://lamp.cfar.umd.edu/viper#\" xmlns:data=\"http://lamp.cfar.umd.edu/viperdata#\">\n");
        fprintf(this->pf,"<config>\n");
        fprintf(this->pf,"<descriptor name=\"Information\" type=\"FILE\">\n");
        fprintf(this->pf,"<attribute dynamic=\"false\" name=\"SOURCETYPE\" type=\"http://lamp.cfar.umd.edu/viperdata#lvalue\">\n");
        fprintf(this->pf,"<data:lvalue-possibles>\n");
        fprintf(this->pf,"<data:lvalue-enum value=\"SEQUENCE\" /> \n");
        fprintf(this->pf,"<data:lvalue-enum value=\"FRAMES\" /> \n");
        fprintf(this->pf,"</data:lvalue-possibles>\n");
        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"<attribute dynamic=\"false\" name=\"NUMFRAMES\" type=\"http://lamp.cfar.umd.edu/viperdata#dvalue\" /> \n");
        fprintf(this->pf,"<attribute dynamic=\"false\" name=\"FRAMERATE\" type=\"http://lamp.cfar.umd.edu/viperdata#fvalue\" /> \n");
        fprintf(this->pf,"<attribute dynamic=\"false\" name=\"H-FRAME-SIZE\" type=\"http://lamp.cfar.umd.edu/viperdata#dvalue\" /> \n");
        fprintf(this->pf,"<attribute dynamic=\"false\" name=\"V-FRAME-SIZE\" type=\"http://lamp.cfar.umd.edu/viperdata#dvalue\" /> \n");
        fprintf(this->pf,"</descriptor>\n");

        fprintf(this->pf, "<descriptor name=\"PutObject\" type=\"OBJECT\">\n");
        fprintf(this->pf, "<attribute dynamic=\"false\" name=\"BoundingBox\" type=\"http://lamp.cfar.umd.edu/viperdata#bbox\" /> \n");
        fprintf(this->pf, "</descriptor>\n");

        fprintf(this->pf, "<descriptor name=\"AbandonedObject\" type=\"OBJECT\">\n");
        fprintf(this->pf, "<attribute dynamic=\"false\" name=\"BoundingBox\" type=\"http://lamp.cfar.umd.edu/viperdata#bbox\" /> \n");
        fprintf(this->pf, "</descriptor>\n");

        fprintf(this->pf, "<descriptor name=\"StolenObject\" type=\"OBJECT\">\n");
        fprintf(this->pf, "<attribute dynamic=\"false\" name=\"BoundingBox\" type=\"http://lamp.cfar.umd.edu/viperdata#bbox\" /> \n");
        fprintf(this->pf, "</descriptor>\n");

        fprintf(this->pf, "<descriptor name=\"AbandonedPerson\" type=\"OBJECT\">\n");
        fprintf(this->pf, "<attribute dynamic=\"false\" name=\"BoundingBox\" type=\"http://lamp.cfar.umd.edu/viperdata#bbox\" /> \n");
        fprintf(this->pf, "</descriptor>\n");

        fprintf(this->pf, "<descriptor name=\"StolenPerson\" type=\"OBJECT\">\n");
        fprintf(this->pf, "<attribute dynamic=\"false\" name=\"BoundingBox\" type=\"http://lamp.cfar.umd.edu/viperdata#bbox\" /> \n");
        fprintf(this->pf, "</descriptor>\n");

        fprintf(this->pf,"</config>\n");
        fprintf(this->pf,"<data>\n");
        char cad[1024];
        int i = 0,k=0;
        for(i=0;i<(int)strlen(filename);i++)
        {
            if(filename[i] == 'C' && filename[i+1] == 'a' && filename[i+2] == 't' && filename[i+3] == 'e' && filename[i+4] == 'g' && filename[i+5] == 'o'&& filename[i+6] == 'r' && filename[i+7] == 'y')
                break;
        }

        for (k=i;k < (int)strlen(filename);k++)
        {
            if(filename[k]== 92)
                cad[k-i] = 47;
            else
                cad[k-i]=filename[k];
        }
        cad[k-i]='\0';

        fprintf(this->pf,"<sourcefile filename=\"%s\">\n",cad);
        fprintf(this->pf,"<file id=\"0\" name=\"Information\">\n");
        fprintf(this->pf,"<attribute name=\"SOURCETYPE\"> \n");
        fprintf(this->pf,"<data:lvalue value=\"SEQUENCE\"/> \n");
        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"<attribute name=\"NUMFRAMES\">\n");
        fprintf(this->pf,"<data:dvalue value=\"%d\" /> \n",numframes);
        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"<attribute name=\"FRAMERATE\">\n");
        fprintf(this->pf,"<data:fvalue value=\"%.1f\" /> \n",framerate);
        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"<attribute name=\"H-FRAME-SIZE\" > \n");
        fprintf(this->pf,"<data:dvalue value=\"%d\" /> \n",_wframe);
        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"<attribute name=\"V-FRAME-SIZE\" > \n");
        fprintf(this->pf,"<data:dvalue value=\"%d\" /> \n",_hframe);
        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"</file>\n");

        this->wframe = _wframe;
        this->hframe = _hframe;
    }

}


/**
 *	Method to write a close event in XML VIPER format
 *
 * \param pEvent The event to write
 *
 * \return int operation code
 */
int EventWriter::writeEventVIPER(Event* pEvent,bool resize) {

    if(this->pf != NULL)
    {
        int duracion = -1;
        int dur = 1;

        //Escribimos por cada evento
        switch (pEvent->getEventType()) {
        case PutObject:
            fprintf(this->pf,"<object framespan=\"%d:%d\" id=\"%d\" name=\"PutObject\">\n",pEvent->getStartFrame()-dur,pEvent->getFinishFrame(),pEvent->getID());
            break;
        case AbandonedObject:

            //As we are writing the past events, finish frame is computed by adding the life to the initial frame
            fprintf(this->pf,"<object framespan=\"%d:%d\" id=\"%d\" name=\"AbandonedObject\">\n",pEvent->getStartFrame()-dur,pEvent->getLife()+pEvent->getStartFrame(),pEvent->getID());
            break;
        case StolenObject:
            fprintf(this->pf,"<object framespan=\"%d:%d\" id=\"%d\" name=\"StolenObject\">\n",pEvent->getStartFrame()-dur,pEvent->getLife()+pEvent->getStartFrame(),pEvent->getID());
            break;
        case AbandonedPerson:
            fprintf(this->pf,"<object framespan=\"%d:%d\" id=\"%d\" name=\"AbandonedPerson\">\n",pEvent->getStartFrame()-dur,pEvent->getFinishFrame(),pEvent->getID());
            break;
        case StolenPerson:
            fprintf(this->pf,"<object framespan=\"%d:%d\" id=\"%d\" name=\"StolenPerson\">\n",pEvent->getStartFrame()-dur,pEvent->getFinishFrame(),pEvent->getID());
            break;

        }

        fprintf(this->pf,"<attribute name=\"BoundingBox\">\n");

        fprintf(this->pf, "<data:bbox height=\"%0d\" width=\"%0d\" x=\"%0d\" y=\"%0d\"/> \n", pEvent->getLocation()->height, pEvent->getLocation()->width, pEvent->getLocation()->x,pEvent->getLocation()->y);// (int)y);

        fprintf(this->pf,"</attribute>\n");
        fprintf(this->pf,"</object>\n");
    }

    return 0;
}


int EventWriter::writeViperEnd()
{
    fprintf(this->pf,"</sourcefile>\n");
    fprintf(this->pf,"</data>\n");
    fprintf(this->pf,"</viper>\n");

    return 1;
}
