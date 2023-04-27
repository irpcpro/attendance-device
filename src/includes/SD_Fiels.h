//* SD Function & Setting *//

// get data from SD cart
void Get_SD_Details( EthernetClient client ){
    // set GetData variable
    // GetData["Data"];
    // GetData["Status"] = true;
    // get main file
    SD_File = SD.open("kara.txt");
    // get data from text and set to GetData variable
    int i = 0;
    if(SD_File){
        while (SD_File.available()) {
            // GetData["Data"][i++] = SD_File.readStringUntil('\n');
            client.print( SD_File.readStringUntil('\n') );
            client.print(",");
        }
        SD_File.close();
    }else{
        // GetData["Status"] = false;
        client.print("");
    }


    // check if temp directory not exists, create one
    if( !SD.exists("tmp") )
        SD.mkdir("tmp");

    // get copy of data to temp file
    SD_File = SD.open("kara.txt", FILE_READ);
    SD_TMP_File = SD.open("tmp/backup.txt", FILE_WRITE);
    if( SD_TMP_File && SD_File ){
        while( SD_File.available() ){
            SD_TMP_File.println( SD_File.readStringUntil('\n') );
        }
        SD_TMP_File.close();
        SD_File.close();
        Serial.println("copied ..");
    }else{
        Serial.println("dont exists TMP ......");
    }
    // remove main file
    SD.remove("kara.txt");
}

// store in SD
boolean StoreInSD( char *Data ){
    SD_File = SD.open("kara.txt", FILE_WRITE);
    if( SD_File ){
        SD_File.println( Data );
        SD_File.close();
        return true;
    }else{
        return false;
    }
}
