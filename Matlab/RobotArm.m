classdef RobotArm < handle
    % ROBOTARM Class for interfacing with the RBE-3001 robot arm.
    
    properties (Access = private)
        % Serial Communication
        comms;  % Serial interface [SerialComms]
        
        % Robot State Variable
        js = zeros(3, 1);   % Joint setpoints [rad]
        ja = zeros(3, 1);   % Joint angles [rad]
        jv = zeros(3, 1);   % Joint velocities [rad/s]
        mv = zeros(3, 1);   % Motor voltages [V]
    end
    
    methods (Access = public)
        % Constructor and Destructor
        function obj = RobotArm(baud_rate)
            %obj = ROBOTARM(baud_rate) Constructor for arm interface.
            %   baud_rate = Serial baud rate [bit/s]
            obj.comms = SerialC('autodetect', baud_rate);
            obj.comms.open();
            obj.comms.flush();
        end
        function delete(obj)
            %DELETE(obj) Destructor which disables robot.
            obj.disable();
        end
        
        % Arm Commands
        function enable(obj)
            %ENABLE(obj) Enables arm motors.
            obj.comms.write(1, 'uint8');
        end
        function disable(obj)
            %DISABLE(obj) Disables arm motors and opens gripper.
            obj.comms.write(2, 'uint8');
        end
        function status = update(obj, do_disp)
            %status = UPDATE(obj, do_disp) Gets state data from robot.
            %   status.js = Joint setpoints ([js1; js2; js3]) [rad]
            %   status.ja = Joint angles ([ja1; ja2; ja3]) [rad]
            %   status.jv = Joint velocities ([jv1; jv2; jv3]) [rad/s]
            %   status.mv = Motor voltage ([v1; v2; v3]) [V]
            %   do_disp = Status print flag (default 0)
            %   do_disp = 1 -> Print status to command line
            %   do_disp = 0 -> Do not print status

            % Send status request and wait for response
            obj.comms.write(3, 'uint8');
            obj.comms.wait(48);
            
            % Read data from buffer
            for i = 1 : 3
                obj.js(i) = obj.comms.read('single');
                obj.ja(i) = obj.comms.read('single');
                obj.jv(i) = obj.comms.read('single');
                obj.mv(i) = obj.comms.read('single');
            end
            
            % Copy to struct
            status = struct();
            status.js = obj.js;
            status.ja = obj.ja;
            status.jv = obj.jv;
            status.mv = obj.mv;
            
            % Optional printout
            if nargin < 2
                do_disp = 1;
            end
            if do_disp
                obj.disp();
            end
        end
        function set_angles(obj, j1, j2, j3)
            %SET_ANGLES(obj, j1, j2, j3) Sends joint setpoints to arm.
            %   j1 = Joint 1 angle command [rad]
            %   j2 = Joint 2 angle command [rad]
            %   j3 = Joint 3 angle command [rad]
            obj.comms.write(4, 'uint8');
            obj.comms.write(j1, 'single');
            obj.comms.write(j2, 'single');
            obj.comms.write(j3, 'single');
        end
        function open(obj)
            %OPEN(obj) Commands robot to open gripper.
            obj.comms.write(5, 'uint8');
        end
        function close(obj)
            %CLOSE(obj) Commands robot to close gripper.
            obj.comms.write(6, 'uint8');
        end
        function home(obj)
            %HOME(obj) Commands arm to home orientation (0 angles).
            obj.set_angles(0, 0, 0);
        end
        
        % Display Methods
        function disp(obj)
            %DISP(obj) Displays robot status to Matlab command line.
            fprintf('Robot Status:\n\n')
            
            % Joint Setpoints
            fprintf('Joint Setpoints [rad]:\n')
            for i = 1 : 3
                fprintf('%d: %.2f\n', i, obj.js(i));
            end
            newline;
            
            % Joint Angles
            fprintf('Joint Angles [rad]:\n')
            for i = 1 : 3
                fprintf('%d: %.2f\n', i, obj.ja(i));
            end
            newline;
            
            % Joint Velocities
            fprintf('Joint Velocities [rad/s]:\n')
            for i = 1 : 3
                fprintf('%d: %.2f\n', i, obj.jv(i));
            end
            newline;
            
            % Motor Voltages
            fprintf('Motor Voltages [V]:\n')
            for i = 1 : 3
                fprintf('%d: %.2f\n', i, obj.mv(i));
            end
            newline;
        end
    end
end