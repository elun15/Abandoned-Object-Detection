function resProps=getParameters(filename)

resProps.bkg            = str2double(filename(7));      %bkg method
resProps.sfgd           = str2double(filename(8));      %sfgd method
resProps.pd             = str2double(filename(9));      %pd method
resProps.soc            = str2double(filename(10));     %soc method
resProps.time2static    = str2double(filename(12:13));  %static time
resProps.flag_people    = str2double(filename(15));     %flag to filter people
resProps.flag_mask      = str2double(filename(17));     %flag to use mask