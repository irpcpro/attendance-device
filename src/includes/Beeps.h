//* all sound of buzzer *//

// Startup Beep sound
void Beep_StartSetup(){
    tone( Buzzer, 200, 60 );
    delay(200);
    tone( Buzzer, 290, 150 );
}

// password success Beep sound
void Beep_PasswordSeccuss(){
    tone( Buzzer, 1070, 60 );
    delay(80);
    tone( Buzzer, 1020, 50 );
}

// error setuo sound
void Beep_ErrorSetup(){
    tone( Buzzer, 100, 1100 );
}

// card success
void Beep_CardSuccess(){
    tone( Buzzer, 1700, 120 );
    tone( Buzzer, 1800, 120 );
    tone( Buzzer, 2490, 500 );
}

// card error
void Beep_CardError(){
    tone( Buzzer, 1800, 300 );
    delay(350); 
    tone( Buzzer, 1800, 500 );
}

// type beep keypad
void Beep_keypress(){
    tone( Buzzer, 1800, 30 );
}
