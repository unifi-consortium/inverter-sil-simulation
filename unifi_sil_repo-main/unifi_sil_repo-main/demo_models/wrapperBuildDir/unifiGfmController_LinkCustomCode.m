%% Custom Code-Base
%
% Add include paths as:
% coder.updateBuildInfo('addIncludePaths','C:\...\...');
% coder.updateBuildInfo('addIncludePaths',......
% ******************************************************
% Add source paths as:
% coder.updateBuildInfo('addSourcePaths','D:\...\...');
% coder.updateBuildInfo('addSourcePaths',.......
% ******************************************************
% Add source files as: 
% coder.updateBuildInfo('addSourceFiles','*.c');
% coder.updateBuildInfo('addSourceFiles',......

coder.updateBuildInfo('addIncludePaths','C:\Users\mawal\Downloads\unifiGFMLibrary\trunk\include');
coder.updateBuildInfo('addSourcePaths','C:\Users\mawal\Downloads\unifiGFMLibrary\trunk\source');
coder.updateBuildInfo('addSourceFiles','compensators.c');
coder.updateBuildInfo('addSourceFiles','primaryControl.c');
coder.updateBuildInfo('addSourceFiles','transformations.c');
coder.updateBuildInfo('addSourceFiles','vectorControl.c');





%% Background Task Period
backGroundTaskPeriod = 5; % unit = sampling period
