#!/bin/bash
#
# SCRIPT TO RUN THE CONFIGURABLE SYSTEM FOR ABANDONED & STOLEN OBJECT DETECTION
#
# 1 "Background subtraction" module:
# 	1: PAWCS
# 	2: MOG2
# 	3: KNN
# 	5: Multimodal
#
# 2 "Stationary foreground" module:
#	1: Subsampling
# 	2: Acc Mask
#
#3 "People detection" module
#	1: HOG
# 	2: DPM

# 4 "Abandoned/Stolen classifier" module
# 	1: High Gradient
# 	2: Histogram
#

#
# Proposed configurations
#	C1 = 2 1 2 1
#	C2 = 3 1 2 1
#	C3 = 5 1 2 1
#	C4 = 2 1 1 1
#

#
# Last update: 2017-09-18


configs=("4431" ) 	#selected system configurations (YOU CAN CHANGE)
seconds_to_static=20;
flag_near_people=0;
flag_context=1;

results_path="./results/"		#path where results  are located (YOU CAN CHANGE)
dataset_path="./datasets"		#path where datasets are located (YOU CAN CHANGE)
binary_path="./System_Evaluation1" 	#path where binaries are located (YOU CAN CHANGE)

Nconfig=${#configs[@]}			#total number of configurations
clear

#process each configuration
for c in $(seq 0 1 $((Nconfig-1))); do

	#echo "Configuration ${configs[c]}"
	bkg_sel=${configs[c]:0:1} #algorithm for "Background subtraction" module
	sfg_sel=${configs[c]:1:1} #algorithm for "Stationary foreground" module
	ped_sel=${configs[c]:2:1} #algorithm for "People detection" module
	asc_sel=${configs[c]:3:1} #algorithm for "Abandoned/Stolen classifier" module
	
	echo "CONFIGURATION bkg=${bkg_sel} sfg=${sfg_sel} ped=${ped_sel} asc=${asc_sel} time=$seconds_to_static flag_people=$flag_near_people flag_context=$flag_context"

	
	#process each category descriptor/file
	for file in $dataset_path$i/*.cat; do

		#read the category name
		read -r cat_name < "${file}"
		echo " CATEGORY ${cat_name} from ${file}"

		#create output directory
		out_path=${results_path}config${configs[c]}"_"$seconds_to_static"_"$flag_near_people"_"$flag_context"/"${cat_name}"/"
		#rm -rf $out_path	#deletes everything in the directory!!!
		
		if [ ! -d "$out_path" ]; then
			# Control will enter here if $out_path does not exist	
			#echo "Creating output path: ${out_path}"
			mkdir $out_path
		fi		
		
		#read the files for each categories (skip first line)
		sed 1d ${file} | while read -r line; do
	
			#process each sequence with the selected configuration
			echo -n "	Processing ${line}..."

			DATE=$(date +%Y-%m-%d.%H-%M)
	
			seq_path=${dataset_path}${line}		#get the full sequence path
			filename=$(basename "$seq_path")	#get the filename of the sequence 
			fname="${filename%.*}"			#get the filename without extension
			out_log=${out_path}"/"${fname}"_${bkg_sel}_${sfg_sel}_${ped_sel}_${asc_sel}_${DATE}.log" #generate output log							
			
			START_TIME=$SECONDS
			#echo "dir "$seq_path
			
			# WITHOUT CONTEXT MASK
			#echo -n ${binary_path} $bkg_sel $sfg_sel $asc_sel $ped_sel ${seq_path} ${out_path} > ${out_log}
			${binary_path} $bkg_sel $sfg_sel $ped_sel $asc_sel ${seq_path} ${out_path} $seconds_to_static $flag_near_people $flag_context  > ${out_log} 2>&1
		

			echo  "done ($(($(($SECONDS- $START_TIME))/60)) min $(($(($SECONDS-$START_TIME))%60)) sec)"
		done
	done
done					#clean command windows
