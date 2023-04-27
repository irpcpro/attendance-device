// LED Errors Functions

// LED Error Green
void LED_Green(){
    digitalWrite( LED_Green_pin, HIGH );
    delay(1500);
    digitalWrite( LED_Green_pin, LOW );
}

// LED Error Red
void LED_Red(){
    digitalWrite( LED_Red_pin, HIGH );
    delay(1500);
    digitalWrite( LED_Red_pin, LOW );
}

// LED Green On
void LED_Green_ON(){
    digitalWrite( LED_Green_pin, HIGH );
}

// LED Green Off
void LED_Green_OFF(){
    digitalWrite( LED_Green_pin, LOW );
}

// LED Red On
void LED_Red_ON(){
    digitalWrite( LED_Red_pin, HIGH );
}

// LED Red Off
void LED_Red_OFF(){
    digitalWrite( LED_Red_pin, LOW );
}
