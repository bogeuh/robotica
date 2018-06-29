#include <OneWire.h>

OneWire DS18B20(14, true); 
// DQ of DS18B20 is connected to pin 14 and Internal Input Pullup enabled
// Note: Second argument only works with latest OneWire Library!

byte parasite_power_mode = 1; // set to 1 if DS18B20 is powered in parasite mode, else set to 0
byte rom_code[8]; // will hold DS18B20's 64 bit rom code (address)
byte data[9];     // will hold the 9 bytes of DS18B20's scratchpad (RAM)

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println("Init");
  
  DS18B20.reset_search();
  delay(250); // cf. 18B20 datasheet for timing
  
  if (DS18B20.search(rom_code))
  {
    if (OneWire::crc8(rom_code, 7) != rom_code[7])
    {
      Serial.println("DS18B20 Address CRC Fail");
    }
    else
    {
      Serial.println("DS18B20 found");
      for (int i = 0; i < 8; i++) 
      {
        Serial.print(rom_code[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
  }
  else
  {
    Serial.println("No DS18B20 found");
  }
  
  
  Serial.println("Selecting 12 bit resolution for temperature conversation");
  // We need to supply byte No 2, 3 and 4 of Scratchpad: TH, TL and Configuration Register, cf. 18B20 datasheet
  DS18B20.reset();
  DS18B20.select(rom_code);
  DS18B20.write(0x4E, parasite_power_mode); // Write "WRITE SCRATCHPAD command"
  DS18B20.write(0x55, parasite_power_mode); // Write TH (in 2s complement), as an example we select 85 degrees Celsius, i.e. 55 in hex
  DS18B20.write(0xF6, parasite_power_mode); // Write TL (in 2s complement), as an example we select -10 degrees Celsius, 1111 0110 = F6 in hex
  DS18B20.write(0x7F, parasite_power_mode); // Write Config. Register. 12 Bit Resolution => Bit 5 and 6 are set: 0 1 1 1 1 1 1 1 = 127d = 7F in hex
  DS18B20.reset();
  
  DS18B20.select(rom_code);
  DS18B20.write(0xBE, parasite_power_mode); // Write "READ SCRATCHPAD command"
  
  // now, read 9 bytes
  for (int j = 0; j < 9; j++)
  {
    data[j] = DS18B20.read();
  }
    
  if (OneWire::crc8(data, 8) != data[8])
  {
    Serial.println("CRC fail, could not read scratchpad");
  }
  else
  {
    if (data[4] == 127) 
    {
      Serial.println("12 bit resolution successfully set");
    }
    else
    {
      Serial.println("12 bit resolution could not be set");
    }
  }

}

void loop()
{
  DS18B20.reset();
  DS18B20.select(rom_code);
  DS18B20.write(0x44, parasite_power_mode); // Write "CONVERT T command"
  delay(900);  // 750 msec would be enough according to the datasheet, but 900 is safer
  
  DS18B20.reset();
  DS18B20.select(rom_code);
  DS18B20.write(0xBE, parasite_power_mode); // Write "READ SCRATCHPAD command"
  
  for (int j = 0; j < 9; j++)
  {
    data[j] = DS18B20.read();
  }
  
  if (OneWire::crc8(data, 8) != data[8])
  {
    Serial.println("CRC fail, could not read scratchpad");
  }
  else
  {
    Serial.println("Temperature successfully read");
  }

  // Temperature value is in 2's complement in byte No 0 and 1 of scratchpad
  // => convert to a 16 bit signed integer (! do not use int, because it might be a 32 bit signed integer data type)
  int16_t raw = (data[1] << 8) | data[0];
  float celsius = (float)raw / 16.0; // type cast
  Serial.print("Temperature in Celsius: ");
  Serial.println(celsius, 4);
  Serial.println();
  delay(5000);
}
