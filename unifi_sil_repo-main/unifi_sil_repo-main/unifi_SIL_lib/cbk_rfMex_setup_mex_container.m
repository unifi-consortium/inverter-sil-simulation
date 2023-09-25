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


