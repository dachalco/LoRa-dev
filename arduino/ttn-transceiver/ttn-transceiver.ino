/*
    device_A.ino
*/

#define _DEBUG_SERIAL_ // Show AT+ commands

#include <M5Stack.h>
#include <LoRaWan.h>

#define SerialUSB Serial

// TTN Device Settings
#define DEVICE_NAME     "m5stack-0"
#define DEVICE_EUI      "0000000000000001"
#define DEVICE_APP_EUI  "70B3D57ED002D1DF"
#define DEVICE_APP_KEY  "E054646FC032468D4A3311B6714A38C5"


// Prepare device for Over The Air Activation (OTAA)
void init_TTN_OTAA(void)
{
    lora.setDeviceMode( LWOTAA );
    lora.setId(NULL, DEVICE_EUI, DEVICE_APP_EUI);
    lora.setKey(NULL, NULL, DEVICE_APP_KEY);
}

void show_info(void)
{
    const size_t buf_len = 128;
    char buf[buf_len] = {0};
    M5.Lcd.println("Programmed Information:");

    lora.getVersion(buf, buf_len, DEFAULT_TIMEOUT);
    M5.Lcd.println(buf);
    lora.getId(buf, buf_len, DEFAULT_TIMEOUT);
    M5.Lcd.println(buf);
    lora.getChannel();
}

void setup(void)
{
    M5.begin();
    SerialUSB.begin(9600);

    // Setup LoRa modem
    lora.init();
    delay(2000); // must delay for lorawan power on
    init_TTN_OTAA();

    // Setup LCD and show config data
    M5.Lcd.setTextFont(2);
    M5.Lcd.println("Device: " DEVICE_NAME);
    M5.Lcd.println("AppEUI: " DEVICE_APP_EUI); 
    M5.Lcd.println("AppKey: " DEVICE_APP_KEY);
    M5.Lcd.setTextColor(BLUE);

    show_info();

}

void lcd_clear_log()
{
    M5.Lcd.setCursor(0, 18);
    M5.Lcd.fillRect(0, 18, 320, 222, BLACK);
}

void loop(void)
{
    if (M5.BtnA.wasPressed())
    {
        lcd_clear_log();
    }

    // Send a TTN 
    if (M5.BtnB.wasPressed()) 
    {
        M5.Lcd.setTextColor(BLUE);
        M5.Lcd.println("Attempting TTN Join...");

        // Attempt a join
        const int timeout_sec = 3;
        bool join_status = lora.setOTAAJoin(JOIN, timeout_sec);
        if (join_status) {
            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.println("Join SUCCESS");
        } else {
            M5.Lcd.setTextColor(RED);
            M5.Lcd.println("Join FAIL");
        }
        M5.Lcd.setTextColor(BLUE);

        // Continuous read attempt
        Serial2.print("AT+TEST=RXLRPKT\r\n");
    }
    M5.update();
}
