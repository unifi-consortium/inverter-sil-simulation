coder.extrinsic('getInterfaceDim_mex');
interfaceDim        = zeros(6,1);
interfaceDim        = getInterfaceDim_mex();

dev                 = sprintf('%s/%s',get_param(gcb, 'Parent'), get_param(gcb, 'Name'));

pos                 = strfind(dev,'/');
blkName             = extractAfter(dev,pos(end));

%############################ wrapperMexContainer ########################
funcPath            = append(dev,'/wrapperMexContainer');
sf                  = sfroot();
block               = sf.find('Path',funcPath,'-isa','Stateflow.EMChart');

text                = fileread('wrapperMexContainer.m');
TextAsCells         = regexp(text, '\n', 'split');

SearchString        = 'numberOfDebugOutVars = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString ,num2str(interfaceDim(2)), ';');

SearchString        = 'numberOfControlOutVars = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString,num2str(interfaceDim(4)), ';');

SearchString        = 'numberOfStatusMonitors = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString,num2str(interfaceDim(6)), ';');

SearchString        = 'wrapper_mex';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(blkName,'_mex'));
TextAsCells{mask}   = newStr;

SearchString        = 'generatedWrapper_mex';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(blkName,'_mex'));
TextAsCells{mask}   = newStr;

TextAsStr           = string(TextAsCells);
nText               = join(TextAsStr,"\n");

block.Script        = sprintf(nText);

%************************** build wrapper mex **********************
%####################### getInterfaceDimensions ########################

fName               = strcat('wrapperBuildDir/',blkName,'.m');
fid                 = fopen(fName,'w');
txt                 = fileread('buildWrapperMex.m');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

text                = fileread(fName);
TextAsCells         = regexp(text, '\n', 'split');

SearchString        = 'runWrapper';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,blkName);
TextAsCells{mask}   = newStr;

SearchString        = 'numberOfDebugOutputs = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString ,num2str(interfaceDim(2)), ';');

SearchString        = 'numberOfControlOutputs = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString,num2str(interfaceDim(4)), ';');

SearchString        = 'numberOfStatusMonitors = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString,num2str(interfaceDim(6)), ';');

currentDir          = pwd;

SearchString        = 'incPath';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(currentDir,'/','wrapperBuildDir'));
TextAsCells{mask}   = newStr;

SearchString        = 'srcPath';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(currentDir,'/','wrapperBuildDir'));
TextAsCells{mask}   = newStr;

SearchString        = 'wrapper.h';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(blkName,'_wrapper.h'));
TextAsCells{mask}   = newStr;

SearchString        = 'wrapper.c';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(blkName,'_wrapper.c'));
TextAsCells{mask}   = newStr;

SearchString        = 'interface.c';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(blkName,'_interface.c'));
TextAsCells{mask}   = newStr;

% Insert text
insFname            = strcat('wrapperBuildDir/',blkName,'_LinkCustomCode.m');
insTxt              = fileread(insFname);
insTextAsCells      = regexp(insTxt, '\n', 'split');
mask                = ~cellfun(@isempty, strfind(TextAsCells, '%% Link custom code-base here'));
insPos              = find(mask);
fid = fopen(fName, 'w');
for I = 1:insPos
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end

for I = 1:length(insTextAsCells)
    fprintf(fid, '%s\n', char(insTextAsCells{I}));
end

for I = (insPos+1):length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);


%% ***************** Mex generation script ********************
fName               = strcat('cbk_build_mex.m');
fid                 = fopen(fName,'w');
txt                 = fileread('genWrapperRunMex.m');
TextAsCells         = regexp(txt, '\n', 'split');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);

text                = fileread(fName);
TextAsCells         = regexp(text, '\n', 'split');

SearchString        = 'wrapperFunc';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
str                 = char(TextAsCells{mask});
newStr              = strrep(str,SearchString,strcat(blkName));
TextAsCells{mask}   = newStr;

SearchString        = 'numberOfFeedbackVars = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString ,num2str(interfaceDim(3)), ';');

SearchString        = 'numberOfDebugInVars = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString,num2str(interfaceDim(1)), ';');

SearchString        = 'numberOfDispatchParameters = ';
mask                = ~cellfun(@isempty, strfind(TextAsCells, SearchString));
TextAsCells{mask}   = append(SearchString,num2str(interfaceDim(5)), ';');

fid = fopen(fName, 'w');
for I = 1:length(TextAsCells)
    fprintf(fid, '%s\n', char(TextAsCells{I}));
end
fclose(fid);
