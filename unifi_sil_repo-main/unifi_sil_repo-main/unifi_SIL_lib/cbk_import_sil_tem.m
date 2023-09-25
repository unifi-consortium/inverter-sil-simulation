dev                 = sprintf('%s/%s',get_param(gcb, 'Parent'), get_param(gcb, 'Name'));

pos                 = strfind(dev,'/');
blkName             = extractAfter(dev,pos(end));

if not(isfolder('wrapperBuildDir'))
    mkdir wrapperBuildDir;
end

%################# wrapper #################
fName               = strcat('wrapperBuildDir/',blkName,'_wrapper.h');
fid                 = fopen(fName,'w');
txt                 = fileread('wrapper.h');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);


fName               = strcat('wrapperBuildDir/',blkName,'_wrapper.c');
fid                 = fopen(fName,'w');
txt                 = fileread('wrapper.c');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

text                = fileread(fName);
TextAsCells         = regexp(text, '\n', 'split');
SearchString        = '<wrapper.h>';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = strcat('#include <',blkName,'_wrapper.h>');

SearchString        = '<interface.h>';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = strcat('#include <',blkName,'_interface.h>');

fid                 = fopen(fName, 'w');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

%################# interface #################
fName               = strcat('wrapperBuildDir/',blkName,'_interface.h');
fid                 = fopen(fName,'w');
txt                 = fileread('interface.h');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

fName               = strcat('wrapperBuildDir/',blkName,'_interface.c');
fid                 = fopen(fName,'w');
txt                 = fileread('interface.c');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

text                = fileread(fName);
TextAsCells         = regexp(text, '\n', 'split');
SearchString        = '<interface.h>';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = strcat('#include <',blkName,'_interface.h>');

fid                 = fopen(fName, 'w');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);


%################# getInterfaceDim #################
fName               = strcat('wrapperBuildDir/',blkName,'_getInterfaceDim.h');
fid                 = fopen(fName,'w');
txt                 = fileread('getInterfaceDim.h');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

fName               = strcat('wrapperBuildDir/',blkName,'_getInterfaceDim.c');
fid                 = fopen(fName,'w');
txt                 = fileread('getInterfaceDim.c');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

text                = fileread(fName);
TextAsCells         = regexp(text, '\n', 'split');
SearchString        = '<getInterfaceDim.h>';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = strcat('#include <',blkName,'_getInterfaceDim.h>');

SearchString        = '<interface.h>';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = strcat('#include <',blkName,'_interface.h>');

fid = fopen(fName, 'w');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

%############################ getInterfaceDimensions ########################
fName               = strcat('wrapperBuildDir/getInterfaceDim.m');
fid                 = fopen(fName,'w');
txt                 = fileread('getInterfaceDimensions.m');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

currentDir          = pwd;
text                = fileread(fName);
TextAsCells         = regexp(text, '\n', 'split');
SearchString        = 'getInterfaceDim.h';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,'getInterfaceDim.h',strcat(currentDir,'/','wrapperBuildDir/',blkName,'_getInterfaceDim.h'));
TextAsCells{mask}   = newStr;

SearchString        = 'getInterfaceDim.c';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,'getInterfaceDim.c',strcat(currentDir,'/','wrapperBuildDir/',blkName,'_getInterfaceDim.c'));
TextAsCells{mask}   = newStr;

fid = fopen(fName, 'w');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

%############################ CustomCodeBase.m ########################
fName               = strcat('wrapperBuildDir/',blkName,'_LinkCustomCode.m');
fid                 = fopen(fName,'w');
txt                 = fileread('customCodeBase.m');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

