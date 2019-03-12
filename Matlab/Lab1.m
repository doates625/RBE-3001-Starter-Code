%% RBE-3001 Lab 1 Demo
% Instructions:
%   - Nothing yet.

%% Script Initialization
clear, clc;
fprintf('RBE-3001 Lab 1 Demo\n\n');

%% Arm Initialization
fprintf('Initializing arm...\n');
baud_rate = 57600;
arm = RobotArm(baud_rate);

%% Joint Setpoints
t = [0; 1; 2; 3; 4];            % Command timestamps [s]
j1 = [0; 0.3; -0.3; 0.3; 0];    % Joint 1 setpoints [rad]
j2 = [0; 0.2; 0.2; 0.2; 0];     % Joint 2 setpoints [rad]
j3 = [0; -0.1; -0.1; -0.1; 0];  % Joint 3 setpoints [rad]
n_cmd = length(t);              % Number of commmands

%% Arm Commands

% Initialization
arm.enable();       % Enable arm motors
t_init = tic();     % Log initial time
t_dur = max(t) + 1; % Demo duration [s]
i_cmd = 1;          % Setpoint cmd index

% Command Loop
while toc(t_init) < t_dur
    
    % Send commands to robot
    if i_cmd <= n_cmd && toc(t_init) >= t(i_cmd)
        arm.set_angles(j1(i_cmd), j2(i_cmd), j3(i_cmd));
        i_cmd = i_cmd + 1;
    end
    
    % Get robot status update
    clc, fprintf('RBE-3001 Lab 1 Demo\n\n');
    status = arm.update(1);
    
    % Slow down loop for printing
    pause(0.2);
    
end

% Disable Arm
clc, fprintf('RBE-3001 Lab 1 Demo\n\n');
fprintf('Arm disabled.\n');
arm.disable();