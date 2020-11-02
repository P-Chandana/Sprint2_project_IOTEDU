# IoT based Air Quality Monitoring using NodeMCU and MQ2

There have been many incidents like explosions and fire due to certain gases leakage. Such incidents can cause dangerous effects if the leakage is not detected at an early stage. Therefore, Measurement and control of these types of toxic gases present in the environment are essential. Monitoring and analyzing multiple environmental parameters in real-time serves as one of the best solutions for controlling the particular process. A portable system regarding the same in real-time, log data to a remote server, and keeping the data updated over the internet can offer an overview of the process. Gases measurements are in  Parts per Million (PPM) metrics.

### Boards and Libraries:

* To add ESP8266 board in Arduino IDE enter ```https://arduino.esp8266.com/stable/package_esp8266com_index.json``` into the Additional Board Manager URLs field.
* [NTP client library](https://github.com/arduino-libraries/...)
* [PubSub client library](https://github.com/knolleary/pubsubcl...)
* [ESP8266 Sketch Data Upload](https://github.com/esp8266/arduino-esp8266fs-plugin/releases/download/0.5.0/ESP8266FS-0.5.0.zip)


![](ARCHITECTURE.png)

## Future Scope:
We can add more sensors to get more information about various gas concentrations.
We can use GSM module to make it cost efficient and easy installation.

![](future_scope.png)


