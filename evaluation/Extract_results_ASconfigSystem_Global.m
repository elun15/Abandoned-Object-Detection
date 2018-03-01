%
% SCRIPT TO PROCESS VIPER FILES & SUMMARIZE RESULTS
%
% Author: Elena Luna Garcia
% Author: Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
% Created on 17/05/2017
% Revised on 18/12/2017
clc; clear all ; close all;

%% CONFIGURATION
%input directories
results_path='./results_stage4/';		%path where results are located (YOU CAN CHANGE)
dataset_path='./datasets/';		%path where datasets are located (YOU CAN CHANGE)

% Display flags
DISPLAY_CONFIGS = 1;
DISPLAY_FILES = 1;

%% DATA PROCESSING
configDirs = dirdir(results_path);                  %get all configuration directories

%for each configuration
for co=1:numel(configDirs)
    
    config_path = [results_path configDirs(co).name]; %configuration path
    catDirs = dirdir(config_path);                    %get all category directories
    
    %for each category
    for ca=1:numel(catDirs)
        
        %files to process for each category
        cat_path=[config_path '/' catDirs(ca).name]; %category path
       
        filesraw = dir([cat_path '/*.raw']);         %all raw files
        
        %variables to keep results of each category
     
        evtgt   = zeros(1,numel(filesraw)); %number of events in ground-truth
        evtde  = zeros(1,numel(filesraw)); %number of events detected
        TruePositives = zeros(1,numel(filesraw));
        FalsePositives = zeros(1,numel(filesraw));
        FalseNegatives = zeros(1,numel(filesraw));
        
        filenames = cell(1,numel(filesraw));%filenames
        
        %for each out file
        for f=1:numel(filesraw)
            %name=fullfile(filesraw(f).folder,filesraw(f).name);         %full path for out file
            name=[cat_path '/' filesraw(f).name];
           
            [evtgt(f),evtde(f),TruePositives(f),FalsePositives(f),FalseNegatives(f)] = processOUTfile(name);   %get values from file
            
            filenames(f) = {filesraw(f).name(1:end-29)};                %get filename
        end %end processing file
        
        %save results for category
        res(co).category(ca) = {struct('name',catDirs(ca).name,'files',{filenames},'TP',TruePositives,'FP', FalsePositives, 'FN', FalseNegatives,'NevtGT',evtgt, 'NevtDet',evtde)};
        
        res(co).config_name = configDirs(co).name;       %configuration name
        
    end  %for each category
end %for each configuration

%% post-processing of data
for co=1:numel(res) %num of configurations
    P = [];
    R = [];
    NevtGT = [];
    NevtDet = [];
    
    TruePositives = [];
    FalsePositives = [];
    FalseNegatives=[];
    
    for ca=1:numel(res(co).category)  %num of categories
        
        %for f=1:numel(res(co).category{ca}.precision)  %num of files
        %accumulate data
       % P = [P res(co).category{ca}.precision]; %precision
       % R = [R res(co).category{ca}.recall];    %recall
        
        NevtGT = [NevtGT res(co).category{ca}.NevtGT];%number of events in ground-truth
        NevtDet = [NevtDet res(co).category{ca}.NevtDet];%number of events detected
        
        TruePositives = [TruePositives res(co).category{ca}.TP];
        FalsePositives = [FalsePositives res(co).category{ca}.FP];
        FalseNegatives = [FalseNegatives res(co).category{ca}.FN];
        
        %end
    end
    
    %     %save accumulated data
%          res(co).P = P;
%          res(co).R = R;
%          res(co).NevtGT = NevtGT;
%          res(co).NevtDet = NevtDet;
         res(co).TP = TruePositives;
         res(co).FP = FalsePositives;
         res(co).FN = FalseNegatives;
    
    %compute some statistics
%   res(co).mP = mean(P);   %mean precision
%   res(co).mR = mean(R);   %mean recall
    res(co).totNevtGT = sum(NevtGT);    %total number of ground-truth events
    res(co).totNevtDet = sum(NevtDet);  %total number of detected events
%     
%     
    PGlobal = sum(TruePositives)/ (sum(TruePositives) + sum(FalsePositives));
    RGlobal = sum(TruePositives)/ ( sum(TruePositives) + sum(FalseNegatives));
    
    res(co).PrecisionGlobal = PGlobal;
    res(co).RecallGlobal = RGlobal;
    res(co).FScoreGlobal = (2* PGlobal*RGlobal) / (RGlobal+PGlobal);
    
    res(co).totTP = sum(TruePositives);
    res(co).totFP = sum(FalsePositives);
    res(co).totFN = sum(FalseNegatives);
    
end

%% PRINT RESULTS
if (DISPLAY_CONFIGS)
    fprintf('Configuration\t\tPrecision\tRecall\t\tFscore\tNevtGT\tNevtDet\n');
    fprintf('-------------------------------------------------------\n');
    for co=1:numel(res) %num of configurations
        %fprintf('%s\t%.2f\t\t%.2f\t%d\t\t%d\n',res(co).config_name,res(co).mP,res(co).mR,res(co).totNevtGT,res(co).totNevtDet);
        fprintf('%s\t%.2f\t\t%.2f\t\t%.2f\t%d\t\t%d\n',res(co).config_name,res(co).PrecisionGlobal,res(co).RecallGlobal,res(co).FScoreGlobal,res(co).totNevtGT,res(co).totNevtDet);
    end
    fprintf('\n\n');
end

% 
% if (DISPLAY_FILES)
%     for co=1:numel(res) %num of configurations
%         fprintf('Configuration\t\tCategory\t\tFilename\t\tPrecision\tRecall\tNevtGT\tNevtDet\n');
%         fprintf('--------------------------------------------------------------------------------------\n');
%         for ca=1:numel(res(co).category)  %num of categories
%             for f=1:numel(res(co).category{ca}.precision)  %num of files
%                 fprintf('%s\t%s\t%s\t',res(co).config_name,res(co).category{ca}.name,res(co).category{ca}.files{f});
%                 fprintf('%.2f\t\t%.2f\t%d\t\t%d\n',res(co).category{ca}.precision(f),res(co).category{ca}.recall(f),res(co).category{ca}.NevtGT(f),res(co).category{ca}.NevtDet(f));
%             end
%         end
%         fprintf('\n\n');
%     end
% end
clearvars -except res results_path dataset_path %remove temporal files