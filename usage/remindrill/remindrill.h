#ifndef _REMINDRILL_H_
#define _REMINDRILL_H_

enum screen_t
{
    clock_screen,
    alarms_screen
};

void select_screen(screen_t screen);

void stand_by();

#endif // _REMINDRILL_H_
