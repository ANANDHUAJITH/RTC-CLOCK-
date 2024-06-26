/// PROJECT MADE BY ANANDHU AJITH CONSIDER FOLLOWING ON GITHUB @ https://github.com/ANANDHUAJITH ON INSTAGRAM @192.168.1.217


#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

char days[7][12]  = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

DS1307 rtc;  // create an RTC object

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // create object of ssd1306 -1 no hardware reset pin

void setup()
{
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  /* Initialize OLED display */
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(10,28);
  display.setTextSize(2);
  display.print("ADTA");
  
  display.display();
  display.clearDisplay();

  display.setCursor(3,28);
  display.setTextSize(2);
  display.setTextColor(WHITE);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("Enter time in the format: YYYY MM DD HH MM SS");
}

void loop() 
{ display.print(".");
  displayTimeDate();  // Display time on OLED
  delay(1000);
  
  if (Serial.available()) {
    setTimeFromSerial();
  }
}

void displayTimeDate()
{
  String meridiem;
  int hour;

  /* Calculate time and date */
  DateTime now = rtc.now();

  int date = now.day();
  int monthNum = now.month();
  int year = now.year();

  String day = days[now.dayOfWeek()];
  int minute = now.minute();
  int second = now.second();

  /* Convert 24hr format to 12hr format */
  if(now.hour() >= 12)
  {
    meridiem = "PM"; 
    hour = now.hour() - 12;
  }
  else
  {
    meridiem = "AM";
    hour = now.hour();
  }

  if(hour == 0)
  {
    hour = 12;
  }

  /* Display time on OLED display */
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(5,10);
  display.print(day);
  
  display.setTextSize(3);
  display.setCursor(58,6);
  if(date < 10) display.print(0);
  display.print(date);

  display.setTextSize(1);
  display.setCursor(97,8);
  display.print(months[monthNum-1]);
  display.setCursor(97,20);
  display.print(year);
  
  display.setCursor(5,39);
  display.setTextSize(3);
  if(hour < 10) display.print(0);
  display.print(hour);
  display.print(":");
  if(minute < 10) display.print(0);
  display.print(minute);
  display.setTextSize(2);
  display.setCursor(100,37);
  if(second < 10) display.print(0);
  display.print(second);
  display.setTextSize(1);
  display.setCursor(100,55);
  display.print(meridiem);
  display.display();
}

void setTimeFromSerial() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int year, month, day, hour, minute, second;
    if (sscanf(input.c_str(), "%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &second) == 6) {
      rtc.adjust(DateTime(year, month, day, hour, minute, second));
      Serial.println("Time set successfully!");
    } else {
      Serial.println("Invalid format. Use: YYYY MM DD HH MM SS");
    }
  }
}
