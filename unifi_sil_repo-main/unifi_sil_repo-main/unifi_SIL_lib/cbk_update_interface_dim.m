%*************** get interface dimensions ****************
cfg             = coder.config('mex');
cfg.TargetLang  = "C";

codegen -config cfg getInterfaceDim ...
    -report -I .\wrapperBuildDir

