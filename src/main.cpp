/*******************************************************
 * Simple test of MAX5719 20bit DAC with arduino
 * 
 * Connect SPI pins CLK, MOSI and CS. MISO is not used
 * Code has hardcoded CS pin 10.
 * 
 * The MAX5719 has only one register to write.
 * 
 * The code below will output a sawtooth waveform
 * with voltage from 0 to REF voltage.
 * 
 * For a very simple test setup, just use 5V from arduino
 * as reference as well as power. But then expect NOISE !!!
 * 
 **********************************************************/

#include <Arduino.h>
#include <SPI.h>

SPISettings settingsA(8000000, MSBFIRST, SPI_MODE0); 
float REF_V= 4.44; // use reference voltage / max voltage

float volt= 0.0;
float inc = 0.010; // sawtooth step

uint32_t voltToCode(float dac_voltage) {
  uint32_t dac_code;
  float float_code;
  float max20bit = 1048575;

  float_code = max20bit * dac_voltage / REF_V;
  if (float_code < 0.0) float_code = 0.0;
  if (float_code > max20bit) float_code = max20bit;

  dac_code = (uint32_t) (float_code); 
  return dac_code;
}

void sendDataOverSPI(uint32_t data) {
  uint8_t val1, val2, val3;

  uint32_t data2 = data << 4;
  val1 = (uint8_t)((data2 >> 16) & 0xFF);
  val2 = (uint8_t)((data2 >> 8) & 0xFF);
  val3 = (uint8_t)((data2 >>  0) & 0xFF);

  SPI.beginTransaction(settingsA);
  digitalWrite (10, LOW);
  
  SPI.transfer(val1); 
  SPI.transfer(val2);    
  SPI.transfer(val3);    

  digitalWrite (10, HIGH);
  SPI.endTransaction();

  /*
  Serial.print(val1, HEX);
  Serial.print(" ");
  Serial.print(val2, HEX);
  Serial.print(" ");
  Serial.print(val3, HEX);
  Serial.println("");
  Serial.flush();
  */
}

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode (10, OUTPUT);
  SPI.begin();
}

void loop() {

  volt = volt + inc;

  if (volt > 4.0) {
    inc = -inc;
  } else if (volt < 0.0) {
    inc = -inc;
  }

  uint32_t code = voltToCode(volt);
  sendDataOverSPI(code);
  
  delay(10);
}
