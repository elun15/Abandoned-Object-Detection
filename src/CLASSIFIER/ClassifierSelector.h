/************************************************************************************/
/* Archivo cabecera de la clase MyCLSSFRSelector:                                   */
/* Esta clase nos permite seleccionar la tecnica a utilizar en la etapa de          */
/* clasificacion robo/abandono del sistema.                                         */
/************************************************************************************/

//Se incluyen los ficheros cabecera de las distintas tecnicas de clasificacion robo/abandono implementadas.

#pragma once
#include "StaticObjectClassifier.h"
#include "StaticObjectClassifierHist.h"
#include "StaticObjectClassifierHighGradient.h"
#include "video.h"

/*#include "StaticObjectClassifierPixelColourContrast.h"
#include "StaticObjectClassifierGradient.h"
#include "StaticObjectClassifierGradientH.h"
#include "StaticObjectClassifierGradientL.h"
*/

//Declaracion de la clase MyCLSSFRSelector
class ClassifierSelector
{
public:
	// Se definen las variables a utilizar durante el procesamiento de la secuencia de video en la estapa de clasificacion del sistema
	int int_CLSSFRid;
    int decision;

	// Creacion de los objetos de las clases que definen las distintas tecnicas de clasificacion robo/abandono.
     StaticObjectClassifierHist *pSOCHist;
     StaticObjectClassifierHighGradient *pSOCHighGradient;
    //StaticObjectClassifierPixelColourContrast *pSOCPixelcolourContrast;
    //StaticObjectClassifierGradientH *pSOCGradientH;
    //StaticObjectClassifierGradientL *pSOCGradientL;

	//Definicion de las funciones de la clase MyCLSSFRSelector
    ClassifierSelector(int CLSSFRid);
    ~ClassifierSelector();
	void init();
    Mat process(cv::Mat frame, cv::Mat bkgImage, cv::Mat StaticFGImage, cv::Mat ForegroundImage, BlobList<ObjectBlob*>* objects, video Video);
   };
