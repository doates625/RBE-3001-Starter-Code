/**
 * @file Encoder.h
 * @brief Library for AS5055A magnetic absolute encoder.
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once
#include <mbed.h>
#include <SPI.h>

class Encoder
{
public:
	Encoder(PinName mosi, PinName miso, PinName sclk, PinName ss);
	void init();
	void set_home(float home_angle);
	void set_home();
	float get_home();
	float get_angle();
private:
	uint16_t read_reg(uint16_t reg);
	void write_reg(uint16_t reg, uint16_t val);
	uint16_t write_spi(uint16_t data);
	void write();

	SPI* spi;
	DigitalOut* ss;
	float home_angle;

	const static float rad_per_cnt = 0.00153435538f;
	const static uint16_t cmd_write = 0x0000;
	const static uint16_t cmd_read = 0x8000;
	const static uint16_t reg_reset = 0x33A5;
	const static uint16_t reg_angle = 0x3FFF;
	const static uint16_t reg_nop = 0x0000;
};