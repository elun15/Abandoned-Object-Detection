#!/bin/bash
#  SCRIPT TO COMPARE RESULTS FOR ABANDONED & STOLEN OBJECT DETECTION USING VIPER
#
#  We use the tool viper-pe to perform the groundtrut-results comparisons
#  Available at http://viper-toolkit.sourceforge.net/ 
#  	  
#  For each results file (*.xml) in the 'results_path', we obtain several files
#  in the same directory as the xml file:
#	- RAW results *.raw [results of evaluation]
#	- OUT results *.out [results of evaluation (human readable)]
#	- LOG results *.logpe [command output generated by viper-pe] 
#
#  Created by Elena  on 24/5/17.
#  Last update: 2017-06-14

results_path="./results_stage4/"		#path where results  are located (YOU CAN CHANGE)
dataset_path="./datasets"		#path where datasets are located (YOU CAN CHANGE)
bin_path="./bin/viper/"	 		#path where binaries are located (YOU CAN CHANGE)

#required files 
viper="viper-pe.jar"			#performance evaluation tool   (YOU CAN CHANGE)
epf="config.epf"			#configuration to match events (YOU CAN CHANGE)
properties="properties.pr"		#selected properties of events (YOU CAN CHANGE)

#define execution command
exe_cmd="java -jar ${bin_path}${viper} -epf ${bin_path}${epf} -pr ${bin_path}${properties}"

#get ground-truth files
FILESGT=$(find ${dataset_path} -name '*.gt') 

#list subdirectories of 'results_path' (aka configurations)
CONFIGS=$(find ${results_path} -maxdepth 1 -type d -not -wholename ${results_path})

#for each configuration
for config_dir in $CONFIGS; do

	#list subdirectories for each 'config_dir' (aka dataset categories)
	CATS=$(find ${config_dir} -maxdepth 1 -type d -not -wholename ${config_dir})

	#for each category
	for cat_dir in $CATS; do
	
		#compose the full path with the data (configuration-category)
		full_res_path=${cat_dir}

		echo "Processing directory $full_res_path ..."
		for fileRes in $full_res_path$i/*.xml; do

			name=${fileRes##*/}
			base=${name%.xml}
			
			for fileGT in $FILESGT; do
				nameGT=${fileGT##*/}
				baseGT=${nameGT%.gt}

				if [[ $base == *"$baseGT"* ]]; then

					echo -n "	Processing file $baseGT ..."
					
					raw_file="${full_res_path}/$base.raw"
					out_file="${full_res_path}/$base.out"
					out_log="${full_res_path}/$base.logpe"

					cmd="${exe_cmd} -g $fileGT -r $fileRes -raw $raw_file -o $out_file > ${out_log}"
					echo -n ${cmd}
					${exe_cmd} -g $fileGT -r $fileRes -raw $raw_file -o $out_file > ${out_log} 2>&1
					echo "done!"
				fi
			done
		done
	done

done
