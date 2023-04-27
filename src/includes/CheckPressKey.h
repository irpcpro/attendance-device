// check for press any key for settings
// 1 = finger print
// 2 = time setting
// 3 = show IP
void Check_Press_Key(){

    // if is setting key
    if( keypressed == '*' ){
        // type beep keypad
        Beep_keypress();
        // clear lcd to show time setting
        lcd.clear();
        lcd.print("     Setting");
        // set loading dots on display
        Set_Loading_Dots( 4, 1 );

        // clear lcd to show time setting
        lcd.clear();
        lcd.setCursor( 0, 0 );
        lcd.print("Enter Password");
        // set loading dots on display
        lcd.setCursor( 0, 1 );
        // start cursor blink
        lcd.blink();
        delay(100);

        // if keypad pressed
        for( int pass = 0; pass < 4; pass++ ){
            while( keypressed2 == NO_KEY ){
                keypressed2 = myKeypad.getKey();
                delay(100);
            }
            // type beep keypad
            Beep_keypress();
            PasswordIP[pass] = keypressed2;
            keypressed2 = NO_KEY;
            lcd.print("*");
        }
        lcd.noBlink();
        if( PasswordIP[0] == '0' && PasswordIP[1] == '7' && PasswordIP[2] == '1' && PasswordIP[3] == '8' ){
            // Success Password
            Beep_PasswordSeccuss();
            // time setting counter
            bool PageCount = true;
            // remove key pressed
            keypressed = NO_KEY;
            // clear lcd
            lcd.clear();
            // type settings
            lcd.setCursor(0,0); lcd.print("1 = FingerPrint");
            lcd.setCursor(0,1); lcd.print("2 = Time setting");
            while( keypressed != '1' && keypressed != '2' && keypressed != '3' && keypressed != '*' ){
                while( keypressed == 'A' || keypressed == 'D' ){
                    // clear lcd
                    lcd.clear();
                    // type beep keypad
                    Beep_keypress();
                    if( PageCount ){
                        lcd.setCursor(0,0); lcd.print("3 = Show IP");
                        lcd.setCursor(0,1); lcd.print("* = Exit");
                        PageCount = false;
                    }else{
                        lcd.setCursor(0,0); lcd.print("1 = FingerPrint");
                        lcd.setCursor(0,1); lcd.print("2 = Time setting");
                        PageCount = true;
                    }
                    // clear key
                    keypressed = NO_KEY;
                }
                // if get arrow type
                keypressed = myKeypad.getKey();
                delay(20);
            }
            // exit from setting
            if( keypressed == '*' ){
                lcd.clear();
                keypressed = NO_KEY;
                Beep_keypress();
                return;
            }
            delay(10);
            // type beep keypad
            Beep_keypress();
            // clear lcd
            lcd.clear();
            // ************** if key press 1, go to FingerPrint
            if( keypressed == '1' ){

                // if nfc is active, dont continue this action
                // IMPORTANT check this line to next develop *****
                if( ac_NFC == 2 ){
                    lcd.setCursor(0,0);lcd.print("fingerprint isnt");
                    lcd.setCursor(0,1);lcd.print("active");
                    keypressed = NO_KEY;
                    delay(3000);
                    return;
                }
                
                delay(200);
                // remove key pressed
                // IMPORTANT check this line to next develop *****
                keypressed = NO_KEY;

                // wait to get new option
                lcd.setCursor(0,0); lcd.print("1 = Add New 1");
                lcd.setCursor(0,1); lcd.print("2 = Add New many");
                bool FP_PageCount = true;
                // 1 = add new finger print
                // 2 = remove finger print
                //while( keypressed != '1' && keypressed != '2' && keypressed != '0' ){
                while( keypressed != '1' && keypressed != '2' && keypressed != '0' ){
                    // if get arrow type
                    keypressed = myKeypad.getKey();
                    // exit from setting
                    if( keypressed == '*' ){
                        lcd.clear();
                        keypressed = NO_KEY;
                        return;
                    }else if( keypressed == 'A' || keypressed == 'D' ){
                        if( FP_PageCount ){
                            lcd.clear();
                            lcd.setCursor(0,0); lcd.print("0 = remove all");
                            lcd.setCursor(0,1); lcd.print("* = Exit");
                            FP_PageCount = false;
                        }else{
                            lcd.clear();
                            lcd.setCursor(0,0); lcd.print("1 = Add New 1");
                            lcd.setCursor(0,1); lcd.print("2 = Add New many");
                            FP_PageCount = true;
                        }
                    }
                    delay(20);
                }
                delay(10);
                // type beep keypad
                Beep_keypress();
                // clear lcd
                lcd.clear();

                // get which option is
                if( keypressed == '1' || keypressed == '2' ){
                    bool multiFP = false;
                    if( keypressed == '2' ){
                        multiFP = true;
                        lcd.clear(); lcd.setCursor(0,0); lcd.print("* = for Exit");
                        lcd.setCursor(0,1); lcd.print("before reg fingr");
                        delay(3000);
                    }
                    // IMPORTANT check this line to next develop *****
                    keypressed = NO_KEY;

                    // ** Add New FingerPrint
                    int16_t fid;
                    if (get_free_id(&fid)){
                        do {
                            int16_t p = -1;
                            lcd.clear(); lcd.print("put valid finger");
                            lcd.setCursor(0,1);
                            while( p != FPM_OK ) {
                                // if get key to exit
                                keypressed = myKeypad.getKey();
                                // exit from setting
                                if( keypressed == '*' ){
                                    lcd.clear();
                                    keypressed = NO_KEY;
                                    Beep_keypress();
                                    return;
                                }
                                keypressed = NO_KEY;

                                p = finger.getImage();
                                switch (p) {
                                    case FPM_OK:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Image taken");
                                        break;
                                    case FPM_NOFINGER:
                                        lcd.print(".");
                                        break;
                                    case FPM_PACKETRECIEVEERR:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("comunicate error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    case FPM_IMAGEFAIL:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Imaging error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    case FPM_TIMEOUT:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Timeout!");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    case FPM_READ_ERROR:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("PID length error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    default:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Unknown error 21");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                }
                            }

                            // OK success!
                            p = finger.image2Tz(1);
                            lcd.clear();lcd.setCursor(0,0);
                            switch (p) {
                                case FPM_OK:
                                    lcd.print("Image converted");
                                    break;
                                case FPM_IMAGEMESS:
                                    lcd.print("Image too messy");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_PACKETRECIEVEERR:
                                    lcd.print("Comunicate error");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_FEATUREFAIL:
                                case FPM_INVALIDIMAGE:
                                    lcd.print("Could not find");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_TIMEOUT:
                                    lcd.print("Timeout!");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_READ_ERROR:
                                    lcd.print("PID length error");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                default:
                                    lcd.print("Unknown error");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                            }
                            delay(10);
                            lcd.clear();
                            Beep_keypress();
                            lcd.print("Remove finger");
                            delay(2500);
                            p = 0;
                            while (p != FPM_NOFINGER) {
                                p = finger.getImage();
                            }

                            p = -1;
                            lcd.clear(); lcd.setCursor(0,0);
                            lcd.print("put same finger");
                            lcd.setCursor(0,1);
                            while (p != FPM_OK) {
                                p = finger.getImage();
                                switch (p) {
                                    case FPM_OK:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Image taken");
                                        break;
                                    case FPM_NOFINGER:
                                        lcd.print(".");
                                        break;
                                    case FPM_PACKETRECIEVEERR:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Comunicate error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    case FPM_IMAGEFAIL:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Imaging error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    case FPM_TIMEOUT:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Timeout!");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        break;
                                    case FPM_READ_ERROR:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("PID length error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                    default:
                                        lcd.clear();lcd.setCursor(0,0);
                                        lcd.print("Unknown error");
                                        Beep_ErrorSetup();
                                        delay(3000);
                                        return;
                                        break;
                                }
                            }

                            // OK success!
                            p = finger.image2Tz(2);
                            switch (p) {
                                case FPM_OK:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("Image converted");
                                    break;
                                case FPM_IMAGEMESS:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("Image too messy");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_PACKETRECIEVEERR:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("Comunicate error");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_FEATUREFAIL:
                                case FPM_INVALIDIMAGE:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("Could not find");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_TIMEOUT:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("Timeout!");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                case FPM_READ_ERROR:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("PID length error");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                                default:
                                    lcd.clear();lcd.setCursor(0,0);
                                    lcd.print("Unknown error");
                                    Beep_ErrorSetup();
                                    delay(3000);
                                    return;
                                    break;
                            }


                            // OK converted!
                            p = finger.createModel();
                            if (p == FPM_OK) {
                                lcd.clear();lcd.setCursor(0,0);
                                lcd.print("Prints matched!");
                            } else if (p == FPM_PACKETRECIEVEERR) {
                                lcd.clear();lcd.setCursor(0,0);
                                lcd.print("Comunicate error");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_ENROLLMISMATCH) {
                                lcd.clear();lcd.setCursor(0,0);
                                lcd.print("Finger not match");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_TIMEOUT) {
                                lcd.clear();lcd.setCursor(0,0);
                                lcd.print("Timeout!");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_READ_ERROR) {
                                lcd.clear();lcd.setCursor(0,0);
                                lcd.print("PID length error");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else {
                                lcd.clear();lcd.setCursor(0,0);
                                lcd.print("Unknown error");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            }

                            // to file h joda bezar - code ro kamel kon
                            lcd.clear();lcd.setCursor(0,0);
                            p = finger.storeModel(fid);
                            if (p == FPM_OK) {
                                Beep_PasswordSeccuss();
                                lcd.setCursor(0,0);
                                lcd.print("Stored!");
                                lcd.setCursor(0,1);
                                lcd.print("ID = "); lcd.print(fid);
                                LED_Green();
                            } else if (p == FPM_PACKETRECIEVEERR) {
                                lcd.print("Comunicate error");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_BADLOCATION) {
                                lcd.print("Could not store");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_FLASHERR) {
                                lcd.print("Eror write flash");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_TIMEOUT) {
                                lcd.print("Timeout!");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else if (p == FPM_READ_ERROR) {
                                lcd.print("PID length error!");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            } else {
                                lcd.print("Unknown error");
                                Beep_ErrorSetup();
                                delay(3000);
                                return;
                            }
                            delay(2200); lcd.clear();
                        } while( multiFP );

                    }else{
                        lcd.print('not enough space');
                        Beep_ErrorSetup();
                        delay(3000);
                    }
                }else if( keypressed == '0' ){
                    // IMPORTANT check this line to next develop *****
                    // get confirmation for remove all fingerprint
                    keypressed = NO_KEY;
                    int counterRemoveFP = 0;
                    lcd.clear(); lcd.setCursor(0,0); lcd.print("Enter 1 to 9");
                    lcd.setCursor(0,1); lcd.print("* = Exit");
                    while( counterRemoveFP != 9 ){
                        keypressed = myKeypad.getKey();
                        // exit from setting
                        if( keypressed == '*' ){
                            lcd.clear();
                            keypressed = NO_KEY;
                            Beep_keypress();
                            return;
                        }else if( keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || 
                                  keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9' ){
                            Beep_keypress();
                            counterRemoveFP++;
                        }
                    }
                    // second confirmation
                    keypressed = NO_KEY;
                    lcd.clear(); lcd.setCursor(0,0); lcd.print("Enter 5 to clear");
                    lcd.setCursor(0,1); lcd.print("* = Exit");
                    while( keypressed != '5' ){
                        keypressed = myKeypad.getKey();
                        // exit from setting
                        if( keypressed == '*' ){
                            lcd.clear();
                            keypressed = NO_KEY;
                            Beep_keypress();
                            return;
                        }
                    }

                    Beep_keypress(); delay(200);
                    // remove all finger prints
                    int16_t p = finger.emptyDatabase();
                    lcd.clear();lcd.setCursor(0,0);
                    // return result
                    if( p == FPM_OK ) {
                        Beep_PasswordSeccuss();
                        lcd.setCursor(0,0);
                        lcd.print("All deleted");
                        LED_Green();
                        delay(500);lcd.clear();
                    } else if (p == FPM_PACKETRECIEVEERR) {
                        lcd.print("Comunicate error");
                        Beep_ErrorSetup();
                        delay(3000);
                        return;
                    } else if (p == FPM_DBCLEARFAIL) {
                        Serial.println("cant delete all");
                        Beep_ErrorSetup();
                        delay(3000);
                        return;
                    } else if (p == FPM_TIMEOUT) {
                        Serial.println("Timeout!");
                        Beep_ErrorSetup();
                        delay(3000);
                        return;
                    } else if (p == FPM_READ_ERROR) {
                        Serial.println("PID length error");
                        Beep_ErrorSetup();
                        delay(3000);
                        return;
                    } else {
                        Serial.println("Unknown error");
                        Beep_ErrorSetup();
                        delay(3000);
                        return;
                    }
                }
                // remove key pressed
                keypressed = NO_KEY;
            }
            
            // ************** if key press 2, go to time setting
            if( keypressed == '2' ){
                // clear lcd to show time setting
                lcd.clear();
                lcd.print("  Time Setting");
                // set loading dots on display
                Set_Loading_Dots( 4, 1 );
                // clear lcd
                lcd.clear();
                // show default time
                lcd.setCursor(0,0);lcd.print("Enter AD Time:");
                lcd.setCursor(0,1);lcd.print("2020/01/20 11:45");

                // start cursor blink
                lcd.blink();
                for( int c = 0; c < 12; c++ ){
                    lcd.setCursor( PositionTimeChar[ c ], 1 );
                    //char keypressed2 = myKeypad.waitForKey();
                    while(
                        ( keypressed2 == NO_KEY || keypressed2 == '*' || keypressed2 == '#' || keypressed2 == 'A' || keypressed2 == 'B' || keypressed2 == 'C' || keypressed2 == 'D' ) ||
                        ( c == 0 && keypressed2 != '2' && keypressed2 != '3' ) ||
                        ( c == 4 && keypressed2 != '0' && keypressed2 != '1' ) ||
                        ( c == 5 && TimeSetting[ 4 ] == '1' && keypressed2 != '0' && keypressed2 != '1' && keypressed2 != '2' ) ||
                        ( c == 6 && keypressed2 != '0' && keypressed2 != '1' && keypressed2 != '2' && keypressed2 != '3' ) ||
                        ( c == 7 && TimeSetting[ 6 ] == '3' && keypressed2 != '0' &&  keypressed2 != '1' ) ||
                        ( c == 8 && keypressed2 != '0' && keypressed2 != '1' && keypressed2 != '2' ) ||
                        ( c == 9 && TimeSetting[ 8 ] == '2' && keypressed2 != '0' && keypressed2 != '1' && keypressed2 != '2' && keypressed2 != '3' ) ||
                        ( c == 10 && keypressed2 != '0' && keypressed2 != '1' && keypressed2 != '2' && keypressed2 != '3' && keypressed2 != '4' && keypressed2 != '5'  )
                    ){
                        // if keypad pressed
                        keypressed2 = myKeypad.getKey();                
                        delay(100);
                    }
                    // type beep keypad
                    Beep_keypress();
                    // lcd print number
                    lcd.print( keypressed2 );
                    // save the number
                    TimeSetting[ c ] = keypressed2;
                    keypressed2 = NO_KEY;
                }
                // set time numbers
                Set_Time_Numbers();
                // clear lcd
                lcd.clear();
                // stop blink lcd character
                lcd.noBlink();
                // print lcd "done !"
                lcd.print( "done !" );
                // Startup Beep sound
                Beep_StartSetup();
            }

            // ************** if key press 3, go to Show IP
            if( keypressed == '3' ){
                lcd.setCursor(0,0);
                lcd.print(IPServer[0]);lcd.print(":");
                lcd.print(IPServer[1]);lcd.print(":");
                lcd.print(IPServer[2]);lcd.print(":");
                lcd.print(IPServer[3]);lcd.print(":");
                lcd.print(PortServer);
                lcd.setCursor(0,1);
                lcd.print("A = Done");
                while( keypressed2 != 'A' ){
                    keypressed2 = myKeypad.getKey();
                    delay(100);
                }
                // type beep keypad
                Beep_keypress();
                // clear keypad pressed
                keypressed2 = NO_KEY;
                // clear lcd
                lcd.clear();
            }

            // exit from setting
            if( keypressed == '*' ){
                lcd.clear();
                keypressed = NO_KEY;
                return;
            }

            // delay
            delay(1000);
        }else{
            // clear lcd and set cursor
            lcd.clear();lcd.setCursor(0,0);
            // print error on lcd
            lcd.print("Access denied !");
            // error setuo sound
            Beep_ErrorSetup();
            // delay
            delay(1500);
        }

    }

    // remove key pressed
    keypressed = NO_KEY;
}
