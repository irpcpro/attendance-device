// set loading dots on display
void Set_Loading_Dots( int letter, int row ){
    int DotDelay = 100;
    lcd.setCursor( letter, row );
    lcd.print(".");delay(DotDelay);
    lcd.print(".");delay(DotDelay);
    lcd.print(".");delay(DotDelay);
    lcd.print(".");delay(DotDelay);
    lcd.print(".");delay(DotDelay);
    lcd.print(".");delay(DotDelay);
    lcd.print(".");delay(DotDelay);
    lcd.print(".");
}
