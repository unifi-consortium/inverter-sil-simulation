function [interFaceDimension]  = getInterfaceDim()

coder.cinclude('C:\Users\mawal\Downloads\localSIL_Library\branches\silLib_automate_codeLinking\demo_models/wrapperBuildDir/unifiGfmController_getInterfaceDim.h');
coder.updateBuildInfo('addSourceFiles','C:\Users\mawal\Downloads\localSIL_Library\branches\silLib_automate_codeLinking\demo_models/wrapperBuildDir/unifiGfmController_getInterfaceDim.c');

interFaceDim = single(zeros(6, 1));
coder.ceval('findInterfaceDimensions', coder.ref(interFaceDim));
interFaceDimension = double(interFaceDim);

end


