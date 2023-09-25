cfg = coder.config('mex');
cfg.TargetLang = "C";

numberOfFeedbackVars = 7;
numberOfDebugInVars = 5;
numberOfDispatchParameters = 6;

feedbacks = zeros(numberOfFeedbackVars, 1);
debugInputVars = zeros(numberOfDebugInVars, 1);
dispatchParams = zeros(numberOfDispatchParameters, 1);

codegen -config cfg wrapperFunc -args {feedbacks,debugInputVars, dispatchParams} ...
    -report -I ./wrapperBuildDir

