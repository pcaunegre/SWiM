EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Sigfox-connected (Arduino-based) Wind Meter"
Date "2021-10-12"
Rev "1.1"
Comp "pascal.caunegre@gmail.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR?
U 1 1 60DAF47D
P 4350 3650
F 0 "#PWR?" H 4350 3400 50  0001 C CNN
F 1 "GND" H 4355 3477 50  0000 C CNN
F 2 "" H 4350 3650 50  0001 C CNN
F 3 "" H 4350 3650 50  0001 C CNN
	1    4350 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 60DAFE6B
P 4350 2200
F 0 "R1" H 4420 2246 50  0000 L CNN
F 1 "100k" H 4420 2155 50  0000 L CNN
F 2 "" V 4280 2200 50  0001 C CNN
F 3 "~" H 4350 2200 50  0001 C CNN
	1    4350 2200
	1    0    0    -1  
$EndComp
$Comp
L pspice:CAP C1
U 1 1 60DB0222
P 4350 3250
F 0 "C1" H 4528 3296 50  0000 L CNN
F 1 "10n" H 4528 3205 50  0000 L CNN
F 2 "" H 4350 3250 50  0001 C CNN
F 3 "~" H 4350 3250 50  0001 C CNN
	1    4350 3250
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 60DB27FB
P 4350 1700
F 0 "#PWR?" H 4350 1550 50  0001 C CNN
F 1 "VCC" H 4365 1873 50  0000 C CNN
F 2 "" H 4350 1700 50  0001 C CNN
F 3 "" H 4350 1700 50  0001 C CNN
	1    4350 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3350 4350 3500
Wire Wire Line
	4350 2350 4350 2400
$Comp
L Device:R R3
U 1 1 60DBFEF6
P 5600 2200
F 0 "R3" H 5670 2246 50  0000 L CNN
F 1 "100k" H 5670 2155 50  0000 L CNN
F 2 "" V 5530 2200 50  0001 C CNN
F 3 "~" H 5600 2200 50  0001 C CNN
	1    5600 2200
	1    0    0    -1  
$EndComp
$Comp
L pspice:CAP C2
U 1 1 60DBFEFC
P 5600 3300
F 0 "C2" H 5778 3346 50  0000 L CNN
F 1 "10n" H 5778 3255 50  0000 L CNN
F 2 "" H 5600 3300 50  0001 C CNN
F 3 "~" H 5600 3300 50  0001 C CNN
	1    5600 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1850 5600 2050
Wire Wire Line
	4350 1700 4350 1850
Connection ~ 4350 1850
Wire Wire Line
	4350 1850 4350 2050
Text GLabel 2700 1850 0    50   Input ~ 0
Arduino_Vcc
Text GLabel 2550 3600 0    50   Input ~ 0
Arduino_Gnd
Text GLabel 2600 2400 0    50   Input ~ 0
Arduino_0
Text GLabel 2550 2750 0    50   Input ~ 0
Arduino_1
Wire Wire Line
	2700 1850 4350 1850
Wire Wire Line
	2550 3600 4000 3600
Connection ~ 4350 3600
Wire Wire Line
	4350 3600 4350 3650
Wire Wire Line
	2600 2400 4350 2400
Connection ~ 4350 2400
Wire Wire Line
	2550 2750 5600 2750
Connection ~ 5600 2750
$Comp
L Connector:Screw_Terminal_01x03 J1
U 1 1 60DD2D62
P 7800 2650
F 0 "J1" H 7880 2692 50  0000 L CNN
F 1 "Screw_Terminal_01x03" H 7880 2601 50  0000 L CNN
F 2 "" H 7800 2650 50  0001 C CNN
F 3 "~" H 7800 2650 50  0001 C CNN
	1    7800 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 60DD3513
P 7850 4250
F 0 "J2" H 7930 4242 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 7930 4151 50  0000 L CNN
F 2 "" H 7850 4250 50  0001 C CNN
F 3 "~" H 7850 4250 50  0001 C CNN
	1    7850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3550 7500 2750
Wire Wire Line
	7500 2750 7600 2750
Connection ~ 6250 3550
Wire Wire Line
	4350 2400 4350 2550
Connection ~ 4350 2550
Text Label 9150 2750 0    50   ~ 0
Black
Text Label 9300 2850 0    50   ~ 0
Red
Text Label 8800 2550 0    50   ~ 0
Yellow
Text Label 9000 2650 0    50   ~ 0
Green
Wire Notes Line
	7850 2550 8800 2550
Wire Notes Line
	7850 2650 8950 2650
Wire Notes Line
	7950 2750 7950 2850
Wire Notes Line
	7950 2850 9250 2850
Wire Notes Line
	7850 2750 9050 2750
Text Label 8000 2450 0    100  ~ 0
Peet_Bross_Sensor_cable
Wire Notes Line
	2800 1200 2800 4900
Wire Notes Line
	2800 4900 850  4900
Wire Notes Line
	850  4900 850  1200
Wire Notes Line
	850  1200 2800 1200
Text Label 1150 1550 0    150  ~ 0
MKRFox1200
$Comp
L Device:Battery_Cell B1
U 1 1 60DF4DBF
P 6250 5250
F 0 "B1" H 6368 5346 50  0000 L CNN
F 1 "Lipo3.7V_650mAh" H 6368 5255 50  0000 L CNN
F 2 "" V 6250 5310 50  0001 C CNN
F 3 "~" V 6250 5310 50  0001 C CNN
	1    6250 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E01E67
P 4400 4800
F 0 "#PWR?" H 4400 4550 50  0001 C CNN
F 1 "GND" H 4405 4627 50  0000 C CNN
F 2 "" H 4400 4800 50  0001 C CNN
F 3 "" H 4400 4800 50  0001 C CNN
	1    4400 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 4700 6250 5050
Text GLabel 2500 4150 0    50   Input ~ 0
Arduino_vin
Wire Wire Line
	4000 3600 4000 4800
Connection ~ 4000 3600
Wire Wire Line
	4000 3600 4350 3600
Wire Wire Line
	4000 4800 4400 4800
Connection ~ 4400 4800
$Comp
L Device:Solar_Cells SolarCell
U 1 1 60E2813A
P 9400 4350
F 0 "SolarCell" H 9508 4396 50  0000 L CNN
F 1 "Solar_Cells" H 9508 4305 50  0000 L CNN
F 2 "" V 9400 4410 50  0001 C CNN
F 3 "~" V 9400 4410 50  0001 C CNN
	1    9400 4350
	1    0    0    -1  
$EndComp
$Comp
L SWiMLib:CN3065 U2
U 1 1 60E2F195
P 6350 4350
F 0 "U2" H 6375 4827 100 0000 C CNN
F 1 "CN3065" H 6375 4661 100 0000 C CNN
F 2 "" H 6250 3800 100 0001 C CNN
F 3 "" H 6250 3800 100 0001 C CNN
	1    6350 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4150 5750 4250
Wire Wire Line
	5750 4250 5900 4250
Wire Wire Line
	5750 4800 5750 4400
Wire Wire Line
	5750 4400 5900 4400
Wire Wire Line
	6850 4250 7650 4250
Wire Wire Line
	6850 4400 7650 4400
Wire Wire Line
	7650 4350 7650 4400
Wire Notes Line
	7900 4250 8950 4250
Wire Notes Line
	8950 4250 8950 4150
Wire Notes Line
	8950 4150 9400 4150
Wire Notes Line
	7900 4350 8950 4350
Wire Notes Line
	8950 4350 8950 4550
Wire Notes Line
	8950 4550 9400 4550
Wire Wire Line
	2500 4150 3300 4150
Wire Wire Line
	4400 4800 5750 4800
Wire Wire Line
	6500 4700 6450 4700
$Comp
L power:GND #PWR?
U 1 1 61063F21
P 6250 5350
F 0 "#PWR?" H 6250 5100 50  0001 C CNN
F 1 "GND" H 6255 5177 50  0000 C CNN
F 2 "" H 6250 5350 50  0001 C CNN
F 3 "" H 6250 5350 50  0001 C CNN
	1    6250 5350
	1    0    0    -1  
$EndComp
Connection ~ 6250 5350
Wire Wire Line
	6250 5350 6500 5350
Wire Wire Line
	6500 5350 6500 4700
$Comp
L Switch:SW_DIP_x01 ON_OFF
U 1 1 6106C62F
P 3600 4150
F 0 "ON_OFF" H 3600 4417 50  0000 C CNN
F 1 "SW_DIP_x01" H 3600 4326 50  0000 C CNN
F 2 "" H 3600 4150 50  0001 C CNN
F 3 "~" H 3600 4150 50  0001 C CNN
	1    3600 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 4150 5750 4150
Wire Wire Line
	5100 2050 5100 1850
Wire Wire Line
	5100 2350 5100 2550
Wire Wire Line
	6250 1850 6250 2000
Connection ~ 5600 1850
$Comp
L Device:R R2
U 1 1 6113A693
P 4700 2550
F 0 "R2" V 4493 2550 50  0000 C CNN
F 1 "1k" V 4584 2550 50  0000 C CNN
F 2 "" V 4630 2550 50  0001 C CNN
F 3 "~" H 4700 2550 50  0001 C CNN
	1    4700 2550
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 611442B6
P 5950 2750
F 0 "R4" V 6050 2700 50  0000 C CNN
F 1 "1k" V 6150 2700 50  0000 C CNN
F 2 "" V 5880 2750 50  0001 C CNN
F 3 "~" H 5950 2750 50  0001 C CNN
	1    5950 2750
	0    1    1    0   
$EndComp
Text Notes 1550 4700 0    50   ~ 0
U1
Text Notes 5450 1600 0    100  ~ 0
We use 100K for external pull-ups (not using internal pull-ups)\nSchottky diodes to suppress overvoltages.\n1K to limit current during overvoltage pulses\n10nF capacitor to debounce signal (better than software debounce)\n
$Comp
L Diode:1N5818 D2
U 1 1 6116621C
P 5100 3150
F 0 "D2" V 5054 3230 50  0000 L CNN
F 1 "1N5818" V 5145 3230 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 5100 2975 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 5100 3150 50  0001 C CNN
	1    5100 3150
	0    1    1    0   
$EndComp
$Comp
L Diode:1N5818 D1
U 1 1 6116D7CA
P 5100 2200
F 0 "D1" V 5054 2280 50  0000 L CNN
F 1 "1N5818" V 5145 2280 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 5100 2025 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 5100 2200 50  0001 C CNN
	1    5100 2200
	0    1    1    0   
$EndComp
$Comp
L Diode:1N5818 D3
U 1 1 6116F033
P 6250 2150
F 0 "D3" V 6204 2230 50  0000 L CNN
F 1 "1N5818" V 6295 2230 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 6250 1975 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 6250 2150 50  0001 C CNN
	1    6250 2150
	0    1    1    0   
$EndComp
$Comp
L Diode:1N5818 D4
U 1 1 61170E18
P 6250 3200
F 0 "D4" V 6204 3280 50  0000 L CNN
F 1 "1N5818" V 6295 3280 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 6250 3025 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 6250 3200 50  0001 C CNN
	1    6250 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 1850 5100 1850
Wire Wire Line
	4350 2550 4550 2550
Connection ~ 4350 3000
Wire Wire Line
	4350 3000 4350 3050
Connection ~ 4350 3500
Wire Wire Line
	4350 2550 4350 3000
Connection ~ 5100 1850
Connection ~ 5100 2550
Wire Wire Line
	5100 1850 5600 1850
Wire Wire Line
	5100 2550 5100 3000
Wire Wire Line
	5100 3300 5100 3550
Wire Wire Line
	4350 3500 4350 3550
Wire Wire Line
	5100 3550 4350 3550
Wire Wire Line
	4350 3550 4350 3600
Connection ~ 5100 3550
Connection ~ 4350 3550
Wire Wire Line
	4850 2550 5100 2550
Wire Wire Line
	5100 2550 7600 2550
Wire Wire Line
	5100 3550 5600 3550
Connection ~ 5600 3550
Wire Wire Line
	5600 3550 6250 3550
Wire Wire Line
	5600 2750 5600 3050
Wire Wire Line
	6250 3550 7500 3550
Wire Wire Line
	6250 3500 6250 3550
Wire Wire Line
	5600 2750 5800 2750
Wire Wire Line
	6250 3350 6250 3550
Wire Wire Line
	5600 1850 6250 1850
Wire Wire Line
	6100 2750 6250 2750
Connection ~ 6250 2750
Wire Wire Line
	6250 2750 6250 3050
Wire Wire Line
	5600 2350 5600 2750
Wire Wire Line
	6250 2300 6250 2750
Wire Wire Line
	6250 2750 6750 2750
Wire Wire Line
	6750 2750 6750 2650
Wire Wire Line
	6750 2650 7600 2650
$EndSCHEMATC
