# AOD_System
Abandoned Object Detection System
elg_developement

************
* FOLDERS: 
************

./build_debug  
./build_release  
./datasets  (containing input video files of each dataset, as well as GT and mask images)  
./lib  (3rd party SW and libraries)
./models  
./proj  (containing Qt project files)  
./results (containing results oredered by configurations)  
./src  (source code)  
 


**************************
* Three EXECUTION ways:  
**************************

INPUT ARGUMENTS  
    // 1: bkg subtraction method  
    // 2: sfgd method  
    // 3: classifier method  
    // 4: people detector method  
    // 5: video file directory  
    // 6: results folder  
    // 7: seconds to static
    // 8: flag near people
    // 9: flag context masks
    

1: With .sh script "Run_AODSystem.sh" located in root by just executing ./Run_AODSystem.sh 
	
2: By terminal command. Executable are placed in build_release and build_debug, respectively. 
	
	- e.g. >> /System_Evaluation 2 1 1 1 ./../videos/AVSS_corto.mov ./../results/ 30 1 1   
	- If no input arguments, default settings will be used
	
3: By using a programming enviroment (Qt Creator or others)

 	- All input arguments are set/modified in the main script 

 
