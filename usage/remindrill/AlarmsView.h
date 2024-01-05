#ifndef _ALARMS_VIEW_H_
#define _ALARMS_VIEW_H_

#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>
#include <array>


struct AlarmEntry
{
    int8_t id;
    bool active;
    int8_t hour;
    int8_t minute;
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
    void save_alarms();

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
};

#endif // _ALARMS_VIEW_H_
