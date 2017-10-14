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

#download dataset 
wget -i http://www-vpu.eps.uam.es/AOD/list -x

# dataset stored in "./www-vpu.eps.uam.es/AOD/"
# move to the main directory 
mv ./www-vpu.eps.uam.es/AOD .

#remove directory "./www-vpu.eps.uam.es"
rm -r www-vpu.eps.uam.es/
