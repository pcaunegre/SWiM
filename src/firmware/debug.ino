/*
*  Various functions to help seeing data during dev
*/

/*
 * Accessory to blink the led for debug
 * blink a nbr of times with a blinking period in ms
*/
void blinkLed(int times, int period) {

  for (int i=0; i<times; i++ ) {
    digitalWrite(Led,HIGH);
    delay(period);
    digitalWrite(Led,LOW);
    delay(period);
  }
 
}

/*
 * Accessory to log infos at init when in debug mode 
*/
void DebugInit( ) {
    
  if (lcd_en) { 
    lcd.begin(16,2);        // used when LCD is plugged for reading the device
    lcd.clear();
    lcd.print("Starting 4");
  }
  if(debugmode) {
    blinkLed(3,800/CPU_SLOW);
    Serial.begin(9600);     // 115200
    int waiting=0;
    while(!Serial && waiting<5) {delay(1000); waiting++;}
  }
  if (lcd_en) { 
    lcd.clear();
    lcd.print("SWiM started 2002");
  }
  if (Serial) Serial.println("SWiM started 2002");

}

/*
 * Accessory to log data via Serial when in debug mode
*/
void DebugLogMeas(int ws, int wd) {

  if(debugmode || lcd_en) {
    lognbr++;
    if (Serial){
      Serial.print("LOG: ");
      Serial.print(lognbr);
      Serial.print("   Speed: ");
      Serial.print(ws);
      Serial.print("   Direction: ");
      Serial.print(wd);
      Serial.print(" ");
      Serial.print(deg2dir(wd));
      Serial.print("   samples: ");
      Serial.print(speedHitCnt);
      Serial.print(" , ");
      Serial.print(dirHitCnt);
      Serial.println("");
    }
    if (lcd_en)  {
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print(lognbr);lcd.print(" ");
      lcd.print("Vi=")  ;lcd.print(ws);lcd.print(" ");
      lcd.print("Di=");lcd.print(deg2dir(wd));
    }
  }
}

/*
* Accessory to log data via Serial when in debug mode
*/
void DebugLogAvgMeas(int ws, int wd) {

  if(debugmode || lcd_en) {
    repnbr++;
    if(Serial) {
      Serial.print("Average Speed : ");
      Serial.print(ws);
      Serial.println("");
      Serial.print("Average Direction : ");
      Serial.print(wd);
      Serial.print(" ");
      Serial.print(deg2dir(wd));
      Serial.println("");
      Serial.print("Nombre of samples : ");
      Serial.print(cnt_ws_samples);
      Serial.print("  ");
      Serial.print(cnt_wd_samples);
      Serial.println("");
      Serial.println("====================");
    }
    if (lcd_en)  {
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(repnbr);lcd.print(" ");
      lcd.print("Vm=");lcd.print(ws);lcd.print(" ");
      lcd.print("Dm=");lcd.print(deg2dir(wd));
    }
  }
}

/*
* Simulate the formatted data sending
*/
void DebugSimSigfoxSend() {

  if(!debugmode) return;
  if(Serial) {
    Serial.println("===== Sigfox telegram ========");
    for (int i=0; i<2; i++) {
      Serial.print(i);
      Serial.print(" - Min Speed : ");
      Serial.print(msg.speedMin[i]);
      Serial.print("  Avg Speed: ");
      Serial.print(msg.speedAvg[i]);
      Serial.print("  Max Speed: ");
      Serial.print(msg.speedMax[i]);
      Serial.print("  Avg Direction : ");
      Serial.print(msg.directionAvg[i]);
      Serial.println("");
    }
  }
}

/*
 * Accessory to convert wind angle into N, S, W... directions
*/
String deg2dir(int wd) {
    if      (wd <0)   return(String("-"));
    else if (wd <22)  return(String("N"));
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
 * Accessory to print the arrays. Only in debug mode.
 *
*/
void printArrays() {

  if (!debugmode) return;

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
