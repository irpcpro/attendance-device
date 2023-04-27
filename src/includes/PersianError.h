// persian errors
// make some custom characters:
byte te[8] = {0,14,0,2,2,30,0,0};
byte ke[8] = {1,2,4,12,2,31,0,0};
byte re_ch[8] = {0,0,0,0,0,3,4,24};
byte re[8] = {0,0,0,0,2,2,4,24};
byte aa[8] = {0,0,2,2,2,2,2,0};
byte ye[8] = {0,3,20,22,17,9,6,0};
byte te_akhar[8] = {0,0,12,0,18,31,0,0};
byte sh_vasat[8] = {4,10,0,21,21,31,0,0};
byte ge_vasat[8] = {5,10,4,12,2,31,0,0};
byte ne_aval[8] = {0,2,0,1,1,30,0,0};
byte khe_aval[8] = {4,0,4,10,1,31,0,0};
byte oo_akhar[8] = {0,0,14,10,10,15,4,24};
byte de[8] = {0,4,2,1,1,14,0,0};
byte ghe_aval[8] = {7,0,7,5,7,30,0,0};
byte he_aval[8] = {0,6,31,21,14,28,0,0};
byte ye_vasat[8] = {0,0,0,2,2,31,0,14};
byte d_akhr[8] = {0,24,4,2,2,31,0,0};
byte lcd_spc[8] = {0,0,0,0,0,0,0,0};

// setup persian character to lcd
void Setup_Persian_Char(){
    lcd.createChar(0, te);
    lcd.createChar(1, ke);
    lcd.createChar(2, re_ch);
    lcd.createChar(5, re);
    lcd.createChar(3, aa);
    lcd.createChar(4, ye);

    lcd.createChar( 6, te_akhar);
    lcd.createChar( 7, sh_vasat);
    lcd.createChar( 8, ge_vasat);
    lcd.createChar( 9, ne_aval);
    lcd.createChar( 10, khe_aval);
    lcd.createChar( 11, oo_akhar);
    lcd.createChar( 12, de);
    lcd.createChar( 13, ghe_aval);
    lcd.createChar( 14, he_aval);
    lcd.createChar( 15, ye_vasat);
    lcd.createChar( 5, d_akhr);
    lcd.createChar( 17, lcd_spc);
    
}

// void print function
void Print_Persian_Character( int type ){
    if( type == 1 ){
        lcd.write((byte)4);
        lcd.write((byte)5);
        lcd.write((byte)3);
        lcd.write((byte)2);
        lcd.write((byte)1);
        lcd.write((byte)0);
    }else if( type == 2 ){

        
        lcd.write((byte)5);
        lcd.write((byte)15);
        lcd.write((byte)14);
        lcd.write((byte)12);
        
    }
}
