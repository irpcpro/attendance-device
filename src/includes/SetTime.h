// get numbers
int concat(int a, int b){
    char s1[20];
    char s2[20];
    sprintf(s1, "%d", a );
    sprintf(s2, "%d", b );
    strcat(s1, s2);
    int concated = atoi(s1);
    return concated;
}

// convert ascii to int
int ConvertAsciiToInt( char val ){
    switch( val )
    {
    case '1':
        return 1;
        break;
    case '2':
        return 2;
        break;
    case '3':
        return 3;
        break;
    case '4':
        return 4;
        break;
    case '5':
        return 5;
        break;
    case '6':
        return 6;
        break;
    case '7':
        return 7;
        break;
    case '8':
        return 8;
        break;
    case '9':
        return 9;
        break;
    default:
        return 0;
        break;
    }
}

// set time numbers
void Set_Time_Numbers(){
    // get variables from array TimeSetting
    int year    = concat( concat(ConvertAsciiToInt(TimeSetting[0]), ConvertAsciiToInt(TimeSetting[1])), concat(ConvertAsciiToInt(TimeSetting[2]), ConvertAsciiToInt(TimeSetting[3])));
    int month   = concat( ConvertAsciiToInt(TimeSetting[4]), ConvertAsciiToInt(TimeSetting[5]));
    int day     = concat( ConvertAsciiToInt(TimeSetting[6]), ConvertAsciiToInt(TimeSetting[7]));
    int hour    = concat( ConvertAsciiToInt(TimeSetting[8]), ConvertAsciiToInt(TimeSetting[9]));
    int minute  = concat( ConvertAsciiToInt(TimeSetting[10]), ConvertAsciiToInt(TimeSetting[11]));

    // set AD[Anno Domini] time to module
    rtc.setTime( hour, minute, 00 );
    rtc.setDate( day, month, year );

    // end of code, remove TimeSetting
    TimeSetting[12] = {};
}

