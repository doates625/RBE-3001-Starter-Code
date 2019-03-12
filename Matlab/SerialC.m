classdef SerialC < handle
    % SERIALC Class for exchanging standard C data types over serial.
    %   Supported data types:
    %   - Signed integers ('int8', 'int16', 'int32')
    %   - Unsigned integers ('uint8', 'uint16', 'uint32')
    %   - IEEE-754 32-bit floating-point ('single')
    
    properties (Access = private, Constant)
        % List of valid data types [cell]
        valid_types = {...
            'int8'; 'int16'; 'int32';
            'uint8'; 'uint16'; 'uint32'; ...
            'single'};
    end
    properties (Access = private)
        % Matlab serial interface [serial]
        serial_port
    end
    
    methods (Access = public)
        function obj = SerialC(port_name, baud_rate)
            %obj = SERIALCOMMS(port, baud) Class constructor.
            %   port_name = Serial port name (ex. 'COM4') [char]
            %   baud_rate = Baud rate (ex. 57600) [double]
            %   If port_name is 'autodetect', the port is auto-detected.
            if strcmp(port_name, 'autodetect')
                try
                    hwinfo = instrhwinfo('serial');
                    port_name = hwinfo(1).AvailableSerialPorts{1};
                catch
                    error('Failed to auto-detect serial port.');
                end
            end
            obj.serial_port = serial(port_name, 'BaudRate', baud_rate);
        end
        function open(obj)
            %OPEN(obj) Attempts to open serial port.
            if ~obj.is_open()
                try
                    fopen(obj.serial_port);
                catch
                    error(['Serial port open failed. ' ...
                        'Check USB connection and port choice.']);
                end
            end
        end
        function s = is_open(obj)
            %s = IS_OPEN(obj) Returns serial port open status.
            %   s = 1 -> Port is open
            %   s = 0 -> Port is closed
            s = strcmp(obj.serial_port.status, 'open');
        end
        function n = available(obj)
            %n = AVAILABLE(obj) Returns number of bytes available in the
            %incoming serial buffer.
            n = obj.serial_port.BytesAvailable;
        end
        function wait(obj, n, timeout)
            %WAIT(obj, n) Waits for bytes to be available in the incoming
            %serial buffer.
            %   n = Number of bytes to wait for [numeric]
            %   timeout = Error timeout (default inf) [s]
            if nargin < 3
                timeout = inf;
            end
            t_init = tic();
            while obj.available() < n
                if toc(t_init) >= timeout
                    error(['Timed out after ' num2str(timeout) ' [s].']);
                end
            end
        end
        function val = read(obj, type)
            %val = READ(obj, type) Reads data from serial buffer.
            %   val = Value returned [numeric]
            %   type = Data type (ex. 'int8', 'single') [char]
            
            % Determine number of bytes to read
            switch type
                case {'int8', 'uint8'}
                    n_read = 1;
                case {'int16', 'uint16'}
                    n_read = 2;
                case {'int32', 'uint32', 'single'}
                    n_read = 4;
                otherwise
                    error(['Invalid data type: ''' type '''']);
            end
            
            % Check for sufficient bytes available
            n_avail = obj.available();
            if n_read <= n_avail
                bytes = uint8(fread(obj.serial_port, n_read));
                val = typecast(bytes, type);
            else
                error(['Attempted to read ' type ...
                    '(' int2str(n_read) ' byte(s)) with only' ...
                    int2str(n_avail) ' bytes available.']);
            end
        end
        function write(obj, val, type)
            %WRITE(obj, val, type) Writes data to serial port.
            %   val = Value to write [numeric]
            %   type = Data type (ex. 'int8') [char]
            if any(strcmp(obj.valid_types, type))
                bytes = typecast(cast(val, type), 'uint8').';
                fwrite(obj.serial_port, bytes);
            else
                error(['Invalid data type: ''' type '''']);
            end
        end
        function flush(obj)
            %FLUSH(obj) Empties incoming serial buffer.
            n = obj.available();
            if n > 0
                fread(obj.serial_port, n);
            end
        end
        function close(obj)
            %CLOSE(obj) Closes serial port.
            fclose(obj.serial_port);
        end
        function delete(obj)
            %DELETE(obj) Destructor which closes serial port.
            obj.close()
        end
    end
end