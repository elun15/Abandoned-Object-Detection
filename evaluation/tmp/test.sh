#!/bin/bash
#
# SCRIPT TO RUN THE CONFIGURABLE SYSTEM FOR ABANDONED & STOLEN OBJECT DETECTION
#
# "Background subtraction" module:
# 	1: LOBSTER
# 	2: PAWCS
# 	3: MOG2
# 	4: KNN
# 	5: Multimodal
#
# "Stationary foreground" module:
#	1: Subsampling
# 	2: Acc Mask
#
# "People detection" module
#	1: HOG
# 	2: DPM
# 	3: Haar Upper Body
# 	4: Haar Frontal Face
# 	5: Haar Full Body
#
# "Abandoned/Stolen classifier" module
# 	1: High Gradient
# 	2: Histogram
#
# Proposed configurations
#	C1 = 2 1 2 1
#	C2 = 3 1 2 1
#	C3 = 5 1 2 1
#	C4 = 2 1 1 1
#
# Last update: 2017-06-13

configs=("2121" "3121" "5121" "2111") 	#selected system configurations (YOU CAN CHANGE)

results_path="./results/"		#path where resultsa are located (YOU CAN CHANGE)
dataset_path="./datasets/"		#path where datasets are located (YOU CAN CHANGE)
binary_path="./bin/AS_ConfigSystem" 	#path where binaries are located (YOU CAN CHANGE)

Nconfig=${#configs[@]}			#total number of configurations
clear					#clean command windows

#process each configuration
for c in $(seq 0 1 $((Nconfig-1)))
do

	#echo "Configuration ${configs[c]}"
	bkg_sel=${configs[c]:0:1} #algorithm for "Background subtraction" module
	sfg_sel=${configs[c]:1:1} #algorithm for "Stationary foreground" module
	ped_sel=${configs[c]:2:1} #algorithm for "People detection" module
	asc_sel=${configs[c]:3:1} #algorithm for "Abandoned/Stolen classifier" module

	echo "CONFIGURATION bkg=${bkg_sel} sfg=${sfg_sel} ped=${ped_sel} asc=${asc_sel}"

	#process each category descriptor/file
	for file in $dataset_path$i/*.cat; 
	do
		#read the category name
		read -r cat_name < "${file}"
		echo " CATEGORY ${cat_name} from ${file}"

		#create output directory
		DIRECTORY=${results_path}${cat_name}
		#rm -rf $DIRECTORY	#deletes everything in the directory!!!
		
		if [ ! -d "$DIRECTORY" ]; then
			# Control will enter here if $DIRECTORY does not exist
			mkdir ${results_path}${cat_name}
		fi
		
		#read the files for each categories (skip first line)
		sed 1d ${file} | while read -r line;
		do	
			#process each sequence with the selected configuration
			echo -n "	Processing ${line}..."

			DATE=$(date +%Y-%m-%d.%H-%M)
	
			seq_path=${dataset_path}${line}		#get the full sequence path
			filename=$(basename "$seq_path")	#get the filename of the sequence 
			fname="${filename%.*}"			#get the filename without extension
			out_path=${results_path}${cat_name}"/"	#generate the output path
			out_log=${out_path}"/"${fname}"_${bkg_sel}_${sfg_sel}_${ped_sel}_${asc_sel}_${DATE}.log"							
			
			START_TIME=$SECONDS
			echo ${binary_path} $bkg_sel $sfg_sel $ped_sel $asc_sel ${seq_path} ${fname} ${out_path} > ${out_log}
			${binary_path} $bkg_sel $sfg_sel $ped_sel $asc_sel ${seq_path} ${fname} ${out_path} > ${out_log} 2>&1
			echo "done ($(($(($SECONDS- $START_TIME))/60)) min $(($(($SECONDS-$START_TIME))%60)) sec)"
		done
	done
done
