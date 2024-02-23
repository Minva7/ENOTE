
/*

#include "Arduino.h"
#include "../lib/FT6336.h"
#include <Wire.h>

FT6336::FT6336(uint8_t _sda, uint8_t _scl, uint8_t _int, uint8_t _rst, uint16_t _width, uint16_t _height) :
  pinSda(_sda), pinScl(_scl), pinInt(39), pinRst(33), width(320), height(480) {

}

static uint8_t _SDA = 32;
static uint8_t _SCL = 25;

void FT6336::begin(uint8_t _addr) {
  addr = _addr;
  //Wire.begin(pinSda, pinScl);
  Wire.begin((int) SDA, (int)SCL);
  Wire.begin(32, 25 );
  Wire.setClock(400000);
  Wire.begin();
  if(reset())
  {
    Serial.print("touch ic error!\r\n");
  }
}



uint8_t FT6336::reset() {
  uint8_t tmp[2];
  pinMode(pinInt, INPUT);
  pinMode(pinRst, OUTPUT);
  digitalWrite(pinInt, 1);
  digitalWrite(pinRst, 1);
  delay(20);
  digitalWrite(pinRst, 0);
  delay(20);
  digitalWrite(pinRst, 1);
  delay(500);
  readBlockData(&tmp[0], FT6336_ID_G_FOCALTECH_ID, 1);
  if(tmp[0] != 0x11)
  {
    return 1;
  }
  readBlockData(tmp, FT6336_ID_G_CIPHER_MID, 2);
  if(tmp[0] != 0x26)
  {
    return 1;
  }
  if((tmp[1]!=0x00)&&(tmp[1]!=0x01)&&(tmp[1]!=0x02))
  {
    return 1;
  }
  readBlockData(&tmp[0], FT6336_ID_G_CIPHER_HIGH, 1);
  if(tmp[0] != 0x64)
  {
    return 1;
  }
  return 0;
}

void FT6336::setRotation(uint8_t rot) {
  rotation = rot;
}

void FT6336::read(void) {
  uint8_t data[4];
  uint8_t pointInfo = readByteData(FT6336_TD_STATUS);
  touches = pointInfo;
  isTouched = (touches > 0 && touches < 3);
  if (isTouched) {
    for (uint8_t i=0; i<touches; i++) {
      readBlockData(data, FT6336_TOUCH_1 + i * 6, 4);
      points[i] = readPoint(data);
    }
  }
}
TP_Point FT6336::readPoint(uint8_t *data) {
  uint16_t temp;
  uint8_t id = data[2] >> 4;
  uint16_t x = (uint16_t)((data[0]&0x0F)<<8) + data[1];
  uint16_t y = (uint16_t)((data[2]&0x0F)<<8) + data[3];
  switch (rotation){
    case ROTATION_NORMAL:
      x = x;
      y = y;
      break;
    case ROTATION_LEFT:
      temp = x;
      x = height - y;
      y = temp;
      break;
    case ROTATION_INVERTED:
      x = width - x;
      y = height - y;
      break;
    case ROTATION_RIGHT:
      temp = x;
      x = y;
      y = width - temp;
      break;
    default:
      break;
  }
  return TP_Point(id, x, y, 0);
}
void FT6336::writeByteData(uint16_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
 // Wire.write(highByte(reg));
  Wire.write(lowByte(reg));
  Wire.write(val);
  Wire.endTransmission();
}
uint8_t FT6336::readByteData(uint16_t reg) {
  uint8_t x;
  Wire.beginTransmission(addr);
 // Wire.write(highByte(reg));
  Wire.write(lowByte(reg));
  Wire.endTransmission();
  Wire.requestFrom(addr, (uint8_t)1);
  x = Wire.read();
  return x;
}
void FT6336::writeBlockData(uint16_t reg, uint8_t *val, uint8_t size) {
  Wire.beginTransmission(addr);
//  Wire.write(highByte(reg));
  Wire.write(lowByte(reg));
  // Wire.write(val, size);
  for (uint8_t i=0; i<size; i++) {
    Wire.write(val[i]);
  }
  Wire.endTransmission();
}
void FT6336::readBlockData(uint8_t *buf, uint16_t reg, uint8_t size) {
  Wire.beginTransmission(addr);
//  Wire.write(highByte(reg));
  Wire.write(lowByte(reg));
  Wire.endTransmission();
  Wire.requestFrom(addr, size);
  for (uint8_t i=0; i<size; i++) {
    buf[i] = Wire.read();
  }
}
TP_Point::TP_Point(void) {
  id = x = y = size = 0;
}
TP_Point::TP_Point(uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _size) {
  id = _id;
  x = _x;
  y = _y;
  size = _size;
}
bool TP_Point::operator==(TP_Point point) {
  return ((point.x == x) && (point.y == y) && (point.size == size));
}
bool TP_Point::operator!=(TP_Point point) {
  return ((point.x != x) || (point.y != y) || (point.size != size));
}
*/

/**************************************************************************/
/*!
  @file     FT6336U.cpp
  Author: Atsushi Sasaki (https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#include "../lib/FT6336.h"

#include <Wire.h>

FT6336U::FT6336U(uint8_t rst_n_pin, uint8_t int_n_pin)
{
    rst_n = rst_n_pin;
    int_n = int_n_pin;
}

FT6336U::FT6336U(uint8_t sda_pin, uint8_t scl_pin, uint8_t rst_n_pin, uint8_t int_n_pin)
{
    sda = sda_pin;
    scl = scl_pin;
    rst_n = rst_n_pin;
    int_n = int_n_pin;
}

FT6336U::~FT6336U() {
}

void FT6336U::begin(void) {
    // Initialize I2C
    if(sda != -1 && scl != -1) {
        Wire.begin((int) sda, (int)scl);
        Wire.begin(32, 25);
        Wire.begin();
    }
    else {
        Wire.begin(); 
    }
	// Int Pin Configuration
	pinMode(int_n, INPUT); 
    // Reset Pin Configuration
	pinMode(rst_n, OUTPUT); 
	digitalWrite(rst_n, LOW); 
	delay(10); 
	digitalWrite(rst_n, HIGH); 
	delay(500); 
}


 // @设置旋转方向,默认为Rotation_0
 // @Rotation：方向 0~3

void FT6336U::setRotation(TouchRotation Rotation)
{
    switch (Rotation) {
    case Rotation_0:
      touch_rotation = Rotation_0;
      break;
    case Rotation_1:
      touch_rotation = Rotation_1;
      break;
    case Rotation_2:
      touch_rotation = Rotation_2;
      break;
    case Rotation_3:
      touch_rotation = Rotation_3;
      break;
    }
}

uint8_t FT6336U::read_device_mode(void) {
    return (readByte(FT6336U_ADDR_DEVICE_MODE) & 0x70) >> 4;
}
void FT6336U::write_device_mode(DEVICE_MODE_Enum mode) {
    writeByte(FT6336U_ADDR_DEVICE_MODE, (mode & 0x07) << 4);
}
uint8_t FT6336U::read_gesture_id(void) {
    return readByte(FT6336U_ADDR_GESTURE_ID);
}
uint8_t FT6336U::read_td_status(void) {
    return readByte(FT6336U_ADDR_TD_STATUS);
}
uint8_t FT6336U::read_touch_number(void) {
    return readByte(FT6336U_ADDR_TD_STATUS) & 0x0F;
}
// Touch 1 functions
uint16_t FT6336U::read_touch1_x(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_X);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_X + 1);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint16_t FT6336U::read_touch1_y(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH1_Y);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH1_Y + 1);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t FT6336U::read_touch1_event(void) {
    return readByte(FT6336U_ADDR_TOUCH1_EVENT) >> 6;
}
uint8_t FT6336U::read_touch1_id(void) {
    return readByte(FT6336U_ADDR_TOUCH1_ID) >> 4;
}
uint8_t FT6336U::read_touch1_weight(void) {
    return readByte(FT6336U_ADDR_TOUCH1_WEIGHT);
}
uint8_t FT6336U::read_touch1_misc(void) {
    return readByte(FT6336U_ADDR_TOUCH1_MISC) >> 4;
}
// Touch 2 functions
uint16_t FT6336U::read_touch2_x(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_X);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_X + 1);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint16_t FT6336U::read_touch2_y(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_TOUCH2_Y);
    read_buf[1] = readByte(FT6336U_ADDR_TOUCH2_Y + 1);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t FT6336U::read_touch2_event(void) {
    return readByte(FT6336U_ADDR_TOUCH2_EVENT) >> 6;
}
uint8_t FT6336U::read_touch2_id(void) {
    return readByte(FT6336U_ADDR_TOUCH2_ID) >> 4;
}
uint8_t FT6336U::read_touch2_weight(void) {
    return readByte(FT6336U_ADDR_TOUCH2_WEIGHT);
}
uint8_t FT6336U::read_touch2_misc(void) {
    return readByte(FT6336U_ADDR_TOUCH2_MISC) >> 4;
}

// Mode Parameter Register
uint8_t FT6336U::read_touch_threshold(void) {
    return readByte(FT6336U_ADDR_THRESHOLD);
}
uint8_t FT6336U::read_filter_coefficient(void) {
    return readByte(FT6336U_ADDR_FILTER_COE);
}
uint8_t FT6336U::read_ctrl_mode(void) {
    return readByte(FT6336U_ADDR_CTRL);
}
void FT6336U::write_ctrl_mode(CTRL_MODE_Enum mode) {
    writeByte(FT6336U_ADDR_CTRL, mode);
}
uint8_t FT6336U::read_time_period_enter_monitor(void) {
    return readByte(FT6336U_ADDR_TIME_ENTER_MONITOR);
}
uint8_t FT6336U::read_active_rate(void) {
    return readByte(FT6336U_ADDR_ACTIVE_MODE_RATE);
}
uint8_t FT6336U::read_monitor_rate(void) {
    return readByte(FT6336U_ADDR_MONITOR_MODE_RATE);
}

// Gesture Parameters
uint8_t FT6336U::read_radian_value(void) {
	return readByte(FT6336U_ADDR_RADIAN_VALUE);
}
void FT6336U::write_radian_value(uint8_t val) {
	writeByte(FT6336U_ADDR_RADIAN_VALUE, val); 
}
uint8_t FT6336U::read_offset_left_right(void) {
	return readByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT);
}
void FT6336U::write_offset_left_right(uint8_t val) {
	writeByte(FT6336U_ADDR_OFFSET_LEFT_RIGHT, val); 
}
uint8_t FT6336U::read_offset_up_down(void) {
	return readByte(FT6336U_ADDR_OFFSET_UP_DOWN);
}
void FT6336U::write_offset_up_down(uint8_t val) {
	writeByte(FT6336U_ADDR_OFFSET_UP_DOWN, val); 
}
uint8_t FT6336U::read_distance_left_right(void) {
	return readByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT);
}
void FT6336U::write_distance_left_right(uint8_t val) {
	writeByte(FT6336U_ADDR_DISTANCE_LEFT_RIGHT, val); 
}
uint8_t FT6336U::read_distance_up_down(void) {
	return readByte(FT6336U_ADDR_DISTANCE_UP_DOWN);
}
void FT6336U::write_distance_up_down(uint8_t val) {
	writeByte(FT6336U_ADDR_DISTANCE_UP_DOWN, val); 
}
uint8_t FT6336U::read_distance_zoom(void) {
	return readByte(FT6336U_ADDR_DISTANCE_ZOOM);
}
void FT6336U::write_distance_zoom(uint8_t val) {
	writeByte(FT6336U_ADDR_DISTANCE_ZOOM, val); 
}


// System Information
uint16_t FT6336U::read_library_version(void) {
    uint8_t read_buf[2];
    read_buf[0] = readByte(FT6336U_ADDR_LIBRARY_VERSION_H);
    read_buf[1] = readByte(FT6336U_ADDR_LIBRARY_VERSION_L);
	return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}
uint8_t FT6336U::read_chip_id(void) {
    return readByte(FT6336U_ADDR_CHIP_ID);
}
uint8_t FT6336U::read_g_mode(void) {
    return readByte(FT6336U_ADDR_G_MODE);
}
void FT6336U::write_g_mode(G_MODE_Enum mode){
	writeByte(FT6336U_ADDR_G_MODE, mode); 
}
uint8_t FT6336U::read_pwrmode(void) {
    return readByte(FT6336U_ADDR_POWER_MODE);
}
uint8_t FT6336U::read_firmware_id(void) {
    return readByte(FT6336U_ADDR_FIRMARE_ID);
}
uint8_t FT6336U::read_focaltech_id(void) {
    return readByte(FT6336U_ADDR_FOCALTECH_ID);
}
uint8_t FT6336U::read_release_code_id(void) {
    return readByte(FT6336U_ADDR_RELEASE_CODE_ID);
}
uint8_t FT6336U::read_state(void) {
    return readByte(FT6336U_ADDR_STATE);
}


//coordinate diagram（FPC downwards）
////y ////////////////////264x176
						//
						//
						//x
						//
						//
FT6336U_TouchPointType FT6336U::scan(void){
    touchPoint.touch_count = read_td_status(); 

    if(touchPoint.touch_count == 0) {
        touchPoint.tp[0].status = release; 
        touchPoint.tp[1].status = release; 
    }
    else if(touchPoint.touch_count == 1) {
        uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream;
        switch (touch_rotation) {
            case Rotation_0://不变
                touchPoint.tp[id1].x = read_touch1_x(); 
                touchPoint.tp[id1].y = read_touch1_y();
            break;
            case Rotation_1:
                touchPoint.tp[id1].x = read_touch1_y(); 
                touchPoint.tp[id1].y = TouchWidth - read_touch1_x();
            break;
            case Rotation_2:
                touchPoint.tp[id1].x = TouchWidth - read_touch1_x(); 
                touchPoint.tp[id1].y = TouchHeight - read_touch1_y();
            break;
            case Rotation_3:
                touchPoint.tp[id1].x = TouchHeight - read_touch1_y(); 
                touchPoint.tp[id1].y = read_touch1_x();
            break;
        }  
        touchPoint.tp[~id1 & 0x01].status = release; 
    }
    else {
        uint8_t id1 = read_touch1_id(); // id1 = 0 or 1
        touchPoint.tp[id1].status = (touchPoint.tp[id1].status == release) ? touch : stream; 
        switch (touch_rotation) {
            case Rotation_0://不变
                touchPoint.tp[id1].x = read_touch1_x(); 
                touchPoint.tp[id1].y = read_touch1_y();
            break;
            case Rotation_1:
                touchPoint.tp[id1].x = read_touch1_y(); 
                touchPoint.tp[id1].y = TouchWidth - read_touch1_x();
            break;
            case Rotation_2:
                touchPoint.tp[id1].x = TouchWidth - read_touch1_x(); 
                touchPoint.tp[id1].y = TouchHeight - read_touch1_y();
            break;
            case Rotation_3:
                touchPoint.tp[id1].x = TouchHeight - read_touch1_y(); 
                touchPoint.tp[id1].y = read_touch1_x();
            break;
        }
        uint8_t id2 = read_touch2_id(); // id2 = 0 or 1(~id1 & 0x01)
        touchPoint.tp[id2].status = (touchPoint.tp[id2].status == release) ? touch : stream; 
        switch (touch_rotation) {
            case Rotation_0://不变
                touchPoint.tp[id2].x = read_touch2_x(); 
                touchPoint.tp[id2].y = read_touch2_y();
            break;
            case Rotation_1:
                touchPoint.tp[id2].x = read_touch2_y(); 
                touchPoint.tp[id2].y = TouchWidth - read_touch2_x();
            break;
            case Rotation_2:
                touchPoint.tp[id2].x = TouchWidth - read_touch2_x(); 
                touchPoint.tp[id2].y = TouchHeight - read_touch2_y();
            break;
            case Rotation_3:
                touchPoint.tp[id2].x = TouchHeight - read_touch2_y(); 
                touchPoint.tp[id2].y = read_touch2_x();
            break;
        }
    }

    return touchPoint; 

}


// Private Function
uint8_t FT6336U::readByte(uint8_t addr) {
    uint8_t rdData; 
    uint8_t rdDataCount; 
    do {
        Wire.beginTransmission(I2C_ADDR_FT6336U); 
        Wire.write(addr); 
        Wire.endTransmission(false); // Restart
        delay(10); 
        rdDataCount = Wire.requestFrom(I2C_ADDR_FT6336U, 1); 
    } while(rdDataCount == 0); 
    while(Wire.available()) {
        rdData = Wire.read(); 
    }
    return rdData; 

}
void FT6336U::writeByte(uint8_t addr, uint8_t data) {
    DEBUG_PRINTLN("")
    DEBUG_PRINT("writeI2C reg 0x")
    DEBUG_PRINT(addr, HEX)
    DEBUG_PRINT(" -> 0x") DEBUG_PRINTLN(data, HEX)
	
	Wire.beginTransmission(I2C_ADDR_FT6336U); 
    Wire.write(addr); 
    Wire.write(data); 
    Wire.endTransmission(); 
}
