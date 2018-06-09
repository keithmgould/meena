## Introduction / Purpose

Meena is a small box containing a timer and a gonger.
Meena is meant to be placed next to a gong or singing bowl (instrument)
In conjunction Meena and the instrument create a pleasant alarm.

## Hardware overview

0. 24V 3A power supply		- required for solenoid
0. Arduino 3.3v Trinket		- brainz
0. LM2596 Buck Converter	- steps down power from 24V to 6V
0. IRF520 MOS FET 				- power supply is 24V (for
0. potentiometer					- set the timer length
0. OLED mini screen				- display the time
0. 24V 400mA solenoid			- actuate the stick

## Usage

In an effort to keep Meena's interface clean and minimal, the **only** input is a single potentiometer.
Once the timer is rotated at all, Meena resets to the new value and begins countdown.
