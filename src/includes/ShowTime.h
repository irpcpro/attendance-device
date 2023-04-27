// get and show time
void Show_Time(){
    // get time from module
    Date now( rtc.getTime().year, rtc.getTime().mon, rtc.getTime().date );
    // set variables
    JY = now.JYear();
    JD = now.JMonth();
    JM = now.JDay();
    hours   = rtc.getTime().hour;
    minutes = rtc.getTime().min;

    //Serial.print("taghvim shamsi:");Serial.print(JY);Serial.print("/");Serial.print(JM);Serial.print("/");Serial.print(JD);Serial.print(" - ");Serial.println( rtc.getTimeStr() );
    lcd.setCursor(0, 0);
    lcd.print(JY);lcd.print("/");
    if( JM < 10 )lcd.print("0"); lcd.print(JM);lcd.print("/");
    if( JD < 10 )lcd.print("0"); lcd.print(JD);
    // lcd.print(" ");
    // lcd.print(" ");lcd.print(" ");
    // lcd.print(" ");lcd.print(" ");
    // lcd.print(" ");
    
    lcd.print(" ");
    if( hours < 10 )lcd.print("0"); lcd.print( hours );lcd.print(":");
    if( minutes < 10 )lcd.print("0"); lcd.print( minutes );

    // lcd.setCursor(0, 1);
    
    // lcd.print( rtc.getTimeStr() );lcd.print(" ");lcd.print(" ");lcd.print(" ");lcd.print(" ");lcd.print(" ");lcd.print(" ");lcd.print(" ");
    // lcd.print( rtc.getTimeStr() );
    // lcd.print(" ");
    // lcd.print( rtc.getTime().hour );
    // lcd.print( rtc.getTime().min );
    // lcd.print( rtc.getTime().sec );
}
