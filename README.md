# Bayang_USB_Joystick
Connecting your transmitter to your PC to play simulators is a pain. There are a few projects out there to let you connect a PPM receiver to your PC, but even a cheap receiver is around $10. Since most of us have multiprotocol transmitters these days, I set about to implement the low latency Bayang protocol, using cheap hardware available everywhere.

STM32 Blue Pill: $2 shipped on Aliexpress
NRF24L01: $1 shipped on Aliexpress

Or you can find them locally for cheap. 7 wires to hook up on the NRF24L01:
Gnd
3.3v (get these from the STM32 pads)
CE to B12
SCK to B13
MISO to B14
MOSI to B15
CSN to B7
(Optional)
LED to A1 (with a resistor, and other side to Gnd)
Push button to A0 (other side to Gnd)

The pushbutton functionality isnt working at the moment - i had intended to have that used to rebind, but i found it easier to just replug the USB cable - it goes into bind mode immediately on startup (fast LED flash).
When plugged into the PC it will get recognised as HID-compliant game controller, with 4 axes, and 8 digital buttons that correspond with channels 5-12*. On www.gamepad-tester.com, it shows up as "BOMJ JOYSTICK (Vendor: 0483 Product: 5750)"
Then select a bayang protocol model on your transmitter and it should bind up and go into a slower flash, before finally settling to on. In gamepad-tester.com you will see the joystick movements and aux channel switches

Channels 5-12 are digital buttons. However due to a quirk in the bayang implementation on Multiprotocol, Deviation and OpenTX (Multiprotocol module) send channels differently. On Deviation, its channels 5-12. On OpenTX its channels 5-10, and then 12 and 13. You can select which one you have in Defines.h (but unless you want all 8 channels its not really important)

Special Thanks to Garrus007 for the PPM to USB Joystick implementation which this is mostly based on (I believe it includes some of the Keil/ST sample code), and also Silverx/NFE for the Bayang implementation that i took from the Silverware flight controller project.
If there is anyone else I should be giving credit to, please let me know.
