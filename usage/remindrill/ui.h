#ifndef _UI_H_
#define _UI_H_

enum screen_t
{
    clock_screen,
    alarms_screen
};

void select_screen(screen_t screen);

void stand_by();

#endif
