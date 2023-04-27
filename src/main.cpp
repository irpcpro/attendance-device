//**** Arduino Settings ****
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
char *DeviceVersion = "2.0.0";


//**** TIME ****//
#include <Wire.h> // For the i2c devices
#include <DS3231.h> // time module
DS3231  rtc(A4, A5); // set time pins - rtc(SDA, SCL);
#include <JalaliDateLib.h> // jalali time library
#define DS3231_I2C_ADDRESS 104 // RTC is connected, address is Hex68 (Decimal 104)
// set time variables
int seconds, minutes, hours, date, month, year;
int jday, jmonth, jyear;
int TimeSetting[12];
int PositionTimeChar[] = {0,1,2,3,5,6,8,9,11,12,14,15};
String jdate;
int JY,JM,JD;


//**** LCD ****//
#include <LiquidCrystal.h> // LCD library
// const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // lcd pins
const int rs = 33, en = 32, d4 = 34, d5 = 35, d6 = 36, d7 = 37; // lcd pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//**** KEYPAD ****//
#include <Keypad.h>
const byte numRows= 4;
const byte numCols = 4;
char keymap[numRows][numCols]= {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[numRows] = {22,24,26,28}; //Rows 0 to 3 
byte colPins[numCols]= {23,25,27,29}; //Columns 0 to 3
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
char keypressed, keypressed2;


//**** Ethernet + SD ****//
#include <Ethernet.h>
#include <SD.h>
#include <Arduino_JSON.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int IPServer[] = {192, 168, 1, 177};
int PortServer = 93;
IPAddress ip( IPServer[0], IPServer[1], IPServer[2], IPServer[3] );
EthernetServer server( PortServer );
File SD_File, SD_TMP_File;
//JSONVar GetData;
char PasswordIP[4];


//**** NFC ****//
#include <SPI.h>
#include <Adafruit_PN532.h>
#define PN532_IRQ   (38)
#define PN532_RESET (39)
Adafruit_PN532 nfc( PN532_IRQ, PN532_RESET );
char outNumberCart[100];
int checkEthernetTime = 0;


//**** FingerPrint ****//
#include <SoftwareSerial.h>
#include <FPM.h>
//SoftwareSerial fserial(2, 3);
FPM finger(&Serial1);
FPM_System_Params params;
int search_database(void);
bool get_free_id(int16_t * fid);
int deleteFingerprint(int fid);
int ac_NFC = 2;



//**** Buzzer ****//
int Buzzer = 40;


// LED Errors
int LED_Green_pin = 31;
int LED_Red_pin = 30;

// Fan
int pin_fan = 42;

// Setup NFC & FingerPrint
int Sit_NFC_out = 41;
int Sit_NFC_in = 43;

// include files and functions
#include "./includes/LED_Errors.h" // LED Errors Functions
#include "./includes/Beeps.h" // all sound of buzzer
#include "./includes/Herlpers.h" // all helper functions
#include "./includes/ShowTime.h" // get and show time
#include "./includes/LoadingDots.h" // set loading dots on display
#include "./includes/SetTime.h" // set time numbers
#include "./includes/CheckPressKey.h" // check for press any key for settings
#include "./includes/SD_Fiels.h" // SD Function & Setting
#include "./includes/EthernetCheck.h" // check Ethernet for client get export
#include "./includes/enroll_FP.h" // get free id from module fingerprint
#include "./includes/remove_FP.h" // remove fingerprint by Finger ID
//#include "PersianError.h" // persian errors


// Portotype Functions
void LED_Green(); // LED Error Green
void LED_Red(); // LED Error Red
void LED_Green_ON(); // LED Green On
void LED_Green_OFF(); // LED Green Off
void LED_Red_ON(); // LED Red On
void LED_Red_OFF(); // LED Red Off
void Beep_StartSetup(); // Startup Beep sound
void Beep_PasswordSeccuss(); // password success Beep sound
void Beep_ErrorSetup(); // error setuo sound
void Beep_CardSuccess(); // card success
void Beep_CardError(); // card error
void Beep_keypress(); // type beep keypad
void Show_Time(); // get and show time
void Check_Press_Key(); // check for press any key for settings
void Set_Loading_Dots( int letter, int row ); // set loading dots on display
void Set_Time_Numbers(); // set time numbers
int ConvertAsciiToInt( char val ); // convert ascii to int
int concat(int a, int b); // get numbers
void Ethernet_Check(); // check Ethernet for client get export
void Get_SD_Details( EthernetClient client ); // get data from SD cart
void StoreInSD( char Data ); // store in SD
void Server_Response_Client_GET( EthernetClient client ); // show Client Server To Method Get
void Server_Response_Client_POST( EthernetClient client ); // sent tot Client export of Data
char *Get_Card_Number( int num[] ); // get cart number from array data NFC
//void Setup_Persian_Char(); // setup persian character to lcd


//**** Setup Hardware ****//
void setup(){
    //* Buzzer *//
    pinMode( Buzzer, OUTPUT );
    Beep_StartSetup();

    // Setup NFC & FP
    pinMode( Sit_NFC_out, OUTPUT );
    delay(150);
    pinMode( Sit_NFC_in, INPUT );
    delay(150);
    digitalWrite( Sit_NFC_out, HIGH );
    delay(150);
    // check Pins are Low or High
    if( digitalRead( Sit_NFC_in ) == HIGH )
        ac_NFC = 3;
    else
        ac_NFC = 2;

    // LED Errors
    pinMode( LED_Red_pin, OUTPUT );
    pinMode( LED_Green_pin, OUTPUT );
    // set LOW to LEDs
    digitalWrite( LED_Red_pin, LOW );
    digitalWrite( LED_Green_pin, LOW );
    // fan
    pinMode( pin_fan, OUTPUT );

    // ** REMOVE THIS ** //
    if( ac_NFC == 3 ){
        digitalWrite( LED_Red_pin, LOW );
        delay(300);
        digitalWrite( LED_Red_pin, HIGH );
        delay(300);
        digitalWrite( LED_Red_pin, LOW );
        delay(300);
        digitalWrite( LED_Red_pin, HIGH );
        delay(300);
        digitalWrite( LED_Red_pin, LOW );
        delay(300);
    }else{
        digitalWrite( LED_Green_pin, LOW );
        delay(300);
        digitalWrite( LED_Green_pin, HIGH );
        delay(300);
        digitalWrite( LED_Green_pin, LOW );
        delay(300);
        digitalWrite( LED_Green_pin, HIGH );
        delay(300);
        digitalWrite( LED_Green_pin, LOW );
        delay(300);
    }
    delay(1000);

    //* Time *//
    Wire.begin();
    rtc.begin();

    //* Serial *//
    Serial.begin(9600);

    delay(10);

    //* LCD *//
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.setCursor(0,0);
    lcd.print("Please wait ..");
    lcd.setCursor(0,1);
    lcd.print("System startup");
    // setup persian character to lcd
    //Setup_Persian_Char();

    delay(10);

    //* NFC *//
    if( ac_NFC == 3 ){
        nfc.begin();
        uint32_t versiondata = nfc.getFirmwareVersion();
        if( !versiondata ){
            Beep_ErrorSetup();
            lcd.setCursor(0,0); lcd.print("Cant find NFC");
            while(true){
                delay(1);
            }
        }
        nfc.SAMConfig(); // configure board to read RFID tags
    }else{
        //* FingerPrint *//
        Serial1.begin(57600);
        // Password of fingerprint module = 1375
        if ( finger.begin() ) {
            //finger.setParam( FPM_FRR_5, 1 );
            finger.readParams(&params);
            Serial.print("Capacity: "); Serial.println(params.capacity);
            Serial.print("Packet length: "); Serial.println(FPM::packet_lengths[params.packet_len]);
        } else {
            Beep_ErrorSetup();
            lcd.clear();
            lcd.setCursor(0,0); lcd.print("didn't find"); lcd.setCursor(0,1); lcd.print("Finger print");
            Serial.println("Did not find fingerprint sensor :(");
            while (1) yield();
        }
    }

    delay(10);

    //* Ethernet + SD *//
    SD.begin( 4 );
    Ethernet.begin(mac, ip);
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        lcd.clear();
        Beep_ErrorSetup();
        lcd.setCursor(0,0); lcd.print("Ethernet module");
        lcd.setCursor(0,1); lcd.print("not found");
        while(true){
            delay(1);
        }
    }
    // start the server Ethernet
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());

    // clear LCD
    delay(1000);
    lcd.clear();

    // pinMode(42,OUTPUT);
}

//**** Main Loop ****//
void loop(){
    while( keypressed == NO_KEY ){
        // digitalWrite(42,HIGH);
        // delay(10000);
        // digitalWrite(42,LOW);
        // delay(10000);
        if( ac_NFC == 3 ){

            // check NFC for read cards
            uint8_t success;                          // Flag to check if there was an error with the PN532
            uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
            uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
            uint8_t currentblock;                     // Counter to keep track of which block we're on
            bool authenticated = false;               // Flag to indicate if the sector is authenticated
            uint8_t data[16];                         // Array to store block data during reads
            char * outCard;
            int CollectCartNumber[16];
            // Keyb on NDEF and Mifare Classic should be the same
            uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
            // 'uid' will be populated with the UID, and uidLength will indicate
            // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
            success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 20 );
            if(success){
                if (uidLength == 4){
                    // Now we try to go through all 16 sectors (each having 4 blocks)
                    // authenticating each sector, and then dumping the blocks
                    for (currentblock = 4; currentblock <= 4; currentblock++){
                        // Check if this is a new block so that we can reauthenticate
                        if (nfc.mifareclassic_IsFirstBlock(currentblock))
                            authenticated = false;

                        // If the sector hasn't been authenticated, do so first
                        if (!authenticated){
                            if (currentblock == 0){
                                // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
                                // or 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 for NDEF formatted cards using key a,
                                // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
                                success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
                            }else{
                                // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
                                // or 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 for NDEF formatted cards using key a,
                                // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
                                success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
                            }
                            if (success){
                                authenticated = true;
                            }else{
                                Serial.print("0");
                            }
                        }
                        // If we're still not authenticated just skip the block
                        if (!authenticated){
                            Serial.print("0");
                        }else{
                            // Authenticated ... we should be able to read the block now
                            // Dump the data into the 'data' array
                            success = nfc.mifareclassic_ReadDataBlock(currentblock, data);
                            if(success){
                                // Dump the raw data
                                //nfc.PrintHexChar(data, 16);
                                for( int ii = 0; ii < 16; ii++ ){
                                    CollectCartNumber[ii] = data[ii];
                                }

                                
                                // for( int oo = 0; oo < 16; oo++ ){
                                //     Serial.print(CollectCartNumber[oo]);
                                //     Serial.print(" ");
                                // }Serial.println(" ");
                                // Serial.print(CollectCartNumber[15]);


                                // set lcd cursor to line 2
                                lcd.setCursor(0,1);
                                //Serial.println("");
                                if( CollectCartNumber[0] != 0 ){
                                    // show error on lcd
                                    lcd.print( "Invalid Card !" );
                                    // card error
                                    Beep_CardError();
                                    // LED Red On
                                    LED_Red_ON();
                                }else if( CollectCartNumber[10] != 0 || (CollectCartNumber[15] == 0 && CollectCartNumber[14] == 0 && CollectCartNumber[13] == 0 && CollectCartNumber[12] == 0) ){
                                    // show error on lcd
                                    lcd.print( "Error cardnumber" );
                                    // card error
                                    Beep_CardError();
                                    // LED Red On
                                    LED_Red_ON();
                                }else{
                                    // get card number and save to variable
                                    outCard = Get_Card_Number(CollectCartNumber);
                                    
                                    // store in SD
                                    SD_File = SD.open("kara.txt", FILE_WRITE);
                                    if( SD_File ){
                                        // print in center position of LCD
                                        int spcs = strlen( outCard );
                                        // add rist 1
                                        SD_File.print("1");
                                        for( int c_ZIRO = 0; c_ZIRO < (10 - spcs); c_ZIRO++ )
                                            SD_File.print("0");
                                        // get time from module
                                        Date get_now( rtc.getTime().year, rtc.getTime().mon, rtc.getTime().date );
                                        // save in SD
                                        SD_File.print( outCard );
                                        // save date and time
                                        int T_JM = get_now.JDay();
                                        int T_JD = get_now.JMonth();
                                        int T_hours   = rtc.getTime().hour;
                                        int T_minutes = rtc.getTime().min;
                                        // if month is less than 9, add ziro
                                        if( T_JM < 10 )
                                            SD_File.print("0");
                                        SD_File.print( T_JM );
                                        // if day is less than 9, add ziro
                                        if( T_JD < 10 )
                                            SD_File.print("0");
                                        SD_File.print( T_JD );
                                        // if hours is less than 9, add ziro
                                        if( T_hours < 10 )
                                            SD_File.print("0");
                                        SD_File.print( T_hours );
                                        // if minute is less than 9, add ziro
                                        if( T_minutes < 10 )
                                            SD_File.print("0");
                                        SD_File.print( T_minutes );
                                        // add last 1
                                        SD_File.println("1");

                                        SD_File.close();
                                        if( spcs == 1 || spcs == 2 || spcs == 3 )
                                            spcs = 7;
                                        else if( spcs == 4 || spcs == 5 )
                                            spcs =  6;
                                        else if( spcs == 6 || spcs == 7 )
                                            spcs = 5;
                                        else if( spcs == 8 || spcs == 9 )
                                            spcs = 4;
                                        else
                                            spcs = 3;
                                        for( spcs; spcs > 0; spcs-- )
                                            lcd.print(" ");

                                        // show cart number on lcd
                                        lcd.print( outCard );

                                        // card success
                                        Beep_CardSuccess();

                                        // LED Green On
                                        LED_Green_ON();
                                    }else{
                                        // show error on lcd
                                        lcd.print("cant read SD");
                                        // card error
                                        Beep_CardError();
                                        // LED Red On
                                        LED_Red_ON();
                                    }

                                }
                                
                            }else{
                                // Oops ... something happened
                                Serial.print("0");
                            }

                        }
                    }
                }else{
                    Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
                }
                // delay
                delay(1500);
                // LED red and green Off
                LED_Red_OFF();
                LED_Green_OFF();
                // clear lcd
                lcd.clear();
            }
        }else{
            // read finger print
            int16_t p = -1;
            p = finger.getImage();
            finger.led_on();
            // set lcd cursor to line 2
            lcd.setCursor(0,1);
            // if nothing to read from finger print
            if( p != FPM_NOFINGER ){
                if( p == FPM_OK ){
                    Serial.println("Image taken");
                    finger.led_off();
                }else if( p == FPM_PACKETRECIEVEERR ){
                    lcd.clear();
                    lcd.print("Error #3"); // Comunicate error
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_IMAGEFAIL ){
                    lcd.clear();
                    lcd.print("Imaging error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_TIMEOUT ){
                    lcd.clear();
                    lcd.print("Timeout!");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_READ_ERROR ){
                    lcd.clear();
                    lcd.print("PID length error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else{
                    lcd.clear();
                    lcd.print("Unknown error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }
                finger.led_on();
                /* convert it */
                p = finger.image2Tz();
                if( p == FPM_OK ){
                    Serial.println("Image converted");
                    finger.led_off();
                }else if( p == FPM_IMAGEMESS ){
                    lcd.clear();
                    lcd.print("Error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_PACKETRECIEVEERR ){
                    lcd.clear();
                    lcd.print("Error #4"); // Comunicate error
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_FEATUREFAIL ){
                    lcd.clear();
                    lcd.print("cant find module");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_INVALIDIMAGE ){
                    lcd.clear();
                    lcd.print("cant find module");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_TIMEOUT ){
                    lcd.clear();
                    lcd.print("Error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else if( p == FPM_READ_ERROR ){
                    lcd.clear();
                    lcd.print("PID length error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }else{
                    lcd.clear();
                    lcd.print("Unknown error");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }

                /* search the database for the converted print */
                uint16_t fid, score;
                p = finger.searchDatabase(&fid, &score);

                // error
                if (p == FPM_OK) {
                    Serial.println("Found a print match!");
                } else if (p == FPM_PACKETRECIEVEERR) {
                    lcd.print("Error"); // Communication error
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                } else if (p == FPM_NOTFOUND) {
                    lcd.print("khata");
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                } else if (p == FPM_TIMEOUT) {
                    lcd.print("Error"); // Timeout!
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                } else if (p == FPM_READ_ERROR) {
                    lcd.print("PID length error"); // Got wrong PID or length!
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                } else {
                    lcd.print("Unknown error"); // Unknown error
                    // beep error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    lcd.clear();
                    continue;
                }

                // turn on LED green
                LED_Green_ON();


                // store in SD
                SD_File = SD.open("kara.txt", FILE_WRITE);
                if( SD_File ){
                    // print in center position of LCD
                    // int spcs = strlen( fid )
                    int spcs = floor(log10(abs(fid))) + 1;
                    if( fid == 0 )
                        spcs = 9;
                    // add rist 1
                    SD_File.print("2");
                    for( int c_ZIRO = 0; c_ZIRO < (10 - spcs); c_ZIRO++ )
                        SD_File.print("0");
                    // get time from module
                    Date get_now( rtc.getTime().year, rtc.getTime().mon, rtc.getTime().date );
                    // save in SD
                    SD_File.print( fid );
                    // save date and time
                    int T_JM = get_now.JDay();
                    int T_JD = get_now.JMonth();
                    int T_hours   = rtc.getTime().hour;
                    int T_minutes = rtc.getTime().min;
                    // if month is less than 9, add ziro
                    if( T_JM < 10 )
                        SD_File.print("0");
                    SD_File.print( T_JM );
                    // if day is less than 9, add ziro
                    if( T_JD < 10 )
                        SD_File.print("0");
                    SD_File.print( T_JD );
                    // if hours is less than 9, add ziro
                    if( T_hours < 10 )
                        SD_File.print("0");
                    SD_File.print( T_hours );
                    // if minute is less than 9, add ziro
                    if( T_minutes < 10 )
                        SD_File.print("0");
                    SD_File.print( T_minutes );
                    // add last 1
                    SD_File.println("2");

                    SD_File.close();
                }else{
                    // show error on lcd
                    lcd.print("cant read SD");
                    // card error
                    Beep_CardError();
                    // LED Red On
                    LED_Red();
                    continue;
                }

                // show cart number on lcd
                lcd.print( fid );

                // card success
                Beep_CardSuccess();

                // LED Green On
                LED_Green_ON();
                // delay
                delay(1500);
                // LED green Off
                LED_Green_OFF();
                // clear lcd
                lcd.clear();
            }
        }

        // Show Time
        Show_Time();

        // key keypad press
        keypressed = myKeypad.getKey();

        // check Ethernet for client get export
        Ethernet_Check();

        // on and off fan
        if( rtc.getTime().hour % 2 == 0 )
            digitalWrite( pin_fan, HIGH );
        else
            digitalWrite( pin_fan, LOW );

        // delay 0.1sec
        delay(100);
    }

    // check for press any key for settings
    Check_Press_Key();

}
