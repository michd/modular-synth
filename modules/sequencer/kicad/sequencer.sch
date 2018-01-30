EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:max7221
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MAX7221 U?
U 1 1 5A514C07
P 7500 2650
F 0 "U?" H 6925 2200 60  0000 C CNN
F 1 "MAX7221" H 7500 4000 60  0000 C CNN
F 2 "" H 7500 3075 60  0001 C CNN
F 3 "" H 7500 3075 60  0001 C CNN
	1    7500 2650
	1    0    0    -1  
$EndComp
$Comp
L CC56-12 AFF?
U 1 1 5A514D2A
P 9650 2400
F 0 "AFF?" H 9650 3150 50  0000 C CNN
F 1 "CC56-12" H 9650 3050 50  0000 C CNN
F 2 "" H 9150 2400 50  0000 C CNN
F 3 "" H 9150 2400 50  0000 C CNN
	1    9650 2400
	1    0    0    -1  
$EndComp
Text Notes 9300 1400 0    60   ~ 0
TODO verify display pinout\n
$Comp
L 4051 U?
U 1 1 5A514DCC
P 2000 2900
F 0 "U?" H 2100 2900 50  0000 C CNN
F 1 "4051" H 2100 2700 50  0000 C CNN
F 2 "" H 2000 2900 60  0001 C CNN
F 3 "" H 2000 2900 60  0001 C CNN
	1    2000 2900
	1    0    0    -1  
$EndComp
$Comp
L 4051 U?
U 1 1 5A514EA4
P 2000 4600
F 0 "U?" H 2100 4600 50  0000 C CNN
F 1 "4051" H 2100 4400 50  0000 C CNN
F 2 "" H 2000 4600 60  0001 C CNN
F 3 "" H 2000 4600 60  0001 C CNN
	1    2000 4600
	1    0    0    -1  
$EndComp
Text Notes 1500 3800 0    60   ~ 0
Analog multiplexers\n
$Comp
L 74LS138 U?
U 1 1 5A514FF7
P 2000 1400
F 0 "U?" H 2100 1900 50  0000 C CNN
F 1 "74LS138" H 2150 851 50  0000 C CNN
F 2 "" H 2000 1400 50  0000 C CNN
F 3 "" H 2000 1400 50  0000 C CNN
	1    2000 1400
	1    0    0    -1  
$EndComp
Text Notes 1650 800  0    60   ~ 0
3 to 8 decoder\n
Text Notes 1150 7200 0    60   ~ 0
Components to add:\n\n- MCP3202 (ADC)\n- MCP23S17 (Port expander)\n- Slide pots (import from envelope gen)\n- transistors and resistors to drive LEDs\n
$Comp
L MCP4921-E/SN U?
U 1 1 5A515273
P 2100 5850
F 0 "U?" H 1650 6150 50  0000 L CNN
F 1 "MCP4921-E/SN" H 2100 6150 50  0000 L CNN
F 2 "" H 2100 5850 50  0000 C CIN
F 3 "" H 2100 5850 50  0000 C CNN
	1    2100 5850
	1    0    0    -1  
$EndComp
$Comp
L TL072 U?
U 1 1 5A51534F
P 4000 5850
F 0 "U?" H 4000 6050 50  0000 L CNN
F 1 "TL072" H 4000 5650 50  0000 L CNN
F 2 "" H 4000 5850 50  0000 C CNN
F 3 "" H 4000 5850 50  0000 C CNN
	1    4000 5850
	1    0    0    -1  
$EndComp
$Comp
L TL072 U?
U 2 1 5A5153CC
P 5000 5850
F 0 "U?" H 5000 6050 50  0000 L CNN
F 1 "TL072" H 5000 5650 50  0000 L CNN
F 2 "" H 5000 5850 50  0000 C CNN
F 3 "" H 5000 5850 50  0000 C CNN
	2    5000 5850
	1    0    0    -1  
$EndComp
Text Notes 2100 5450 0    60   ~ 0
DAC
$EndSCHEMATC
