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
LIBS:distortion-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Foldback Distortion"
Date "2017-07-31"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TL074 U2
U 2 1 597DD56C
P 3350 2800
F 0 "U2" H 3350 3000 50  0000 L CNN
F 1 "TL974" H 3400 2650 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 3300 2900 50  0001 C CNN
F 3 "" H 3400 3000 50  0000 C CNN
	2    3350 2800
	1    0    0    1   
$EndComp
$Comp
L TL074 U2
U 3 1 597DD651
P 3350 3600
F 0 "U2" H 3350 3800 50  0000 L CNN
F 1 "TL974" H 3350 3400 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 3300 3700 50  0001 C CNN
F 3 "" H 3400 3800 50  0000 C CNN
	3    3350 3600
	1    0    0    1   
$EndComp
$Comp
L TL074 U2
U 1 1 597DD6DE
P 2450 1400
F 0 "U2" H 2450 1600 50  0000 L CNN
F 1 "TL974" H 2450 1200 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 2400 1500 50  0001 C CNN
F 3 "" H 2500 1600 50  0000 C CNN
	1    2450 1400
	1    0    0    1   
$EndComp
$Comp
L TL074 U2
U 4 1 597DD798
P 3600 1500
F 0 "U2" H 3600 1700 50  0000 L CNN
F 1 "TL974" H 3600 1300 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 3550 1600 50  0001 C CNN
F 3 "" H 3650 1700 50  0000 C CNN
	4    3600 1500
	1    0    0    1   
$EndComp
$Comp
L CONN_01X03 P1
U 1 1 597DDA91
P 1100 5300
F 0 "P1" H 1100 5500 50  0000 C CNN
F 1 "CONN_01X03" V 1200 5300 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-03_03x2.54mm_Straight" H 1100 5300 50  0001 C CNN
F 3 "" H 1100 5300 50  0000 C CNN
	1    1100 5300
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P2
U 1 1 597DDBD0
P 3350 5300
F 0 "P2" H 3350 5500 50  0000 C CNN
F 1 "CONN_01X03" V 3450 5300 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-03_03x2.54mm_Straight" H 3350 5300 50  0001 C CNN
F 3 "" H 3350 5300 50  0000 C CNN
	1    3350 5300
	-1   0    0    1   
$EndComp
$Comp
L TL074 U3
U 4 1 597DDDFF
P 5550 1400
F 0 "U3" H 5550 1600 50  0000 L CNN
F 1 "TL074" H 5550 1200 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 5500 1500 50  0001 C CNN
F 3 "" H 5600 1600 50  0000 C CNN
	4    5550 1400
	1    0    0    1   
$EndComp
$Comp
L TL074 U3
U 1 1 597DDE8E
P 7900 2400
F 0 "U3" H 7900 2600 50  0000 L CNN
F 1 "TL074" H 7900 2200 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 7850 2500 50  0001 C CNN
F 3 "" H 7950 2600 50  0000 C CNN
	1    7900 2400
	1    0    0    -1  
$EndComp
$Comp
L TL074 U3
U 2 1 597DDEF7
P 8850 3600
F 0 "U3" H 8850 3800 50  0000 L CNN
F 1 "TL074" H 8850 3400 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 8800 3700 50  0001 C CNN
F 3 "" H 8900 3800 50  0000 C CNN
	2    8850 3600
	1    0    0    1   
$EndComp
$Comp
L TL074 U3
U 3 1 597DDF56
P 5550 2300
F 0 "U3" H 5550 2500 50  0000 L CNN
F 1 "TL074" H 5550 2100 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 5500 2400 50  0001 C CNN
F 3 "" H 5600 2500 50  0000 C CNN
	3    5550 2300
	1    0    0    1   
$EndComp
$Comp
L 4066 U4
U 1 1 597DDFDB
P 6600 1400
F 0 "U4" H 6800 1251 50  0000 C CNN
F 1 "4066" H 6800 1550 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 6600 1400 60  0001 C CNN
F 3 "" H 6600 1400 60  0001 C CNN
	1    6600 1400
	1    0    0    -1  
$EndComp
$Comp
L 4066 U4
U 3 1 597DE030
P 6950 5200
F 0 "U4" H 7150 5051 50  0000 C CNN
F 1 "4066" H 7150 5350 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 6950 5200 60  0001 C CNN
F 3 "" H 6950 5200 60  0001 C CNN
	3    6950 5200
	1    0    0    -1  
$EndComp
$Comp
L 4066 U4
U 2 1 597DE081
P 6600 2300
F 0 "U4" H 6800 2151 50  0000 C CNN
F 1 "4066" H 6800 2450 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 6600 2300 60  0001 C CNN
F 3 "" H 6600 2300 60  0001 C CNN
	2    6600 2300
	1    0    0    -1  
$EndComp
$Comp
L 4066 U4
U 4 1 597DE116
P 8000 5200
F 0 "U4" H 8200 5051 50  0000 C CNN
F 1 "4066" H 8200 5350 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket_LongPads" H 8000 5200 60  0001 C CNN
F 3 "" H 8000 5200 60  0001 C CNN
	4    8000 5200
	1    0    0    -1  
$EndComp
Text GLabel 1500 5100 2    60   Input ~ 0
SignalIn
Text GLabel 1500 5300 2    60   Input ~ 0
ThresholdIn
Text GLabel 1500 5500 2    60   Input ~ 0
SignalOut
$Comp
L POT RV2
U 1 1 597F8DD1
P 750 1000
F 0 "RV2" H 750 920 50  0000 C CNN
F 1 "100k" H 750 1000 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Bourns_PTV09A-1_Horizontal" H 750 1000 50  0001 C CNN
F 3 "" H 750 1000 50  0000 C CNN
	1    750  1000
	0    1    1    0   
$EndComp
Text Notes 600  1250 1    60   ~ 0
THRESHOLD
$Comp
L R R3
U 1 1 597F98C1
P 1100 1000
F 0 "R3" V 1180 1000 50  0000 C CNN
F 1 "75k" V 1100 1000 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1030 1000 50  0001 C CNN
F 3 "" H 1100 1000 50  0000 C CNN
	1    1100 1000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR01
U 1 1 597F993E
P 2100 1550
F 0 "#PWR01" H 2100 1300 50  0001 C CNN
F 1 "GND" H 2100 1400 50  0000 C CNN
F 2 "" H 2100 1550 50  0000 C CNN
F 3 "" H 2100 1550 50  0000 C CNN
	1    2100 1550
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 597F99CB
P 1050 1650
F 0 "R4" V 1130 1650 50  0000 C CNN
F 1 "100k" V 1050 1650 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 980 1650 50  0001 C CNN
F 3 "" H 1050 1650 50  0000 C CNN
	1    1050 1650
	0    1    1    0   
$EndComp
$Comp
L POT RV3
U 1 1 597FA99E
P 700 1650
F 0 "RV3" H 700 1570 50  0000 C CNN
F 1 "100k" H 700 1650 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Bourns_PTV09A-1_Horizontal" H 700 1650 50  0001 C CNN
F 3 "" H 700 1650 50  0000 C CNN
	1    700  1650
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR02
U 1 1 597FADB7
P 1850 1800
F 0 "#PWR02" H 1850 1550 50  0001 C CNN
F 1 "GND" H 1850 1650 50  0000 C CNN
F 2 "" H 1850 1800 50  0000 C CNN
F 3 "" H 1850 1800 50  0000 C CNN
	1    1850 1800
	1    0    0    -1  
$EndComp
Text GLabel 2000 1600 3    60   Input ~ 0
ThresholdIn
$Comp
L R R5
U 1 1 597FC1C5
P 2300 1000
F 0 "R5" V 2380 1000 50  0000 C CNN
F 1 "47k" V 2300 1000 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2230 1000 50  0001 C CNN
F 3 "" H 2300 1000 50  0000 C CNN
	1    2300 1000
	0    1    1    0   
$EndComp
$Comp
L R R8
U 1 1 597FC58F
P 3450 1050
F 0 "R8" V 3530 1050 50  0000 C CNN
F 1 "100k" V 3450 1050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3380 1050 50  0001 C CNN
F 3 "" H 3450 1050 50  0000 C CNN
	1    3450 1050
	0    1    1    0   
$EndComp
$Comp
L R R7
U 1 1 597FC683
P 3050 1400
F 0 "R7" V 3130 1400 50  0000 C CNN
F 1 "100k" V 3050 1400 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2980 1400 50  0001 C CNN
F 3 "" H 3050 1400 50  0000 C CNN
	1    3050 1400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR03
U 1 1 597FC767
P 3250 1650
F 0 "#PWR03" H 3250 1400 50  0001 C CNN
F 1 "GND" H 3250 1500 50  0000 C CNN
F 2 "" H 3250 1650 50  0000 C CNN
F 3 "" H 3250 1650 50  0000 C CNN
	1    3250 1650
	1    0    0    -1  
$EndComp
$Comp
L VEE #PWR04
U 1 1 59800C8A
P 3100 2400
F 0 "#PWR04" H 3100 2250 50  0001 C CNN
F 1 "VEE" H 3100 2550 50  0000 C CNN
F 2 "" H 3100 2400 50  0000 C CNN
F 3 "" H 3100 2400 50  0000 C CNN
	1    3100 2400
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR05
U 1 1 598017B6
P 3100 4000
F 0 "#PWR05" H 3100 3850 50  0001 C CNN
F 1 "VCC" H 3100 4150 50  0000 C CNN
F 2 "" H 3100 4000 50  0000 C CNN
F 3 "" H 3100 4000 50  0000 C CNN
	1    3100 4000
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 598023C8
P 2500 3200
F 0 "R6" V 2580 3200 50  0000 C CNN
F 1 "100k" V 2500 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2430 3200 50  0001 C CNN
F 3 "" H 2500 3200 50  0000 C CNN
	1    2500 3200
	0    1    1    0   
$EndComp
$Comp
L D D1
U 1 1 5980721B
P 3850 2800
F 0 "D1" H 3850 2900 50  0000 C CNN
F 1 "D" H 3850 2700 50  0000 C CNN
F 2 "Diodes_ThroughHole:D_DO-35_SOD27_P7.62mm_Horizontal" H 3850 2800 50  0001 C CNN
F 3 "" H 3850 2800 50  0000 C CNN
	1    3850 2800
	-1   0    0    1   
$EndComp
$Comp
L D D2
U 1 1 5980736E
P 3850 3600
F 0 "D2" H 3850 3700 50  0000 C CNN
F 1 "D" H 3850 3500 50  0000 C CNN
F 2 "Diodes_ThroughHole:D_DO-35_SOD27_P7.62mm_Horizontal" H 3850 3600 50  0001 C CNN
F 3 "" H 3850 3600 50  0000 C CNN
	1    3850 3600
	-1   0    0    1   
$EndComp
$Comp
L R R10
U 1 1 59807488
P 4050 3400
F 0 "R10" V 4130 3400 50  0000 C CNN
F 1 "100k" V 4050 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3980 3400 50  0001 C CNN
F 3 "" H 4050 3400 50  0000 C CNN
	1    4050 3400
	-1   0    0    1   
$EndComp
$Comp
L R R9
U 1 1 59809155
P 4050 3000
F 0 "R9" V 4130 3000 50  0000 C CNN
F 1 "100k" V 4050 3000 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3980 3000 50  0001 C CNN
F 3 "" H 4050 3000 50  0000 C CNN
	1    4050 3000
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 59809530
P 4200 3250
F 0 "#PWR06" H 4200 3000 50  0001 C CNN
F 1 "GND" H 4200 3100 50  0000 C CNN
F 2 "" H 4200 3250 50  0000 C CNN
F 3 "" H 4200 3250 50  0000 C CNN
	1    4200 3250
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 5980CC1D
P 5400 1000
F 0 "R15" V 5480 1000 50  0000 C CNN
F 1 "180k" V 5400 1000 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5330 1000 50  0001 C CNN
F 3 "" H 5400 1000 50  0000 C CNN
	1    5400 1000
	0    1    1    0   
$EndComp
$Comp
L R R11
U 1 1 5980CE0A
P 4500 850
F 0 "R11" V 4580 850 50  0000 C CNN
F 1 "47k" V 4500 850 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4430 850 50  0001 C CNN
F 3 "" H 4500 850 50  0000 C CNN
	1    4500 850 
	0    1    1    0   
$EndComp
$Comp
L R R12
U 1 1 5980CEC1
P 5000 1500
F 0 "R12" V 5080 1500 50  0000 C CNN
F 1 "100k" V 5000 1500 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4930 1500 50  0001 C CNN
F 3 "" H 5000 1500 50  0000 C CNN
	1    5000 1500
	0    1    1    0   
$EndComp
$Comp
L R R16
U 1 1 5980DF85
P 5400 1900
F 0 "R16" V 5480 1900 50  0000 C CNN
F 1 "180k" V 5400 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5330 1900 50  0001 C CNN
F 3 "" H 5400 1900 50  0000 C CNN
	1    5400 1900
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 5980E1EB
P 4500 1050
F 0 "R13" V 4580 1050 50  0000 C CNN
F 1 "47k" V 4500 1050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4430 1050 50  0001 C CNN
F 3 "" H 4500 1050 50  0000 C CNN
	1    4500 1050
	0    1    1    0   
$EndComp
$Comp
L R R14
U 1 1 5980E2FD
P 5000 2400
F 0 "R14" V 5080 2400 50  0000 C CNN
F 1 "100k" V 5000 2400 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4930 2400 50  0001 C CNN
F 3 "" H 5000 2400 50  0000 C CNN
	1    5000 2400
	0    1    1    0   
$EndComp
$Comp
L POT RV4
U 1 1 5980F16C
P 4100 950
F 0 "RV4" H 4100 870 50  0000 C CNN
F 1 "100k" H 4100 950 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Bourns_PTV09A-1_Horizontal" H 4100 950 50  0001 C CNN
F 3 "" H 4100 950 50  0000 C CNN
	1    4100 950 
	0    1    -1   0   
$EndComp
Text Notes 4500 700  2    60   ~ 0
SYMMETRY
$Comp
L GND #PWR07
U 1 1 598173AB
P 6600 2600
F 0 "#PWR07" H 6600 2350 50  0001 C CNN
F 1 "GND" H 6600 2450 50  0000 C CNN
F 2 "" H 6600 2600 50  0000 C CNN
F 3 "" H 6600 2600 50  0000 C CNN
	1    6600 2600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR08
U 1 1 5981801C
P 6600 1100
F 0 "#PWR08" H 6600 950 50  0001 C CNN
F 1 "VCC" H 6600 1250 50  0000 C CNN
F 2 "" H 6600 1100 50  0000 C CNN
F 3 "" H 6600 1100 50  0000 C CNN
	1    6600 1100
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 5981A98F
P 7600 1350
F 0 "R17" V 7680 1350 50  0000 C CNN
F 1 "47k" V 7600 1350 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7530 1350 50  0001 C CNN
F 3 "" H 7600 1350 50  0000 C CNN
	1    7600 1350
	0    1    1    0   
$EndComp
$Comp
L R R18
U 1 1 5981BE62
P 7600 1550
F 0 "R18" V 7680 1550 50  0000 C CNN
F 1 "47k" V 7600 1550 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7530 1550 50  0001 C CNN
F 3 "" H 7600 1550 50  0000 C CNN
	1    7600 1550
	0    1    1    0   
$EndComp
$Comp
L POT RV5
U 1 1 5981E24A
P 7250 1450
F 0 "RV5" H 7250 1370 50  0000 C CNN
F 1 "100k" H 7250 1450 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Bourns_PTV09A-1_Horizontal" H 7250 1450 50  0001 C CNN
F 3 "" H 7250 1450 50  0000 C CNN
	1    7250 1450
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR09
U 1 1 59823181
P 7800 2050
F 0 "#PWR09" H 7800 1900 50  0001 C CNN
F 1 "VCC" H 7800 2200 50  0000 C CNN
F 2 "" H 7800 2050 50  0000 C CNN
F 3 "" H 7800 2050 50  0000 C CNN
	1    7800 2050
	1    0    0    -1  
$EndComp
$Comp
L VEE #PWR010
U 1 1 598239B4
P 7800 2750
F 0 "#PWR010" H 7800 2600 50  0001 C CNN
F 1 "VEE" H 7800 2900 50  0000 C CNN
F 2 "" H 7800 2750 50  0000 C CNN
F 3 "" H 7800 2750 50  0000 C CNN
	1    7800 2750
	-1   0    0    1   
$EndComp
Text Notes 7100 1600 1    60   ~ 0
DEPTH
$Comp
L R R20
U 1 1 598277F5
P 8700 3200
F 0 "R20" V 8780 3200 50  0000 C CNN
F 1 "330k" V 8700 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8630 3200 50  0001 C CNN
F 3 "" H 8700 3200 50  0000 C CNN
	1    8700 3200
	0    1    1    0   
$EndComp
$Comp
L POT RV6
U 1 1 5982796D
P 6850 3500
F 0 "RV6" H 6850 3420 50  0000 C CNN
F 1 "100k" H 6850 3500 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Bourns_PTV09A-1_Horizontal" H 6850 3500 50  0001 C CNN
F 3 "" H 6850 3500 50  0000 C CNN
	1    6850 3500
	0    1    -1   0   
$EndComp
$Comp
L R R19
U 1 1 59827C91
P 7200 3500
F 0 "R19" V 7280 3500 50  0000 C CNN
F 1 "100k" V 7200 3500 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7130 3500 50  0001 C CNN
F 3 "" H 7200 3500 50  0000 C CNN
	1    7200 3500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR011
U 1 1 5982811E
P 8500 3750
F 0 "#PWR011" H 8500 3500 50  0001 C CNN
F 1 "GND" H 8500 3600 50  0000 C CNN
F 2 "" H 8500 3750 50  0000 C CNN
F 3 "" H 8500 3750 50  0000 C CNN
	1    8500 3750
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 59829951
P 9400 3600
F 0 "C10" H 9425 3700 50  0000 L CNN
F 1 "1u" H 9425 3500 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 9438 3450 50  0001 C CNN
F 3 "" H 9400 3600 50  0000 C CNN
	1    9400 3600
	0    -1   -1   0   
$EndComp
$Comp
L R R21
U 1 1 59829F0F
P 9750 3600
F 0 "R21" V 9830 3600 50  0000 C CNN
F 1 "1k" V 9750 3600 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9680 3600 50  0001 C CNN
F 3 "" H 9750 3600 50  0000 C CNN
	1    9750 3600
	0    1    1    0   
$EndComp
Text GLabel 9950 3600 2    60   Output ~ 0
SignalOut
$Comp
L TL072 U1
U 1 1 5982C971
P 1950 3200
F 0 "U1" H 1950 3400 50  0000 L CNN
F 1 "TL072" H 1950 3000 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 1950 3200 50  0001 C CNN
F 3 "" H 1950 3200 50  0000 C CNN
	1    1950 3200
	1    0    0    1   
$EndComp
$Comp
L R R2
U 1 1 5982CF1F
P 1800 2750
F 0 "R2" V 1880 2750 50  0000 C CNN
F 1 "330k" V 1800 2750 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1730 2750 50  0001 C CNN
F 3 "" H 1800 2750 50  0000 C CNN
	1    1800 2750
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 5982CFE0
P 1000 3750
F 0 "R1" V 1080 3750 50  0000 C CNN
F 1 "100k" V 1000 3750 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 930 3750 50  0001 C CNN
F 3 "" H 1000 3750 50  0000 C CNN
	1    1000 3750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR012
U 1 1 5982D3DB
P 1600 3350
F 0 "#PWR012" H 1600 3100 50  0001 C CNN
F 1 "GND" H 1450 3300 50  0000 C CNN
F 2 "" H 1600 3350 50  0000 C CNN
F 3 "" H 1600 3350 50  0000 C CNN
	1    1600 3350
	1    0    0    -1  
$EndComp
$Comp
L POT RV1
U 1 1 5982DC10
P 650 3750
F 0 "RV1" H 650 3670 50  0000 C CNN
F 1 "100k" H 650 3750 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Bourns_PTV09A-1_Horizontal" H 650 3750 50  0001 C CNN
F 3 "" H 650 3750 50  0000 C CNN
	1    650  3750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR013
U 1 1 5982DFBD
P 1250 3300
F 0 "#PWR013" H 1250 3050 50  0001 C CNN
F 1 "GND" H 1250 3150 50  0000 C CNN
F 2 "" H 1250 3300 50  0000 C CNN
F 3 "" H 1250 3300 50  0000 C CNN
	1    1250 3300
	1    0    0    -1  
$EndComp
Text GLabel 1250 2900 1    60   Input ~ 0
SignalIn
$Comp
L VEE #PWR014
U 1 1 5982FF8D
P 9300 5350
F 0 "#PWR014" H 9300 5200 50  0001 C CNN
F 1 "VEE" H 9300 5500 50  0000 C CNN
F 2 "" H 9300 5350 50  0000 C CNN
F 3 "" H 9300 5350 50  0000 C CNN
	1    9300 5350
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR015
U 1 1 59830D34
P 9300 4650
F 0 "#PWR015" H 9300 4500 50  0001 C CNN
F 1 "VCC" H 9300 4800 50  0000 C CNN
F 2 "" H 9300 4650 50  0000 C CNN
F 3 "" H 9300 4650 50  0000 C CNN
	1    9300 4650
	1    0    0    -1  
$EndComp
Text Notes 6800 3850 0    60   ~ 0
POST GAIN
Text Notes 600  4100 0    60   ~ 0
PRE GAIN
$Comp
L TL072 U1
U 2 1 59837B2F
P 9400 5000
F 0 "U1" H 9400 5200 50  0000 L CNN
F 1 "TL072" H 9400 4800 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 9400 5000 50  0001 C CNN
F 3 "" H 9400 5000 50  0000 C CNN
	2    9400 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5983B2CF
P 8900 4950
F 0 "#PWR016" H 8900 4700 50  0001 C CNN
F 1 "GND" H 8750 4900 50  0000 C CNN
F 2 "" H 8900 4950 50  0000 C CNN
F 3 "" H 8900 4950 50  0000 C CNN
	1    8900 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 5983E107
P 7650 5400
F 0 "#PWR017" H 7650 5150 50  0001 C CNN
F 1 "GND" H 7500 5350 50  0000 C CNN
F 2 "" H 7650 5400 50  0000 C CNN
F 3 "" H 7650 5400 50  0000 C CNN
	1    7650 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5983E279
P 6600 5400
F 0 "#PWR018" H 6600 5150 50  0001 C CNN
F 1 "GND" H 6450 5350 50  0000 C CNN
F 2 "" H 6600 5400 50  0000 C CNN
F 3 "" H 6600 5400 50  0000 C CNN
	1    6600 5400
	1    0    0    -1  
$EndComp
$Comp
L CP C3
U 1 1 598444DD
P 4500 5100
F 0 "C3" H 4525 5200 50  0000 L CNN
F 1 "10u" H 4525 5000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 4538 4950 50  0001 C CNN
F 3 "" H 4500 5100 50  0000 C CNN
	1    4500 5100
	1    0    0    -1  
$EndComp
$Comp
L CP C4
U 1 1 59844B98
P 4500 5500
F 0 "C4" H 4525 5600 50  0000 L CNN
F 1 "10u" H 4525 5400 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 4538 5350 50  0001 C CNN
F 3 "" H 4500 5500 50  0000 C CNN
	1    4500 5500
	1    0    0    -1  
$EndComp
$Comp
L CP C5
U 1 1 59845848
P 4800 5100
F 0 "C5" H 4825 5200 50  0000 L CNN
F 1 "10u" H 4825 5000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 4838 4950 50  0001 C CNN
F 3 "" H 4800 5100 50  0000 C CNN
	1    4800 5100
	1    0    0    -1  
$EndComp
$Comp
L CP C6
U 1 1 598458F5
P 4800 5500
F 0 "C6" H 4825 5600 50  0000 L CNN
F 1 "10u" H 4825 5400 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 4838 5350 50  0001 C CNN
F 3 "" H 4800 5500 50  0000 C CNN
	1    4800 5500
	1    0    0    -1  
$EndComp
$Comp
L CP C7
U 1 1 598459A5
P 5100 5100
F 0 "C7" H 5125 5200 50  0000 L CNN
F 1 "10u" H 5125 5000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 5138 4950 50  0001 C CNN
F 3 "" H 5100 5100 50  0000 C CNN
	1    5100 5100
	1    0    0    -1  
$EndComp
$Comp
L CP C8
U 1 1 59845B6B
P 5100 5500
F 0 "C8" H 5125 5600 50  0000 L CNN
F 1 "10u" H 5125 5400 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 5138 5350 50  0001 C CNN
F 3 "" H 5100 5500 50  0000 C CNN
	1    5100 5500
	1    0    0    -1  
$EndComp
$Comp
L CP C9
U 1 1 5984768F
P 5400 5100
F 0 "C9" H 5425 5200 50  0000 L CNN
F 1 "10u" H 5425 5000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D4.0mm_P1.50mm" H 5438 4950 50  0001 C CNN
F 3 "" H 5400 5100 50  0000 C CNN
	1    5400 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5984999C
P 3950 5350
F 0 "#PWR019" H 3950 5100 50  0001 C CNN
F 1 "GND" H 3950 5200 50  0000 C CNN
F 2 "" H 3950 5350 50  0000 C CNN
F 3 "" H 3950 5350 50  0000 C CNN
	1    3950 5350
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG020
U 1 1 59849F25
P 3950 5250
F 0 "#FLG020" H 3950 5345 50  0001 C CNN
F 1 "PWR_FLAG" V 3850 5400 50  0000 C CNN
F 2 "" H 3950 5250 50  0000 C CNN
F 3 "" H 3950 5250 50  0000 C CNN
	1    3950 5250
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG021
U 1 1 5984A0AB
P 3700 4900
F 0 "#FLG021" H 3700 4995 50  0001 C CNN
F 1 "PWR_FLAG" H 3700 5080 50  0000 C CNN
F 2 "" H 3700 4900 50  0000 C CNN
F 3 "" H 3700 4900 50  0000 C CNN
	1    3700 4900
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG022
U 1 1 5984A29A
P 3700 5700
F 0 "#FLG022" H 3700 5795 50  0001 C CNN
F 1 "PWR_FLAG" H 3700 5880 50  0000 C CNN
F 2 "" H 3700 5700 50  0000 C CNN
F 3 "" H 3700 5700 50  0000 C CNN
	1    3700 5700
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR023
U 1 1 5984BAE0
P 3750 4800
F 0 "#PWR023" H 3750 4650 50  0001 C CNN
F 1 "VCC" H 3750 4950 50  0000 C CNN
F 2 "" H 3750 4800 50  0000 C CNN
F 3 "" H 3750 4800 50  0000 C CNN
	1    3750 4800
	1    0    0    -1  
$EndComp
$Comp
L VEE #PWR024
U 1 1 5984CA09
P 3750 5750
F 0 "#PWR024" H 3750 5600 50  0001 C CNN
F 1 "VEE" H 3750 5900 50  0000 C CNN
F 2 "" H 3750 5750 50  0000 C CNN
F 3 "" H 3750 5750 50  0000 C CNN
	1    3750 5750
	-1   0    0    1   
$EndComp
$Comp
L CP C2
U 1 1 5984F2D1
P 4200 5500
F 0 "C2" H 4225 5600 50  0000 L CNN
F 1 "470u" H 4225 5400 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D8.0mm_P3.50mm" H 4238 5350 50  0001 C CNN
F 3 "" H 4200 5500 50  0000 C CNN
	1    4200 5500
	1    0    0    -1  
$EndComp
$Comp
L CP C1
U 1 1 5984F393
P 4200 5100
F 0 "C1" H 4225 5200 50  0000 L CNN
F 1 "470u" H 4225 5000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D8.0mm_P3.50mm" H 4238 4950 50  0001 C CNN
F 3 "" H 4200 5100 50  0000 C CNN
	1    4200 5100
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P8
U 1 1 5986028C
P 1800 1000
F 0 "P8" H 1800 1200 50  0000 C CNN
F 1 "CONN_01X03" V 1900 1000 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 1800 1000 50  0001 C CNN
F 3 "" H 1800 1000 50  0000 C CNN
	1    1800 1000
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR025
U 1 1 59861315
P 2050 850
F 0 "#PWR025" H 2050 700 50  0001 C CNN
F 1 "VCC" H 2050 1000 50  0000 C CNN
F 2 "" H 2050 850 50  0000 C CNN
F 3 "" H 2050 850 50  0000 C CNN
	1    2050 850 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 59861581
P 2050 1150
F 0 "#PWR026" H 2050 900 50  0001 C CNN
F 1 "GND" H 1900 1100 50  0000 C CNN
F 2 "" H 2050 1150 50  0000 C CNN
F 3 "" H 2050 1150 50  0000 C CNN
	1    2050 1150
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P7
U 1 1 59865D3F
P 1650 1650
F 0 "P7" H 1650 1850 50  0000 C CNN
F 1 "CONN_01X03" V 1750 1650 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 1650 1650 50  0001 C CNN
F 3 "" H 1650 1650 50  0000 C CNN
	1    1650 1650
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X03 P5
U 1 1 59865E23
P 1450 1650
F 0 "P5" H 1450 1850 50  0000 C CNN
F 1 "CONN_01X03" V 1550 1650 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 1450 1650 50  0001 C CNN
F 3 "" H 1450 1650 50  0000 C CNN
	1    1450 1650
	1    0    0    -1  
$EndComp
Text Notes 700  2250 1    60   ~ 0
THRESHOLD\nDEPTH
$Comp
L CONN_01X03 P3
U 1 1 598690DE
P 1000 3100
F 0 "P3" H 1000 3300 50  0000 C CNN
F 1 "CONN_01X03" V 1100 3100 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 1000 3100 50  0001 C CNN
F 3 "" H 1000 3100 50  0000 C CNN
	1    1000 3100
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 598691E4
P 1400 3750
F 0 "P4" H 1400 3950 50  0000 C CNN
F 1 "CONN_01X03" V 1500 3750 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 1400 3750 50  0001 C CNN
F 3 "" H 1400 3750 50  0000 C CNN
	1    1400 3750
	1    0    0    1   
$EndComp
$Comp
L CONN_01X03 P10
U 1 1 5986E3F5
P 4550 1800
F 0 "P10" H 4550 2000 50  0000 C CNN
F 1 "CONN_01X03" V 4650 1800 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 4550 1800 50  0001 C CNN
F 3 "" H 4550 1800 50  0000 C CNN
	1    4550 1800
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X03 P9
U 1 1 5986F016
P 4950 950
F 0 "P9" H 4950 1150 50  0000 C CNN
F 1 "CONN_01X03" V 5050 950 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 4950 950 50  0001 C CNN
F 3 "" H 4950 950 50  0000 C CNN
	1    4950 950 
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P14
U 1 1 598742B2
P 7900 3500
F 0 "P14" H 7900 3700 50  0000 C CNN
F 1 "CONN_01X03" V 8000 3500 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 7900 3500 50  0001 C CNN
F 3 "" H 7900 3500 50  0000 C CNN
	1    7900 3500
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 598745F5
P 8150 3650
F 0 "#PWR027" H 8150 3400 50  0001 C CNN
F 1 "GND" H 8150 3500 50  0000 C CNN
F 2 "" H 8150 3650 50  0000 C CNN
F 3 "" H 8150 3650 50  0000 C CNN
	1    8150 3650
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P12
U 1 1 59874E93
P 7600 3500
F 0 "P12" H 7600 3700 50  0000 C CNN
F 1 "CONN_01X03" V 7700 3500 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 7600 3500 50  0001 C CNN
F 3 "" H 7600 3500 50  0000 C CNN
	1    7600 3500
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P11
U 1 1 59879C10
P 7150 2300
F 0 "P11" H 7150 2500 50  0000 C CNN
F 1 "CONN_01X03" V 7250 2300 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 7150 2300 50  0001 C CNN
F 3 "" H 7150 2300 50  0000 C CNN
	1    7150 2300
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P13
U 1 1 5987ADAE
P 8000 1450
F 0 "P13" H 8000 1650 50  0000 C CNN
F 1 "CONN_01X03" V 8100 1450 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 8000 1450 50  0001 C CNN
F 3 "" H 8000 1450 50  0000 C CNN
	1    8000 1450
	1    0    0    1   
$EndComp
Wire Wire Line
	7400 1450 7800 1450
Wire Wire Line
	7400 2400 7350 2400
Wire Wire Line
	7350 2300 7600 2300
Wire Wire Line
	7350 2200 7400 2200
Wire Wire Line
	1200 3850 750  3850
Wire Wire Line
	1200 3650 750  3650
Wire Wire Line
	1250 3200 1200 3200
Wire Wire Line
	1200 3100 1650 3100
Wire Wire Line
	1250 3000 1200 3000
Wire Wire Line
	950  1100 950  1200
Wire Wire Line
	7250 1650 7250 1600
Wire Wire Line
	7400 1650 7250 1650
Wire Wire Line
	7400 1550 7400 1650
Wire Wire Line
	7250 1250 7250 1300
Wire Wire Line
	7400 1250 7250 1250
Wire Wire Line
	7400 1350 7400 1250
Wire Wire Line
	7400 2200 7400 1850
Wire Wire Line
	7400 2400 7400 3050
Wire Wire Line
	6500 3050 6500 4200
Wire Wire Line
	7400 3050 6500 3050
Wire Wire Line
	7550 2500 7550 3000
Wire Wire Line
	7000 3700 7000 3600
Wire Wire Line
	6850 3700 7000 3700
Wire Wire Line
	7000 3300 6850 3300
Wire Wire Line
	7000 3400 7000 3300
Wire Wire Line
	8150 3600 8150 3650
Wire Wire Line
	8100 3600 8150 3600
Connection ~ 8250 3000
Wire Wire Line
	8250 3400 8100 3400
Wire Wire Line
	4250 950  4750 950 
Wire Wire Line
	4250 750  4250 850 
Wire Wire Line
	4100 750  4250 750 
Wire Wire Line
	4100 800  4100 750 
Wire Wire Line
	4100 1150 4100 1100
Wire Wire Line
	4200 1150 4100 1150
Wire Wire Line
	4200 1050 4200 1150
Wire Wire Line
	4800 2200 5250 2200
Wire Wire Line
	4850 1800 4850 2050
Wire Wire Line
	4750 1800 4850 1800
Wire Wire Line
	4800 1700 4800 1300
Wire Wire Line
	4750 1700 4800 1700
Wire Wire Line
	4800 1900 4800 2200
Wire Wire Line
	4750 1900 4800 1900
Wire Wire Line
	650  3900 650  4000
Wire Wire Line
	750  3950 650  3950
Wire Wire Line
	750  3850 750  3950
Wire Wire Line
	650  3550 650  3600
Wire Wire Line
	750  3550 650  3550
Wire Wire Line
	750  3650 750  3550
Wire Wire Line
	1250 3300 1250 3200
Wire Wire Line
	1250 2900 1250 3000
Wire Wire Line
	850  1850 850  1750
Wire Wire Line
	700  1850 850  1850
Wire Wire Line
	700  1800 700  1900
Wire Wire Line
	850  1450 850  1550
Wire Wire Line
	700  1450 850  1450
Wire Wire Line
	700  1500 700  1450
Wire Wire Line
	2000 1600 2000 1550
Wire Wire Line
	1850 1750 1850 1800
Wire Wire Line
	750  1150 750  1250
Wire Wire Line
	750  800  750  850 
Wire Wire Line
	2050 1100 2050 1150
Wire Wire Line
	2050 850  2050 900 
Wire Wire Line
	950  800  750  800 
Wire Wire Line
	950  900  950  800 
Wire Wire Line
	950  1200 750  1200
Connection ~ 9200 3600
Wire Wire Line
	9200 3200 9200 3600
Wire Wire Line
	9900 3600 9950 3600
Wire Wire Line
	9550 3600 9600 3600
Connection ~ 4200 4900
Wire Wire Line
	4200 4900 4200 4950
Connection ~ 4200 5300
Wire Wire Line
	4200 5250 4200 5350
Connection ~ 4200 5700
Wire Wire Line
	4200 5700 4200 5650
Connection ~ 3750 5700
Connection ~ 3750 4900
Connection ~ 3950 5300
Wire Wire Line
	3950 5250 3950 5350
Connection ~ 4500 5700
Wire Wire Line
	3750 5400 3550 5400
Wire Wire Line
	3750 5400 3750 5750
Connection ~ 4500 4900
Wire Wire Line
	3750 5200 3550 5200
Wire Wire Line
	3750 4800 3750 5200
Connection ~ 4800 4900
Wire Wire Line
	4500 4900 4500 4950
Connection ~ 5100 4900
Wire Wire Line
	4800 4900 4800 4950
Wire Wire Line
	5100 4900 5100 4950
Wire Wire Line
	3700 4900 5400 4900
Wire Wire Line
	5400 4900 5400 4950
Connection ~ 4800 5700
Wire Wire Line
	4500 5700 4500 5650
Wire Wire Line
	4800 5700 4800 5650
Wire Wire Line
	3700 5700 5100 5700
Wire Wire Line
	5100 5700 5100 5650
Connection ~ 4500 5300
Wire Wire Line
	4500 5250 4500 5350
Connection ~ 4800 5300
Wire Wire Line
	4800 5250 4800 5350
Connection ~ 5100 5300
Wire Wire Line
	5100 5250 5100 5350
Wire Wire Line
	3550 5300 5400 5300
Wire Wire Line
	5400 5300 5400 5250
Connection ~ 6600 5200
Wire Wire Line
	7300 5200 7250 5200
Wire Wire Line
	7300 4900 7300 5200
Wire Wire Line
	6600 4900 7300 4900
Connection ~ 6600 5350
Wire Wire Line
	6650 5200 6600 5200
Wire Wire Line
	6600 5350 6650 5350
Wire Wire Line
	6600 4900 6600 5400
Connection ~ 7650 5350
Connection ~ 7650 5200
Wire Wire Line
	7650 5350 7700 5350
Wire Wire Line
	8350 5200 8300 5200
Wire Wire Line
	8350 4900 8350 5200
Wire Wire Line
	7650 4900 8350 4900
Wire Wire Line
	7650 4900 7650 5400
Wire Wire Line
	7700 5200 7650 5200
Wire Wire Line
	8900 4900 8900 4950
Wire Wire Line
	9100 4900 8900 4900
Wire Wire Line
	9750 5000 9700 5000
Wire Wire Line
	9750 5600 9750 5000
Wire Wire Line
	9050 5600 9750 5600
Wire Wire Line
	9050 5100 9050 5600
Wire Wire Line
	9100 5100 9050 5100
Wire Wire Line
	9300 4700 9300 4650
Wire Wire Line
	9300 5350 9300 5300
Wire Wire Line
	1600 3300 1650 3300
Wire Wire Line
	1600 3350 1600 3300
Connection ~ 2300 2750
Wire Wire Line
	1950 2750 2300 2750
Connection ~ 1600 3100
Wire Wire Line
	1600 2750 1650 2750
Wire Wire Line
	1600 3100 1600 2750
Wire Wire Line
	8500 3700 8550 3700
Wire Wire Line
	8500 3750 8500 3700
Wire Wire Line
	9150 3600 9250 3600
Wire Wire Line
	8850 3200 9200 3200
Wire Wire Line
	8100 3500 8550 3500
Connection ~ 8500 3500
Wire Wire Line
	8500 3500 8500 3200
Wire Wire Line
	8500 3200 8550 3200
Wire Wire Line
	8250 2400 8200 2400
Wire Wire Line
	8250 2400 8250 3400
Wire Wire Line
	7550 3000 8250 3000
Wire Wire Line
	7600 2500 7550 2500
Wire Wire Line
	7800 2050 7800 2100
Wire Wire Line
	7800 2750 7800 2700
Wire Wire Line
	7400 1850 6950 1850
Connection ~ 2300 3200
Wire Wire Line
	2300 4200 6500 4200
Connection ~ 6950 1850
Wire Wire Line
	6600 1100 6600 1150
Wire Wire Line
	6600 2550 6600 2600
Connection ~ 4050 3600
Wire Wire Line
	6200 3600 6200 2450
Wire Wire Line
	6200 2450 6300 2450
Connection ~ 4050 2800
Wire Wire Line
	6050 2800 6050 1550
Wire Wire Line
	6050 1550 6300 1550
Wire Wire Line
	6950 2300 6900 2300
Wire Wire Line
	6950 1400 6950 2300
Wire Wire Line
	6900 1400 6950 1400
Connection ~ 5900 2300
Connection ~ 5900 1400
Wire Wire Line
	2250 3200 2350 3200
Wire Wire Line
	2300 2050 2300 4200
Wire Wire Line
	4850 2050 2300 2050
Connection ~ 2800 2200
Wire Wire Line
	2800 2200 3950 2200
Wire Wire Line
	3950 2200 3950 2400
Wire Wire Line
	3950 2400 4850 2400
Wire Wire Line
	5150 2400 5250 2400
Connection ~ 5200 2200
Wire Wire Line
	5200 1900 5200 2200
Wire Wire Line
	5250 1900 5200 1900
Wire Wire Line
	5850 2300 6300 2300
Wire Wire Line
	5900 1900 5900 2300
Wire Wire Line
	5550 1900 5900 1900
Wire Wire Line
	5150 1500 5250 1500
Connection ~ 5200 1300
Wire Wire Line
	4800 1300 5250 1300
Wire Wire Line
	5200 1000 5200 1300
Wire Wire Line
	5250 1000 5200 1000
Wire Wire Line
	5850 1400 6300 1400
Wire Wire Line
	5900 1000 5900 1400
Wire Wire Line
	5550 1000 5900 1000
Wire Wire Line
	4200 3200 4200 3250
Connection ~ 4050 3200
Wire Wire Line
	4050 3200 4200 3200
Wire Wire Line
	4050 3150 4050 3250
Wire Wire Line
	4050 2800 4050 2850
Wire Wire Line
	4000 2800 6050 2800
Wire Wire Line
	4050 3600 4050 3550
Wire Wire Line
	4000 3600 6200 3600
Wire Wire Line
	3650 3600 3700 3600
Wire Wire Line
	3650 2800 3700 2800
Connection ~ 3950 1500
Wire Wire Line
	3950 1900 2950 1900
Wire Wire Line
	2950 1900 2950 2700
Wire Wire Line
	2950 2700 3050 2700
Wire Wire Line
	2800 3700 3050 3700
Connection ~ 3000 3200
Wire Wire Line
	3000 3200 2650 3200
Wire Wire Line
	3000 2900 3050 2900
Wire Wire Line
	3000 2900 3000 3500
Wire Wire Line
	3000 3500 3050 3500
Wire Wire Line
	3100 4050 3100 4000
Wire Wire Line
	3250 4050 3100 4050
Wire Wire Line
	3250 3900 3250 4050
Wire Wire Line
	3100 2350 3100 2400
Wire Wire Line
	3250 2350 3100 2350
Wire Wire Line
	3250 2500 3250 2350
Wire Wire Line
	3250 1600 3300 1600
Wire Wire Line
	3250 1650 3250 1600
Connection ~ 2800 1400
Connection ~ 3250 1400
Wire Wire Line
	3200 1400 3300 1400
Wire Wire Line
	3250 1050 3250 1400
Wire Wire Line
	3300 1050 3250 1050
Wire Wire Line
	3900 1500 4850 1500
Wire Wire Line
	3950 1050 3950 1900
Wire Wire Line
	3950 1050 3600 1050
Wire Wire Line
	2750 1400 2900 1400
Wire Wire Line
	2800 1000 2800 3700
Wire Wire Line
	2450 1000 2800 1000
Connection ~ 2100 1000
Wire Wire Line
	1900 1650 1900 1300
Connection ~ 2100 1300
Wire Wire Line
	2100 1500 2150 1500
Wire Wire Line
	2100 1550 2100 1500
Wire Wire Line
	1900 1300 2150 1300
Wire Wire Line
	2100 1300 2100 1000
Wire Wire Line
	2000 1000 2150 1000
Wire Wire Line
	1400 5500 1500 5500
Wire Wire Line
	1400 5400 1400 5500
Wire Wire Line
	1400 5100 1500 5100
Wire Wire Line
	1400 5200 1400 5100
Wire Wire Line
	1300 5400 1400 5400
Wire Wire Line
	1300 5300 1500 5300
Wire Wire Line
	1300 5200 1400 5200
Wire Wire Line
	7000 3600 7400 3600
Wire Wire Line
	7400 3400 7000 3400
Wire Wire Line
	6850 3300 6850 3350
Wire Wire Line
	6850 3650 6850 3750
$Comp
L GNDA #PWR028
U 1 1 5986C957
P 700 1900
F 0 "#PWR028" H 700 1650 50  0001 C CNN
F 1 "GNDA" H 700 1750 50  0000 C CNN
F 2 "" H 700 1900 50  0000 C CNN
F 3 "" H 700 1900 50  0000 C CNN
	1    700  1900
	1    0    0    -1  
$EndComp
Connection ~ 700  1850
$Comp
L GNDA #PWR029
U 1 1 5986D13B
P 750 1250
F 0 "#PWR029" H 750 1000 50  0001 C CNN
F 1 "GNDA" H 750 1100 50  0000 C CNN
F 2 "" H 750 1250 50  0000 C CNN
F 3 "" H 750 1250 50  0000 C CNN
	1    750  1250
	1    0    0    -1  
$EndComp
Connection ~ 750  1200
$Comp
L GNDA #PWR030
U 1 1 5986FEC9
P 650 4000
F 0 "#PWR030" H 650 3750 50  0001 C CNN
F 1 "GNDA" H 650 3850 50  0000 C CNN
F 2 "" H 650 4000 50  0000 C CNN
F 3 "" H 650 4000 50  0000 C CNN
	1    650  4000
	1    0    0    -1  
$EndComp
Connection ~ 650  3950
$Comp
L GNDA #PWR031
U 1 1 59871C2F
P 6850 3750
F 0 "#PWR031" H 6850 3500 50  0001 C CNN
F 1 "GNDA" H 6850 3600 50  0000 C CNN
F 2 "" H 6850 3750 50  0000 C CNN
F 3 "" H 6850 3750 50  0000 C CNN
	1    6850 3750
	1    0    0    -1  
$EndComp
Connection ~ 6850 3700
$Comp
L GNDA #PWR032
U 1 1 59875892
P 3000 5650
F 0 "#PWR032" H 3000 5400 50  0001 C CNN
F 1 "GNDA" H 3000 5500 50  0000 C CNN
F 2 "" H 3000 5650 50  0000 C CNN
F 3 "" H 3000 5650 50  0000 C CNN
	1    3000 5650
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG033
U 1 1 59876034
P 3000 5600
F 0 "#FLG033" H 3000 5695 50  0001 C CNN
F 1 "PWR_FLAG" H 3000 5780 50  0000 C CNN
F 2 "" H 3000 5600 50  0000 C CNN
F 3 "" H 3000 5600 50  0000 C CNN
	1    3000 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 5600 3000 5650
$Comp
L CONN_01X03 P6
U 1 1 598600AD
P 1500 1000
F 0 "P6" H 1500 1200 50  0000 C CNN
F 1 "CONN_01X03" V 1600 1000 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 1500 1000 50  0001 C CNN
F 3 "" H 1500 1000 50  0000 C CNN
	1    1500 1000
	1    0    0    1   
$EndComp
Wire Wire Line
	950  1000 900  1000
Wire Wire Line
	1300 900  950  900 
Wire Wire Line
	1300 1000 1250 1000
Wire Wire Line
	1300 1100 950  1100
Wire Wire Line
	2050 900  2000 900 
Wire Wire Line
	2000 1100 2050 1100
Wire Wire Line
	850  1650 900  1650
Wire Wire Line
	1850 1650 1900 1650
Wire Wire Line
	2000 1550 1850 1550
Wire Wire Line
	1250 1650 1200 1650
Wire Wire Line
	850  1550 1250 1550
Wire Wire Line
	850  1750 1250 1750
Wire Wire Line
	4250 850  4350 850 
Wire Wire Line
	4650 850  4750 850 
Wire Wire Line
	4200 1050 4350 1050
Wire Wire Line
	4650 1050 4750 1050
Wire Wire Line
	7400 1350 7450 1350
Wire Wire Line
	7750 1350 7800 1350
Wire Wire Line
	7400 1550 7450 1550
Wire Wire Line
	7750 1550 7800 1550
Wire Wire Line
	7000 3500 7050 3500
Wire Wire Line
	7350 3500 7400 3500
Wire Wire Line
	800  3750 850  3750
Wire Wire Line
	1150 3750 1200 3750
$EndSCHEMATC
