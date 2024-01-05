#include <lv_conf.h>
#include "ClockView.h"
#include "ui.h"

LV_FONT_DECLARE(font_firacode_60);


ClockView *ClockView::m_Instance = nullptr;

ClockView::ClockView()
    : m_Screen(nullptr)
    , m_Grid(nullptr)
    , m_SleepButton(nullptr)
    , m_Battery(nullptr)
    , m_DummyCount(1)
    , m_UpdateTime(0)
{
    m_Instance = this;
}

ClockView &ClockView::getInstance()
{
    return *m_Instance;
}

void ClockView::setup()
{
    lv_style_init(&m_NoBgStyle);
    lv_style_set_bg_opa(&m_NoBgStyle, LV_OPA_TRANSP);
    lv_style_set_bg_color(&m_NoBgStyle, lv_color_black());
    lv_style_set_border_color(&m_NoBgStyle, lv_color_black());
    lv_style_set_border_opa(&m_NoBgStyle, LV_OPA_TRANSP);

    m_Screen = lv_obj_create(nullptr);
    lv_obj_add_style(m_Screen, &m_NoBgStyle, LV_STATE_DEFAULT);

    static lv_coord_t col_dsc[] = {40, LV_GRID_FR(1), 40, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {40, LV_GRID_FR(1), 40, LV_GRID_TEMPLATE_LAST};
    m_Grid = lv_obj_create(m_Screen);
    lv_obj_set_size(m_Grid, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(m_Grid);
    lv_obj_set_grid_dsc_array(m_Grid, col_dsc, row_dsc);
    lv_obj_add_style(m_Grid, &m_NoBgStyle, LV_STATE_DEFAULT);
    //lv_obj_set_style_border_opa(m_Grid, LV_OPA_COVER, 0);
    //lv_obj_set_style_border_color(m_Grid, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);

    m_SleepButton = lv_btn_create(m_Grid);
    lv_obj_align(m_SleepButton, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_font(m_SleepButton, &lv_font_montserrat_16, 0);
    lv_obj_add_event_cb(m_SleepButton, event_onSleep, LV_EVENT_CLICKED, NULL);

    lv_obj_t *sleepLabel = lv_label_create(m_SleepButton);
    lv_label_set_text(sleepLabel, LV_SYMBOL_POWER);
    lv_obj_center(sleepLabel);
    lv_obj_set_grid_cell(m_SleepButton,
                         LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);

    auto batteryCell = lv_obj_create(m_Grid);
    lv_obj_set_scrollbar_mode(batteryCell, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(batteryCell, &m_NoBgStyle, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(batteryCell,
                         LV_GRID_ALIGN_END, 1, 2,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    m_Battery = lv_label_create(batteryCell);
    lv_obj_align(m_Battery, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_font(m_Battery, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(m_Battery, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_set_style_text_align(m_Battery, LV_TEXT_ALIGN_RIGHT, 0);
    lv_label_set_text(m_Battery, LV_SYMBOL_BATTERY_FULL "%");

    lv_style_init(&m_ClockStyle);
    lv_style_set_text_font(&m_ClockStyle, &font_firacode_60);
    lv_style_set_text_color(&m_ClockStyle, lv_color_make(0xFF, 0xFF, 0));
    lv_style_set_text_opa(&m_ClockStyle, LV_OPA_COVER);
    lv_style_set_text_align(&m_ClockStyle, LV_TEXT_ALIGN_CENTER);

    auto clockCell = lv_obj_create(m_Grid);
    lv_obj_set_width(clockCell, LV_HOR_RES);
    lv_obj_set_scrollbar_mode(clockCell, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(clockCell, &m_NoBgStyle, LV_STATE_DEFAULT);
    //lv_obj_set_style_border_opa(clockCell, LV_OPA_COVER, 0);
    //lv_obj_set_style_border_color(clockCell, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_set_grid_cell(clockCell,
                         LV_GRID_ALIGN_STRETCH, 0, 3,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    m_ClockLabel = lv_label_create(clockCell);
    lv_obj_center(m_ClockLabel);
    lv_obj_add_style(m_ClockLabel, &m_NoBgStyle, LV_STATE_DEFAULT);
    lv_obj_add_style(m_ClockLabel, &m_ClockStyle, LV_STATE_DEFAULT);
    lv_label_set_text(m_ClockLabel, "12:00");

    m_AlarmsTuneButton = lv_btn_create(m_Grid);
    lv_obj_align(m_AlarmsTuneButton, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_font(m_AlarmsTuneButton, &lv_font_montserrat_16, 0);
    lv_obj_add_event_cb(m_AlarmsTuneButton, event_onAlarmsTune, LV_EVENT_CLICKED, this);
    lv_obj_set_grid_cell(m_AlarmsTuneButton,
                         LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_CENTER, 2, 1);

    lv_obj_t *alarmsTuneLabel = lv_label_create(m_AlarmsTuneButton);
    lv_label_set_text(alarmsTuneLabel, LV_SYMBOL_BELL);
    lv_obj_center(alarmsTuneLabel);
}

void ClockView::loop()
{
    if( m_UpdateTime < millis() )
    {
        m_UpdateTime = millis() + 1000;

        const auto batteryLife = watch.getBatteryPercent();
        const char * batterySymbol = LV_SYMBOL_BATTERY_FULL;
        if(batteryLife <= 5)
            batterySymbol = LV_SYMBOL_BATTERY_EMPTY;
        else if (batteryLife <= 35)
            batterySymbol = LV_SYMBOL_BATTERY_1;
        else if (batteryLife <= 65)
            batterySymbol = LV_SYMBOL_BATTERY_2;
        else if (batteryLife <= 95)
            batterySymbol = LV_SYMBOL_BATTERY_3;

        // Update battery indicator
        lv_label_set_text_fmt(m_Battery, "%s%3d%%", batterySymbol, batteryLife);

        char time_text[] = "00:00";
        strncpy_P(time_text, watch.strftime(DATETIME_FORMAT_HM), sizeof(time_text));

        // Blink colon in the string of RTC time
        time_text[2] = watch.hwClockRead() % 2 ? ':' : ' ';
        lv_label_set_text(m_ClockLabel, time_text);
    }
}

lv_obj_t *ClockView::getScreen()
{
    return m_Screen;
}

void ClockView::event_onSleep(lv_event_t *e)
{
    LV_LOG_USER("Sleep Clicked");

    stand_by();
}

void ClockView::event_onAlarmsTune(lv_event_t *e)
{
    LV_LOG_USER("Alarms Tune Clicked");

    select_screen( screen_t::alarms_screen );
}