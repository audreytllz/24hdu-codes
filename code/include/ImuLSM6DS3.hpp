#include <Arduino.h>
#include <Wire.h>

// This is a CPP code for defining a class named ImuLSM6DS3
class ImuLSM6DS3 {
  private:
    TwoWire & _wire; // Reference to an instance of TwoWire class
    uint8_t _address = 0x6A; // Default address of the sensor
    bool _found = false; // A flag to indicate if the sensor is found or not

  protected:
    // Methods for reading and writing registers
    uint8_t readRegister(uint8_t address);
    void readRegisters(uint8_t address, uint8_t * data, size_t length);
    void writeRegister(uint8_t address, uint8_t value);

  public:
    typedef std::array<int16_t, 3> data_t;

    // constructors
    ImuLSM6DS3(TwoWire& wire) : _wire(wire) {}
    ImuLSM6DS3(TwoWire& wire, uint8_t address) : _wire(wire), _address(address) {}

    // Method for initializing the sensor
    void init();

    // Methods for reading accelerometer data
    data_t readAccelerometer();
    bool accelerometerDataReady();

    // Methods for reading gyroscope data
    data_t readGyroscope();
    bool gyroscopeDataReady();

};
