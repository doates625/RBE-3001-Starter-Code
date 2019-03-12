/**
 * @file Servo.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <Servo.h>
#include <CppUtil.h>

/**
 * @brief Constructor for servo interface.
 * @param pin_pwm PWM servo control pin.
 * @param cmd_max Maximum command value.
 * 
 * The range [-cmd_max, cmd_max] maps to the entire
 * servo operating range.
 */
Servo::Servo(PinName pin_pwm, float cmd_max)
{
	this->pwm_out = new PwmOut(pin_pwm);
	this->pulse_per_cmd = pulse_range / cmd_max;
	this->enabled = false;
}

/**
 * @brief Initializes servo (must call before other methods).
 */
void Servo::init()
{
	pwm_out->period_ms(pulse_period * 1000.0f);
	disable();
}

/**
 * @brief Enables servo and sets command to 0.
 */
 void Servo::enable()
 {
	this->enabled = true;
	set_cmd(0.0f);
 }

/**
 * @brief Disables servo and disconnects PWM.
 */
 void Servo::disable()
 {
	pwm_out->pulsewidth(0.0f);
	this->enabled = false;
 }

/**
 * @brief Sets voltage command for motor if enabled.
 * @param voltage Limited to [-supply_voltage, supply_voltage]
 */
void Servo::set_cmd(float cmd)
{
	if(enabled)
	{
		float pulse = pulse_offset + pulse_per_cmd * cmd;
		pulse = clamp_limit(pulse, pulse_min, pulse_max);
		pwm_out->pulsewidth(pulse);
	}
}