function [interFaceDimension]  = getInterfaceDim()

coder.cinclude('getInterfaceDim.h');
coder.updateBuildInfo('addSourceFiles','getInterfaceDim.c');

interFaceDim = single(zeros(6, 1));
coder.ceval('findInterfaceDimensions', coder.ref(interFaceDim));
interFaceDimension = double(interFaceDim);

end
