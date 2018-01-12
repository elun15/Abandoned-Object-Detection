function newfile = processGTxml(filename,outpath,resPar,gt)

%parse XML ground-truth
xDoc = xmlread(char(filename));
xRoot = xDoc.getDocumentElement;

%get fps of input video
dataNode = xDoc.getElementsByTagName('file').item(0);       %get the <file> item
node = dataNode.getElementsByTagName('attribute').item(2);  %get the 3rd <attribute> sub-item
node = node.getElementsByTagName('data:fvalue').item(0);    %get the <data:fvalue> sub-item
fpsvideo = str2double(node.getAttribute('value'));               %get the associated value

%get the framespan for each object
dataNode = xDoc.getElementsByTagName('data').item(0);       %get the <data> item
annot = dataNode.getElementsByTagName('object').item(0);      %get the <object> subitems

c=0;

while ~isempty(annot)            

    name = annot.getAttribute('name');        
    if (strcmp(name,gt.evtName) == 1)
        framespan = annot.getAttribute('framespan');                        
        [frI, frE] = strtok(char(framespan),':');                
        frI = str2double(frI) + fpsvideo * (resPar.time2static-gt.time2static); %we update the initial frame
        framespan_new = sprintf('%d%s',frI, frE);
         annot.setAttribute('framespan',framespan_new);                        
    end
    c = c + 1;
    annot = dataNode.getElementsByTagName('object').item(c);      %get the <object> subitems
end

[path,name,ext]=fileparts(char(filename));
newfile=[outpath char(name) '.gt'];
xmlwrite(newfile,xDoc);       