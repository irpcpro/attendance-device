// show Client Server To Method Get
void Server_Response_Client_GET( EthernetClient client ){
    // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<h1>Hello from device server ..</h1>");
    client.println("Device Version:");
    client.print( DeviceVersion );
}

// sent tot Client export of Data
void Server_Response_Client_POST( EthernetClient client ){
    // set output HTML type
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    // print result to client
    client.println();
    // set out json variable
    Get_SD_Details( client );
    // client.print( GetData );
    //GetData = {""};
}

// check Ethernet for client get export
void Ethernet_Check(){
    // listen for incoming clients
    String req_str = "";
    EthernetClient client = server.available();
    if( client ){
        Serial.println("new client");

        // an http request ends with a blank line
        boolean currentLineIsBlank = true;

        while( client.connected() ){
            if( client.available() ){
                char c = client.read();
                Serial.write(c);
                req_str += c;

                // for GET method - show index of device server page
                if (c == '\n' && currentLineIsBlank && req_str.startsWith("GET")) {
                    // lcd.clear();
                    // lcd.setCursor(0,0);lcd.print("Please Wait");
                    // lcd.setCursor(0,1);lcd.print("Export Data");
                    Server_Response_Client_GET( client );
                    break;
                }

                // for POST method - send data of SD to client
                if (c == '\n' && currentLineIsBlank && req_str.startsWith("POST") ){
                    lcd.clear();
                    lcd.setCursor(0,0);lcd.print("Please Wait ...");
                    lcd.setCursor(0,1);lcd.print("Export Data ...");
                    Server_Response_Client_POST( client );
                    lcd.clear();
                    break;
                }

                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }

        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}
