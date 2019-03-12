/**
 * @file JointConsts.h
 * @brief RBE-3001 robotic arm joint angle constants.
 * @author Dan Oates (WPI Class of 2020)
 * 
 * This file contains joint angle constants for the robot,
 * including home angles (for calibration) and joint angle
 * command limits to prevent the robot from breaking itself.
 * 
 * WARNING:
 * These command limits are not exhaustive and still allow
 * the robot to collide with the ground and the camera bars
 * in certain configurations. Proceed with caution.
 */
#pragma once

// Encoder Home Angles [rad]
const float home_angles[3] = 
{
	4.82f,	// Joint 1
	4.79f,	// Joint 2
	3.03f,	// Joint 3
};

// Minimum Angle Commands [rad]
const float min_angle_cmds[3] = 
{
	-1.00f,	// Joint 1
	-0.10f,	// Joint 2
	-0.35f,	// Joint 3
};

// Maximum Angle Commands [rad]
const float max_angle_cmds[3] = 
{
	+1.00f,	// Joint 1
	+1.10f,	// Joint 2
	+0.90f,	// Joint 3
};