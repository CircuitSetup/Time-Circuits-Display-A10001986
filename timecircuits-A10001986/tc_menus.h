/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Time Circuits Display
 * (C) 2021-2022 John deGlavina https://circuitsetup.us
 * (C) 2022-2024 Thomas Winischhofer (A10001986)
 * https://github.com/realA10001986/Time-Circuits-Display
 * https://tcd.out-a-ti.me
 *
 * Keypad Menu handling
 *
 * Based on code by John Monaco, Marmoset Electronics
 * https://www.marmosetelectronics.com/time-circuits-clock
 * -------------------------------------------------------------------
 * License: MIT
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the 
 * Software, and to permit persons to whom the Software is furnished to 
 * do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _TC_MENUS_H
#define _TC_MENUS_H

extern bool keypadInMenu;
extern bool isYearUpdate;

extern uint8_t        autoInterval;
extern const uint8_t  autoTimeIntervals[6];

extern bool    alarmOnOff;
extern uint8_t alarmHour;
extern uint8_t alarmMinute;
extern uint8_t alarmWeekday;

extern uint8_t remMonth;
extern uint8_t remDay;
extern uint8_t remHour;
extern uint8_t remMin;

void enter_menu();

void alarmOff();
bool alarmOn();
int  toggleAlarm();
int  getAlarm();
const char *getAlWD(int wd);

bool loadAutoInterval();

void waitAudioDone();

void start_file_copy();
void file_copy_progress();
void file_copy_done();
void file_copy_error();

void enterkeyScan();

void myloops(bool menuMode);

#endif
