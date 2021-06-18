/*

Sigfox Wind Meter 
based on Peet Bros. (PRO) anemometer

Read the wind speed and direction interrupts from two input pins, compute wind
Send through Sigfox

Run on MKRFox1200

*/



#include <SigFox.h>
#include <math.h> 
#include "def.h"
#include <LiquidCrystal.h>

volatile unsigned long  timeOffset    ;       // offset of millis time to limit size of times variables

volatile unsigned long  speedLastPulseT  ;    // last time in millis the speed sensor triggers
volatile int            speedTimeArray[100];  // array to store times of speed sensor pulses
volatile int            speedHitCnt;          // counter of speed pulses
volatile unsigned long  dirLastPulseT  ;      // last time in millis the dir sensor triggers
volatile int            dirTimeArray[100];    // array to store times of dir sensor pulses
volatile int            dirHitCnt;            // counter of dir pulses
volatile unsigned long  last_sampleT  ;       // last time in millis a measure sampling has been done
volatile unsigned long  last_reportT  ;       // last time in millis a sigfox telegram has been sent

volatile int            max_wspeed    ;  // max wind speed over the sample period
volatile int            min_wspeed    ;  // min wind speed over the sample period
volatile int            cnt_ws_samples;  // count the number of samples we have to compute speed avg
volatile int            cnt_wd_samples;  // count the number of samples we have to compute dir avg
volatile int            acc_wspeed    ;  // wind speed accumulator to compute avg
volatile float          acc_wdX       ;  // accumulate wind dir projection on X axis (cos)
volatile float          acc_wdY       ;  // accumulate wind dir projection on X axis (sin)
volatile int            prevWindDir   ;  // memorize the last dir in case we cannot compute new one (speed too low)

volatile int            lognbr        ;  // number of logs
volatile bool           debugmode     ;  // 1=debug mode
volatile bool           lcd_en        ;  // lcd plugged or not

//
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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
  reset_stat();  
  prevWindDir      = -1;
  timeOffset       = millis();
  debugmode        = DEBUG;
  lcd_en           = LCD;
  
  // in debug mode we send back logs to the PC in Arduino IDE via Serial
  if(debugmode) {
    pinMode(Led, OUTPUT);
    digitalWrite(Led,HIGH); // blinks when starting in debug mode
    blinkLed(3,800);
    lcd.begin(16,2);        // used when LCD is plugged for reading the device
    lcd.print("Starting");
    Serial.begin(9600);     // 115200
    while(!Serial) {};
    lcd.clear();
    lcd.print("SWiM started 2000");    
    Serial.println("SWiM started 2000");
  }
  // slowing down to 1MHz
//  cpu_speed(CPU_DIVISOR);

}


void loop() {

  unsigned long now = millis();
  int    dt1;  // ellapsed time since last sample
  int    dt2;  // ellapsed time since last report
  int    ws;   // temporary wind speed
  int    wd;   // temporary wind dir angle
  
  if (now > 2419200) reboot(); // every 28 days to avoid managing millis reset after 2**32-1 ms
  dt1 = now - last_sampleT;
  dt2 = now - last_reportT;

  // if it's time to sample, get measures and store for stats
  if (dt1 > SAMPLING_PERIOD) {
    noInterrupts();
    timeOffset = millis();
    last_sampleT = now;
    ws = computeWindSpeed();
    wd = computeWindDir();
    store_for_stat(ws,wd);
    DebugLogMeas(ws,wd);
//    printArrays();   // debug purpose
    resetSampler();
    interrupts();
    blinkLed(2,100); // debug purpose
    
  }

  if (dt2 > REPORT_PERIOD) {
    last_reportT = now;
    // report the values
    ws = wspeed_avg();
    wd = wdir_avg();
    
    DebugLogAvgMeas(ws,wd);

    // send sigfox telegram
    
    
    // clear stats
    reset_stat();
  }



}

/*
 * Interrupt called when the rotating part of the wind instrument
 * has completed one rotation. Rising edge.
 *
*/
void isr_speed() {
  int now = millis();
  
  // avoid switch bounce to call this routine multiple times
  if ((now - speedLastPulseT) < DEBOUNCE_TIME) {
    return;
  }
  speedLastPulseT = now; 
  speedTimeArray[speedHitCnt] = now-timeOffset;
  speedHitCnt++;

}
/*
 * Interrupt called when the direction reed sensor triggers. Rising edge.
 *
*/
void isr_direction() {
  int now = millis();
  
  // avoid switch bounce to call this routine multiple times
  if ((now - dirLastPulseT) < DEBOUNCE_TIME) {
    return;
  }
  dirLastPulseT = now;   
  dirTimeArray[dirHitCnt] = now-timeOffset;
  dirHitCnt++;

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
 * Accessory to reset the sampler variables.
 *
*/
void resetSampler() {

  clearArrays();
  speedHitCnt = 0;
  dirHitCnt   = 0;
    
}


// /*
//  * Compute the wind speed and dir over the sample period.
//  *
//  * update the ws and wd global vars
// */
int computeWindSpeed() {
  
  float mph;
   
  // not enough pulses to compute wind
  if (speedHitCnt<2) return(0.0);
  
// Serial.print("speedHitCnt  ");
// Serial.print(speedHitCnt);
// Serial.println("");

  // rotation per seconds
  int dt = speedTimeArray[speedHitCnt-1]-speedTimeArray[0];
  float rps = 1000.0 * (float)(speedHitCnt-1) / (float)dt;
  
  // calibration formulas given by Peet Bros sensor vendor
  // calibration formulas given by Peet Bros sensor vendor
  if      (rps < 0.010)  mph = 0.0;
  else if (rps < 3.229)  mph = -0.1095*rps*rps + 2.9318*rps - 0.1412;
  else if (rps < 54.362) mph =  0.0052*rps*rps + 2.1980*rps + 1.1091; // 54rps is 216km/h !
  // useless: the sensor will be destroyed before this wind speed !
  //else if (rps < 66.332) mph =  0.1104*rps*rps - 9.5685*rps + 329.87;
  
  int kmh = int(mph * 1.609344);  
  return(kmh);
}


// /*
//  * Compute the wind dir over the sample period.
//  *
// */
int computeWindDir() {
  int   wdir=-1;         // wind dir in degree to return
  int   dp;              // time of wind dir pulse
  float acc_ratio = 0.0; // sum of dir/speed times ratio to average over the sampling period
  int   hitCnt      = 0; // nbr of elements in the average calc
  
  // not enough pulses to compute wind
  if (speedHitCnt<2) return(wdir);
  
  for (int i = 0; i < speedHitCnt-1; i++) {
    // look for a dir pulse comprised between two bounds of speed pulses
    dp=searchElem(speedTimeArray[i],speedTimeArray[i+1]);
  
    if (dp>0) {
        acc_ratio += (float)(dp - speedTimeArray[i]) / (float)(speedTimeArray[i+1]-speedTimeArray[i]);
        hitCnt++;
    }
  }
  if (!hitCnt) return(wdir); // not possible to compute wind direction
  
  wdir = int(360 * acc_ratio / hitCnt);

  return(wdir);
}

// /*
//  * Accessory to search in the Dir pulse times array
//  * a value between LL and UL
//  * 
// */
int searchElem(int LL, int UL) {

  for (int i=0; i<dirHitCnt; i++ ) {
    if (dirTimeArray[i]> UL)  return(0); // 
    if ((dirTimeArray[i] >= LL) && (dirTimeArray[i] <= UL)) {
        return(dirTimeArray[i]);
    }
  }
  return(0);
}


/*
 * Take measures and compute some statistics over a report period
 * min, max, accumulate for average, mean wind dir
 * 
*/
void store_for_stat(int ws, int wd)  {
  // wind speed in km/h
  // wind dir in degrees
  
  if (ws > max_wspeed)  max_wspeed = ws;
  if (ws < min_wspeed)  min_wspeed = ws;
  acc_wspeed += ws;
  cnt_ws_samples++;
  
  if (wd>-1)  {
    // projection of direction is weighted by the speed
    acc_wdX += ws * cos(PI*wd/180.);
    acc_wdY += ws * sin(PI*wd/180.);
    cnt_wd_samples++;
  }

}
/*
 * Resets all counters and accumulator when a report has been done
 * min, max, accumulate for average, mean wind dir
 * 
*/
void reset_stat()  {
  max_wspeed    = 0  ;
  min_wspeed    = 1000;
  acc_wspeed    = 0  ;
  cnt_ws_samples= 0  ;
  cnt_wd_samples= 0  ;
  acc_wdX       = 0.0;
  acc_wdY       = 0.0;

}

/*
 * Computes avg wind speed
*/
int wspeed_avg()  {
    return(int(acc_wspeed/cnt_ws_samples));
}

/*
 * Computes avg wind direction
*/
int wdir_avg()  {
    int avgdir; 
    
    // when no dir samples, keep the previous direction
    if (!cnt_wd_samples) return(prevWindDir);
    
    avgdir = int(atan2(acc_wdY,acc_wdX)*180/PI);
    if (avgdir<0)  avgdir += 360;
    prevWindDir = avgdir;   // store this in a globvar
    
    return(avgdir);
}


/*
 * Accessory to convert wind angle into N, S, W... directions
*/
String deg2dir(int wd) {
    if      (wd <22)  return(String("N"));
    else if (wd <67)  return(String("NE"));
    else if (wd <112) return(String("E"));
    else if (wd <157) return(String("SE"));
    else if (wd <202) return(String("S"));
    else if (wd <247) return(String("SW"));
    else if (wd <292) return(String("W"));
    else if (wd <337) return(String("NW"));
    else              return(String("N"));    
}


/*
 * system functions
*/
void reboot() {
  NVIC_SystemReset();
  while (1);
}

void cpu_speed(int divisor){
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(divisor) |         // Divide the 48MHz clock source by divisor 48: 48MHz/48=1MHz
                   GCLK_GENDIV_ID(0);            // Select Generic Clock (GCLK) 0
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization      
}
