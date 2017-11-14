
 # 1: LOBSTER
 # 2: PAWCS
 # 3: MOG2
 # 4: KNN
 # 5: Multimodal

 # 1: Subsampling
 # 2: Acc Mask

 # 1: High Gradient
 # 2: Histogram

 # 1: HOG
 # 2: DPM
 # 3: Haar Upper Body
 # 4: Haar Frontal Face
 # 5: Haar Full Body


#Configurations

#C1 = 2 1 2 1
#C2 = 3 1 2 1
#C3 = 5 1 2 1
#C4 = 2 1 1 1


bkg_list=(2)
sfgd_list=(1)
class_list=(1 2)
pd_list=(1)

for i in ${bkg_list[@]};
do

for j in ${sfgd_list[@]};
do

for k in ${class_list[@]};
do

for l in ${pd_list[@]};
do

#VISOR
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/VISOR/visor_Video00.avi VISOR_00
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/VISOR/visor_Video01.avi VISOR_01
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/VISOR/visor_Video02.avi VISOR_02
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/VISOR/visor_Video03.avi VISOR_03

#PETS 2006 EASY
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S1/S1-T1-C1.mpeg PETS2006_EASY_S1_C1
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S1/S1-T1-C3.mpeg PETS2006_EASY_S1_C3
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S1/S1-T1-C4.mpeg PETS2006_EASY_S1_C4

#PETS 2006 MEDIUM
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S5/S5-T1-C1.mpeg PETS2006_MEDIUM_S5_C1
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S5/S5-T1-C3.mpeg PETS2006_MEDIUM_S5_C3
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S5/S5-T1-C4.avi PETS2006_MEDIUM_S5_C4

#PETS 2006 HARD
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S4/S4-T5-C1.avi PETS2006_HARD_S4_C1
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S4/S4-T5-C3.avi PETS2006_HARD_S4_C3
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/PETS2006_S4/S4-T5-C4.avi PETS2006_HARD_S4_C4

#CAVIAR
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/CAVIAR/LeftBag_AtChair.mpeg LeftBag_AtChair
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/CAVIAR/LeftBag_PickedUp.mpeg LeftBag_PickedUp
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/CAVIAR/LeftBox.mpeg LeftBox
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/CAVIAR/LeftBag.mpeg LeftBag

#AVSS 2007
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/AVSS2007/AVSSS07_indoor_abandoned_object_easy_4cif_mpeg.mpg AVSS2007_EASY /home/vpu/Desktop/TFM_GitHub/TFM/images/AVSS_EASY_Mask_edited.jpg
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/AVSS2007/AVSSS07_indoor_abandoned_object_medium_4cif_mpeg.mpg AVSS2007_MEDIUM /home/vpu/Desktop/TFM_GitHub/TFM/images/AVSS_EASY_Mask_edited.jpg
#./System_Evaluation $i $j $k $l /home/vpu/Desktop/AVSS2007/AVSSS07_indoor_abandoned_object_hard_4cif_mpeg.mpg AVSS2007_HARD /home/vpu/Desktop/TFM_GitHub/TFM/images/AVSS_EASY_Mask_edited.jpg

./System_Evaluation $i $j $k $l /home/vpu/Desktop/AVSS_AB_LARGE.avi AVSS_AB_LARGE




done

done

done

done



