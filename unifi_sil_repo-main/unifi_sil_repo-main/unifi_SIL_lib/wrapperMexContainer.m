function [controlOutputs, debugOutputVars, statusMonitors] = runWrapperMex(feedbacks, debugInputVars, dispatchParams)
numberOfDebugOutVars = 10;
numberOfControlOutVars = 5;
numberOfStatusMonitors = 5;

controlOutputs  = zeros(numberOfControlOutVars, 1);
debugOutputVars = zeros(numberOfDebugOutVars, 1);
statusMonitors  = zeros(numberOfStatusMonitors, 1);
coder.extrinsic('wrapper_mex');

[vcontrolOutputs, vdebugOutputVars, vstatusMonitors] = ...
    generatedWrapper_mex(feedbacks,debugInputVars, dispatchParams);

controlOutputs  = double(vcontrolOutputs);
debugOutputVars = double(vdebugOutputVars);
statusMonitors  = double(vstatusMonitors);
end


