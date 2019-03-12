/**
 * @file Servo.h
 * @brief Servo library for RBE-3001 robotic arm servos.
 * @author Dan Oates (WPI Class of 2020)
 */
#include <mbed.h>

class Servo
{
public:
	Servo(PinName pin_pwm, float cmd_max);
	void init();
	void enable();
	void disable();
	void set_cmd(float cmd);
private:
	static const float pulse_period = 0.0050f;
	static const float pulse_min = 0.0010f;
	static const float pulse_max = 0.0020f;
	static const float pulse_offset = 0.0015f;
	static const float pulse_range = 0.0005f;
	float pulse_per_cmd;
	bool enabled;
	PwmOut* pwm_out;
};