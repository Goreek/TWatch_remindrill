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
    {0, false, 8, 30},
    {1, false, 10, 0},
    {2, false, 11, 30},
    {3, false, 13, 0},
    {4, false, 14, 30},
    {5, false, 16, 0},
    {6, false, 17, 30},
    {7, false, 19, 0},
    {8, false, 20, 30},
    {9, false, 01, 0}    
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
        auto activeCheckbox = lv_checkbox_create(m_Grid);
        if(m_alarms[ii].active)
            lv_obj_add_state(activeCheckbox, LV_STATE_CHECKED);
        lv_obj_add_event_cb(activeCheckbox, event_onCheck, LV_EVENT_CLICKED, &m_alarms.at(ii) );
        
        lv_obj_set_grid_cell(activeCheckbox,
                            LV_GRID_ALIGN_STRETCH, 0, 1,
                            LV_GRID_ALIGN_STRETCH, ii+1, 1);

        auto hourButton = lv_btn_create(m_Grid);
        lv_obj_add_event_cb(hourButton, event_onHour, LV_EVENT_CLICKED, &m_alarms.at(ii) );
        auto hourLabel = lv_label_create(hourButton);
        lv_label_set_text_fmt(hourLabel, "%02d", m_alarms[ii].hour);
        lv_obj_set_grid_cell(hourButton,
                            LV_GRID_ALIGN_STRETCH, 1, 1,
                            LV_GRID_ALIGN_STRETCH, ii+1, 1);
        
        auto minuteButton = lv_btn_create(m_Grid);
        lv_obj_add_event_cb(minuteButton, event_onMinute, LV_EVENT_CLICKED, &m_alarms.at(ii) );
        auto minuteLabel = lv_label_create(minuteButton);
        lv_label_set_text_fmt(minuteLabel, "%02d", m_alarms[ii].minute);
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
}

void AlarmsView::event_onHour(lv_event_t *e)
{
}

void AlarmsView::event_onMinute(lv_event_t *e)
{
}
