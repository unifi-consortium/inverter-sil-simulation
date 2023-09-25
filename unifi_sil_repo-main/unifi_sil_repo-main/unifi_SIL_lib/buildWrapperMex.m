function [controlOutputs, debugOutputVars, statusMonitors]  = runWrapper(feedbacks, debugInputVars, dispatchParams)
numberOfDebugOutputs = 10;
numberOfControlOutputs = 5;
numberOfStatusMonitors = 5; 

coder.updateBuildInfo('addIncludePaths','incPath');
coder.updateBuildInfo('addSourcePaths','srcPath');
coder.cinclude('wrapper.h');
coder.updateBuildInfo('addSourceFiles','wrapper.c');
coder.updateBuildInfo('addSourceFiles','interface.c');

%% Link custom code-base here



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
