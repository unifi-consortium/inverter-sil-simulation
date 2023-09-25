function [controlOutputs, debugOutputVars, statusMonitors]  = unifiGfmController(feedbacks, debugInputVars, dispatchParams)
numberOfDebugOutputs = 10;
numberOfControlOutputs = 5;
numberOfStatusMonitors = 5;

coder.updateBuildInfo('addIncludePaths','C:\Users\mawal\Downloads\localSIL_Library\branches\silLib_automate_codeLinking\demo_models/wrapperBuildDir');
coder.updateBuildInfo('addSourcePaths','C:\Users\mawal\Downloads\localSIL_Library\branches\silLib_automate_codeLinking\demo_models/wrapperBuildDir');
coder.cinclude('unifiGfmController_wrapper.h');
coder.updateBuildInfo('addSourceFiles','unifiGfmController_wrapper.c');
coder.updateBuildInfo('addSourceFiles','unifiGfmController_interface.c');

%% Link custom code-base here
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




%%
persistent simIdx;
persistent backgroundTaskCounter;
persistent sstatusMonitors;
if(isempty(simIdx))
    simIdx                  = 0;
    backgroundTaskCounter   = 0;
    sstatusMonitors         = single(zeros(numberOfStatusMonitors, 1));
    coder.ceval('wrapper_init');
    
end

ctrlOut = single(zeros(numberOfControlOutputs, 1));
coder.ceval('update_wrapper_Out', coder.ref(ctrlOut));
controlOutputs = double(ctrlOut);

debugOutputVars = single(zeros(numberOfDebugOutputs, 1));
coder.ceval('updateWrapperDebugOutVars',coder.ref(debugOutputVars));

sfeedbacks = single(feedbacks);
coder.ceval('update_wrapper_in', coder.ref(sfeedbacks));

sdebugInputVars = single(debugInputVars);
coder.ceval('updateWrapperDebugInVars',coder.ref(sdebugInputVars));

coder.ceval('wrapper_isr');

backgroundTaskCounter = backgroundTaskCounter + 1;

sdispatchParams = single(dispatchParams);
if(backgroundTaskCounter == backGroundTaskPeriod)
	backgroundTaskCounter = 0;
    
    coder.ceval('update_status_monitors', coder.ref(sstatusMonitors));
    
    coder.ceval('update_dispatch_parameters',coder.ref(sdispatchParams));
    
    coder.ceval('wrapper_background_task_update');
    
end
statusMonitors = double(sstatusMonitors);
end


