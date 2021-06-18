
// debug mode

#define DEBUG 1
#define LCD   1  // enable output to a lcd display

// cpu clock to reduce power 2mA@1Mhz / 14mA@48Mhz
#define CPU_DIVISOR 48
#define FULL 1


// Wind Speed & Direction
// use 2 of interruptible pins among: 0, 1, 4, 5, 6, 7, 8, A1 -or 16-, A2 - or 17
#define pinSpeed  7     // reed sensor for speed     - yellow wire

#define pinDir    8     // reed sensor for direction - green wire


// 
#define DEBOUNCE_TIME   25    // 25ms correspond to speed 155km/h (40rps)

#define ARRAYLEN        99    // size of arrays to store pulses (40rps gives 120 values in 3s)

#define SAMPLING_PERIOD 3000  // instantaneous wind is measured on a period of 3s (common rule)

// report period of 10min=600s (both the period to avg the wind speed and the sigfox report period)
// #define REPORT_PERIOD 600000  

// shorter time for debug: 1min
#define REPORT_PERIOD 60000  

#define Led LED_BUILTIN 


// 3.2rps = 13km/h, 0.24rps = 1km/h, 1rps=4km/h mini detectable
