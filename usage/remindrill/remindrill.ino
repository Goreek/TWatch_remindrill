/*
 An example digital clock using a TFT LCD screen to show the time.
 Demonstrates use of the font printing routines. (Time updates but date does not.)

 It uses the time of compile/upload to set the time
 For a more accurate clock, it would be better to use the RTClib library.
 But this is just a demo...

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################

 Based on clock sketch by Gilchrist 6/2/2014 1.0

A few colour codes:

code    color
0x0000  Black
0xFFFF  White
0xBDF7  Light Gray
0x7BEF  Dark Gray
0xF800  Red
0xFFE0  Yellow
0xFBE0  Orange
0x79E0  Brown
0x7E0   Green
0x7FF   Cyan
0x1F    Blue
0xF81F  Pink

 */

#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>
#include "ui.h"
#include "ClockView.h"
#include "AlarmsView.h"

#define TFT_GREY 0x5AEB

// Invoke custom library

const int8_t timeFont = 7;
const int8_t batteryFont = 4;

uint32_t targetTime = 0; // for next 1 second timeout

const int16_t defaultAwakeTime = 3000;

uint8_t ss = 0;
int16_t sleepCountdown_sec = defaultAwakeTime;

bool pmu_flag = false;

ClockView clockView;
AlarmsView alarmsView;

screen_t currentScreen = screen_t::clock_screen;

void logToSerial(const char * buf)
{
    Serial.println(buf);
}

// ISR for Power Button (crown button)
void setPMUFlag(void)
{
    if(watch.getTouched())
        pmu_flag = true;
}

void select_screen(screen_t screen)
{
    switch (screen)
    {
    case screen_t::clock_screen:
        lv_scr_load(clockView.getScreen());
        currentScreen = screen_t::clock_screen;
        break;

    case screen_t::alarms_screen:
        lv_scr_load(alarmsView.getScreen());
        currentScreen = screen_t::alarms_screen;
        break;

    default:
        Serial.println(("Invalid screen selection: " + std::to_string(screen)).c_str());
    }
}

void setup(void)
{
    // Serial.begin(115200);
    watch.begin();
    watch.setRotation(2);
    watch.fillScreen(TFT_BLACK);

    watch.setTextFont(4);
    watch.setTextSize(1);
    watch.setTextColor(TFT_YELLOW, TFT_BLACK);

    targetTime = millis() + 1000;

    // Set the interrupt handler of the PMU
    watch.attachPMU(setPMUFlag);

    // Initialize UI
    beginLvglHelper(false);
    lv_log_register_print_cb( logToSerial );

    clockView.setup();
    alarmsView.setup();

    lv_scr_load(clockView.getScreen());

    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());
}

void loop()
{
    lv_task_handler();

    switch(currentScreen) 
    {
    case screen_t::clock_screen:
        clockView.loop();
        break;

    case screen_t::alarms_screen:
        alarmsView.loop();
        break;
    };

    if (watch.getTouched()) // reset awake timer if the screen touched
        sleepCountdown_sec = defaultAwakeTime;

    if (targetTime < millis())
    {
        // Set next update for 1 second later
        targetTime = millis() + 1000;
        ss++;
        sleepCountdown_sec--;
    }

    // If crown button was pressed and screen touched, then enforce sleep
    if (pmu_flag && watch.getTouched())
    {

        // After the PMU interrupt is triggered, the interrupt status must be cleared,
        // otherwise the next interrupt will not be triggered
        watch.clearPMU();

        // Set to wake by pressing the button on the crown
        watch.setSleepMode(PMU_BTN_WAKEUP);

        watch.sleep();
    }

    delay(5);
}
