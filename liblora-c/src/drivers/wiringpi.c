/*
 * Created on Tue Sep 14 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#include "_drivers.h"

#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>


typedef struct {
    int spi_dev;
    int spi_ss;
    int spi_clck_speed;
} _wiringpi_ops_t;

// GPIO
static int _wiringpi_setup(void* config)
{
    return wiringPiSetup();
}

static void _wiringpi_pin_mode(int pin, int sel, void* config)
{
    pinMode(pin, sel);
}

static uint8_t _wiringpi_pin_read(int pin, void* config)
{
    return (uint8_t)digitalRead(pin);
}

static void _wiringpi_pin_write(int pin, int val, void* config)
{
    digitalWrite(pin, val);
}


// Timing
static void _wiringpi_delay(int ms, void* config)
{
    delay(ms);
}


// SPI
static int _wiringpi_spi_rw(uint8_t *buffer, int len, void* config)
{
    _wiringpi_ops_t* ops = (_wiringpi_ops_t*)config;
    _wiringpi_pin_write(ops->spi_ss, LOW, config);
    int nread = wiringPiSPIDataRW(ops->spi_dev, buffer, len);
    _wiringpi_pin_write(ops->spi_ss, HIGH, config);

    return nread;
}

static int _wiringpi_spi_init(void* config)
{
    _wiringpi_ops_t* ops = (_wiringpi_ops_t*)config;
    _wiringpi_pin_mode(ops->spi_ss, OUTPUT, config);
    return wiringPiSPISetup(ops->spi_dev, ops->spi_clck_speed);
}

static uint8_t _wiringpi_spi_read(uint8_t reg, void* config) {
    uint8_t buffer[2];
    buffer[0] = reg & 0x7F;
    buffer[1] = 0x00;

    _wiringpi_spi_rw(buffer, 2, config);
    return buffer[1];
}

static uint8_t _wiringpi_spi_read_burst(uint8_t reg, uint8_t *buf, int len, void* config)
{
    uint8_t *buffer = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    buffer[0] = reg & 0x7F;
    _wiringpi_spi_rw(buffer, 2, config);

    memcpy(buf, buffer++, len);
    // free(buffer);
}

static void _wiringpi_spi_write(uint8_t reg, uint8_t val, void* config)
{
    uint8_t buffer[2];
    buffer[0] = reg | 0x80;
    buffer[1] = val;

    _wiringpi_spi_rw(buffer, 2, config);
}

static void _wiringpi_spi_write_burst(uint8_t reg, uint8_t *buf, int len, void* config)
{
    uint8_t *buffer = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    buffer[0] = reg & 0x80;
    for (int i = 0; i < len; i++)
        buffer[i + 1] = buf[i];

    _wiringpi_spi_rw(buffer, len + 1, config);

    // free(buffer);
}

// Interrupt
void (*wiringpi_isr_callbacks[64])(int) = {NULL, };
void _wiringpi_isr_callback(int pin)
{
    if (wiringpi_isr_callbacks[pin] != NULL)
        wiringpi_isr_callbacks[pin](pin);
}

void _wiringpi_isr_callback_pin0(void) { _wiringpi_isr_callback(0); }
void _wiringpi_isr_callback_pin1(void) { _wiringpi_isr_callback(1); }
void _wiringpi_isr_callback_pin2(void) { _wiringpi_isr_callback(2); }
void _wiringpi_isr_callback_pin3(void) { _wiringpi_isr_callback(3); }
void _wiringpi_isr_callback_pin4(void) { _wiringpi_isr_callback(4); }
void _wiringpi_isr_callback_pin5(void) { _wiringpi_isr_callback(5); }
void _wiringpi_isr_callback_pin6(void) { _wiringpi_isr_callback(6); }
void _wiringpi_isr_callback_pin7(void) { _wiringpi_isr_callback(7); }
void _wiringpi_isr_callback_pin8(void) { _wiringpi_isr_callback(8); }
void _wiringpi_isr_callback_pin9(void) { _wiringpi_isr_callback(9); }
void _wiringpi_isr_callback_pin10(void) { _wiringpi_isr_callback(10); }
void _wiringpi_isr_callback_pin11(void) { _wiringpi_isr_callback(11); }
void _wiringpi_isr_callback_pin12(void) { _wiringpi_isr_callback(12); }
void _wiringpi_isr_callback_pin13(void) { _wiringpi_isr_callback(13); }
void _wiringpi_isr_callback_pin14(void) { _wiringpi_isr_callback(14); }
void _wiringpi_isr_callback_pin15(void) { _wiringpi_isr_callback(15); }
void _wiringpi_isr_callback_pin16(void) { _wiringpi_isr_callback(16); }
void _wiringpi_isr_callback_pin17(void) { _wiringpi_isr_callback(17); }
void _wiringpi_isr_callback_pin18(void) { _wiringpi_isr_callback(18); }
void _wiringpi_isr_callback_pin19(void) { _wiringpi_isr_callback(19); }
void _wiringpi_isr_callback_pin20(void) { _wiringpi_isr_callback(20); }
void _wiringpi_isr_callback_pin21(void) { _wiringpi_isr_callback(21); }
void _wiringpi_isr_callback_pin22(void) { _wiringpi_isr_callback(22); }
void _wiringpi_isr_callback_pin23(void) { _wiringpi_isr_callback(23); }
void _wiringpi_isr_callback_pin24(void) { _wiringpi_isr_callback(24); }
void _wiringpi_isr_callback_pin25(void) { _wiringpi_isr_callback(25); }
void _wiringpi_isr_callback_pin26(void) { _wiringpi_isr_callback(26); }
void _wiringpi_isr_callback_pin27(void) { _wiringpi_isr_callback(27); }
void _wiringpi_isr_callback_pin28(void) { _wiringpi_isr_callback(28); }
void _wiringpi_isr_callback_pin29(void) { _wiringpi_isr_callback(29); }
void _wiringpi_isr_callback_pin30(void) { _wiringpi_isr_callback(30); }
void _wiringpi_isr_callback_pin31(void) { _wiringpi_isr_callback(31); }
void _wiringpi_isr_callback_pin32(void) { _wiringpi_isr_callback(32); }
void _wiringpi_isr_callback_pin33(void) { _wiringpi_isr_callback(33); }
void _wiringpi_isr_callback_pin34(void) { _wiringpi_isr_callback(34); }
void _wiringpi_isr_callback_pin35(void) { _wiringpi_isr_callback(35); }
void _wiringpi_isr_callback_pin36(void) { _wiringpi_isr_callback(36); }
void _wiringpi_isr_callback_pin37(void) { _wiringpi_isr_callback(37); }
void _wiringpi_isr_callback_pin38(void) { _wiringpi_isr_callback(38); }
void _wiringpi_isr_callback_pin39(void) { _wiringpi_isr_callback(39); }
void _wiringpi_isr_callback_pin40(void) { _wiringpi_isr_callback(40); }
void _wiringpi_isr_callback_pin41(void) { _wiringpi_isr_callback(41); }
void _wiringpi_isr_callback_pin42(void) { _wiringpi_isr_callback(42); }
void _wiringpi_isr_callback_pin43(void) { _wiringpi_isr_callback(43); }
void _wiringpi_isr_callback_pin44(void) { _wiringpi_isr_callback(44); }
void _wiringpi_isr_callback_pin45(void) { _wiringpi_isr_callback(45); }
void _wiringpi_isr_callback_pin46(void) { _wiringpi_isr_callback(46); }
void _wiringpi_isr_callback_pin47(void) { _wiringpi_isr_callback(47); }
void _wiringpi_isr_callback_pin48(void) { _wiringpi_isr_callback(48); }
void _wiringpi_isr_callback_pin49(void) { _wiringpi_isr_callback(49); }
void _wiringpi_isr_callback_pin50(void) { _wiringpi_isr_callback(50); }
void _wiringpi_isr_callback_pin51(void) { _wiringpi_isr_callback(51); }
void _wiringpi_isr_callback_pin52(void) { _wiringpi_isr_callback(52); }
void _wiringpi_isr_callback_pin53(void) { _wiringpi_isr_callback(53); }
void _wiringpi_isr_callback_pin54(void) { _wiringpi_isr_callback(54); }
void _wiringpi_isr_callback_pin55(void) { _wiringpi_isr_callback(55); }
void _wiringpi_isr_callback_pin56(void) { _wiringpi_isr_callback(56); }
void _wiringpi_isr_callback_pin57(void) { _wiringpi_isr_callback(57); }
void _wiringpi_isr_callback_pin58(void) { _wiringpi_isr_callback(58); }
void _wiringpi_isr_callback_pin59(void) { _wiringpi_isr_callback(59); }
void _wiringpi_isr_callback_pin60(void) { _wiringpi_isr_callback(60); }
void _wiringpi_isr_callback_pin61(void) { _wiringpi_isr_callback(61); }
void _wiringpi_isr_callback_pin62(void) { _wiringpi_isr_callback(62); }
void _wiringpi_isr_callback_pin63(void) { _wiringpi_isr_callback(63); }

void _wiringpi_attach_interrupt(int pin, int edge_type, void(*callback)(int), void* config)
{
    wiringpi_isr_callbacks[pin] = callback;

    void (*func)(void);
    switch(pin) {
        case 0: func = &_wiringpi_isr_callback_pin0; break;
        case 1: func = &_wiringpi_isr_callback_pin1; break;
        case 2: func = &_wiringpi_isr_callback_pin2; break;
        case 3: func = &_wiringpi_isr_callback_pin3; break;
        case 4: func = &_wiringpi_isr_callback_pin4; break;
        case 5: func = &_wiringpi_isr_callback_pin5; break;
        case 6: func = &_wiringpi_isr_callback_pin6; break;
        case 7: func = &_wiringpi_isr_callback_pin7; break;
        case 8: func = &_wiringpi_isr_callback_pin8; break;
        case 9: func = &_wiringpi_isr_callback_pin9; break;
        case 10: func = &_wiringpi_isr_callback_pin10; break;
        case 11: func = &_wiringpi_isr_callback_pin11; break;
        case 12: func = &_wiringpi_isr_callback_pin12; break;
        case 13: func = &_wiringpi_isr_callback_pin13; break;
        case 14: func = &_wiringpi_isr_callback_pin14; break;
        case 15: func = &_wiringpi_isr_callback_pin15; break;
        case 16: func = &_wiringpi_isr_callback_pin16; break;
        case 17: func = &_wiringpi_isr_callback_pin17; break;
        case 18: func = &_wiringpi_isr_callback_pin18; break;
        case 19: func = &_wiringpi_isr_callback_pin19; break;
        case 20: func = &_wiringpi_isr_callback_pin20; break;
        case 21: func = &_wiringpi_isr_callback_pin21; break;
        case 22: func = &_wiringpi_isr_callback_pin22; break;
        case 23: func = &_wiringpi_isr_callback_pin23; break;
        case 24: func = &_wiringpi_isr_callback_pin24; break;
        case 25: func = &_wiringpi_isr_callback_pin25; break;
        case 26: func = &_wiringpi_isr_callback_pin26; break;
        case 27: func = &_wiringpi_isr_callback_pin27; break;
        case 28: func = &_wiringpi_isr_callback_pin28; break;
        case 29: func = &_wiringpi_isr_callback_pin29; break;
        case 30: func = &_wiringpi_isr_callback_pin30; break;
        case 31: func = &_wiringpi_isr_callback_pin31; break;
        case 32: func = &_wiringpi_isr_callback_pin32; break;
        case 33: func = &_wiringpi_isr_callback_pin33; break;
        case 34: func = &_wiringpi_isr_callback_pin34; break;
        case 35: func = &_wiringpi_isr_callback_pin35; break;
        case 36: func = &_wiringpi_isr_callback_pin36; break;
        case 37: func = &_wiringpi_isr_callback_pin37; break;
        case 38: func = &_wiringpi_isr_callback_pin38; break;
        case 39: func = &_wiringpi_isr_callback_pin39; break;
        case 40: func = &_wiringpi_isr_callback_pin40; break;
        case 41: func = &_wiringpi_isr_callback_pin41; break;
        case 42: func = &_wiringpi_isr_callback_pin42; break;
        case 43: func = &_wiringpi_isr_callback_pin43; break;
        case 44: func = &_wiringpi_isr_callback_pin44; break;
        case 45: func = &_wiringpi_isr_callback_pin45; break;
        case 46: func = &_wiringpi_isr_callback_pin46; break;
        case 47: func = &_wiringpi_isr_callback_pin47; break;
        case 48: func = &_wiringpi_isr_callback_pin48; break;
        case 49: func = &_wiringpi_isr_callback_pin49; break;
        case 50: func = &_wiringpi_isr_callback_pin50; break;
        case 51: func = &_wiringpi_isr_callback_pin51; break;
        case 52: func = &_wiringpi_isr_callback_pin52; break;
        case 53: func = &_wiringpi_isr_callback_pin53; break;
        case 54: func = &_wiringpi_isr_callback_pin54; break;
        case 55: func = &_wiringpi_isr_callback_pin55; break;
        case 56: func = &_wiringpi_isr_callback_pin56; break;
        case 57: func = &_wiringpi_isr_callback_pin57; break;
        case 58: func = &_wiringpi_isr_callback_pin58; break;
        case 59: func = &_wiringpi_isr_callback_pin59; break;
        case 60: func = &_wiringpi_isr_callback_pin60; break;
        case 61: func = &_wiringpi_isr_callback_pin61; break;
        case 62: func = &_wiringpi_isr_callback_pin62; break;
        case 63: func = &_wiringpi_isr_callback_pin63; break;
    }

    wiringPiISR(pin, edge_type, func);
}


liblora_driver_t* liblora_driver_wiringpi(int spi_dev, int spi_ss)
{
    _wiringpi_ops_t* ops = (_wiringpi_ops_t*)malloc(sizeof(_wiringpi_ops_t));
    ops->spi_dev = spi_dev;
    ops->spi_ss = spi_ss;
    ops->spi_clck_speed = 500000;

    liblora_driver_t* driver = (liblora_driver_t*)malloc(sizeof(liblora_driver_t));

    driver->config = ops;
    driver->setup = _wiringpi_setup;
    driver->pin_mode = _wiringpi_pin_mode;
    driver->pin_write = _wiringpi_pin_write;
    driver->pin_read = _wiringpi_pin_read;
    driver->delay = _wiringpi_delay;
    driver->spi_init = _wiringpi_spi_init;
    driver->spi_read = _wiringpi_spi_read;
    driver->spi_read_burst = _wiringpi_spi_read_burst;
    driver->spi_write = _wiringpi_spi_write;
    driver->spi_write_burst = _wiringpi_spi_write_burst;
    driver->attach_interrupt = _wiringpi_attach_interrupt;

    return driver;
}
