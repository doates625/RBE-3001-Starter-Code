/**
 * @file Encoder.cpp
 * @author Dan Oates (WPI Class of 2020)
 * @see Encoder.h
 */
#include <Encoder.h>

/**
 * @brief Encoder interface constructor
 * @param mosi SPI MISO pin
 * @param miso SPI MISO pin
 * @param sclk SPI serial clock pin
 * @param ss SPI slave select pin
 */
Encoder::Encoder(PinName mosi, PinName miso, PinName sclk, PinName ss)
{
	this->spi = new SPI(mosi, miso, sclk);
	this->ss = new DigitalOut(ss);
	home_angle = 0.0f;
}

/**
 * @brief Initializes encoder
 */
void Encoder::init()
{
	ss->write(1);				// Set ss high (disabling)
	spi->format(8, 1);			// Format: 8 bits, mode 1
	spi->frequency(1000000);	// Frequency = 1MHz
	write_reg(reg_reset, 0x00);	// Reset the chip
}

/**
 * @brief Sets home ('0') position to given angle.
  */
void Encoder::set_home(float home_angle)
{
	this->home_angle = home_angle;
}

/**
 * @brief Sets home ('0') position to current angle.
 */
void Encoder::set_home()
{
	home_angle = get_angle() + home_angle;
}

/**
 * @brief Returns home position of encoder.
 */
float Encoder::get_home()
{
	return home_angle;
}

/**
 * @brief Reads encoder
 * @return Angle in radians
 */
float Encoder::get_angle()
{
	uint16_t angle_cnts = (read_reg(reg_angle) >> 2) & 0x0FFF;
	return angle_cnts * rad_per_cnt - home_angle;
}

/**
 * @brief Reads SPI register from device
 * @param reg Register to read
 */
uint16_t Encoder::read_reg(uint16_t reg)
{
	// Add read cmd and parity bits
	reg = (reg << 1) | cmd_read;
	reg |= __builtin_parity(reg);

	// Get data from register
	write_spi(reg);
	uint16_t val = write_spi(reg_nop);
	return val;
}

/**
 * @brief Writes value to SPI register
 * @param reg Register to write
 * @param val Value to write
 */
void Encoder::write_reg(uint16_t reg, uint16_t val)
{
	// Format data to send
	reg = (reg << 1) | cmd_write;
	reg |= __builtin_parity(reg);
	val = (val << 2);
	val |= __builtin_parity(val);

	// Send data
	write_spi(reg);
	write_spi(val);
}

/**
 * @brief Writes data to SPI and returns response.
 * @param data Data to send
 */
uint16_t Encoder::write_spi(uint16_t data)
{
	// Start SPI
	spi->lock();
	ss->write(0);

	// Send data and get response
	uint8_t msb = spi->write(data >> 8);
	uint8_t lsb = spi->write(data | 0x00FF);
	uint16_t response = (msb << 8) | lsb;

	// Stop SPI
	ss->write(1);
	spi->unlock();

	return response;
}