#define SOFTVERSION 210620


// debug facilities
#define DEBUG 0
#define LCD   0  // enable output to a lcd display

#define CPU_FULL   1  // normal cpu rate (48Mhz)
#define CPU_SLOW   16  // divided by 16 so 3Mhz for power saving, so we have ~3mA and a peak of ~45mA during 3s every 10min


// Wind Speed & Direction
// use 2 of interruptible pins among: 0, 1, 4, 5, 6, 7, 8, A1 -or 16-, A2 - or 17
#define pinSpeed  0     // reed sensor for speed     - yellow wire
#define pinDir    1     // reed sensor for direction - green wire
#define pinBattV  A5    // pin to measure Battery voltage 

// 
#define DEBOUNCE_TIME   16       // 16ms (25ms correspond to speed 155km/h=40rps)

#define ARRAYLEN        99       // size of an array to store pulses (40rps gives 120 values in 3s)

#define SAMPLING_PERIOD 2992     // instantaneous wind is measured on a period of 3s (common rule)
#define REBOOT_PERIOD   2419200  // reboot micro every 28 days to avoid managing millis reset after 2**32-1 ms

#define REPORT_PERIOD 600000  // in production, report period is 10min=600s (both the period to avg the wind speed and the sigfox report period)

// #define REPORT_PERIOD 60000 // shorter time for debug: 1min  

#define Led LED_BUILTIN 


// Reminder: 3.2rps = 13km/h, 0.24rps = 1km/h, 1rps=4km/h mini detectable



/*
* This part of code is about packing data to comply with the format expected by OpenWindMap
*
* Numbers are arranged to fit into 8 bytes (2 periods, 4 data)
* xxx[0] -> data from T-10 to T-5 min
* xxx[1] -> data from T-5 min to T
* T being the time of emission
*/
typedef struct __attribute__ ((packed)) sigfox_wind_message {
        int8_t speedMin[2];
        int8_t speedAvg[2];
        int8_t speedMax[2];
        int8_t directionAvg[2];
} SigfoxWindMessage;


// wind speed encoding over 1 byte
// 
uint8_t encodeWindSpeed (float speedKmh) {
  uint8_t encodedSpeed;
  if (speedKmh < 10.) {
    // 0 to 9.75 kmh : 0.25 km/h resolution
    encodedSpeed = (uint8_t)(float)(speedKmh * 4. + 0.5);
  } else if (speedKmh < 80.) {
    // 10 to 79.5 kmh  : 0.5 km/h resolution
    encodedSpeed = (uint8_t)(float)(speedKmh * 2. + 0.5) + 20;
  } else if (speedKmh < 120.) {
    // 80 to 119 kmh  : 1 km/h resolution
    encodedSpeed = (uint8_t)(float)(speedKmh + 0.5) + 100;
  } else if (speedKmh < 190.) {
    // 120 to 188 kmh  : 2 km/h resolution
    encodedSpeed = (uint8_t)(float)(speedKmh / 2. + 0.5) + 160;
  } else {
    // 190 or + : out of range
    encodedSpeed = 0xFF;
  }
  return encodedSpeed;
}

// wind direction encoding over 1 byte
// here direction comes 0-359 degrees (not like in Pioupiou)
uint8_t encodeWindDirection (int direction) {   // degrees

  // encode with 2 deg precision
  // add 0.5 for rounding when converting from (float) to (int)
  return (uint8_t)(float)(direction / 2. + 0.5);
}
