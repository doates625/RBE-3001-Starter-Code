/**
 * @file main.cpp
 * @brief RBE3001 robotic arm firmware
 * @author Dan Oates (WPI Class of 2020)
 */

/**
 * Firmware Dependencies
 */

// Internal Headers
#include <PinConsts.h>
#include <JointConsts.h>
#include <PidConsts.h>

// External Libraries
#include <mbed.h>
#include <CppUtil.h>
#include <Encoder.h>
#include <SerialC.h>
#include <Servo.h>
#include <Pid.h>

/**
 * General System Constants
 */

const float f_ctrl = 100.0f;	// Controller frequency [Hz]
const float v_supply = 7.3f;	// Motor supply voltage [V]
const float grip_open = 1.0f;	// Gripper open cmd [-1,1]
const float grip_close = -1.0f;	// Gripper close cmd [-1,1]
const uint32_t baud = 57600;	// Serial baud rate [bit/s]

/**
 * Hardware Interfaces
 */

// Input Interfaces
Encoder encoders[3] =
{
	Encoder(pin_enc1_mosi, pin_enc1_miso, pin_enc1_sclk, pin_enc1_ss),
	Encoder(pin_enc2_mosi, pin_enc2_miso, pin_enc2_sclk, pin_enc2_ss),
	Encoder(pin_enc3_mosi, pin_enc3_miso, pin_enc3_sclk, pin_enc3_ss),
};
Serial serial(pin_serial_tx, pin_serial_rx);
SerialC comms(serial);

// Output Interfaces
Servo motors[3] =
{
	Servo(pin_motor1_pwm, v_supply),
	Servo(pin_motor2_pwm, v_supply),
	Servo(pin_motor3_pwm, v_supply),

};
Servo gripper = Servo(pin_gripper_pwm, 1.0f);

/**
 * Controller Objects
 */

// Joint Angle PID Controllers
Pid pids[3] = {
	Pid(pid_j1_kp, pid_j1_ki, pid_j1_kd, -pid_v_max, pid_v_max, f_ctrl),
	Pid(pid_j2_kp, pid_j2_ki, pid_j2_kd, -pid_v_max, pid_v_max, f_ctrl),
	Pid(pid_j3_kp, pid_j3_ki, pid_j3_kd, -pid_v_max, pid_v_max, f_ctrl),
};

// PID Control Timer
Ticker controller_timer;

/**
 * Robot State Variables
 */
bool enable_motors = false;
float joint_setpoints[3];
float joint_angles[3];
float previous_joint_angles[3];
float joint_velocities[3];
float motor_voltages[3];

/**
 * @brief Runs one iteration of the robot control loop.
 * Tasks:
 * - Update robot state estimates
 * - Compute PID voltage commands
 * - Send voltage commands to motors
 */
void run_control_loop()
{
	// For each joint:
	for(uint8_t i = 0; i < 3; i++)
	{
		// Read joint angle from encoder
		joint_angles[i] = encoders[i].get_angle();

		// Estimate joint velocity
		joint_velocities[i] = (joint_angles[i] - previous_joint_angles[i]) * f_ctrl;
		previous_joint_angles[i] = joint_angles[i];

		// Run joint angle PID controller
		if(enable_motors)
		{
			float angular_error = joint_setpoints[i] - joint_angles[i];
			motor_voltages[i] = pids[i].update(angular_error);
			motors[i].set_cmd(motor_voltages[i]);
		}
	}
}

/**
 * @brief Main function.
 * Tasks:
 * - Initialize hardware
 * - Set up controller interrupts
 * - Loop communication with Matlab
 */
int main()
{
	// Initialize serial
	serial.baud(baud);
	
	// Initialize motors, encoders, and state variables
	for(uint8_t i = 0; i < 3; i++)
	{
		motors[i].init();
		encoders[i].init();
		encoders[i].set_home(home_angles[i]);
		joint_setpoints[i] = 0.0f;
		joint_angles[i] = 0.0f;
		previous_joint_angles[i] = 0.0f;
		joint_velocities[i] = 0.0f;
		motor_voltages[i] = 0.0f;
	}

	// Open gripper servo
	gripper.init();
	gripper.enable();
	gripper.set_cmd(grip_open);

	// Initialize controller interrupt timer
	const float t_ctrl = 1.0f / f_ctrl;
	controller_timer.attach(&run_control_loop, t_ctrl);

	// Matlab serial communication loop
	while(1)
	{
		// Wait for serial command
		switch(comms.read_uint8())
		{
		case 0x01:
			// Enable motors and controllers
			enable_motors = true;
			for(uint8_t i = 0; i < 3; i++) {
				motors[i].enable();
				pids[i].reset();
			}
			break;

		case 0x02:
			// Disable motors and open gripper
			enable_motors = false;
			for(uint8_t i = 0; i < 3; i++) {
				motor_voltages[i] = 0.0f;
				motors[i].disable();
			}
			gripper.set_cmd(grip_open);
			break;

		case 0x03:
			// Send robot status to Matlab
			for(uint8_t i = 0; i < 3; i++)
			{
				comms.write_float(joint_setpoints[i]);
				comms.write_float(joint_angles[i]);
				comms.write_float(joint_velocities[i]);
				comms.write_float(motor_voltages[i]);
			}
			break;

		case 0x04:
			// Get joint setpoints from Matlab
			for(uint8_t i = 0; i < 3; i++)
			{
				joint_setpoints[i] = clamp_limit(
					comms.read_float(),
					min_angle_cmds[i],
					max_angle_cmds[i]);
			}
			break;

		case 0x05:
			// Open gripper
			gripper.set_cmd(grip_open);
			break;

		case 0x06:
			// Close gripper
			gripper.set_cmd(grip_close);
			break;
		}
	}
}