## Introduction / Purpose

Meena is a small box containing a timer and a striker, which is intended
to strike an instrument like a singing bowl or gong.

## Hardware overview

0. 24V 3A power supply			- required for solenoid
0. Adafruit 5v Pro Micro  	- brainz
0. LM2596 Buck Converter		- steps down power from 24V to 6V
0. IRF520 MOS FET 					- lets arduino control Solenoid
0. rotart encoder & button 	- input: set & start timer
0. OLED mini screen					- display the time
0. 24V 400mA solenoid				- actuate the striker

## Usage

In an effort to keep Meena's interface clean and minimal, the **only** input is a single knob/button.

### Setting The Timer

Once Meena is plugged in, there is a 3 second start screen, and then Meena will be in SETUP MODE.

Rotate the knob to increase/decrease the time.
Once the desired time is in place, push down on the knob to start the timer.


### Changing the Time During Countdown

If during the countdown the knob is rotated, Meena will go back into SETUP MODE. See previous 
section.

### Upon Countdown Completion

Once the time is up, the striker will strike, and the finish screen will show. Either turn the knob
or press the button to get back into SETUP MODE.
