/*
 * ESP32_ILI9341_ntp_Clock_Digital
 * https://arduino-er.blogspot.com/2021/01/esp32-get-time-from-poolntporg.html
 * Combine the examples of
 * Examples > ESP32 > Time > ESP32_SimpleTime and
 * Examples > TFT_eSPI > 320 x 240 > TFT_DIGITAL_CLOCK
 * 
 * Get time from ntpServer "pool.ntp.org",
 * display on SPI ILI9341 screen in digital clock form.
 * 
 * You have to change ssid/password to match with your wifi network.
 * and change gmtOffset_sec to match your timezone.
 =========================================================================
 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.
 
 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
For using SPI ILI9341 screen on ESP32,
refer Arduino-er: ESP32 + 2.4" 320X240 Display (SPI ILI9341), 
using TFT_eSPI, prepare user setup file.
http://arduino-er.blogspot.com/2021/01/esp32-24-320x240-display-spi-ili9341.html

A few colour codes:

code  color
0x0000  Black
0xFFFF  White
0xBDF7  Light Gray
0x7BEF  Dark Gray
0xF800  Red
0xFFE0  Yellow
0xFBE0  Orange
0x79E0  Brown
0x7E0 Green
0x7FF Cyan
0x1F  Blue
0xF81F  Pink

 */

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <WiFi.h>
#include "time.h"


const char* ssid       = "ssid";
const char* password   = "password";

const char* ntpServer = "pool.ntp.org";
//+8 hr@HongKong
//8 * 60 * 60 = 28800
const long  gmtOffset_sec = 28800; //-8hr@Hong Kong
const int   daylightOffset_sec = 3600;

#define TFT_GREY 0x5AEB

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

uint32_t targetTime = 0;                    // for next 1 second timeout


/* The original example get H, M, S from compile time
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x

uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), 
        ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
*/

/*
 *  Get H,M, S after ntp server connected, 
 *  will load in printLocalTime()
 */
uint8_t hh,mm,ss;


byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  /* to read elements from getLocalTime(&timeinfo)
   * ref:
   * http://www.cplusplus.com/reference/ctime/tm/
   */
  int time_yday = timeinfo.tm_yday; //days since January 1
  int time_wday = timeinfo.tm_wday; //days since Sunday
  int time_year = timeinfo.tm_year; // years since 1900
  int time_mon  = timeinfo.tm_mon;   //months since January
  int time_mday = timeinfo.tm_mday; //day of the month
  int time_hr   = timeinfo.tm_hour;
  int time_min  = timeinfo.tm_min;
  int time_sec  = timeinfo.tm_sec;
  Serial.println("tm_yday: " + String(time_yday));
  Serial.println("tm_wday: " + String(time_wday));
  Serial.println("tm_year: " + String(time_year));
  Serial.println("tm_mon:  " + String(time_mon));
  Serial.println("tm_mday: " + String(time_mday));
  Serial.println("tm_hr:   " + String(time_hr));
  Serial.println("tm_min:  " + String(time_min));
  Serial.println("tm_sec:  " + String(time_sec)); 
  hh = time_hr;
  mm = time_min;
  ss = time_sec;
}

void setup(void) {
  Serial.begin(115200);
  /*
   * Connect to WiFi, and get time from ntpServer
   * To simplify, only once when power-up.
   */
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  /*
   * Prepare for TFT
   */
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  targetTime = millis() + 1000;
}

void loop() {
  if (targetTime < millis()) {
    // Set next update for 1 second later
    targetTime = millis() + 1000;

    // Adjust the time values by adding 1 second
    ss++;              // Advance second
    if (ss == 60) {    // Check for roll-over
      ss = 0;          // Reset seconds to zero
      omm = mm;        // Save last minute time for display update
      mm++;            // Advance minute
      if (mm > 59) {   // Check for roll-over
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
          hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
        }
      }
    }


    // Update digital time
    int xpos = 0;
    int ypos = 85; // Top left corner ot clock text, about half way down
    int ysecs = ypos + 24;

    if (omm != mm) { // Redraw hours and minutes time every minute
      omm = mm;
      // Draw hours and minutes
      if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
      xpos += tft.drawNumber(hh, xpos, ypos, 8);             // Draw hours
      xcolon = xpos; // Save colon coord for later to flash on/off later
      xpos += tft.drawChar(':', xpos, ypos - 8, 8);
      if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
      xpos += tft.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
      xsecs = xpos; // Sae seconds 'x' position for later display updates
    }
    if (oss != ss) { // Redraw seconds time every second
      oss = ss;
      xpos = xsecs;

      if (ss % 2) { // Flash the colons on/off
        tft.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
        tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
      }
      else {
        tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
      }

      //Draw seconds
      if (ss < 10) xpos += tft.drawChar('0', xpos, ysecs, 6); // Add leading zero
      tft.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
    }
  }
}

/*
// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
*/
