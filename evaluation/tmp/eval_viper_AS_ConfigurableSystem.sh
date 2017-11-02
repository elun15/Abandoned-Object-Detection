#!/bin/sh

#  script_viper.sh
#  
#
#  Created by Elena  on 24/5/17.
#


#java -jar viper-pe.jar -epf config.epf -g AVSS_AB_Easy.xml -r results.xml -o graphic2.out -raw file.out



results_path="/Users/Elena/TFM/results_new_system/"
dataset=(VISOR)
#dataset=(AVSS2007 CAVIAR PETS_2006_S1_EASY PETS2006_S4_HARD PETS2006_S5_MEDIUM VISOR)


for i in ${dataset[@]}; do


    for filenameGT in $results_path$i/*.txt; do


        nameGT=${filenameGT##*/}
        baseGT=${nameGT%.txt}
        


        #For each GT file (.txt)

            for filename in $results_path$i/*.xml; do
           

                name=${filename##*/}
                base=${name%.xml}

                raw_file="raw_$base.out"
                out_file="out_$base.out"

                raw_result_file="$results_path$i/$raw_file"
                out_result_file="$results_path$i/$out_file"

                if [[ $base == *"$baseGT"* ]]; then

                    java -jar viper-pe.jar -epf config.epf -g $filenameGT -r $filename -raw $raw_result_file -o $out_result_file -pr properties.pr


                fi

            done

    done

done

