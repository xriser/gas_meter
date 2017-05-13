# gas_meter
esp8266_Hall_sensor gas impulse meter

In the last digit of the counting mechanism of the gas meter, a small permanent magnet is built in. We will catch the magnetic field created by a permanent magnet, at the moment when the wheel of the counting mechanism passes near the Hall sensor.
Thus, a logical unit is formed at the output of the sensor, which we will write in the timing database InfluxDb

