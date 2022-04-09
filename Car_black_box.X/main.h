#ifndef MAIN_H
#define MAIN_H

#include<xc.h>
#include<string.h>
#include"clcd.h"
#include"digital_keypad.h"
#include"adc.h"
#include"ds1307.h"
#include"i2c.h"
#include"car_black_box_def.h"
#include"timers.h"

#define DASH_BOARD_FLAG 1
#define LOGIN_FLAG 2
#define RESET_PASSWORD 0x01
#define RESET_NOTHING  0xFF
#define RETURN_BACK  0x02
#define TASK_SUCCESS 0x11
#define TASK_FAIL 0x22
#define LOGIN_MENU_FLAG 0x06
#define RESET_LOGIN_MENU 0x13
#define VIEW_LOG_FLAG  0x08
#define VIEW_LOG_RESET  0x15
#define CLEAR_LOG_FLAG 0x0A
#define RESET_NOTHING 0xFF
#define RESET_MEMORY 0x17
#define CHANGE_PASSWORD_FLAG 0x0c
#define RESET_CHANGE_PASSWORD 0x19

#endif

