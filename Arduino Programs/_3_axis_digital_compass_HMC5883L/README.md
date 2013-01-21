This arduino sketch contains the program to operate a Triple Axis Magnetometer which is a breakout board for Honeywell's HMC5883L, a 3-axis digital compass.. More information on this breakout board can be found on: https://www.sparkfun.com/products/10530.

Communication with the HMC5883L is simple and all done through an I2C interface. On the Arduino Mega 2560 board (the one used in this project) pins 20 and 21 are SDA (data line) and SCL (clock line) respectively. 
