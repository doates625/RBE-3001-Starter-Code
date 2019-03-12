/**
 * @file PinConsts.h
 * @brief RBE-3001 robotic arm Nucleo pin settings.
 * @author Dan Oates (WPI Class of 2020)
 * 
 * This file contains the full digital IO configuration for
 * the Nucleo-F746ZG MCU, including encoder, servo, and
 * serial communication ports.
 */
#pragma once
#include <mbed.h>

// Encoder 1 (SPI)
const PinName pin_enc1_mosi = PC_12;	// Master-out slave-in
const PinName pin_enc1_miso = PC_11;	// Master-in slave-out
const PinName pin_enc1_sclk = PC_10;	// Serial slock
const PinName pin_enc1_ss = PD_2;		// Slave select

// Encoder 2 (SPI)
const PinName pin_enc2_mosi = PE_6;		// Master-out slave-in
const PinName pin_enc2_miso = PE_5;		// Master-in slave-out
const PinName pin_enc2_sclk = PE_2;		// Serial slock
const PinName pin_enc2_ss = PE_4;		// Slave select

// Encoder 3 (SPI)
const PinName pin_enc3_mosi = PF_9;		// Master-out slave-in
const PinName pin_enc3_miso = PF_8;		// Master-in slave-out
const PinName pin_enc3_sclk = PF_7;		// Serial slock
const PinName pin_enc3_ss = PE_3;		// Slave select

// Servo Motors (PWM)
const PinName pin_motor1_pwm = PE_9;	// Motor 1
const PinName pin_motor2_pwm = PE_11;	// Motor 2
const PinName pin_motor3_pwm = PE_13;	// Motor 3
const PinName pin_gripper_pwm = PB_10;	// Gripper

// Serial Communication
const PinName pin_serial_tx = USBTX;	// TX pin
const PinName pin_serial_rx = USBRX;	// RX pin