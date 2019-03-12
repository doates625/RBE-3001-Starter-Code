/**
 * @file PidConsts.h
 * @brief RBE-3001 robotic arm PID controller constants.
 * @author Dan Oates (WPI Class of 2020)
 * 
 * This file contains the PID controller gains and voltage
 * command limits for the three joint motors. The joint-
 * angle PID controllers convert error in radians to
 * motor commands in Volts.
 */
#pragma once

// Motor Voltage Command Limit [V]
const float pid_v_max = 3.0f;

// Joint 1 PID Controller
const float pid_j1_kp = 10.0f;	// P-gain [V/rad]
const float pid_j1_ki = 3.0f;	// I-gain [V/(rad*s)]
const float pid_j1_kd = 0.0f;	// D-gain [V/(rad/s)]

// Joint 2 PID Controller
const float pid_j2_kp = 15.0f;	// P-gain [V/rad]
const float pid_j2_ki = 1.0f;	// I-gain [V/(rad*s)]
const float pid_j2_kd = 0.0f;	// D-gain [V/(rad/s)]

// Joint 3 PID Controller
const float pid_j3_kp = 15.0f;	// P-gain [V/rad]
const float pid_j3_ki = 40.0f;	// I-gain [V/(rad*s)]
const float pid_j3_kd = 0.0f;	// D-gain [V/(rad/s)]