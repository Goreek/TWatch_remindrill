#ifndef _ALARMS_VIEW_H_
#define _ALARMS_VIEW_H_

#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>

class AlarmsView {
public:
    AlarmsView();

    static AlarmsView & getInstance();

    void setup();
    void loop();


    lv_obj_t* getScreen(); 
protected:
    static void event_onClock(lv_event_t *e);

  
private:
    static AlarmsView * m_Instance;

    lv_obj_t *m_Screen;
    lv_obj_t *m_Grid;
    lv_obj_t *m_ClockButton;
};

#endif // _ALARMS_VIEW_H_
