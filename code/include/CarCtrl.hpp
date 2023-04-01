#ifndef CARCTRL_HPP
#define CARCTRL_HPP

#include <CarBoard.hpp>
#include <Simulation.hpp>
#include <ESP8266WiFi.h>

class CarCtrlBase {
	protected:
		// This class contains a CarBoard object, which presumably controls the physical car.
		CarBoard _car;
		// It also contains a WiFiServer object, which will be used for logging.
		WiFiServer _log_server{23};
		WiFiClient _log_client;
		// This boolean flag can be set to initiate a shutdown sequence.
		bool _shutdown = false;

	public:
		// This function returns a Stream object, which will be used for logging.
		Stream & log();
};

class CarCtrlConfig : virtual public CarCtrlBase {
	protected:
		// This struct contains configuration data for the car, such as its name and admin password.
		struct Config {
			char header[8] = "CarNode";
			uint8_t version = 1;
			char name[17] = "";
			std::array<uint8_t, 6> adminpass {0, 0, 0, 0, 0, 0};
			int16_t steeringTrim = 0;
			uint16_t throttle_start_fw = 0;
			uint16_t throttle_start_bw = 0;

			Config & operator=(const Config &) = default;
		};
		// These functions allow the configuration to be set and retrieved.
		void setConfig(const Config & c);
		void useDefaultConfig();

	private:
		Config _config;

		// This function sets the configuration name based on the MAC address.
		void setConfigNameMAC();

	public:
		void setConfigName(String name);
		const char * configName();
		String hostname();
		void setConfigAdminPass(std::array<uint8_t, 6> pass);
		const std::array<uint8_t, 6> configAdminPass() const;
		void setConfigSteeringTrim(int16_t value);
		const int16_t configSteeringTrim() const;
		void setConfigThrottleStart(uint16_t fw, uint16_t bw);
		const uint16_t configThrottleStartFw() const;
		const uint16_t configThrottleStartBw() const;
		void saveConfig();
};

class CarCtrlPilot : virtual public CarCtrlBase {
	private:
		// These variables store the current speed and angle of the car.
		int16_t _angle = 0;
		int16_t _speed = 0;
		bool _speed_down = false;
		bool _pilot_started = false;
		// These flags control whether the throttle and steering directions are inverted.
		bool _invert_throttle = false;
		bool _invert_steering = false;
		// These variables define the maximum positive and negative speeds of the car.
		int16_t _speed_max_pos = 8192;
		int16_t _speed_max_neg = -8192;

	protected:
		// This class contains a Simulation object, which presumably simulates the car's behavior.
		Simulation _sim;

	public:
		const Simulation & simulation() const { return _sim; }
		// These functions control the car's "pilot mode", in which it is controlled by an external source.
		void start_pilot(bool on);
		void pilot(int16_t i_speed, int16_t i_angle);
		// These functions retrieve the current speed, angle, and other status flags.
		int16_t speed() { return _speed; }
		int16_t angle() { return _angle; }
		bool speed_down() { return _speed_down; }
		bool pilot_started() { return _pilot_started; }
		void invertSteering(bool on) { _invert_steering = on; }
		void invertThrottle(bool on) { _invert_throttle = on; }
		void limitSpeed(int16_t pos, int16_t neg);
};

// Car control class for controlling the headlights of a car
class CarCtrlHeadlights : virtual public CarCtrlBase {
	private:
		bool _blink = false;
		uint16_t _headlights_pwr = 0;
		uint16_t _headlights_pwr_set = 0;

	protected:
		// Set the power of the headlights
		void setHeadlightsPower(uint16_t i_pwr);

	public:
		// Set whether the headlights should blink
		void setBlink(bool on) { _blink = on; }
		// Returns true if the headlights are set to blink
		bool blinks() const { return _blink; }
		// Set the power of the headlights to the specified value
		void setHeadlights(uint16_t i_pwr);
		// Returns the current power of the headlights
		uint16_t headlightsPower() const;
};

// Car control class for controlling the rear lights of a car
class CarCtrlRearlight : virtual public CarCtrlBase {
	public:
		// Define a type for a color, which is an array of three uint8_t values
		typedef std::array<uint8_t, 3> color_t;

	private:
		color_t _color = {255, 0, 255};
		color_t _color_set = {0, 0, 0};

	protected:
	// Set the color of the displayed lights
		void setDisplayedColor(const color_t c);

	public:
		// Set the color of the lights to the specified color
		void setColor(const color_t c);
		// Returns the current color of the lights
		color_t color() const;
		// Returns the currently displayed color of the lights
		color_t displayed_color() const;
};

// Car control class for reading data from various sensors
class CarCtrlLL : virtual public CarCtrlBase {
	public:
		// Returns the current battery level in ADC units
		uint16_t batteryLevel_ADC() const { return _car.batteryLevel_ADC(); }
		// Returns the current battery state of charge (SOC) as a percentage
		int16_t batterySOC() const { return _car.batterySOC(); }

		// Returns an array of accelerometer data from the IMU
		std::array<int16_t, 3> imu_accelerometerData() const { return _car.imuAccelerometerData(); }
		// Returns an array of gyroscope data from the IMU
		std::array<int16_t, 3> imu_gyroscopeData() const { return _car.imuGyroscopeData(); }
		// Returns the current IR value
		uint8_t ir_value() const { return _car.ir_value(); }
};

// Main car control class that inherits from all the other car control classes
class CarCtrl : public CarCtrlConfig, public CarCtrlPilot, public CarCtrlHeadlights, public CarCtrlRearlight, public CarCtrlLL {
	public:
		// Initializes the car control
		void init();
		// Main loop for the car control
		void loop();

		// Starts or stops the car engine
		void start_engine(bool on);
		// Shuts down the car control
		void shutdown();
};

#endif
