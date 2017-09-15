%Extract times
clc; clear all; close all;
folders =dir();
folders = folders (4:7);
total_times=[];



for i = 1:numel(folders)
    
    
    files = dir(folders(i).name);
    files = files(3:end);
    
    % eval(['Times_C' num2str(i) '=[];'])
    
    tmp=[];
    data_config=[];
    
    for f=1:numel(files)
        
        data = textread(files(f).name);
        data = data.*1000;
        
        data_config= [data_config ; data];
        
    end
    
    

    
    tmp=[];
    tmp(:,1)  = (data_config(:,3));
    tmp(:,2)  = (data_config(:,4));
    tmp(:,3)  = (data_config(:,5));
    tmp(:,4)  = (data_config(:,6));
    tmp(:,5)  = (data_config(:,7));
    
    tmp(:,6)  = (data_config(:,2));
    total_times = [total_times tmp(:,6)];
   
    
   % eval(['Data_C' num2str(i) '= tmp;'])
    % eval(['Times_var_C' num2str(i) '= Prueba_var;'])
    
    figure;
    T=bplot(tmp, 'linewidth',3);
    legend(T,'Fontsize',40)
    legend('Location','northwest')
    set(gca,'Fontsize',40);
    % xlabel('Modules');
    ylabel('ms per frame');
   % ylim([0 800]);
    
    set(gca,'XTickLabel',{' ','BKG','SFGD','PD','CLASS','WRITE','TOTAL'})
    
    eval(['title(''C' num2str(i) ''')']);
    
end

%read new config
files_new= dir('./5/');
files_new = files_new(3:end);
data_config_new=[];

for j=1:numel(files_new)
    
    data_new = textread(files_new(j).name);
    data_new = data_new.*1000;
    
    data_config_new= [data_config_new ; data_new];
    
    
end


total_times = [total_times data_config_new(:,2)];
figure;
T=bplot(total_times, 'linewidth',3);
legend(T,'Fontsize',40)
legend('Location','northeastclo')
set(gca,'Fontsize',40);

ylabel('ms per frame');

set(gca,'XTickLabel',{' ','C1','C2','C3','C4','PROPOSED'})



