#ifndef _ALARMS_VIEW_H_
#define _ALARMS_VIEW_H_

#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>
#include <array>


struct AlarmEntry
{
    bool active;
    int8_t hour;
    int8_t minute;
};

struct AlarmControl
{
    lv_obj_t * activeCheck;
    lv_obj_t * hourLabel;
    lv_obj_t * minuteLabel;
    AlarmEntry * alarm;
};


enum
{
    AlarmsNum = 10
};

class AlarmsView {
public:
    AlarmsView();

    static AlarmsView & getInstance();

    void setup();
    void loop();


    lv_obj_t* getScreen(); 

    void load_alarms();
    void store_alarms() const;

    int32_t get_next_alarm_sec() const;

protected:
    static void event_onClock(lv_event_t *e);
    static void event_onCheck(lv_event_t *e);
    static void event_onHour(lv_event_t *e);
    static void event_onMinute(lv_event_t *e);

  
private:
    static AlarmsView * m_Instance;

    lv_obj_t *m_Screen;
    lv_obj_t *m_Grid;
    lv_obj_t *m_ClockButton;

    std::array<AlarmEntry,AlarmsNum> m_alarms;
    std::array<AlarmControl,AlarmsNum> m_controls;
};

#endif // _ALARMS_VIEW_H_
