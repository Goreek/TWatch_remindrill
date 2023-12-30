#include "AlarmsView.h"
#include "ui.h"

AlarmsView *AlarmsView::m_Instance = nullptr;

AlarmsView::AlarmsView()
    : m_Screen(nullptr)
    , m_Grid(nullptr)
    , m_ClockButton(nullptr)
{
    m_Instance = this;
}

AlarmsView &AlarmsView::getInstance()
{
    return *m_Instance;
}

void AlarmsView::setup()
{
    m_Screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(m_Screen, lv_color_black(), 0);

    static lv_coord_t col_dsc[] = {40, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {40, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    m_Grid = lv_obj_create(m_Screen);
    lv_obj_set_size(m_Grid, LV_HOR_RES, LV_VER_RES);
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
}

void AlarmsView::loop()
{
}

lv_obj_t *AlarmsView::getScreen()
{
    return m_Screen;
}

void AlarmsView::event_onClock(lv_event_t *e)
{
    select_screen( screen_t::clock_screen );
}
