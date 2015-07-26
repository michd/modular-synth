# Modular Synth Power Supply

This power supply is meant to power the whole thing. I threw some arbitrary and not so arbitrary numbers at what I want from it.

## Requirements:

- Symmetrical supply (audio needs positive and negative voltages)
- Voltages: -5V and +5V (5V seems like a good number to also power microcontrollers and various ICs)
- Max output current: 5A on each side, so a combined 10A. (This probably massively overshoots what will actually be required, given that actual audio power amplification for speakers is not in the scope of the project. Better to have more available than too little, though.)

## Parts

### Parts on PCB

The part labels here refer to parts as seen on the schematic (power-supply.sch or power-supply.sch.png)

- J1, J2: 3-way PCB mount terminal block; 3.5mm pin pitch
- B1: GBPC1506W Rectifier bridge
- C1, C2: Electrolytic capacitor, rated at 4700uF, 16V
- C3, C4: Electrolyic capacitor, rated at 0.33uF, 16V (or more)
- C5, C6: Capacitor, 0.1uF
- R1, R2: Resistor, 3 Ohms, 5% tolerance or better, 3W
- IC1: LM7805 - Positive linear voltage regulator
- IC2: LM7905 - Negative linear voltage regulator
- Q1: MJL1302A - High power PNP transistor (massive amount of headroom on this one)
- Q2: MJL3281A - High power NPN transistor (massive amount of headroom on this one)

### External parts (not on schematic)

- TF1: Transformer, 50VA or higher, dual output transformer (6V per output)
- J3: Male panel-mount IEC connector, optionally with a power switch, preferrably with built-in fuse

## Wiring

TF1's two outputs should be connected in series. The connecting midpoint should be connected to J1-2 on the PCB (TF Mid), the two other remaining secondary pins of the transformer should be connected to J1-1 and J1-2.

The primary winding of TF1 should be connected to the neutral and live terminals of the IEC connector.

J2 should be wired to the array of banana sockets to be built into the synth enclosure. Preferable colouring: +5V = red, 0V (GND) = black, -5V = blue. I will probably put them left to right: blue, black, red.

[TODO: add a wiring diagram image here]

## Acknowledgements

The method for allowing more current while using 78XX and 79XX (each of which normally delivers 1-1.5A max) was taken from the Fairchild 78xx datasheet. ([Datasheet](http://pdf.datasheetcatalog.com/datasheets/228/390068_DS.pdf) - Page 23, Figure 12)

This method was also [suggested](https://www.reddit.com/r/AskElectronics/comments/3ef350/is_there_a_negative_equivalent_of_the_lm338/ctecdds) by reddit user "Linker3000".

The transistors Q1 and Q2 (MJL1302A and MJL3281A) were [suggested](https://www.reddit.com/r/AskElectronics/comments/3ef350/is_there_a_negative_equivalent_of_the_lm338/ctei40j) by reddit user "fatangaboo".

Thanks, r/AskElectronics!
