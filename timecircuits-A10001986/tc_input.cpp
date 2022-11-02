/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Time Circuits Display
 * (C) 2022 Thomas Winischhofer (A10001986)
 * https://github.com/realA10001986/Time-Circuits-Display-A10001986
 *
 * Keypad_I2C and Button handling
 *
 * This is a customized fork and unification of the Keypad and the
 * Keypad_I2C libraries, as well as the OneButton library
 * Authors of original Keypad library: Mark Stanley, Alexander Brevig
 * Authors of original Keypad_I2C library: G. D. (Joe) Young, ptw
 * Author of original OneButton library: Matthias Hertel
 * -------------------------------------------------------------------
 * License of original KeyPad/Keypad_I2C libraries and this version:
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; version
 * 2.1 of the License or later versions.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>
 * -------------------------------------------------------------------
 * Original OneButton library 
 * Copyright (c) 2005-2014 by Matthias Hertel,  http://www.mathertel.de/
 * 
 * Software License Agreement (BSD License)
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * - Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution. 
 * - Neither the name of the copyright owners nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <Arduino.h>

#include "tc_input.h"

#define OPEN    LOW
#define CLOSED  HIGH

static void defaultDelay(unsigned int mydelay);

/*
 * Key class
 */

TCKey::TCKey()
{
    kchar = NO_KEY;
    kstate = IDLE;
    stateChanged = false;
}

/*
 * Keypad_i2c class
 */

Keypad_I2C::Keypad_I2C(char *userKeymap, const uint8_t *row, const uint8_t *col,
                       uint8_t numRows, uint8_t numCols,
                       int address, int width, TwoWire *awire)
{
    _keymap = userKeymap;

    _rowPins = row;
    _columnPins = col;
    _rows = numRows;
    _columns = numCols;

    _i2caddr = address;
    _i2cwidth = width;
    _wire = awire;

    setScanInterval(10);
    setHoldTime(500);
    
    _keypadEventListener = NULL;

    _customDelayFunc = defaultDelay;
}

// Initialize I2C
void Keypad_I2C::begin()
{
    // Set pins to power-up state
    port_write(0xffff);

    // Read initial value for pinState
    _pinState = readPinState();

    // Build rowMask for quick scanning
    _rowMask = 0;
    for(int i = 0; i < _rows; i++) {
        _rowMask |= (1 << _rowPins[i]);
    }

    // Keys cleared by key constructor
}

void Keypad_I2C::setScanInterval(unsigned int interval)
{
    _scanInterval = (interval < 1) ? 1 : interval;
}

void Keypad_I2C::setHoldTime(unsigned int hold)
{
    _holdTime = hold;
}

void Keypad_I2C::addEventListener(void (*listener)(char, KeyState))
{
    _keypadEventListener = listener;
}

void Keypad_I2C::setCustomDelayFunc(void (*myDelay)(unsigned int))
{
    _customDelayFunc = myDelay;
}

// Trigger a scan and populate the key list
bool Keypad_I2C::scanKeypad()
{
    bool keyActivity = false;

    if((millis() - _startTime) > _scanInterval) {
        scanKeys();
        keyActivity = updateList();
        _startTime = millis();
    }

    return keyActivity;
}

/*
 * Private
 */

// Hardware scan
void Keypad_I2C::scanKeys()
{
    uint16_t pinVals[3][16];
    uint16_t pinVal = 0;
    bool     repeat;
    int      maxRetry = 5;

    do {

        repeat = false;
      
        for(uint8_t d = 0; d < 3; d++) {
    
            for(uint8_t c = 0; c < _columns; c++) {
    
                pin_write(_columnPins[c], LOW);
    
                _wire->requestFrom(_i2caddr, _i2cwidth);
                pinVal = _wire->read();
                if(_i2cwidth > 1) {
                    pinVal |= (_wire->read() << 8);
                }
                pinVals[d][c] = pinVal & _rowMask;
    
                pin_write(_columnPins[c], HIGH);
                
            }
    
            if(d < 2) (*_customDelayFunc)(5);
          
        }

        for(uint8_t c = 0; c < _columns; c++) {
            if((pinVals[0][c] != pinVals[1][c]) || (pinVals[0][c] != pinVals[2][c])) 
                repeat = true;
        }
        
    } while(maxRetry-- && repeat);

    for(uint8_t c = 0; c < _columns; c++) {       
        for(uint8_t r = 0; r < _rows; r++) {
            // keypress is active low, so invert read result
            bitWrite(_bitMap[r], c, (pinVals[0][c] & (1 << _rowPins[r])) ? 0 : 1);
        }
    }
}

// Manage the list without rearranging the keys
// Returns true if any of the keys on the list changed state
bool Keypad_I2C::updateList()
{
    // Delete IDLE keys
    for(uint8_t i = 0; i < KI2C_LISTMAX; i++) {
        if(_key[i].kstate == IDLE) {
            _key[i].kchar = NO_KEY;
            _key[i].kcode = -1;
            _key[i].stateChanged = false;
        }
    }

    // Add new keys to empty slots in the key list
    for(uint8_t r = 0; r < _rows; r++) {

        for(uint8_t c = 0; c < _columns; c++) {

            bool kstate = bitRead(_bitMap[r], c);
            int  keyCode = r * _columns + c;
            char keyChar = _keymap[keyCode];
            int  idx = findInList(keyCode);

            if(idx > -1) {
              
                // Key is already on the list so set its next state
                nextKeyState(idx, kstate);
                
            } else if((idx == -1) && kstate) {
              
                // Key is not on the list so add it
                for(uint8_t i = 0; i < KI2C_LISTMAX; i++) {
                    // Find an empty slot or don't add key to list
                    if(_key[i].kchar == NO_KEY) {
                        _key[i].kchar = keyChar;
                        _key[i].kcode = keyCode;
                        _key[i].kstate = IDLE;
                        nextKeyState (i, kstate);
                        break;
                    }
                }
                
            }
        }
    }

    // Report if any of the keys changed state
    for(uint8_t i = 0; i < KI2C_LISTMAX; i++) {
        if(_key[i].stateChanged) return true;
    }

    return false;
}

void Keypad_I2C::nextKeyState(uint8_t idx, bool kstate)
{
    _key[idx].stateChanged = false;

    switch(_key[idx].kstate) {
    case IDLE:
        if(kstate == CLOSED) {
            transitionTo(idx, PRESSED);
            _key[idx].holdTimer = millis();
        }
        break;
    case PRESSED:
        if((millis() - _key[idx].holdTimer) > _holdTime)
            transitionTo(idx, HOLD);
        else if(kstate == OPEN)
            transitionTo(idx, RELEASED);
        break;
    case HOLD:
        if(kstate == OPEN)
            transitionTo(idx, RELEASED);
        break;
    case RELEASED:
        transitionTo(idx, IDLE);
        break;
    }
}

// Search by code for a key in the list of active keys
// Returns index or -1 if not found
int Keypad_I2C::findInList(int keyCode)
{
    for(uint8_t i = 0; i < KI2C_LISTMAX; i++) {
        if(_key[i].kcode == keyCode)
            return i;
    }

    return -1;
}

void Keypad_I2C::transitionTo(uint8_t idx, KeyState nextState)
{
    _key[idx].kstate = nextState;
    _key[idx].stateChanged = true;

    if(_keypadEventListener != NULL) {
        _keypadEventListener(_key[idx].kchar, _key[idx].kstate);
    }
}

void Keypad_I2C::pin_write(uint8_t pinNum, bool level)
{
    uint16_t mask = 1 << pinNum;

    if(level == HIGH) {
      	_pinState |= mask;
    } else {
        _pinState &= ~mask;
    }

    port_write(_pinState);
}

void Keypad_I2C::port_write(uint16_t i2cportval)
{
    _wire->beginTransmission(_i2caddr);
    _wire->write(i2cportval & 0x00FF);
    if(_i2cwidth > 1) {
        _wire->write(i2cportval >> 8);
    }
    _wire->endTransmission();
    _pinState = i2cportval;
}

uint16_t Keypad_I2C::readPinState()
{
    _wire->requestFrom(_i2caddr, _i2cwidth);

    _pinState = _wire->read();
    if(_i2cwidth > 1) {
        _pinState |= _wire->read() << 8;
    }

    return _pinState;
}

static void defaultDelay(unsigned int mydelay)
{
    delay(mydelay);
}

/*
 * TCButton class (ex-OneButton)
 */

/* pin: The pin to be used
 * activeLow: Set to true when the input level is LOW when the button is pressed, Default is true.
 * pullupActive: Activate the internal pullup when available. Default is true.
 */
TCButton::TCButton(const int pin, const boolean activeLow, const bool pullupActive)
{
    _pin = pin;

    _buttonPressed = activeLow ? LOW : HIGH;
  
    pinMode(pin, pullupActive ? INPUT_PULLUP : INPUT);
}


// Number of millisec that have to pass by before a click is assumed stable.
void TCButton::setDebounceTicks(const int ticks)
{
    _debounceTicks = ticks;
}


// Number of millisec that have to pass by before a click (short press) is detected.
void TCButton::setClickTicks(const int ticks)
{
    _clickTicks = ticks;
}


// Number of millisec that have to pass by before a long press is detected.
void TCButton::setPressTicks(const int ticks)
{
    _pressTicks = ticks;
}

// Register function for click (short press) event
void TCButton::attachClick(void (*newFunction)(void))
{
    _clickFunc = newFunction;
}

// Register function for long press start event
void TCButton::attachLongPressStart(void (*newFunction)(void))
{
    _longPressStartFunc = newFunction;
}

// Register function for long press stop event
void TCButton::attachLongPressStop(void (*newFunction)(void))
{
    _longPressStopFunc = newFunction;
}

void TCButton::reset(void)
{
    _state = OCS_INIT;
    _lastState = OCS_INIT;
    _nClicks = 0;
    _startTime = 0;
}

// Check input of the pin and advance the state machine
void TCButton::tick(void)
{
    if(_pin >= 0) {
        tick(digitalRead(_pin) == _buttonPressed);
    }
}

// Advance to new state
void TCButton::_newState(stateMachine_t nextState)
{
    _lastState = _state;
    _state = nextState;
}

// Run the state machine using given level
void TCButton::tick(bool activeLevel)
{
    unsigned long now = millis();
    unsigned long waitTime = now - _startTime;

    switch(_state) {
    case OCS_INIT:    // Wait for level to go active
        if(activeLevel) {
            _newState(OCS_DOWN);
            _startTime = now;
            _nClicks = 0;
        }
        break;
  
    case OCS_DOWN:  // Wait for pin to go inactive        
        if((!activeLevel) && (waitTime < _debounceTicks)) {
            // Released too quickly, assume some bouncing
            _newState(_lastState);
        } else if(!activeLevel) {
            _newState(OCS_UP);
            _startTime = now;
        } else if((activeLevel) && (waitTime > _pressTicks)) {
            if(_longPressStartFunc) _longPressStartFunc();
            _newState(OCS_PRESS);
        }
        break;
  
    case OCS_UP:    // Wait for pin to get released
        if((activeLevel) && (waitTime < _debounceTicks)) {
            // Pressed too quickly, assume some bouncing
            _newState(_lastState);
        } else if(waitTime >= _debounceTicks) {
            // Count as a short button down
            _nClicks++;
            _newState(OCS_COUNT);
        }
        break;
  
    case OCS_COUNT: // Process click(s)        
        if(activeLevel) {
            _newState(OCS_DOWN);
            _startTime = now;
        } else if((waitTime > _clickTicks) || (_nClicks == _maxClicks)) {
            if(_clickFunc) _clickFunc();
            reset();
        }
        break;
  
    case OCS_PRESS: // Wait for release after long press
        if(!activeLevel) {
            _newState(OCS_PRESSEND);
            _startTime = now;
        } 
        break;
  
    case OCS_PRESSEND:  // Button was released
        if((activeLevel) && (waitTime < _debounceTicks)) {
            // Released too quickly, assume some bouncing
            _newState(_lastState);
        } else if(waitTime >= _debounceTicks) {
            if(_longPressStopFunc) _longPressStopFunc();
            reset();
        }
        break;
  
    default:  // Unknown state detected -> reset state machine
        _newState(OCS_INIT);
        break;
    }

}
 
