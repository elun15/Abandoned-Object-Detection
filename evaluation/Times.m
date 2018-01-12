%Extract times
clc; clear all; close all;

total_times=[];
results_path='./results_stage4/';		%path where results are located (YOU CAN CHANGE)

%% DATA PROCESSING
configDirs = dirdir(results_path);                  %get all configuration directories

%for each configuration
for co=1:numel(configDirs)
    
    cont_frames_config = 0;

    config_path = [results_path configDirs(co).name]; %configuration path
    catDirs = dirdir(config_path);                    %get all category directories
    
     fprintf('--------------------------------------------------------------------\n');
    fprintf('Configuration\t%s\n',configDirs(co).name);
   
    fprintf('Total\t\tBKG\t\tSFGD\t\tPD\t\tCLASS\t\tWRITE\n');
    tmp_sum=[0 0 0 0 0 0];
    tmp_avg=[0 0 0 0 0 0];
    
     
%for each category
    for ca=1:numel(catDirs)
        
        %files to process for each category
        cat_path=[config_path '/' catDirs(ca).name]; %category path
        filestime = dir([cat_path '/*.time']);         %all out files
        cont_frames_category = 0;
        
        %for each out file
        for f=1:numel(filestime)
            
            cont_frames_video = 0;
            fid = fopen([cat_path '/' filestime(f).name],'r');
            %disp(filestime(f).name);
      
           tmp_video=[0 0 0 0 0 0];
            
            tline = fgets(fid);
            
            while ischar(tline)
                % disp(tline)
                if (tline(1) ~= '#')
                    
                    scan = textscan(tline,'%d %f %f %f %f %f %f');
                    tmp =  [sum([scan{3:end}]) scan{3:end}];
                    tmp_sum = tmp_sum + tmp;
                    tmp_video = tmp_video + tmp;
                    
                    cont_frames_config = cont_frames_config +1;
                    cont_frames_video= cont_frames_video +1;
                    cont_frames_category = cont_frames_category + 1;
                    
                end
                
                tline = fgets(fid);
            end
            
            tmp_avg_vid= tmp_video ./ cont_frames_video;
            %disp(tmp_avg_vid)
            fclose(fid);
            
            
            %             fprintf('%s\t%s\t%s\n',configDirs(co).name,catDirs(ca).name,filestime(f).name);
            %             fprintf('--------------------------------------------------------------------\n');
            %             fprintf('Total\t\tBKG\t\tSFGD\t\tPD\t\tCLASS\t\tWRITE\n');
            %             fprintf('%f\t%f\t%f\t%f\t%f\t%f\n',tmp_avg(1),tmp_avg(2),tmp_avg(3),tmp_avg(4),tmp_avg(5),tmp_avg(6));
            %
            %
        end % time file
       
       % tmp_avg_cat= tmp_sum ./ cont_frames_category;
        %disp( tmp_avg_cat);
        
    end  %for each category
    
    tmp_avg_config= tmp_sum ./ cont_frames_config;
    fprintf('%f\t%f\t%f\t%f\t%f\t%f\n',tmp_avg_config(1),tmp_avg_config(2),tmp_avg_config(3),tmp_avg_config(4),tmp_avg_config(5),tmp_avg_config(6));
         
    
end %for each configuration



% results_path='./results/';		%path where results are located (YOU CAN CHANGE)
%
%
% for i = 1:numel(folders)
%
%
%     files = dir(folders(i).name);
%     files = files(3:end);
%
%     % eval(['Times_C' num2str(i) '=[];'])
%
%     tmp=[];
%     data_config=[];
%
%     for f=1:numel(files)
%
%         data = textread(files(f).name);
%         data = data.*1000;
%
%         data_config= [data_config ; data];
%
%     end
%
%
%
%
%     tmp=[];
%     tmp(:,1)  = (data_config(:,3));
%     tmp(:,2)  = (data_config(:,4));
%     tmp(:,3)  = (data_config(:,5));
%     tmp(:,4)  = (data_config(:,6));
%     tmp(:,5)  = (data_config(:,7));
%
%     tmp(:,6)  = (data_config(:,2));
%     total_times = [total_times tmp(:,6)];
%
%
%    % eval(['Data_C' num2str(i) '= tmp;'])
%     % eval(['Times_var_C' num2str(i) '= Prueba_var;'])
%
%     figure;
%     T=bplot(tmp, 'linewidth',3);
%     legend(T,'Fontsize',40)
%     legend('Location','northwest')
%     set(gca,'Fontsize',40);
%     % xlabel('Modules');
%     ylabel('ms per frame');
%    % ylim([0 800]);
%
%     set(gca,'XTickLabel',{' ','BKG','SFGD','PD','CLASS','WRITE','TOTAL'})
%
%     eval(['title(''C' num2str(i) ''')']);
%
% end
%
% %read new config
% files_new= dir('./5/');
% files_new = files_new(3:end);
% data_config_new=[];
%
% for j=1:numel(files_new)
%
%     data_new = textread(files_new(j).name);
%     data_new = data_new.*1000;
%
%     data_config_new= [data_config_new ; data_new];
%
%
% end
%
%
% total_times = [total_times data_config_new(:,2)];
% figure;
% T=bplot(total_times, 'linewidth',3);
% legend(T,'Fontsize',40)
% legend('Location','northeastclo')
% set(gca,'Fontsize',40);
%
% ylabel('ms per frame');
%
% set(gca,'XTickLabel',{' ','C1','C2','C3','C4','PROPOSED'})
%
%
%
