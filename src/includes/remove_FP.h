int deleteFingerprint(int fid) {
    int p = -1;
    p = finger.deleteModel(fid);

    lcd.clear();
    if (p == FPM_OK) {
        Beep_PasswordSeccuss();
        lcd.setCursor(0,0);
        lcd.print("Deleted!");
        LED_Green();
    } else if (p == FPM_PACKETRECIEVEERR) {
        lcd.print("Comunicate error");
        Beep_ErrorSetup();
        delay(500);
        return p;
    } else if (p == FPM_BADLOCATION) {
        lcd.print("Could not delete");
        Beep_ErrorSetup();
        delay(500);
        return p;
    } else if (p == FPM_FLASHERR) {
        lcd.print("Eror write flash");
        Beep_ErrorSetup();
        delay(500);
        return p;
    } else if (p == FPM_TIMEOUT) {
        lcd.print("Timeout!");
        Beep_ErrorSetup();
        delay(500);
        return p;
    } else if (p == FPM_READ_ERROR) {
        lcd.print("PID length error");
        Beep_ErrorSetup();
        delay(500);
        return p;
    } else {
        lcd.print("Unknown error 0x"); lcd.print(p, HEX);
        Beep_ErrorSetup();
        delay(500);
        return p;
    }
    delay(3000);
}