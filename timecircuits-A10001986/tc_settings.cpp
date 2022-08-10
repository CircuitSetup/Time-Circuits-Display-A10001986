/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Time Circuits Display
 * Code adapted from Marmoset Electronics 
 * https://www.marmosetelectronics.com/time-circuits-clock
 * by John Monaco
 * Enhanced/modified in 2022 by Thomas Winischhofer (A10001986)
 * -------------------------------------------------------------------
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "tc_settings.h"

/*
 * Read configuration from JSON config file
 * If config file not found, create one with default settings
 */

bool haveFS = false;

/*
 * settings_setup()
 * 
 */
void settings_setup() 
{
  bool writedefault = false;

  #ifdef TC_DBG
  Serial.println("settings_setup: Mounting FS...");
  #endif

  if(SPIFFS.begin()) {

    #ifdef TC_DBG
    Serial.println("settings_setup: Mounted file system");
    #endif
    
    haveFS = true;
    
    if(SPIFFS.exists("/config.json")) {
      
      //file exists, load and parse it     
      File configFile = SPIFFS.open("/config.json", "r");
      
      if (configFile) {

        #ifdef TC_DBG
        Serial.println("settings_setup: Opened config file");
        #endif

        StaticJsonDocument<1024> json;
        DeserializationError error = deserializeJson(json, configFile);

        #ifdef TC_DBG
        serializeJson(json, Serial);
        #endif
        
        if (!error) {

          #ifdef TC_DBG
          Serial.println("\nsettings_setup: Parsed json");
          #endif
          
          if(json["ntpServer"]) {
              strcpy(settings.ntpServer, json["ntpServer"]);
          } else writedefault = true;          
          if(json["timeZone"]) {
              strcpy(settings.timeZone, json["timeZone"]);
          } else writedefault = true;
          if(json["autoRotateTimes"]) {
              strcpy(settings.autoRotateTimes, json["autoRotateTimes"]);
          } else writedefault = true;
          if(json["destTimeBright"]) {
              strcpy(settings.destTimeBright, json["destTimeBright"]);
          } else writedefault = true;
          if(json["presTimeBright"]) {
              strcpy(settings.presTimeBright, json["presTimeBright"]);
          } else writedefault = true;
          if(json["lastTimeBright"]) {
              strcpy(settings.lastTimeBright, json["lastTimeBright"]);
          } else writedefault = true;
          //if(json["beepSound"]) {
          //  strcpy(settings.beepSound, json["beepSound"]);
          //} else writedefault = true;
          if(json["wifiConRetries"]) {
              strcpy(settings.wifiConRetries, json["wifiConRetries"]);
          } else writedefault = true;
          if(json["wifiConTimeout"]) {
              strcpy(settings.wifiConTimeout, json["wifiConTimeout"]);
          } else writedefault = true;
          if(json["mode24"]) {
              strcpy(settings.mode24, json["mode24"]);
          } else writedefault = true;
          if(json["timeTrPers"]) {
              strcpy(settings.timesPers, json["timeTrPers"]);
          } else writedefault = true;
          #ifdef FAKE_POWER_ON
          if(json["fakePwrOn"]) {
              strcpy(settings.fakePwrOn, json["fakePwrOn"]);
          } else writedefault = true;
          #endif
          
        } else {
          
          Serial.println("settings_setup: Failed to parse json config");

          writedefault = true;
          
        }
        
        configFile.close();
      }
      
    } else {

      writedefault = true;
      
    }

    if(writedefault) {
      
      // config file does not exist or is incomplete - create one 
      
      Serial.println("settings_setup: Settings missing or incomplete; writing new config file");
      
      write_settings();
      
    }
    
  } else {
    
    Serial.println("settings_setup: Failed to mount FS");
  
  }
}

void write_settings() 
{
    StaticJsonDocument<1024> json;
  
    if(!haveFS) {
      Serial.println("write_settings: Cannot write settings, FS not mounted");
      return;
    } 
  
    #ifdef TC_DBG
    Serial.println("write_settings: Writing config file");
    #endif
    
    json["ntpServer"] = settings.ntpServer;
    json["timeZone"] = settings.timeZone;
    json["autoRotateTimes"] = settings.autoRotateTimes;
    json["destTimeBright"] = settings.destTimeBright;
    json["presTimeBright"] = settings.presTimeBright;
    json["lastTimeBright"] = settings.lastTimeBright;
    //json["beepSound"] = settings.beepSound;
    json["wifiConRetries"] = settings.wifiConRetries;
    json["wifiConTimeout"] = settings.wifiConTimeout;
    json["mode24"] = settings.mode24;
    json["timeTrPers"] = settings.timesPers;
    #ifdef FAKE_POWER_ON    
    json["fakePwrOn"] = settings.fakePwrOn;    
    #endif
  
    File configFile = SPIFFS.open("/config.json", FILE_WRITE);
  
    #ifdef TC_DBG
    serializeJson(json, Serial);
    Serial.println("\n");
    #endif
    
    serializeJson(json, configFile);
  
    configFile.close(); 
  
}

/* 
 *  Load the Alarm time and settings from alarmconfig
 *  
 */

bool loadAlarm()
{
    bool writedefault = false;
    
    if(!haveFS) {
      
        Serial.println("loadAlarm(): FS not mounted, using EEPROM");
        
        return loadAlarmEEPROM();
        
    } 

    if(SPIFFS.exists("/alarmconfig.json")) {
      
      //file exists, load and parse it     
      File configFile = SPIFFS.open("/alarmconfig.json", "r");
      
      if (configFile) {

        #ifdef TC_DBG
        Serial.println("loadAlarm: Opened alarmconfig file");
        #endif

        StaticJsonDocument<1024> json;
        DeserializationError error = deserializeJson(json, configFile);

        #ifdef TC_DBG
        serializeJson(json, Serial);
        #endif
        
        if (!error) {

          #ifdef TC_DBG
          Serial.println("\nloadAlarm: Parsed json");
          #endif
          
          if(json["alarmonoff"]) {
              alarmOnOff = (atoi(json["alarmonoff"]) != 0) ? true : false;              
          } else writedefault = true;          
          if(json["alarmhour"]) {
              alarmHour = atoi(json["alarmhour"]);              
          } else writedefault = true;
          if(json["alarmmin"]) {
              alarmMinute = atoi(json["alarmmin"]); 
          } else writedefault = true;                    
          
        } else {
          
          Serial.println("loadAlarm: Failed to parse json config");

          writedefault = true;
          
        }
        
        configFile.close();
      }
      
    } else {

      writedefault = true;
      
    }

    if(writedefault) {
      
      // alarmconfig file does not exist or is incomplete - create one 
      
      Serial.println("loadAlarm: Alarm settings missing or incomplete; writing new config file");
      
      saveAlarm();
      
    }    
}

bool loadAlarmEEPROM() 
{
    #ifdef TC_DBG
    Serial.println("Load Alarm EEPROM");
    #endif

    uint8_t loadBuf[4];    // on/off, hour, minute, checksum
    uint16_t sum = 0;
    int i;    
        
    for(i = 0; i < 4; i++) {
        loadBuf[i] = EEPROM.read(ALARM_PREF + i);
        if(i < 3) sum += (loadBuf[i] ^ 0x55);
    }

    if((sum & 0xff) != loadBuf[3]) {
          
        Serial.println("loadAlarm: Invalid alarm data in EEPROM");

        alarmOnOff = false;
        alarmHour = alarmMinute = 255;    // means "unset"
    
        return false;
    }        

    alarmOnOff = loadBuf[0] ? true : false;
    alarmHour = loadBuf[1];
    alarmMinute = loadBuf[2];

    return true;
}

void saveAlarm() 
{
    StaticJsonDocument<1024> json;
    char hourBuf[8];
    char minBuf[8];

    #ifdef TC_DBG
    Serial.println("Save Alarm");
    #endif

    if(!haveFS) {
        Serial.println("saveAlarm(): FS not mounted, using EEPROM");
        
        saveAlarmEEPROM();
        
        return;        
    } 
  
    json["alarmonoff"] = alarmOnOff ? "1" : "0";

    sprintf(hourBuf, "%d", alarmHour);
    sprintf(minBuf, "%d", alarmMinute);
    
    json["alarmhour"] = hourBuf;
    json["alarmmin"] = minBuf;

    File configFile = SPIFFS.open("/alarmconfig.json", FILE_WRITE);
  
    #ifdef TC_DBG
    serializeJson(json, Serial);
    Serial.println("\n");
    #endif
    
    serializeJson(json, configFile);
  
    configFile.close(); 
}    

void saveAlarmEEPROM()
{
    uint8_t savBuf[4];    // on/off, hour, minute, checksum
    uint16_t sum = 0;
    int i;    

    savBuf[0] = alarmOnOff ? 1 : 0;
    savBuf[1] = alarmHour;
    savBuf[2] = alarmMinute;
        
    for(i = 0; i < 3; i++) {
        EEPROM.write(ALARM_PREF + i, savBuf[i]);
        sum += (savBuf[i] ^ 0x55);
    }
    EEPROM.write(ALARM_PREF + 3, (sum & 0xff));   
    
    EEPROM.commit();
    
    return;
}
