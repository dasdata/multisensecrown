/*! @file Zanshin_BME680.h

@mainpage Arduino Library to control a Bosch BME680 Environmental Sensor

@section Zanshin_BME680_section Description

Class definition header for the Bosch BME680 temperature / humidity / pressure / air quality sensor. The sensor is 
described at https://www.bosch-sensortec.com/bst/products/all_products/BME680 and the datasheet is available from 
Bosch at https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001-00.pdf 
\n\n

The BME680 can use either SPI or I2C for communications. This library supports I2C at various bus speeds as well as 
SPI using either the standard Arduino hardware SPI or software SPI.\n\n

The most recent version of the library is available at https://github.com/SV-Zanshin/BME680 and extensive 
documentation of the library as well as example programs are described in the project's wiki pages located at
https://github.com/SV-Zanshin/BME680/wiki
\n
The BME680 is an extremely small physical package that is so tiny as to be impossible to solder at home, hence it 
will be used as part of a third-party breakout board. There are several such boards available at this time, for 
example:\n
Company  | Link
-------  | ----------
SparkFun | https://www.sparkfun.com/products/14570
BlueDot  | https://www.bluedot.space/sensor-boards/bme680/
Adafruit | https://learn.adafruit.com/adafruit-BME680-humidity-barometric-pressure-temperature-sensor-breakout 

\n
Bosch supplies sample software that runs on various platforms, including the Arduino family; this can be downloaed
at https://github.com/BoschSensortec/BSEC-Arduino-library . This software is part of the Bosch "BSEC" (Bosch 
Sensortec Environmental Cluster) framework and somewhat bulky and unwieldy for typical Arduino applications, it 
won't run on most Arduinos and the only example is for the Arduino Mega 2560 (due to the memory required), hence
the decision to make a more compact and rather less abstract library that will run on typical Arduino hardware.

@section license License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details. You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section author Author

Written by Arnd\@SV-Zanshin

@section versions Changelog

Version | Date       | Developer                     | Comments
------- | ---------- | ----------------------------- | --------
1.0.5   | 2020-05-21 | https://github.com/SV-Zanshin | Issue #12 - First call to getSensorData() returns invalid data
1.0.4   | 2020-05-14 | https://github.com/SV-Zanshin | Issue  #9 - Allow 2 devices when using I2C
1.0.4   | 2020-05-14 | https://github.com/SV-Zanshin | Issue  #9 - Allow 2 devices when using I2C
1.0.3   | 2020-05-09 | https://github.com/SV-Zanshin | Issue  #5 - Adjust readings. Subsequently removed code again
1.0.3   | 2020-05-09 | https://github.com/SV-Zanshin | Issue  #8 - clean up comments and code
1.0.2   | 2019-01-26 | https://github.com/SV-Zanshin | Issue  #3 - Converted documentation to doxygen style
1.0.1   | 2018-07-22 | https://github.com/SV-Zanshin | Corrected I2C datatypes
1.0.1   | 2018-07-03 | https://github.com/SV-Zanshin | Issue  #1. Added waitForReading and param to getSensorData()
1.0.0   | 2018-07-02 | https://github.com/SV-Zanshin | Added guard code against multiple I2C constants definitions
1.0.0   | 2018-07-01 | https://github.com/SV-Zanshin | Added and tested I2C, SPI and software SPI connections
1.0.0a  | 2018-06-30 | https://github.com/SV-Zanshin | Cloned from BME280 library and started recoding
*/
#include "Arduino.h" // Arduino data type definitions
#include <Wire.h>    // Standard I2C "Wire" library
#include <SPI.h>     // Standard SPI library
#ifndef BME680_h
  /** @brief  Guard code definition for the library header */
  #define BME680_h
  #define CONCAT_BYTES(msb, lsb) (((uint16_t)msb << 8) | (uint16_t)lsb) ///< Inline to combine msb and lsb
  /*****************************************************************************************************************
  ** Declare constants used in the class                                                                          **
  *****************************************************************************************************************/
  #ifndef I2C_MODES
  /** @brief  Guard code definition for the I2C modes */
  #define I2C_MODES
    const uint32_t I2C_STANDARD_MODE              =  100000; ///< Default normal I2C 100KHz speed
    const uint32_t I2C_FAST_MODE                  =  400000; ///< Fast mode
    const uint32_t I2C_FAST_MODE_PLUS             = 1000000; ///< Really fast mode
    const uint32_t I2C_HIGH_SPEED_MODE            = 3400000; ///< Turbo mode
  #endif
  const uint32_t SPI_HERZ                         =  500000; ///< SPI speed in Hz
  const uint8_t  BME680_STATUS_REGISTER           =    0x1D; ///< Device status register
  const uint8_t  BME680_GAS_HEATER_REGISTER0      =    0x5A; ///< Heater Register 0 address
  const uint8_t  BME680_GAS_DURATION_REGISTER0    =    0x64; ///< Heater Register 0 address
  const uint8_t  BME680_CONTROL_GAS_REGISTER1     =    0x70; ///< Gas control register on/off
  const uint8_t  BME680_CONTROL_GAS_REGISTER2     =    0x71; ///< Gas control register settings
  const uint8_t  BME680_CONTROL_HUMIDITY_REGISTER =    0x72; ///< Humidity control register
  const uint8_t  BME680_SPI_REGISTER              =    0x73; ///< Status register for SPI memory
  const uint8_t  BME680_CONTROL_MEASURE_REGISTER  =    0x74; ///< Temp, Pressure control register
  const uint8_t  BME680_CONFIG_REGISTER           =    0x75; ///< Configuration register
  const uint8_t  BME680_CHIPID_REGISTER           =    0xD0; ///< Chip-Id register
  const uint8_t  BME680_SOFTRESET_REGISTER        =    0xE0; ///< Reset when 0xB6 is written here
  const uint8_t  BME680_CHIPID                    =    0x61; ///< Hard-coded value 0x61 for BME680
  const uint8_t  BME680_RESET_CODE                =    0xB6; ///< Reset when this put in reset reg
  const uint8_t  BME680_MEASURING_BIT_POSITION    =       5; ///< Bit position for measuring flag
  const uint8_t  BME680_I2C_MIN_ADDRESS           =    0x76; ///< Minimum possible address for BME680
  const uint8_t  BME680_I2C_MAX_ADDRESS           =    0x77; ///< Minimum possible address for BME680
  const uint8_t  BME680_SPI_MEM_PAGE_POSITION     =       4; ///< Bit position for the memory page value
  const uint8_t  BME680_HUMIDITY_MASK             =    0xF8; ///< Mask is binary B11111000
  const uint8_t  BME680_TEMPERATURE_MASK          =    0xE3; ///< Mask is binary B11100011
  const uint8_t  BME680_PRESSURE_MASK             =    0x1F; ///< Mask is binary B00011111
  /*********************************************** 
  ** Declare the constants used for calibration **
  ***********************************************/
  const uint8_t BME680_COEFF_SIZE1                =      25; ///< First array with coefficients
  const uint8_t BME680_COEFF_SIZE2                =      16; ///< Second array with coefficients
  const uint8_t BME680_COEFF_START_ADDRESS1       =    0x89; ///< start address for array 1
  const uint8_t BME680_COEFF_START_ADDRESS2       =    0xE1; ///< start address for array 2
  const uint8_t BME680_HUM_REG_SHIFT_VAL          =       4; ///< Ambient humidity shift value
  const uint8_t BME680_BIT_H1_DATA_MSK            =    0x0F; ///< Mask for humidity
  const uint8_t BME680_T2_LSB_REG                 =       1; ///< Register for temperature calibration
  const uint8_t BME680_T2_MSB_REG                 =       2; ///< Register for temperature calibration
  const uint8_t BME680_T3_REG		                  =       3; ///< Register for temperature calibration
  const uint8_t BME680_P1_LSB_REG   	            =       5; ///< Register for pressure calibration
  const uint8_t BME680_P1_MSB_REG	                =       6; ///< Register for pressure calibration
  const uint8_t BME680_P2_LSB_REG	                =       7; ///< Register for pressure calibration
  const uint8_t BME680_P2_MSB_REG	                =       8; ///< Register for pressure calibration
  const uint8_t BME680_P3_REG		                  =       9; ///< Register for pressure calibration
  const uint8_t BME680_P4_LSB_REG           	    =      11; ///< Register for pressure calibration
  const uint8_t BME680_P4_MSB_REG          	      =      12; ///< Register for pressure calibration
  const uint8_t BME680_P5_LSB_REG             	  =      13; ///< Register for pressure calibration
  const uint8_t BME680_P5_MSB_REG           	    =      14; ///< Register for pressure calibration
  const uint8_t BME680_P7_REG  	                  =      15; ///< Register for pressure calibration
  const uint8_t BME680_P6_REG	                    =      16; ///< Register for pressure calibration
  const uint8_t BME680_P8_LSB_REG    	            =      19; ///< Register for pressure calibration
  const uint8_t BME680_P8_MSB_REG	                =      20; ///< Register for pressure calibration
  const uint8_t BME680_P9_LSB_REG      	          =      21; ///< Register for pressure calibration
  const uint8_t BME680_P9_MSB_REG	                =      22; ///< Register for pressure calibration
  const uint8_t BME680_P10_REG		                =      23; ///< Register for pressure calibration
  const uint8_t BME680_H2_MSB_REG	                =       0; ///< Register for humidity calibration
  const uint8_t BME680_H2_LSB_REG	                =       1; ///< Register for humidity calibration
  const uint8_t BME680_H1_LSB_REG    	            =       1; ///< Register for humidity calibration
  const uint8_t BME680_H1_MSB_REG	                =       2; ///< Register for humidity calibration
  const uint8_t BME680_H3_REG	                    =       3; ///< Register for humidity calibration
  const uint8_t BME680_H4_REG    	                =       4; ///< Register for humidity calibration
  const uint8_t BME680_H5_REG	                    =       5; ///< Register for humidity calibration
  const uint8_t BME680_H6_REG                     =       6; ///< Register for humidity calibration
  const uint8_t BME680_H7_REG                     =       7; ///< Register for humidity calibration
  const uint8_t BME680_T1_LSB_REG	                =       8; ///< Register for gas calibration
  const uint8_t BME680_T1_MSB_REG      	          =       9; ///< Register for gas calibration
  const uint8_t BME680_GH2_LSB_REG                =      10; ///< Register for gas calibration
  const uint8_t BME680_GH2_MSB_REG                =      11; ///< Register for gas calibration
  const uint8_t BME680_GH1_REG	                  =      12; ///< Register for gas calibration
  const uint8_t BME680_GH3_REG	                  =      13; ///< Register for gas calibration
  const uint8_t BME680_ADDR_RES_HEAT_RANGE_ADDR   =    0x02; ///< Register for gas calibration
  const uint8_t BME680_RHRANGE_MSK                =    0x30; ///< Register for gas calibration
  const uint8_t BME680_ADDR_RES_HEAT_VAL_ADDR     =    0x00; ///< Register for gas calibration
  const uint8_t BME680_ADDR_RANGE_SW_ERR_ADDR     =    0x04; ///< Register for gas calibration
  const uint8_t BME680_RSERROR_MSK	              =    0xF0; ///< Register for gas calibration

  #ifndef _BV
    #define _BV(bit) (1 << (bit)) ///< Some implementations don't have this bit-shift macro pre-defined
  #endif

  /*****************************************************************************************************************
  ** Declare enumerated types used in the class                                                                   **
  *****************************************************************************************************************/
  /*! @brief  Enumerate the sensor type */
  enum sensorTypes       {TemperatureSensor, HumiditySensor, PressureSensor, GasSensor, UnknownSensor};
  /*! @brief  Enumerate the Oversampling types */
  enum oversamplingTypes {SensorOff,Oversample1,Oversample2,Oversample4,Oversample8,Oversample16,UnknownOversample};
  /*! @brief  Enumerate the iir filter types */
  enum iirFilterTypes    {IIROff, IIR2, IIR4, IIR8, IIR16, IIR32, IIR64, IIR128, UnknownIIR};
  
  class BME680_Class 
  {
    /*!
    * @class BME680_Class
    * @brief Main BME680 class for the temperature / humidity / pressure sensor
    */
    public:
      BME680_Class();                                                         // Class constructor (unused)
      ~BME680_Class();                                                        // Class destructor (unused)
      bool     begin();                                                       // Start using I2C Communications
      bool     begin(const uint32_t i2cSpeed);                                // I2C with a non-default speed
      bool     begin(const uint8_t chipSelect);                               // Start using either I2C or HW-SPI
      bool     begin(const uint32_t i2cSpeed, const uint8_t i2cAddress);      // Explicitly set speed and I2C Addr.
      bool     begin(const uint8_t chipSelect, const uint8_t mosi,            // Start using software SPI
                     const uint8_t miso, const uint8_t sck);                  //
      bool     setOversampling(const uint8_t sensor, const uint8_t sampling); // Set enum sensorType Oversampling
      bool     setGas(uint16_t GasTemp, uint16_t GasMillis);                  // Gas heating temperature and time
      uint8_t  setIIRFilter(const uint8_t iirFilterSetting=UINT8_MAX);        // Set IIR Filter and return value
      void     getSensorData(int32_t &temp, int32_t &hum,                     // get most recent readings
                             int32_t &press, int32_t &gas,                    //
                             const bool waitSwitch = true);                   //
      void     reset();                                                       // Reset the BME680
  private:                                                                    //
      bool     commonInitialization();                                        ///< Common initialization code
      uint8_t  readByte(const uint8_t addr);                                  ///< Read byte from register address
      void     readSensors(const bool waitSwitch);                            ///< read the registers in one burst
      void     waitForReadings();                                             ///< Wait for readings to finish
      void     getCalibration();                                              ///< Load calibration from registers
      uint8_t  _I2CAddress = 0;                                               ///< Default is I2C address is unknown
      uint8_t  _cs,_sck,_mosi,_miso;                                          ///< Hardware and software SPI pins
      uint8_t  _H6,_P10,_res_heat_range;                                      ///< unsigned configuration vars
      int8_t   _H3,_H4,_H5,_H7,_G1,_G3,_T3,_P3,_P6,_P7,_res_heat,_rng_sw_err; ///< signed configuration vars
      uint16_t _H1,_H2,_T1,_P1;                                               ///< unsigned 16bit configuration vars
      int16_t  _G2,_T2,_P2,_P4,_P5,_P8,_P9;                                   ///< signed 16bit configuration vars
      int32_t  _tfine,_Temperature,_Pressure,_Humidity,_Gas;                  ///< signed 32bit configuration vars
      /*************************************************************************************************************
      ** Declare the getData and putData methods as template functions. All device I/O is done through these two  **
      ** functions regardless of whether I2C, hardware SPI or software SPI is being used. The two functions are   **
      ** designed so that only the address and a variable are passed in and the functions determine the size of   **
      ** the parameter variable and reads or writes that many bytes. So if a read is called using a character     **
      ** array[10] then 10 bytes are read, if called with a int8 then only one byte is read. The return value, if **
      ** used, is the number of bytes read or written. Since this is implemented by using template function       **
      ** definitions, they need to be defined in this header file rather than in the c++ program library file.    **
      *************************************************************************************************************/
      template< typename T > uint8_t &getData(const uint8_t addr,T &value)
      {
        /*!
          @brief     Template for reading from I2C or SPI using any data type
          @details   As a template it can support compile-time data type definitions
          @param[in] addr Memory address
          @param[in] value Data Type "T" to read
          @return    Size of data read in bytes
        */
        uint8_t* bytePtr    = (uint8_t*)&value;                              // Pointer to structure beginning
        static uint8_t  structSize = sizeof(T);                              // Number of bytes in structure
        if (_I2CAddress)                                                     // Using I2C if address is non-zero
        {                                                                    //
          Wire.beginTransmission(_I2CAddress);                               // Address the I2C device
          Wire.write(addr);                                                  // Send register address to read
          Wire.endTransmission();                                            // Close transmission
          Wire.requestFrom(_I2CAddress, sizeof(T));                          // Request 1 byte of data
          structSize = Wire.available();                                     // Use the actual number of bytes
          for (uint8_t i=0;i<structSize;i++) *bytePtr++ = Wire.read();       // loop for each byte to be read
        }                                                                    //
        else                                                                 //
        {                                                                    //
          if (_sck==0)                                                       // if sck is zero then hardware SPI
          {                                                                  //
            SPI.beginTransaction(SPISettings(SPI_HERZ,MSBFIRST,SPI_MODE0));  // Start the SPI transaction
            digitalWrite(_cs, LOW);                                          // Tell BME680 to listen up
            SPI.transfer(addr | 0x80);                                       // bit 7 is high, so read a byte
            for (uint8_t i=0;i<structSize;i++) *bytePtr++ = SPI.transfer(0); // loop for each byte to be read
            digitalWrite(_cs, HIGH);                                         // Tell BME680 to stop listening
            SPI.endTransaction();                                            // End the transaction
          }                                                                  //
          else                                                               //
          {                                                                  // otherwise we are using soft SPI
            int8_t i,j;                                                      // Loop variables
            uint8_t reply;                                                   // return byte for soft SPI transfer
            digitalWrite(_cs, LOW);                                          // Tell BME680 to listen up
            for (j=7; j>=0; j--)                                             // First send the address byte
            {                                                                //
              digitalWrite(_sck, LOW);                                       // set the clock signal
              digitalWrite(_mosi, ((addr)|0x80)&(1<<j));                     // set the MOSI pin state
              digitalWrite(_sck, HIGH);                                      // reset the clock signal
            } // of for-next each bit                                        //
            for (i=0;i<structSize;i++)                                       // Loop for each byte to read
            {                                                                //
              reply = 0;                                                     // reset our return byte
              for (j=7; j>=0; j--)                                           // Now read the data at that byte
              {                                                              //
                reply <<= 1;                                                 // shift buffer one bit left
                digitalWrite(_sck, LOW);                                     // set and reset the clock signal
                digitalWrite(_sck, HIGH);                                    // pin to get the next MISO bit
                if (digitalRead(_miso)) reply |= 1;                          // read the MISO bit, add to reply
              } // of for-next each bit                                      //
              *bytePtr++ = reply;                                            // Add byte just read to return data
            } // of for-next each byte to be read                            //
            digitalWrite(_cs, HIGH);                                         // Tell BME680 to stop listening
          } // of  if-then-else we are using hardware SPI
        } // of if-then-else we are using I2C
        return(structSize);
      } // of method getData()
      template< typename T > uint8_t &putData(const uint8_t addr,const T &value)
      {
        /*!
          @brief     Template for writing to I2C or SPI using any data type
          @details   As a template it can support compile-time data type definitions
          @param[in] addr Memory address
          @param[in] value Data Type "T" to write
          @return    Size of data written in bytes
        */
        const uint8_t* bytePtr = (const uint8_t*)&value;                     // Pointer to structure beginning
        static uint8_t  structSize = sizeof(T);                              // Number of bytes in structure
        if (_I2CAddress)                                                     // Using I2C if address is non-zero
        {                                                                    //
          Wire.beginTransmission(_I2CAddress);                               // Address the I2C device
          Wire.write(addr);                                                  // Send register address to write
          for (uint8_t i=0;i<sizeof(T);i++) Wire.write(*bytePtr++);          // loop for each byte to be written
          Wire.endTransmission();                                            // Close transmission
        }                                                                    //
        else                                                                 //
        {                                                                    //
          if (_sck==0)                                                       // if sck is zero then hardware SPI
          {                                                                  //
            SPI.beginTransaction(SPISettings(SPI_HERZ,MSBFIRST,SPI_MODE0));  // start the SPI transaction
            digitalWrite(_cs, LOW);                                          // Tell BME680 to listen up
            SPI.transfer(addr & ~0x80);                                      // bit 7 is low, so write a byte
            for (uint8_t i=0;i<structSize;i++) { SPI.transfer(*bytePtr++); } // loop for each byte to be written
            digitalWrite(_cs, HIGH);                                         // Tell BME680 to stop listening
            SPI.endTransaction();                                            // End the transaction
          }                                                                  //
          else                                                               // Otherwise soft SPI is used
          {                                                                  //
            int8_t i,j;                                                      // Loop variables
            uint8_t reply;                                                   // return byte for soft SPI transfer
            for (i=0;i<structSize;i++)                                       // Loop for each byte to read
            {                                                                //
              reply = 0;                                                     // reset our return byte
              digitalWrite(_cs, LOW);                                        // Tell BME680 to listen up
              for (j=7; j>=0; j--)                                           // First send the address byte
              {                                                              //
                digitalWrite(_sck, LOW);                                     // set the clock signal
                digitalWrite(_mosi, (addr&~0x80)&(1<<j));                    // set the MOSI pin state
                digitalWrite(_sck, HIGH);                                    // reset the clock signal
              } // of for-next each bit                                      //
              for (j=7; j>=0; j--)                                           // Now read the data at that byte
              {                                                              //
                reply <<= 1;                                                 // shift buffer one bit left
                digitalWrite(_sck, LOW);                                     // set the clock signal
                digitalWrite(_mosi, *bytePtr&(1<<j));                        // set the MOSI pin state
                digitalWrite(_sck, HIGH);                                    // reset the clock signal
              } // of for-next each bit                                      //
              digitalWrite(_cs, HIGH);                                       // Tell BME680 to stop listening
              bytePtr++;                                                     // go to next byte to write 
            } // of for-next each byte to be read
          } // of  if-then-else we are using hardware SPI
        } // of if-then-else we are using I2C
        return(structSize);
      } // of method putData()
  }; // of BME680 class definition
#endif