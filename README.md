# gas_meter
esp8266_Hall_sensor gas impulse meter

There is a small permanent magnet built in the last digit of the counting mechanism of the gas meter. We will catch the magnetic field created by a permanent magnet, at the moment when the wheel of the counting mechanism passes near the Hall sensor.
Thus, a logical unit is formed at the output of the sensor, which we will write in the timing database InfluxDb



!["Gas meter with Hall sensor A3144](gas_meter1.jpg?raw=true "Gas meter with Hall sensor A3144")
