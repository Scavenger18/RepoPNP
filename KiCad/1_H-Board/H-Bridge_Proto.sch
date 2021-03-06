EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:H-Bridge_Proto-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "DAC Sumo Robot"
Date "2017-12-16"
Rev "2.0"
Comp "Hochschule Luzern"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DRV8835 U1
U 1 1 5A9E9815
P 2800 2700
F 0 "U1" H 2350 3300 60  0000 C CNN
F 1 "DRV8835" H 2500 2250 60  0000 C CNN
F 2 "Housings_SON:WSON-12-1EP_3x2mm_Pitch0.5mm" H 2800 2700 60  0001 C CNN
F 3 "" H 2800 2700 60  0001 C CNN
	1    2800 2700
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5A9E981B
P 1800 1850
F 0 "C6" H 1825 1950 50  0000 L CNN
F 1 "100n" H 1825 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1838 1700 50  0001 C CNN
F 3 "" H 1800 1850 50  0001 C CNN
	1    1800 1850
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5A9E981C
P 1500 1850
F 0 "C3" H 1525 1950 50  0000 L CNN
F 1 "1u" H 1525 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1538 1700 50  0001 C CNN
F 3 "" H 1500 1850 50  0001 C CNN
	1    1500 1850
	1    0    0    -1  
$EndComp
$Comp
L CP C2
U 1 1 5A9E981D
P 1200 1850
F 0 "C2" H 1225 1950 50  0000 L CNN
F 1 "220u" H 1225 1750 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_6.3x7.7" H 1238 1700 50  0001 C CNN
F 3 "" H 1200 1850 50  0001 C CNN
	1    1200 1850
	1    0    0    -1  
$EndComp
$Comp
L CP C1
U 1 1 5A9E981E
P 900 1850
F 0 "C1" H 925 1950 50  0000 L CNN
F 1 "220u" H 925 1750 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_6.3x7.7" H 938 1700 50  0001 C CNN
F 3 "" H 900 1850 50  0001 C CNN
	1    900  1850
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5A9E981F
P 1650 2400
F 0 "C4" H 1675 2500 50  0000 L CNN
F 1 "3n3" H 1675 2300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1688 2250 50  0001 C CNN
F 3 "" H 1650 2400 50  0001 C CNN
	1    1650 2400
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5A9E9820
P 1650 2850
F 0 "C5" H 1675 2950 50  0000 L CNN
F 1 "3n3" H 1675 2750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1688 2700 50  0001 C CNN
F 3 "" H 1650 2850 50  0001 C CNN
	1    1650 2850
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5A9E9821
P 4400 2050
F 0 "C7" H 4425 2150 50  0000 L CNN
F 1 "100n" H 4425 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4438 1900 50  0001 C CNN
F 3 "" H 4400 2050 50  0001 C CNN
	1    4400 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 5A9E9822
P 2800 3400
F 0 "#PWR01" H 2800 3150 50  0001 C CNN
F 1 "GND" H 2800 3250 50  0000 C CNN
F 2 "" H 2800 3400 50  0001 C CNN
F 3 "" H 2800 3400 50  0001 C CNN
	1    2800 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5A9E9823
P 900 2050
F 0 "#PWR02" H 900 1800 50  0001 C CNN
F 1 "GND" H 900 1900 50  0000 C CNN
F 2 "" H 900 2050 50  0001 C CNN
F 3 "" H 900 2050 50  0001 C CNN
	1    900  2050
	1    0    0    -1  
$EndComp
$Comp
L +8V #PWR03
U 1 1 5A9E9824
P 900 1650
F 0 "#PWR03" H 900 1500 50  0001 C CNN
F 1 "+8V" H 900 1790 50  0000 C CNN
F 2 "" H 900 1650 50  0001 C CNN
F 3 "" H 900 1650 50  0001 C CNN
	1    900  1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5A9E9829
P 4400 1900
F 0 "#PWR04" H 4400 1650 50  0001 C CNN
F 1 "GND" H 4400 1750 50  0000 C CNN
F 2 "" H 4400 1900 50  0001 C CNN
F 3 "" H 4400 1900 50  0001 C CNN
	1    4400 1900
	-1   0    0    1   
$EndComp
$Comp
L +3V3 #PWR05
U 1 1 5A9E982A
P 4000 2050
F 0 "#PWR05" H 4000 1900 50  0001 C CNN
F 1 "+3V3" H 4000 2190 50  0000 C CNN
F 2 "" H 4000 2050 50  0001 C CNN
F 3 "" H 4000 2050 50  0001 C CNN
	1    4000 2050
	1    0    0    -1  
$EndComp
Text Label 3850 2550 2    60   ~ 0
Dir_A
Text Label 3850 2700 2    60   ~ 0
PWM_A
Text Label 3850 2850 2    60   ~ 0
Dir_B
Text Label 3850 3000 2    60   ~ 0
PWM_B
Text Notes 2350 1750 0    118  ~ 0
Motor Driver
Wire Wire Line
	2100 3000 2050 3000
Wire Wire Line
	2050 3000 2050 3350
Wire Wire Line
	2100 2850 1850 2850
Wire Wire Line
	1450 3000 1850 3000
Wire Wire Line
	1450 2550 2100 2550
Wire Wire Line
	2100 2400 1850 2400
Wire Wire Line
	1850 2400 1850 2250
Wire Wire Line
	1850 2250 1450 2250
Connection ~ 1500 2000
Connection ~ 1200 2000
Wire Wire Line
	900  1700 2050 1700
Wire Wire Line
	900  2000 1800 2000
Connection ~ 1500 1700
Connection ~ 1200 1700
Connection ~ 1800 1700
Wire Wire Line
	900  1650 900  1700
Wire Wire Line
	900  2050 900  2000
Connection ~ 900  2000
Connection ~ 900  1700
Wire Wire Line
	1450 2700 2100 2700
Wire Wire Line
	1850 3000 1850 2850
Wire Wire Line
	2100 2250 2050 2250
Connection ~ 1650 2250
Connection ~ 1650 3000
Connection ~ 1650 2550
Connection ~ 1650 2700
Wire Wire Line
	2050 2250 2050 1700
Wire Wire Line
	3500 2550 4850 2550
Wire Wire Line
	3500 2700 3950 2700
Wire Wire Line
	3500 2850 3900 2850
Wire Wire Line
	3500 3000 4850 3000
Connection ~ 4400 2200
Wire Wire Line
	3500 2400 5000 2400
Wire Wire Line
	2800 3300 2800 3400
Wire Wire Line
	2050 3350 2800 3350
Connection ~ 2800 3350
Wire Wire Line
	3900 2850 3900 2900
Wire Wire Line
	3900 2900 4850 2900
$Comp
L +3V3 #PWR06
U 1 1 5A9EB688
P 5000 2150
F 0 "#PWR06" H 5000 2000 50  0001 C CNN
F 1 "+3V3" H 5000 2290 50  0000 C CNN
F 2 "" H 5000 2150 50  0001 C CNN
F 3 "" H 5000 2150 50  0001 C CNN
	1    5000 2150
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR07
U 1 1 5A9EB6BA
P 5000 1700
F 0 "#PWR07" H 5000 1450 50  0001 C CNN
F 1 "GND" H 5000 1550 50  0000 C CNN
F 2 "" H 5000 1700 50  0001 C CNN
F 3 "" H 5000 1700 50  0001 C CNN
	1    5000 1700
	0    1    1    0   
$EndComp
$Comp
L +8V #PWR08
U 1 1 5A9EB761
P 5000 1600
F 0 "#PWR08" H 5000 1450 50  0001 C CNN
F 1 "+8V" H 5000 1740 50  0000 C CNN
F 2 "" H 5000 1600 50  0001 C CNN
F 3 "" H 5000 1600 50  0001 C CNN
	1    5000 1600
	0    -1   -1   0   
$EndComp
$Comp
L Conn_01x02 J1
U 1 1 5A9EBC56
P 1250 2450
F 0 "J1" H 1250 2550 50  0000 C CNN
F 1 "Conn_01x02" H 1250 2250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1250 2450 50  0001 C CNN
F 3 "" H 1250 2450 50  0001 C CNN
	1    1250 2450
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02 J2
U 1 1 5A9EBD13
P 1250 2900
F 0 "J2" H 1250 3000 50  0000 C CNN
F 1 "Conn_01x02" H 1250 2700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1250 2900 50  0001 C CNN
F 3 "" H 1250 2900 50  0001 C CNN
	1    1250 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 2250 1450 2350
Wire Wire Line
	1450 2450 1450 2550
Wire Wire Line
	1450 2700 1450 2800
Wire Wire Line
	1450 2900 1450 3000
$Comp
L Conn_01x02 J4
U 1 1 5A9E9774
P 5050 2900
F 0 "J4" H 5050 3000 50  0000 C CNN
F 1 "Conn_01x02" H 5050 2700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5050 2900 50  0001 C CNN
F 3 "" H 5050 2900 50  0001 C CNN
	1    5050 2900
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02 J3
U 1 1 5A9E98CD
P 5050 2550
F 0 "J3" H 5050 2650 50  0000 C CNN
F 1 "Conn_01x02" H 5050 2350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5050 2550 50  0001 C CNN
F 3 "" H 5050 2550 50  0001 C CNN
	1    5050 2550
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02 J5
U 1 1 5A9E994E
P 5200 1600
F 0 "J5" H 5200 1700 50  0000 C CNN
F 1 "Conn_01x02" H 5200 1400 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5200 1600 50  0001 C CNN
F 3 "" H 5200 1600 50  0001 C CNN
	1    5200 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 5A9E9A4A
P 5000 2050
F 0 "#PWR09" H 5000 1800 50  0001 C CNN
F 1 "GND" H 5000 1900 50  0000 C CNN
F 2 "" H 5000 2050 50  0001 C CNN
F 3 "" H 5000 2050 50  0001 C CNN
	1    5000 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 2700 3950 2650
Wire Wire Line
	3950 2650 4850 2650
Wire Wire Line
	4000 2050 4000 2200
Connection ~ 4000 2200
$Comp
L Conn_01x03 J6
U 1 1 5A9EA7BF
P 5200 2150
F 0 "J6" H 5200 2350 50  0000 C CNN
F 1 "Conn_01x03" H 5200 1950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 5200 2150 50  0001 C CNN
F 3 "" H 5200 2150 50  0001 C CNN
	1    5200 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2400 5000 2250
Wire Wire Line
	4400 2200 3500 2200
Wire Wire Line
	3500 2200 3500 2250
$EndSCHEMATC
