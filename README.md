# AOD_System
Abandoned Object Detection System


************
* FOLDERS: *
************

./build_debug  
./build_release  
./datasets  (containing input video files of each dataset)  
./images  (containing context mask images)  
./lib  
./models  
./proj  (containing Qt project files)  
./results (containing results oredered by configurations)  
./src  (source code)  
 


**************************
* Three EXECUTION ways:  *
**************************

INPUT ARGUMENTS
    // 1: bkg subtraction method  
    // 2: sfgd method  
    // 3: classifier method  
    // 4: people detector method  
    // 5: video file directory  
    // 6: video name  
    // 7: results folder  
    // 8: video context mask directory (not mandatory)  


ONLY option 1 takes into account difficulty categories, best option to evaluate several videos
OPTIONS 2 and 3 are suitable for simple proofs with just one video sequence

IMPORTANT: Comment and/or uncomment the corresponding part of the code. 

1: With .sh script "Run_AODSystem.sh" located in root by just executing ./Run_AODSystem.sh 
	
	- Configurations (e.g. 2 1 1 1) have to be set inside manually
	- Mask directory has to be set inside if required

2: By terminal command. Executable are placed in build_release and build_debug, respectively. 
	
	-e.g. >> /System_Evaluation 2 1 1 1 ./../videos/AVSS_corto.mov VideoName ./../results/  
	
3: By using a programming enviroment (Qt Creator or others)

 
	- All input arguments are set in the main script 

 
