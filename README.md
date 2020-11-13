# Bayang USB Joystick
Connecting your transmitter to your PC to play simulators is a pain. There are a few projects out there to let you connect a PPM receiver to your PC, but even a cheap receiver is around $10, and Arduino Pro Minis are $5-10. Since most of us have multiprotocol transmitters these days, I set about to implement the low latency Bayang protocol, using cheap hardware available everywhere.

<UL>
  <li>STM32 Blue Pill (STM32F103C8T6, 64KB): $2 shipped on Aliexpress</li>
  <li>NRF24L01 (PA/LNA optional): $1 shipped on Aliexpress</li>
  <li>Or you can find them locally for cheap.</li>
</UL>

7 wires to hook up on the NRF24L01:
<ol>
<li>Gnd</li>
<li>3.3v (get these from the STM32 pads)</li>
<li>CE to B12</li>
<li>SCK to B13</li>
<li>MISO to B14</li>
<li>MOSI to B15</li>
<li>CSN to B7</li>
<li>(Optional)</li>
LED to A1 (with a resistor, and other side to Gnd)<br>
  Push button to A0 (other side to Gnd)</li>
</ol>
  
The pushbutton functionality isnt working at the moment - i had intended to have that used to rebind, but i found it easier to just replug the USB cable - it goes into bind mode immediately on startup (fast LED flash).<br>
When plugged into the PC it will get recognised as HID-compliant game controller, with 4 axes, and 8 digital buttons that correspond with channels 5-12*. On www.gamepad-tester.com, it shows up as "BOMJ JOYSTICK (Vendor: 0483 Product: 5750)"<br>
Then select a bayang protocol model on your transmitter and it should bind up and go into a slower flash, before finally settling to on. In gamepad-tester.com you will see the joystick movements and aux channel switches

Channels 5-12 are digital buttons. However due to a quirk in the bayang implementation on Multiprotocol, Deviation and OpenTX (Multiprotocol module) send channels differently. On Deviation, its channels 5-12. On OpenTX its channels 5-10, and then 12 and 13. You can select which one you have in Defines.h (but unless you want all 8 channels its not really important)

Special Thanks to Garrus007 for the PPM to USB Joystick implementation which this is mostly based on (I believe it includes some of the Keil/ST sample code), and also Silverx/NFE for the Bayang implementation that i took from the Silverware flight controller project.
If there is anyone else I should be giving credit to, please let me know.
