/*

Sigfox Wind Meter 
based on Peet Bros. (PRO) anemometer

Read the wind speed and direction interrupts from two input pins, compute wind
Send through Sigfox

Run on MKRFox1200

*/


#include <SigFox.h>
#include <limits.h>
#include <math.h> 
#include "def.h"


volatile unsigned long  first_spulseT ;  // first time in millis the speed sensor triggers
volatile unsigned long  last_spulseT  ;  // last time in millis the speed sensor triggers
volatile int            scount        ;  // counter of speed sensor pulse

volatile unsigned long  last_dpulseT  ;  // last time in millis the dir sensor triggers
volatile int            dcount        ;  // counter of wind dir ratio accumulated in acc_ratio
volatile unsigned int   ddeltaT       ;  // delta time between dir pulse and last speed pulse
volatile float          acc_ratio     ;  // wind dir ratio accumulater over the sample period

volatile unsigned long  last_sampleT  ;  // last time in millis a measure sampling has been done
volatile float          max_wspeed    ;  // max wind speed over the sample period
volatile float          min_wspeed    ;  // min wind speed over the sample period
volatile int            cnt_ws_samples;  // count the number of samples we have to compute speed avg
volatile float          acc_wspeed    ;  // wind speed accumulator to compute avg
volatile double         acc_wdX       ;  // accumulate wind dir projection on X axis (cos)
volatile double         acc_wdY       ;  // accumulate wind dir projection on X axis (sin)


void setup() {

  // pins to sensor
  pinMode(pinSpeed, INPUT_PULLUP); // yellow wire of Peet Bros cable
  pinMode(pinDir  , INPUT_PULLUP); // green  wire
  
  // install interruptions functions
  attachInterrupt(digitalPinToInterrupt(pinSpeed), isr_speed    , RISING); // pin2 is int0.
  attachInterrupt(digitalPinToInterrupt(pinDir)  , isr_direction, RISING); // pin3 is int1.

  // init variables
  scount        = 0;
  dcount        = 0;
  acc_ratio     = 0.0;
  last_spulseT  = 0;
  last_dpulseT  = 0;
  last_sampleT  = millis();

  reset_stat();  
 
  Serial.begin(9600);
}


void loop() {

  unsigned long now = millis();
  unsigned int  dt; 
  float         ws; // temporary wind speed
  int           wd; // temporary wind dir angle
  
  if (now < last_sampleT)  dt = now + (ULONG_MAX - last_sampleT);
  else                     dt = now - last_sampleT;

  // if it's time to sample, get measures and store for stats
  if (dt > SAMPLING_PERIOD) {
    last_sampleT = now;
    ws = wspeed_sample();  
    wd = wdir_sample();
    compute_stat(ws,wd);

    if(DEBUG) {
    
    }
  }
  if (dt > REPORTING_PERIOD) {
    // report the values
    
    
    // clear stats
    reset_stat();
  }



}

/*
 * Interrupt called when the rotating part of the wind instrument
 * has completed one rotation. Rising edge.
 *
 * time between two pulses varies in the range 100000:15 ms for speed in 0.3:290 km/h
 *      e.g.: 13km/h = 3.22 rps = 310ms
 *
 * We increment a counter each time a pulse is detected
 * We compute the wind dir ratio, now that the speed period is known
*/
void isr_speed() {
  unsigned long now = millis();
  unsigned int  speriod;  // period of speed sensor

  float ratio;
  
  // counter is at 0 at beginning of sample period
  if (scount==0) {
    first_spulseT = now;
    last_spulseT  = now;
    scount = 1;
  } else {
    
    // avoid switch bounce to call this routine multiple times
    if ((now - last_spulseT) < DEBOUNCE_TIME) return;
    
    else {
      
      // take care of millis counter back to 0 (after 50 days)
      if (now < last_spulseT) speriod = now + (ULONG_MAX - last_spulseT);
      else                    speriod = now - last_spulseT;
      
      // compute wind dir ratio and accumulate
      if (ddeltaT > 0) {
        ratio = speriod / ddeltaT;
        acc_ratio += ratio; // sum up for computing average later 
        dcount++;
      }
         
      // store last value
      last_spulseT = now;
      scount++;
    }     
  }     
}

/*
 * Interrupt called when the direction reed sensor triggers. Rising edge.
 *
 * We compute the delay between this edge and the previous speed sensor edge
 * and store that in ddeltaT
*/
void isr_direction() {
  unsigned long now = millis();

  // last_spulseT at 0 means no speed pulse detected yet (startup case), so skip this time
  if (last_spulseT == 0) return; 
  
  // avoid switch bounce to call this routine multiple times
  if ((now - last_spulseT) < DEBOUNCE_TIME) return;
  else {  
    // track this time only to work the switch debounce mechanism
    last_dpulseT = now;
  }
  
  // take care of millis counter back to 0 (after 50 days)
  if (now < last_spulseT)  ddeltaT = now + (ULONG_MAX - last_spulseT);
  else                     ddeltaT = now - last_spulseT;

  
}

/*
 * Compute the wind speed over the sample period.
 *
 * Reset the counters
 * 
*/
float wspeed_sample() {
  float mph = NAN;
  float kmh = NAN;
  float rps; // rotation per seconds
  unsigned int delta; // number of millis over a number of pulses
  
  // take care of millis counter back to 0 (after 50 days)
  if (last_spulseT < first_spulseT) delta = last_spulseT + (ULONG_MAX - first_spulseT);
  else                              delta = last_spulseT - first_spulseT;
  
  rps = 1000 * (scount-1) / delta;

  // calibration formulas given by Peet Bros sensor vendor
  if      rps < 0.010   mph = 0.0;
  else if rps < 3.229   mph = -0.1095*rps*rps + 2.9318*rps - 0.1412;
  else if rps < 54.362  mph = 0.0052*rps*rps + 2.1980*rps + 1.1091;
  else if rps < 66.332  mph = 0.1104*rps*rps - 9.5685*rps + 329.87;
  
  kmh = mph / 1.609344;
  
  scount = 0;
  return(kmh);

}


/*
 * Compute the wind dir in degrees (0-359) over the sample period.
 *
 * Reset the counters
 * 
*/
int wdir_sample() {
  int windangle;
  
  // compute the mean of ratio over a number of hits
  windangle = int((360 * acc_ratio) / dcount);
  
  // reset the ctr of values in acc_ratio
  dcount = 0;
  return(windangle);
}

/*
 * Take measures and compute some statistics over a report period
 * min, max, accumulate for average, mean wind dir
 * 
*/
void compute_stat(ws,wd)  {
  float ws; // wind speed in km/h
  int   wd; // wind dir in degrees
  
  if (ws > max_wspeed)  max_wspeed = ws;
  if (ws < min_wspeed)  min_wspeed = ws;
  acc_wspeed += ws;
  cnt_ws_samples++;
  
  // projection of direction is weighted by the speed
  acc_wdX += ws * cos(PI*wd/180.)
  acc_wdY += ws * sin(PI*wd/180.)

}
/*
 * Resets all counters and accumulator when a report has been done
 * min, max, accumulate for average, mean wind dir
 * 
*/
void reset_stat()  {
  max_wspeed    = 0.0;
  min_wspeed    = 1000.0;
  acc_wspeed    = 0.0;
  cnt_ws_samples= 0  ;
  acc_wdX       = 0.0;
  acc_wdY       = 0.0;

}

/*
 * Computes avg wind speed
*/
float wspeed_avg()  {
    return(acc_wspeed/cnt_ws_samples);
}

/*
 * Computes avg wind direction
*/
int wdir_avg()  {
    int avgdir; 
    
    avgdir = int(atan2(acc_wdY,acc_wdX)*180/PI);
    if (avgdir<0)  avgdir += 360;
    
    return(avgdir);
}

