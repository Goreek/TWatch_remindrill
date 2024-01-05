#include "AlarmsView.h"
#include "ui.h"

constexpr const int32_t alarmsMagic = 0x0A1ABE50;

struct AlarmsSet
{
    int32_t magic;
    std::array<AlarmEntry,AlarmsNum> alarms;
};


enum
{
    AlarmsSetSize = sizeof(AlarmsSet)
};

static std::array<AlarmEntry,AlarmsNum> defaultAlarms = {{
    {false, 8, 30},
    {false, 10, 0},
    {false, 11, 30},
    {false, 13, 0},
    {false, 14, 30},
    {false, 16, 0},
    {false, 17, 30},
    {false, 19, 0},
    {false, 20, 30},
    {false, 01, 0}    
}};

static RTC_SLOW_ATTR uint8_t persistentAlarmsSet[AlarmsSetSize] = {};

AlarmsView *AlarmsView::m_Instance = nullptr;

AlarmsView::AlarmsView()
    : m_Screen(nullptr)
    , m_Grid(nullptr)
    , m_ClockButton(nullptr)
    , m_alarms{ defaultAlarms }
{
    m_Instance = this;
}

AlarmsView &AlarmsView::getInstance()
{
    return *m_Instance;
}

void AlarmsView::setup()
{
    load_alarms();

    m_Screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(m_Screen, lv_color_black(), 0);

    static lv_coord_t col_dsc[] = {40, LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {40, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, LV_GRID_TEMPLATE_LAST};
    m_Grid = lv_obj_create(m_Screen);
    lv_obj_set_size(m_Grid, LV_HOR_RES, 240);
    lv_obj_center(m_Grid);
    lv_obj_set_grid_dsc_array(m_Grid, col_dsc, row_dsc);
    lv_obj_set_style_bg_opa(m_Grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(m_Grid, LV_OPA_TRANSP, 0);

    m_ClockButton = lv_btn_create(m_Grid);
    lv_obj_align(m_ClockButton, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_font(m_ClockButton, &lv_font_montserrat_16, 0);
    lv_obj_add_event_cb(m_ClockButton, event_onClock, LV_EVENT_CLICKED, NULL);
    lv_obj_set_grid_cell(m_ClockButton,
                         LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_obj_t *clockLabel = lv_label_create(m_ClockButton);
    lv_label_set_text(clockLabel, LV_SYMBOL_BACKSPACE);
    lv_obj_center(clockLabel);

    for(int ii = 0; ii < AlarmsNum; ii++)
    {
        auto control = &m_controls.at(ii);
        control->alarm = &m_alarms.at(ii);

        control->activeCheck = lv_checkbox_create(m_Grid);
        if(m_alarms[ii].active)
            lv_obj_add_state(control->activeCheck, LV_STATE_CHECKED);
        lv_obj_add_event_cb(control->activeCheck, event_onCheck, LV_EVENT_CLICKED, control );
        
        lv_obj_set_grid_cell(control->activeCheck,
                            LV_GRID_ALIGN_STRETCH, 0, 1,
                            LV_GRID_ALIGN_STRETCH, ii+1, 1);

        auto hourButton = lv_btn_create(m_Grid);
        lv_obj_add_event_cb(hourButton, event_onHour, LV_EVENT_CLICKED, control );
        control->hourLabel = lv_label_create(hourButton);
        lv_label_set_text_fmt(control->hourLabel, "%02d", m_alarms[ii].hour);
        lv_obj_set_grid_cell(hourButton,
                            LV_GRID_ALIGN_STRETCH, 1, 1,
                            LV_GRID_ALIGN_STRETCH, ii+1, 1);
        
        auto minuteButton = lv_btn_create(m_Grid);
        lv_obj_add_event_cb(minuteButton, event_onMinute, LV_EVENT_CLICKED, control );
        control->minuteLabel = lv_label_create(minuteButton);
        lv_label_set_text_fmt(control->minuteLabel, "%02d", m_alarms[ii].minute);
        lv_obj_set_grid_cell(minuteButton,
                            LV_GRID_ALIGN_STRETCH, 2, 1,
                            LV_GRID_ALIGN_STRETCH, ii+1, 1);
    }
}

void AlarmsView::loop()
{
}

lv_obj_t *AlarmsView::getScreen()
{
    return m_Screen;
}

void AlarmsView::load_alarms()
{
    AlarmsSet temp;
    memcpy( &temp, persistentAlarmsSet, AlarmsSetSize );
    if(temp.magic == alarmsMagic )
        m_alarms = temp.alarms;
    else
        m_alarms = defaultAlarms;
}

void AlarmsView::save_alarms()
{
    AlarmsSet temp{ alarmsMagic, m_alarms };
    memcpy( persistentAlarmsSet, &temp, AlarmsSetSize );
}

void AlarmsView::event_onClock(lv_event_t *e)
{
    select_screen( screen_t::clock_screen );
}

void AlarmsView::event_onCheck(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    AlarmControl *control = static_cast<AlarmControl *>( lv_event_get_user_data(e) );
    auto state = lv_obj_get_state(control->activeCheck);
    control->alarm->active = ( 0 != state & LV_STATE_CHECKED );
}

void AlarmsView::event_onHour(lv_event_t *e)
{
}

void AlarmsView::event_onMinute(lv_event_t *e)
{
}
