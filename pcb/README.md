# STM32 Digital Potentiomer Module

### Description:

The main purpose of this module is to replace the need to use a physical multi turn potentiometer in places where they
are frequently used and therefore easly broken. Instead with the help of the digital potentiometer you can use an
encoder which is far more affordable.

### How to use

1. Connect the rotary encoder to the "ENC" connector
2. Connect the "POT" connector to the required potentiometer output that you want to emulate
3. Connect "POWER" connector
4. The "POWER" LED will light up and the module will start converting the digital signals from the encoder into analog ones

### Important information

1. The module is designed to handle <strong>MAX</strong> voltage of <strong>15V</strong>
2. If an error occurs, the "ERROR" LED will light up and the module will restart itself after 1 second
3. In order to debug or update the firmware you should connect the "SWD" and "USART" connectors to a PC with ST-Link Debugger 