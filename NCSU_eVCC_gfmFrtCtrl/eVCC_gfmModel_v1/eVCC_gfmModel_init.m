% *************************************************************************
%    ########  ########    ########  ########  #######   #      #
%    #         #       #   #         #         #      #  # #  # #
%    ########  ########    ########  ########  #      #  #  ##  #
%    #         #       #   #         #         #      #  #      #
%    #         #        #  ########  ########  #######   #      #
% 
% *************************************************************************
% *************************************************************************
%  DESCRIPTION
%  Author       : Siye Cen
%  Date         : July 5th, 2024
% 
% 
% *************************************************************************

%% VSC Parameters
vsc.VnomRms	        = 690.0;						    % [V] 
vsc.Vnom			= vsc.VnomRms*sqrt(2/3); 		    % [V] 
vsc.fnom			= 60.0;						        % [Hz]
vsc.Wnom 		    = vsc.fnom*2*pi;				    % [rad/s]
vsc.Snom 			= 1e6;							    % [VA]
vsc.Zbase			= vsc.VnomRms^2/vsc.Snom; 		    % [Ohm]
vsc.Inom 			= vsc.Vnom/vsc.Zbase;               
vsc.Lbase			= vsc.Zbase/vsc.Wnom;
vsc.Cbase			= 1/(vsc.Wnom*vsc.Zbase);

CdcPerKVA			= 2e-6;						        % [F] 
vsc.Cdc.C 			= CdcPerKVA*vsc.Snom/1e3; 		    % [F]
vsc.Cdc.R 			= 30e-3;						    % [Ohm]
vsc.Vdc				= vsc.Vnom*2/1.15*1.3;
vsc.L1.L		    = vsc.Lbase*0.07;
vsc.L1.R 			= vsc.Zbase*0.01;
vsc.L2.L		    = vsc.Lbase*0.05;
vsc.L2.R 			= vsc.Zbase*0.01;
vsc.Cf.C		    = vsc.Cbase*0.05;
vsc.Cf.R 			= 1.5;

vsc.fsw 		    = 2.520e3;
vsc.fs			    = vsc.fsw * 2;

%% Current controller
cont.cc.Kp			= 2*pi*vsc.fs*0.04*(vsc.L1.L+vsc.L2.L);
cont.cc.Tr 			= 1/(vsc.fs*0.04*0.2);
cont.cc.Kd			= 0.01;
cont.cc.max		    = vsc.Vdc/2*1.15*0.95;
cont.cc.min		    = -vsc.Vdc/2*1.15*0.95;

%% Virtual admittance
cont.Zv.L 			= vsc.Lbase*0.25;
cont.Zv.R			= vsc.Zbase*0.075;

%% Elliptical limiter
cont.Imax			= vsc.Inom*1.2/sqrt(2);

%% Droop controller
cont.mp				= vsc.Wnom*0.01/vsc.Snom;
cont.mq				= vsc.Vnom*0.05/vsc.Snom;
cont.tP				= 1/(2*pi*10.0);
cont.tQ				= 1/(2*pi*10.0);

%% Reference adaptation based on +ve seq Vg magnitude
cont.ravgmag.tau    = 0.01;
cont.ravgmag.thrsh  = 0.8;
cont.ravgmag.Vnom   = vsc.Vnom;

%% Seq Comp. extraction
cont.scd.Kd 		= 1.1;

%% Grid
grid.Zg.L			= vsc.Lbase*0.25;
grid.Zg.R			= vsc.Zbase*0.025;
faultSeq            = [0.0  1.0  1.25 3.0;      % time [s]
                       1.0  0.2  1.0  1.0;];    % Voltage [pu]

save('eVCC_gfmModel.mat', "vsc", "cont","grid");
save("tsFaultSeq.mat","faultSeq");
