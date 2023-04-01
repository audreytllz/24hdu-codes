#ifndef CARBOARD_HPP
#define CARBOARD_HPP

#include <array>
#include <cinttypes>

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Servo.h>

class CarBoard {
	// There are also private member variables for the minimum and maximum steering values, and the starting values for the throttle going 
	// forward and backward.
	private:
		Servo _throttleServo;
		Servo _steeringServo;

		uint16_t _batt_adc = 0;
		unsigned long _batt_adc_time = 0;

		std::array<int16_t, 3> _imu_xl;
		std::array<int16_t, 3> _imu_g;
		unsigned long _imu_sample_time = 0;

		uint8_t _ir_value;
		unsigned long _ir_time = 0;

		// value for forward, backward, left/right turn
		uint16_t _steering_left = 2000;
		uint16_t _steering_right = 1000;
		uint16_t _throttle_start_fw = 0;
		uint16_t _throttle_start_bw = 0;

	// The class provides public methods for initializing the car, running the car's main loop, getting the MAC address, loading and saving
	// data to EEPROM
	public:
		void init();
		void loop();

		std::array<uint8_t, 6> mac() const;

		template<typename T>
		T eeprom_load() const {
			T ret;
			EEPROM.get(0, ret);
			return ret;
		}
		template<typename T>
		void eeprom_save(const T & data) {
			EEPROM.put(0, data);
			EEPROM.commit();
		}

		Stream & debug_serial() const;

		// setting the throttle, steering, headlights, and color of the car, adjusting the steering trim and starting throttle values
		void setThrottle(int16_t i_speed);
		void setSteering(int16_t i_angle);
		void setHeadlights(uint16_t i_pwr);
		void setColor(uint8_t r, uint8_t g, uint8_t b);

		void setSteeringTrim(int16_t v);
		void setThrottleStart(uint16_t fw, uint16_t bw);

		// Getting the battery level and state of charge
		uint16_t batteryLevel_ADC() const;
		int16_t batterySOC() const;

		// getting the IMU and IR sensor data
		std::array<int16_t, 3> imuAccelerometerData() const { return _imu_xl; }
		std::array<int16_t, 3> imuGyroscopeData() const { return _imu_g; }

		uint8_t ir_value() const { return _ir_value; }
};

#endif
