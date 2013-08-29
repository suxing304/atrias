% This returns the EtherCAT Slave Configuration struct the EtherLab custom slave Simulink block expects.
% Documentation for this structure can be found by looking at the help for EtherLab's Generic Slave Simulink block.
% This structure contains much of the same data as the ENI file (the XML file) used by Beckhoff's tools.

function slave = medulla_hip()
	% Basic slave information
	slave.SlaveConfig.vendor      = 1551;           % This is our Vendor ID
	slave.SlaveConfig.product     = hex2dec('2');   % Product 2 is the Hip Medulla.
	slave.SlaveConfig.description = 'Hip Medulla'; % This is optional, but it might be nice.

	% SyncManager configuration. We use a fairly standard EtherCAT setup: Syncmanagers 0 and 1 are
	% disabled; 2 and 3 are enabled. SM 2 has outputs (RxPDOs) and SM 3 has inputs (TxPDOs).

	% Our outputs
	Entries_RxPDO1 = [ hex2dec('5') 1  8    % Command
	                   hex2dec('6') 1 16    % Counter
	                   hex2dec('4') 1 32 ]; % Motor Current
	RxPDO1 = {hex2dec('1601') Entries_RxPDO1};

	% and inputs (We have several PDOs here...)
	Entries_TxPDO1 = [ hex2dec('5')  2  8    % ID
	                   hex2dec('5')  3  8    % State
	                   hex2dec('5')  4  8    % Counter
	                   hex2dec('5')  5  8    % Error Flags
	                   hex2dec('5')  6  8 ]; % Limit Switch
	Entries_TxPDO2 = [ hex2dec('7')  1 32    % Encoder
	                   hex2dec('6')  2 16 ]; % Timestamp
	Entries_TxPDO3 = [ hex2dec('6')  3 16    % Motor Voltage
	                   hex2dec('6')  4 16 ]; % Logic Voltage
	Entries_TxPDO4 = [ hex2dec('6')  5 16    % Thermistor 0
	                   hex2dec('6')  6 16    % Thermistor 1
	                   hex2dec('6')  7 16 ]; % Thermistor 2
	Entries_TxPDO5 = [ hex2dec('2')  1 16 ]; % Amp Measured Current
	Entries_TxPDO6 = [ hex2dec('6')  8 16    % Incremental Encoder
	                   hex2dec('6')  9 16 ]; % Incremental Encoder Timestamp
	Entries_TxPDO7 = [ hex2dec('6') 10 16    % Robot Current 50 Amp
	                   hex2dec('6') 11 16 ]; % Robot Current 600 Amp
	TxPDO1 = {hex2dec('1A08') Entries_TxPDO1};
	TxPDO2 = {hex2dec('1A09') Entries_TxPDO2};
	TxPDO3 = {hex2dec('1A0a') Entries_TxPDO3};
	TxPDO4 = {hex2dec('1A0b') Entries_TxPDO4};
	TxPDO5 = {hex2dec('1A0c') Entries_TxPDO5};
	TxPDO6 = {hex2dec('1A0e') Entries_TxPDO6};
	TxPDO7 = {hex2dec('1A0f') Entries_TxPDO7};

	% Package things together
	Sm2PDO = {RxPDO1};
	Sm3PDO = {TxPDO1, TxPDO2, TxPDO3, TxPDO4, TxPDO5, TxPDO6, TxPDO7};
	Sm2 = {2, 0, Sm2PDO}; % SyncManager 2; Direction = 0 (output)
	Sm3 = {3, 1, Sm3PDO}; % SyncManager 3; Direction = 1 (input)

	% Place the SM config into SlaveConfig
	slave.SlaveConfig.sm = {Sm2, Sm3};

	% Distributed Clock (DC for short) configuration. We use a cycle time
	% of 1000000 nanoseconds, with a shift of -300000 nanoseconds
	%slave.SlaveConfig.dc = hex2dec('300');
	slave.SlaveConfig.dc = [hex2dec('300'), 1000000, 0, -300000, 0, 0, 0, 0, 0, 0];

	% These are data types for ports on the Simulink block
	UINT8  = 1008;
	UINT16 = 1016;
	UINT32 = 1032;
	INT8   = 2008;
	INT16  = 2016;
	INT32  = 2032;

	% PortConfig configures the ports on the Simulink block
	% pdo setup: [ SyncManager PDOIndex EntryIndex ElementEndex]
	% pdo is 0-indexed.
	% We don't need to set input data types here because we're just forwarding
	% the PDOs directly through. Output data types are still necessary, however
	% Note: output refers to the block's outputs, which are TxPDOs! (Inputs in
	% EtherCAT jargon!)
	slave.PortConfig.input(1).pdo             = [ 0 0 0 0 ]; % Command
	slave.PortConfig.input(1).pdo_data_type   = UINT8;
	slave.PortConfig.input(2).pdo             = [ 0 0 1 0 ]; % Counter
	slave.PortConfig.input(2).pdo_data_type   = UINT16;
	slave.PortConfig.input(3).pdo             = [ 0 0 2 0 ]; % Motor Current
	slave.PortConfig.input(3).pdo_data_type   = INT32;
	slave.PortConfig.output(1).pdo            = [ 1 0 0 0 ]; % ID
	slave.PortConfig.output(1).pdo_data_type  = UINT8;
	slave.PortConfig.output(2).pdo            = [ 1 0 1 0 ]; % State
	slave.PortConfig.output(2).pdo_data_type  = UINT8;
	slave.PortConfig.output(3).pdo            = [ 1 0 2 0 ]; % Counter
	slave.PortConfig.output(3).pdo_data_type  = UINT8;
	slave.PortConfig.output(4).pdo            = [ 1 0 3 0 ]; % Error Flags
	slave.PortConfig.output(4).pdo_data_type  = UINT8;
	slave.PortConfig.output(5).pdo            = [ 1 0 4 0 ]; % Limit Switch
	slave.PortConfig.output(5).pdo_data_type  = UINT8;
	slave.PortConfig.output(6).pdo            = [ 1 1 0 0 ]; % Encoder
	slave.PortConfig.output(6).pdo_data_type  = UINT32;
	slave.PortConfig.output(7).pdo            = [ 1 1 1 0 ]; % Timestamp
	slave.PortConfig.output(7).pdo_data_type  = UINT16;
	slave.PortConfig.output(8).pdo            = [ 1 2 0 0 ]; % Motor Voltage
	slave.PortConfig.output(8).pdo_data_type  = UINT16;
	slave.PortConfig.output(9).pdo            = [ 1 2 1 0 ]; % Logic Voltage
	slave.PortConfig.output(9).pdo_data_type  = UINT16;
	slave.PortConfig.output(10).pdo           = [ 1 3 0 0 ]; % Thermistor 0
	slave.PortConfig.output(10).pdo_data_type = UINT16;
	slave.PortConfig.output(11).pdo           = [ 1 3 1 0 ]; % Thermistor 1
	slave.PortConfig.output(11).pdo_data_type = UINT16;
	slave.PortConfig.output(12).pdo           = [ 1 3 2 0 ]; % Thermistor 2
	slave.PortConfig.output(12).pdo_data_type = UINT16;
	slave.PortConfig.output(13).pdo           = [ 1 4 0 0 ]; % Amp Measured Current
	slave.PortConfig.output(13).pdo_data_type = INT16;
	slave.PortConfig.output(14).pdo           = [ 1 5 0 0 ]; % Incremental Encoder
	slave.PortConfig.output(14).pdo_data_type = UINT16;
	slave.PortConfig.output(15).pdo           = [ 1 5 1 0 ]; % Incremental Encoder Timestamp
	slave.PortConfig.output(15).pdo_data_type = UINT16;
	slave.PortConfig.output(16).pdo           = [ 1 6 0 0 ]; % Robot Current 50 Amp
	slave.PortConfig.output(16).pdo_data_type = UINT16;
	slave.PortConfig.output(17).pdo           = [ 1 6 1 0 ]; % Robot Current 600 Amp
	slave.PortConfig.output(17).pdo_data_type = UINT16;
end
