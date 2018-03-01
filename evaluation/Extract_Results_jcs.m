% Elena Luna Garcia
% Results Evaluation
% 17/05/2017
clc; clear all ; close all;

%% input directories
results_path='./results/';		%path where results are located (YOU CAN CHANGE)
dataset_path='./datasets/';		%path where datasets are located (YOU CAN CHANGE)

%%file patterns
pattern_summary='#BEGIN_SUMMARY';
pattern_TOTAL = 'TOTAL';

%% data processing
precision =[];
recall = [];
config_names = [];

%get all configuration directories
Nconfig_d = dir(results_path);
Nconfig_d = Nconfig_d(4:end);

precision= NaN(numel(Nconfig_d),3,7);
recall= NaN(numel(Nconfig_d),3,7);


%for each configuration
for co=1:numel(Nconfig_d)
    
    %accumulate configuration names
    config_names = [config_names Nconfig_d(co).name];
    
    %get each configuration path
    config_path = [results_path Nconfig_d(co).name];
    
    %get all category directories
    Ncat_d = dir(config_path);
    Ncat_d =  Ncat_d(3:end);
    cat_names = [];
    
    %for each category
    for ca=1:numel(Ncat_d)
        
        %accumulate category names
        cat_names{ca} = Ncat_d(ca).name;
        
        %get each category path
        cat_path=[config_path '/' Ncat_d(ca).name];
        
        filesout = dir([cat_path '/*.out']);
        filesraw = dir([cat_path '/*.raw']);
        
        for i=1:numel(filesout)
           
            % name=filesout(i).name;
            name=fullfile(filesraw(i).folder,filesraw(i).name);
            fid = fopen(name);
            
            tline = fgets(fid);
            disp(tline);
            
            while ischar(tline)
                
                tline = fgets(fid);
                disp(tline);
                
                pos_TOTAL = strfind(tline,pattern_TOTAL);
                
                if (pos_TOTAL == 1) %TOTAL object line pos1
                    
                    scan = textscan(tline,'%s %d %d %d %d');
                    precision(co,ca,i) = scan{4}; %scan{5} is precision
                    recall(co,ca,i) = scan{5}; % scan{4} is recall
              
                end;
            end;
            fclose(fid);
            
        end; %end processing file
    end;  %for each category
end %for each configuration

%% final results
%clear all variables except the important variables
clearvars -except precision recall config_names cat_names results_path dataset_path

%mean precision/recall for each configuration
for co=1:size(precision,1) %configurations
    for ca=1:size(precision,2) %category
        
        mP = nanmean(precision(co,ca,:)); %average precision for all sequences
        mR = nanmean(recall(co,ca,:)); %average recall for all sequences
        
        if (mP == 0 && mR == 0 )
            mF = 0;
        else
            mF = 2 *( ( mP*mR )/( mP + mR) );
        end;
        
        %accumulate in an array
        ConCat_avg(co,ca,1) = mP;
        ConCat_avg(co,ca,2) = mR;
        ConCat_avg(co,ca,3) = mF;
    end
end

%% save data by category
for ca=1:size(precision,2) %category
    
    %compose the filename for the output
    %cat_file = fullfile(results_path, cat_names{ca});
    
    %get the output matrix
    cat_data = ConCat_avg(:,ca,:);
    cat_data = reshape(cat_data, [size(precision,1) 3]); %reshape matrix as "Nconfig x 3"
    
    %save data
    name = [cat_names{ca} '.mat'];
    save(name,'cat_data');
end
