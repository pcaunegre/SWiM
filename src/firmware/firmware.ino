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


volatile unsigned long  speedLastPulseT  ;    // last time in millis the speed sensor triggers
volatile unsigned long  speedTimeArray[100];  // array to store times of speed sensor pulses
volatile int            speedHitCnt;          // counter of speed pulses
volatile unsigned long  dirLastPulseT  ;      // last time in millis the dir sensor triggers
volatile unsigned long  dirTimeArray[100];    // array to store times of dir sensor pulses
volatile int            dirHitCnt;            // counter of dir pulses
volatile unsigned long  last_sampleT  ;       // last time in millis a measure sampling has been done
volatile unsigned long  last_reportT  ;       // last time in millis a sigfox telegram has been sent



volatile int            lognbr;               // number of logs


// volatile unsigned long  first_spulseT ;  // first time in millis the speed sensor triggers
// volatile unsigned long  last_spulseT  ;  // last time in millis the speed sensor triggers
// volatile int            scount        ;  // counter of speed sensor pulse
// 
// volatile int            dcount        ;  // counter of wind dir ratio accumulated in acc_ratio
// volatile unsigned int   ddeltaT       ;  // delta time between dir pulse and last speed pulse
// volatile float          acc_ratio     ;  // wind dir ratio accumulater over the sample period
// 
// volatile float          max_wspeed    ;  // max wind speed over the sample period
// volatile float          min_wspeed    ;  // min wind speed over the sample period
// volatile int            cnt_ws_samples;  // count the number of samples we have to compute speed avg
// volatile float          acc_wspeed    ;  // wind speed accumulator to compute avg
// volatile double         acc_wdX       ;  // accumulate wind dir projection on X axis (cos)
// volatile double         acc_wdY       ;  // accumulate wind dir projection on X axis (sin)
// volatile int            prev_winddir  ;  // memory of previous direction


void setup() {

  // pins to sensor
  pinMode(pinSpeed, INPUT); // yellow wire of Peet Bros cable
  pinMode(pinDir  , INPUT); // green  wire
  
  // install interruptions functions
  attachInterrupt(digitalPinToInterrupt(pinSpeed), isr_speed    , FALLING);
  attachInterrupt(digitalPinToInterrupt(pinDir)  , isr_direction, FALLING);

  // init variables
  speedLastPulseT  = 0;
  dirLastPulseT    = 0;
  speedHitCnt      = 0;
  dirHitCnt        = 0;
  clearArrays();
  lognbr           = 0;
  
  
//   scount        = 0;
//   dcount        = 0;
//   acc_ratio     = 0.0;
//   last_sampleT  = millis();
//   last_reportT  = millis();
//   prev_winddir  = -1;
// 
//   reset_stat();  
 
  // in debug mode we send back logs to the PC in Arduino IDE via Serial
  if(DEBUG) {
    pinMode(Led, OUTPUT);
    digitalWrite(Led,HIGH); // blinks when starting in debug mode
    blinkLed(3,800);
    Serial.begin(9600); // 115200
    while(!Serial) {};
    Serial.println("SWiM started 1533");
  }
}


void loop() {

  unsigned long now = millis();
  unsigned int  dt1;  // ellapsed time since last sample
  unsigned int  dt2;  // ellapsed time since last report
  float         ws;   // temporary wind speed
  int           wd;   // temporary wind dir angle
  
  if (now < last_sampleT)  dt1 = now + (ULONG_MAX - last_sampleT);
  else                     dt1 = now - last_sampleT;

  if (now < last_reportT)  dt2 = now + (ULONG_MAX - last_reportT);
  else                     dt2 = now - last_reportT;

  // if it's time to sample, get measures and store for stats
  if (dt1 > SAMPLING_PERIOD) {
    noInterrupts();
    last_sampleT = now;
//     ws = wspeed_sample();  
//     wd = wdir_sample();
//     store_for_stat(ws,wd);
    printArrays();
    resetSampler();
    clearArrays();
    interrupts();
    blinkLed(2,500);
    
//     DebugLogMeas(ws,wd);
  }

//   if (dt2 > REPORT_PERIOD) {
//     last_reportT = now;
//     // report the values
//     ws = wspeed_avg();
//     wd = wdir_avg();
//     
//     DebugLogAvgMeas(ws,wd);
// 
//     // send sigfox telegram
//     
//     
//     // clear stats
//     reset_stat();
//   }



}

/*
 * Accessory to log data via Serial when in debug mode
*/
void DebugLogMeas(float ws, int wd) {
  if(DEBUG) {
    lognbr++;
    Serial.print("LOG: ");
    Serial.println(lognbr);
    Serial.print("Speed: ");
    Serial.println(ws);
    Serial.print("Direction: ");
    Serial.println(wd);
    
    Serial.println("----------------");
  }
}
///*
// * Accessory to log data via Serial when in debug mode
//*/
//void DebugLogAvgMeas(float ws, int wd) {
//  if(DEBUG) {
//    Serial.print("Average Speed : ");
//    Serial.print(ws);
//    Serial.println("");
//    Serial.print("Average Direction : ");
//    Serial.print(wd);
//    Serial.println("");
//    Serial.print("Nombre of samples :");
//    Serial.print(cnt_ws_samples);
//    Serial.println("");
//    Serial.print("Sampling period :");
//    Serial.print(REPORT_PERIOD);
//    Serial.println("");
//  }
//}

/*
 * Interrupt called when the rotating part of the wind instrument
 * has completed one rotation. Rising edge.
 *
*/
void isr_speed() {
  unsigned long now = millis();
  
  // avoid switch bounce to call this routine multiple times
  if ((now - speedLastPulseT) < DEBOUNCE_TIME) {
    return;
  }
  speedTimeArray[speedHitCnt] = now;
  speedHitCnt++;
  speedLastPulseT = now; 

}
/*
 * Interrupt called when the direction reed sensor triggers. Rising edge.
 *
*/
void isr_direction() {
  unsigned long now = millis();
  
  // avoid switch bounce to call this routine multiple times
  if ((now - dirLastPulseT) < DEBOUNCE_TIME) {
    return;
  }
  dirTimeArray[dirHitCnt] = now;
  dirHitCnt++;
  dirLastPulseT = now; 

}
/*
 * Accessory to reset the arrays.
 *
*/
void clearArrays() {
  for (int i = 0; i < ARRAYLEN; i++) {
    speedTimeArray[i]=0;
    dirTimeArray[i]=0;
  }
}
/*
 * Accessory to print the arrays.
 *
*/
void printArrays() {
  Serial.print("SPEED HITS : ");
  Serial.println(speedHitCnt);
  for (int i = 0; i < speedHitCnt; i++) {
    Serial.println(speedTimeArray[i]);
  }
  Serial.print("DIR HITS : ");
  Serial.println(dirHitCnt);
  for (int i = 0; i < dirHitCnt; i++) {
    Serial.println(dirTimeArray[i]);
  }
  Serial.println("--------------------------");
}
/*
 * Accessory to reset the sampler variables.
 *
*/
void resetSampler() {

  clearArrays();
  speedHitCnt = 0;
  dirHitCnt   = 0;
    
}

/*
 * Accessory to blink the led for debug
 * blink a nbr of times with a blinking period in ms
*/
void blinkLed(int times, int period) {

  if (!DEBUG) return;
  
  for (int i=0; i<times; i++ ) {
    digitalWrite(Led,HIGH);
    delay(period);
    digitalWrite(Led,LOW);
    delay(period);
  }
 
}

// /*
//  * Compute the wind speed over the sample period.
//  *
//  * Reset the counters
//  * 
// */
// float wspeed_sample() {
//   float mph = NAN;
//   float kmh = NAN;
//   float rps; // rotation per seconds
//   unsigned int delta; // number of millis over a number of pulses
//   
//   // take care of millis counter back to 0 (after 50 days)
//   if (last_spulseT < first_spulseT) delta = last_spulseT + (ULONG_MAX - first_spulseT);
//   else                              delta = last_spulseT - first_spulseT;
//   
//   Serial.println("wspeed_sample inputs");
//   Serial.println(delta);
//   Serial.println(scount);
//   
//   // no pulse occured, first hit gives scount=2; 15ms is the delta max
//   if ((scount < 2) || (delta < 15)) {
//     rps = 0.0;
//     Serial.println("no pulse detected");
//     Serial.println("");
//   
//   } else  { 
//     rps = 1000.0 * (float)(scount-1) / (float)delta;
//   
//   }
// 
//   // calibration formulas given by Peet Bros sensor vendor
//   if      (rps < 0.010)  mph = 0.0;
//   else if (rps < 3.229)  mph = -0.1095*rps*rps + 2.9318*rps - 0.1412;
//   else if (rps < 54.362) mph =  0.0052*rps*rps + 2.1980*rps + 1.1091;
//   else if (rps < 66.332) mph =  0.1104*rps*rps - 9.5685*rps + 329.87;
//   
//   kmh = mph / 1.609344;
//   
//   scount = 0;
//   return(kmh);
// 
// }
// 
// 
// /*
//  * Compute the wind dir in degrees (0-359) over the sample period.
//  *
//  * Reset the counters
//  * 
// */
// int wdir_sample() {
//   int windangle;
// 
//   Serial.println("wdir_sample inputs");
//   Serial.println(acc_ratio);
//   Serial.println(dcount);
//   
//   if (dcount==0) {
//     Serial.println("No hit for dir");
//     windangle = prev_winddir;
//   
//   } else {
//     // compute the mean of ratio over a number of hits
//     windangle = int((360 * acc_ratio) / dcount);
//   
//   } 
//   
//   // reset the ctr of values in acc_ratio
//   ddeltaT = 0;
//   dcount  = 0;
//   acc_ratio = 0.0;
//   return(windangle);
// }
// 
// /*
//  * Take measures and compute some statistics over a report period
//  * min, max, accumulate for average, mean wind dir
//  * 
// */
// void store_for_stat(float ws, int wd)  {
//   // wind speed in km/h
//   // wind dir in degrees
//   
//   if (ws > max_wspeed)  max_wspeed = ws;
//   if (ws < min_wspeed)  min_wspeed = ws;
//   acc_wspeed += ws;
//   cnt_ws_samples++;
//   
//   // projection of direction is weighted by the speed
//   acc_wdX += ws * cos(PI*wd/180.);
//   acc_wdY += ws * sin(PI*wd/180.);
// 
// }
// /*
//  * Resets all counters and accumulator when a report has been done
//  * min, max, accumulate for average, mean wind dir
//  * 
// */
// void reset_stat()  {
//   max_wspeed    = 0.0;
//   min_wspeed    = 1000.0;
//   acc_wspeed    = 0.0;
//   cnt_ws_samples= 0  ;
//   acc_wdX       = 0.0;
//   acc_wdY       = 0.0;
// 
// }
// 
// /*
//  * Computes avg wind speed
// */
// float wspeed_avg()  {
//     return(acc_wspeed/cnt_ws_samples);
// }
// 
// /*
//  * Computes avg wind direction
// */
// int wdir_avg()  {
//     int avgdir; 
//     
//     avgdir = int(atan2(acc_wdY,acc_wdX)*180/PI);
//     if (avgdir<0)  avgdir += 360;
//     
//     return(avgdir);
// }
// void reboot() {
//   NVIC_SystemReset();
//   while (1);
// }
