% Elena Luna Garcia
% Results Evaluation
% 17/05/2017


clc; clear all ; close all;
complex = {'Easy' , 'Medium' , 'Hard'};

for (j=1:3)
    
    folder = [ '/Users/Elena/TFM/evaluation/My classification/' complex{j} '_results/'];
    list_files = dir([folder '*.out']);
    
    list_names={list_files(:).name};
    pattern_summary='#BEGIN_SUMMARY';
    pattern_TOTAL = 'TOTAL';
    C1 = '2_1_2_1';
    C2 = '3_1_2_1';
    C3 = '5_1_2_1';
    C4 = '2_1_1_1';
    
    
    col1 = 0;
    col2 = 0;
    col3= 0;
    col4 = 0;
    
    
    
    precision =[];
    recall = [];
    
    for(i=1:length(list_names))
        
        name=list_names{i};
        
        C1_found = 0;
        C2_found = 0;
        C3_found = 0;
        C4_found = 0;
        C1_found =strfind(name,C1);
        C2_found =strfind(name,C2);
        C3_found =strfind(name,C3);
        C4_found =strfind(name,C4);
        
        if (C1_found > 0)
            C = 1;
        elseif (C2_found > 0)
            C = 2;
        elseif (C3_found > 0)
            C = 3;
        elseif (C4_found > 0)
            C = 4;
        end;
        
        fid = fopen([folder name]);
        
        tline = fgets(fid);
        disp(tline);
        
        while ischar(tline)
            
            tline = fgets(fid);
            disp(tline);
            
            
            pos_TOTAL = strfind(tline,pattern_TOTAL);
            
            if (pos_TOTAL == 1) %TOTAL object line pos1
                
                scan = textscan(tline,'%s %d %d %d %d');
                
                %         scan{5};%precision
                %          scan{6};%recall
                
                
                if ( C == 1)
                    col1 = col1 +1;
                    precision(C,col1) =  scan{4};
                    recall(C,col1) =  scan{5};
                    
                elseif ( C == 2)
                    col2 = col2 +1;
                    precision(C,col2) =  scan{4};
                    recall(C,col2) =  scan{5};
                    
                elseif ( C == 3)
                    col3 = col3 + 1;
                    precision(C,col3) =  scan{4};
                    recall(C,col3) =  scan{5};
                    
                elseif (C4_found > 0)
                    col4 = col4 +1;
                    precision(C,col4) =  scan{4};
                    recall(C,col4) =  scan{5};
                end;
                
                
            end;
            
            
        end;
        fclose(fid);
        
    end;
    
    MAT = [];
    
    for (c = 1:4) % CONFIGURACION
        %1 = precision 2 = recall 3 = f-score
        MAT(c,1)= mean(precision(c,:));
        MAT(c,2)= mean(recall(c,:));
        
        if (MAT(c,1)== 0 && MAT(c,2) == 0 )
            MAT(c,3) = 0;
        else
            MAT(c,3) = 2 *( ( MAT(c,1)* MAT(c,2))/( MAT(c,1)+ MAT(c,2)) );
        end;
        
    end;
    
    name_matrix = [complex{j} '.mat'];
    save(name_matrix, 'MAT');
    
end;


