# MIDI in module

## Printed circuit boards

### Display board

#### Dimensions

Board width: 101.6mm (4")
Board height: 63.5mm (2.5")

Screw diameter: 3mm

Screw positions from bottom left (X,Y):

  - 5.08mm, 5.08mm
  - 5.08mm, 58.42mm
  - 96.52mm, 58.42mm
  - 96.52mm, 5.08mm

Summarized, the 4 screws are offset 5.08mm (.2") from the sides.

This makes them span a rectangle of 81.28mm x 43.18mm (3.2" x 1.7")

#### Part list

- R1-16: 220 Ohm 1/8W, 5% tolerance resistor
- R17, R18: 10 KOhm 1/8W, 5% tolerance resistor
- T1: BC547C NPN Transistor
- C1: 100 nF ceramic capacitor (can be replaced with electrolytic too)
- D1, D2: TDSG5150 or equivalent Common Anode 7 segment display
- V1, V2: 74595 8-bit shift register
- SV1: Standard .100" pitch pin header, or leave empty to solder wires directly to board


#### Description
  
The display board houses two 7-segment LED displays, to display the currently selected MIDI channel.

The shift registers on the board keep hold of the digits being displayed, and are fed data by the microcontroller on the main board.

R1-16 are current-limiting resistors for each of the 8 LEDs per display.

The transistor circuit connected to the shift registers' reset pins causes the registers to reset when the circuit is being powered up. The registers' reset pin needs to be held low (0V) in order to reset. On power-up, C1 has no charge. A current will flow "through" C1 and R18 (10K), causing a voltage drop across R18. Having the voltage drop across R18 brings the point connected to the reset pins (junction between R18 and C1) to 0V, resetting the shift registers. Once the capacitor is charged enough, the voltage drop has shifted to be across C1, and the reset pins are back to 5V, meaning the registers are no longer being reset. Normal operation can begin.

[Fun fact: while writing that power-up description I noticed I designed it wrong in the schematic: I'd be shorting VCC to GND through T1, blowing up the transistor. Documenting is good.]
