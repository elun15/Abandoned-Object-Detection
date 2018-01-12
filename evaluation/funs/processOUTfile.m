function [eventsGT,eventsDet, TP, FP, FN] = processOUTfile(filename)
%file patterns
pattern_false='FALSE';
pattern_detect= 'DETECT';
pattern_TOTAL = 'TOTAL';

%output variables
eventsGT =0;
eventsDet = 0;
FP = 0;
TP = 0;
FN = 0;

%open file
fid = fopen(filename);
tline = fgets(fid);

%process file to extract values
while ischar(tline)
    tline = fgets(fid);
    %disp(tline);
    
    if (strfind(tline,pattern_false))
        FP = FP + 1;
    end
    
    if (strfind(tline,pattern_detect))
        TP = TP + 1;
    end
    
  
    if (strfind(tline,pattern_TOTAL)) %TOTAL object line pos1
        
        scan = textscan(tline,'%s %d %d %d %d');
        eventsGT= scan{2}; %sGT
       

    end
    
    
    
end


FN = eventsGT - TP;
eventsDet = TP + FP;

fclose(fid);

end