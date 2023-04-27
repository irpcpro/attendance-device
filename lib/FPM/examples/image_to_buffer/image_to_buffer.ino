#include <SoftwareSerial.h>
#include <FPM.h>

/* Save a fingerprint image to a RAM buffer/array.
 * THIS WON'T COMPILE UNLESS YOU HAVE AN MCU WITH AT LEAST ~37K RAM
 * LIKE THE ESP32.
 */

/*  pin #2 is IN from sensor (GREEN wire)
    pin #3 is OUT from arduino  (WHITE/YELLOW wire)
*/
SoftwareSerial fserial(2, 3);

FPM finger(&fserial);
FPM_System_Params params;

void setup()
{
    Serial.begin(57600);
    Serial.println("SAVE IMAGE TO BUFFER test");
    fserial.begin(57600);

    if (finger.begin()) {
        finger.readParams(&params);
        Serial.println("Found fingerprint sensor!");
        Serial.print("Capacity: "); Serial.println(params.capacity);
        Serial.print("Packet length: "); Serial.println(FPM::packet_lengths[params.packet_len]);
    }
    else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) yield();
    }
}

void loop() {
    stream_image();
    while (1) yield();
}

#define IMAGE_SZ	36864UL
uint8_t image_buffer[IMAGE_SZ];

void stream_image(void) {
    if (!set_packet_len_128()) {
        Serial.println("Could not set packet length");
        return;
    }

    delay(100);
    
    int16_t p = -1;
    Serial.println("Waiting for a finger...");
    while (p != FPM_OK) {
        p = finger.getImage();
        switch (p) {
            case FPM_OK:
                Serial.println("Image taken");
                break;
            case FPM_NOFINGER:
                break;
            case FPM_PACKETRECIEVEERR:
                Serial.println("Communication error");
                break;
            case FPM_IMAGEFAIL:
                Serial.println("Imaging error");
                break;
            default:
                Serial.println("Unknown error");
                break;
        }
        yield();
    }

    p = finger.downImage();
    switch (p) {
        case FPM_OK:
            Serial.println("Starting image stream...");
            break;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return;
        case FPM_UPLOADFAIL:
            Serial.println("Cannot transfer the image");
            return;
    }

    bool read_finished;
	uint16_t readlen = IMAGE_SZ;
    uint16_t pos = 0;
    int16_t count = 0;

    while (true) {
        bool ret = finger.readRaw(FPM_OUTPUT_TO_BUFFER, image_buffer + pos, &read_finished, &readlen);
        if (ret) {
            count++;
			pos += readlen;
			readlen = IMAGE_SZ - pos;
            if (read_finished)
                break;
        }
        else {
            Serial.print("\r\nError receiving packet ");
            Serial.println(count);
            return;
        }
        yield();
    }

    Serial.println();
    Serial.print(count * FPM::packet_lengths[params.packet_len]); Serial.println(" bytes read.");
    Serial.println("Image stream complete.");
}


/* no need to call this for R308 */
bool set_packet_len_128(void) {
    uint8_t param = FPM_SETPARAM_PACKET_LEN; // Example
    uint8_t value = FPM_PLEN_128;
    int16_t p = finger.setParam(param, value);
    switch (p) {
        case FPM_OK:
            Serial.println("Packet length set to 128 bytes");
            break;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Comms error");
            break;
        case FPM_INVALIDREG:
            Serial.println("Invalid settings!");
            break;
        default:
            Serial.println("Unknown error");
    }

    return (p == FPM_OK);
}