#!/bin/bash
# SCRIPT TO DOWNLOAD AND USE THE SEQUENCES AOD 
#
# More info at http://www-vpu.eps.uam.es/AOD/
#
# Author: Juan C. SanMiguel (juancarlos.sanmiguel@uam.es)
# Date: Oct 2017

#echo -n "#Downloading files from http://www-vpu.eps.uam.es/AOD..."
#wget -r --no-parent http://www-vpu.eps.uam.es/AOD
#echo "done!"

#remove previous downloads except @AVSS_test@ & @VISOR_test@, "*.sh" and "*.cat" files
find . -maxdepth 1 \! \( -name "AVSS_test" -o -name "VISOR_test" -o -name "*.cat"  -o -name "*.sh" \) -exec rm -rf '{}' \;

#download dataset and mirror the directory structure
#wget -x -i http://www-vpu.eps.uam.es/AOD/list
wget http://assets.ii.uam.es/AOD/list 
wget -x -i list  --base http://assets.ii.uam.es/AOD/

# downloaded data stored in "./www-vpu.eps.uam.es/AOD/"
# move data to the main directory 
#cd ./www-vpu.eps.uam.es/AOD/ 
cd ./assets.ii.uam.es/AOD/
mv -f * ../../
cd ../../

#remove empty directories & files
#rm -r www-vpu.eps.uam.es/
rm -rf ./assets.ii.uam.es/
rm -rf list