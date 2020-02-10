# DropTheBass

Computer Engineering Senior Project for California State University, Long Beach Class of 2017.

# About

The project collects a clean digital signal and calculate the frequency. It compares the frequency of the signal to the selected pitch and controls the motors to get the desired frequency. 

# Setup

## Development Environment:
  * Keil uVision 4
  * ARMCC compiler 4.X or above
  * Texas Instrument TM4C123GHPM Launchpad

## External Circuit:
* A low pass filter is required to reduce the noise that is produced when strumming a bass guitar
* Normally the signal from the output jack is very small so an non-inverting Op-Amp is recommended to increase the amplitude
* A schmitt trigger is required to convert the analog signal to a digital signal
* 4 servo motors along with a custom gears to produced up to 204 oz/in torque
   * 6 teeth gears were attached to each motors
   * 24 teeth gears were attached to each tuning peg
   * The gear reduction of 4 provides the best results

## Hardware User Interface:
 * Four buttons for string select
 * ST7735 LCD display to see selected pitch and pitch correction indictor

