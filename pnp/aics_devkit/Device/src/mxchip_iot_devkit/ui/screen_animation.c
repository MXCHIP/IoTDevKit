#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wiring.h"
#include "IoT_DevKit_HW.h"
#include "screen.h"
#include "screen_animation.h"

#define OLED_SINGLE_FRAME_BUFFER 576
#define OFFSET_X    20
#define OFFSET_Y    2

static const unsigned char block[]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const unsigned char blockGap[]  = {0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E};
static const unsigned char blockVGap[]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
static const unsigned char cross[]  = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};
static const unsigned char diagRL[] = {0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x03};
static const unsigned char diagLR[] = {0x03, 0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0, 0xC0};
static const unsigned char horzB[]  = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
static const unsigned char horzT[]  = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
static const unsigned char vertL[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
static const unsigned char vertR[]  = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char circle[] = {0x18, 0x7E, 0x7E, 0xFF, 0xFF, 0x7E, 0x7E, 0x18};
static const unsigned char clear[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char borderBottom[]  = {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0};
static const unsigned char borderTop[]  = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
static const unsigned char borderLeft[]  = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char borderRight[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};
static const unsigned char cornerLB[]  = {0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0};
static const unsigned char cornerRB[]  = {0xC0, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF};
static const unsigned char cornerLT[]  = {0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
static const unsigned char cornerRT[]  = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF};
static const unsigned char circleLT[]  = {0x00, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE};
static const unsigned char circleRT[]  = {0xFE, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF0, 0x00};
static const unsigned char circleLB[]  = {0x00, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F};
static const unsigned char circleRB[]  = {0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x00};

static void render_frame_to_buffer(unsigned char *screenBuffer, const char *image)
{
    int columnPad = 3;
    int colLimit = 8;

    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < colLimit; x++) {
            if (image[(y * colLimit) + x] == 'B')
                memcpy(screenBuffer + columnPad, block, 8);
            else if (image[(y * colLimit) + x] == '.')
                memcpy(screenBuffer + columnPad, clear, 8);
            else if (image[(y * colLimit) + x] == 'G')
                memcpy(screenBuffer + columnPad, blockGap, 8);
            else if (image[(y * colLimit) + x] == 'g')
                memcpy(screenBuffer + columnPad, blockVGap, 8);
            else if (image[(y * colLimit) + x] == 'X')
                memcpy(screenBuffer + columnPad, cross, 8);
            else if (image[(y * colLimit) + x] == 'L')
                memcpy(screenBuffer + columnPad, diagLR, 8);
            else if (image[(y * colLimit) + x] == 'R')
                memcpy(screenBuffer + columnPad, diagRL, 8);
            else if (image[(y * colLimit) + x] == 'H')
                memcpy(screenBuffer + columnPad, horzT, 8);
            else if (image[(y * colLimit) + x] == 'h')
                memcpy(screenBuffer + columnPad, horzB, 8);
            else if (image[(y * colLimit) + x] == 'V')
                memcpy(screenBuffer + columnPad, vertL, 8);
            else if (image[(y * colLimit) + x] == 'v')
                memcpy(screenBuffer + columnPad, vertR, 8);
            else if (image[(y * colLimit) + x] == 'O')
                memcpy(screenBuffer + columnPad, circle, 8);
            else if (image[(y * colLimit) + x] == 'T')
                memcpy(screenBuffer + columnPad, borderTop, 8);
            else if (image[(y * colLimit) + x] == 'b')
                memcpy(screenBuffer + columnPad, borderBottom, 8);
            else if (image[(y * colLimit) + x] == '<')
                memcpy(screenBuffer + columnPad, borderLeft, 8);
            else if (image[(y * colLimit) + x] == '>')
                memcpy(screenBuffer + columnPad, borderRight, 8);
            else if (image[(y * colLimit) + x] == '1')
                memcpy(screenBuffer + columnPad, cornerLT, 8);
            else if (image[(y * colLimit) + x] == '2')
                memcpy(screenBuffer + columnPad, cornerRT, 8);
            else if (image[(y * colLimit) + x] == '3')
                memcpy(screenBuffer + columnPad, cornerLB, 8);
            else if (image[(y * colLimit) + x] == '4')
                memcpy(screenBuffer + columnPad, cornerRB, 8);
            else if (image[(y * colLimit) + x] == '!')
                memcpy(screenBuffer + columnPad, circleLT, 8);
            else if (image[(y * colLimit) + x] == '@')
                memcpy(screenBuffer + columnPad, circleRT, 8);
            else if (image[(y * colLimit) + x] == '#')
                memcpy(screenBuffer + columnPad, circleLB, 8);
            else if (image[(y * colLimit) + x] == '$')
                memcpy(screenBuffer + columnPad, circleRB, 8);

            columnPad = columnPad + 8;
        }
        columnPad = columnPad + 8;
    }
}

static void render_frame_to_screen(unsigned char *screenBuffer, int numFrames, int offset_x, int offset_y, unsigned frameDelay)
{
    const int width = 64;
    unsigned char xs = 0;
    unsigned char ys = offset_y;
    unsigned char xe = 0;
    unsigned char ye = 8;

    for (int i = 0; i < numFrames; i++)
    {
        unsigned char * buffer = screenBuffer + (i * OLED_SINGLE_FRAME_BUFFER);

        xe = width + 8 + offset_x;
        drawDevKitScreen(xs + offset_x, ys, xe, ye, buffer);

        if (frameDelay > 0)
        {
            delay(frameDelay);
        }
    }
}

static void animate_circular(const char ** source, int numFrames, int times)
{
    unsigned char *buffer = calloc(numFrames * OLED_SINGLE_FRAME_BUFFER, 1);
    if (buffer)
    {
        for (int i = 0; i < numFrames; i++)
        {
            render_frame_to_buffer(buffer + (i * OLED_SINGLE_FRAME_BUFFER), source[i]);
        }

        for(int i = 0; i < times; i++)
        {
            render_frame_to_screen(buffer, numFrames, OFFSET_X, OFFSET_Y, 50);
        }

        free(buffer);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Countdown
static const char die1[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', '.', '.', '.', '>', '.', '.', '.',
    '<', '.', 'O', '.', '>', '.', '.', '.',
    '<', '.', '.', '.', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die2[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', '.', '.', '>', '.', '.', '.',
    '<', '.', '.', '.', '>', '.', '.', '.',
    '<', '.', '.', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die3[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', '.', '.', '>', '.', '.', '.',
    '<', '.', 'O', '.', '>', '.', '.', '.',
    '<', '.', '.', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die4[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '<', '.', '.', '.', '>', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die5[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '<', '.', 'O', '.', '>', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die6[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die7[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '<', 'O', 'O', 'O', '>', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die8[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', 'O', 'O', '>', '.', '.', '.',
    '<', 'O', '.', 'O', '>', '.', '.', '.',
    '<', 'O', 'O', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char die9[] = {
    '1', 'T', 'T', 'T', '2', '.', '.', '.',
    '<', 'O', 'O', 'O', '>', '.', '.', '.',
    '<', 'O', 'O', 'O', '>', '.', '.', '.',
    '<', 'O', 'O', 'O', '>', '.', '.', '.',
    '3', 'b', 'b', 'b', '4', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char *die[] = { die1, die2, die3, die4, die5, die6, die7, die8, die9 };

void play_countdown_animation(int value) 
{
    unsigned char buffer[OLED_SINGLE_FRAME_BUFFER] = {0};
    char msg[32];
    
    if (value <= 0 || value > 9)
    {
        value = 9;
    }

    cleanDevKitScreen();
    snprintf(msg, sizeof(msg), "Countdown\r\n  from %d to 1", value);
    textOutDevKitScreen(0, msg, 1);
    delay(1000);
    textOutDevKitScreen(1, " \r\n \r\n ", 1);

    for (int i = 0; i < value; i++)
    {
        render_frame_to_buffer(buffer, die[value - i - 1]);
        render_frame_to_screen(buffer, 1, OFFSET_X, OFFSET_Y, 500);
    }

    screen_main();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Fan speed
static const char fan1[] = {
    '.', 'L', '.', '.', '.', '.', 'R', '.',
    '.', '.', 'L', '.', '.', 'R', '.', '.',
    '.', '.', '.', 'X', 'X', '.', '.', '.',
    '.', '.', '.', 'X', 'X', '.', '.', '.',
    '.', '.', 'R', '.', '.', 'L', '.', '.',
    '.', 'R', '.', '.', '.', '.', 'L', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char fan2[] = {
    '.', '.', '.', 'V', 'v', '.', '.', '.',
    '.', '.', '.', 'V', 'v', '.', '.', '.',
    '.', 'H', 'H', 'X', 'X', 'H', 'H', '.',
    '.', 'h', 'h', 'X', 'X', 'h', 'h', '.',
    '.', '.', '.', 'V', 'v', '.', '.', '.',
    '.', '.', '.', 'V', 'v', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char *fan[] = {fan1, fan2};

void play_fanspeed_animation(int value)
{
    unsigned char *buffer;
    char msg[32];

    cleanDevKitScreen();
    snprintf(msg, sizeof(msg), "Fan speed\r\nis changed to\r\n  %d", value);
    textOutDevKitScreen(0, msg, 1);
    delay(1000);
    textOutDevKitScreen(1, " \r\n \r\n ", 1);

    buffer = calloc(2 * OLED_SINGLE_FRAME_BUFFER, 1);
    if (buffer)
    {
        for (int i = 0; i < 2; i++)
        {
            render_frame_to_buffer(buffer + (i * OLED_SINGLE_FRAME_BUFFER), fan[i]);
        }
        for(int i = 0; i < 15; i++) 
        {
            render_frame_to_screen(buffer, 2, OFFSET_X, OFFSET_Y, 20);
        }

        free(buffer);
    }
    screen_main();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Current
static const char current1[] = {
    'g', 'g', '.', '.', '.', '.', '.', '.',
    'g', 'g', '.', '.', '.', '.', '.', '.',
    'g', 'g', '.', '.', '.', '.', '.', '.',
    'g', 'g', '.', '.', '.', '.', '.', '.',
    'g', 'g', '.', '.', '.', '.', '.', '.',
    'g', 'g', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char current2[] = {
    'g', 'g', 'g', 'g', 'g', 'g', '.', '.',
    'g', 'g', 'g', 'g', 'g', 'g', '.', '.',
    'g', 'g', 'g', 'g', 'g', 'g', '.', '.',
    'g', 'g', 'g', 'g', 'g', 'g', '.', '.',
    'g', 'g', 'g', 'g', 'g', 'g', '.', '.',
    'g', 'g', 'g', 'g', 'g', 'g', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char current3[] = {
    'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
    'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
    'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
    'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
    'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
    'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char *current[] = {current1, current2, current3, current2};

void play_current_animation(int value)
{
    char msg[32];

    cleanDevKitScreen();
    snprintf(msg, sizeof(msg), "Current\r\nis changed to\r\n  %d", value);
    textOutDevKitScreen(0, msg, 1);
    delay(1000);
    textOutDevKitScreen(1, " \r\n \r\n ", 1);

    animate_circular(current, 4, 5);

    screen_main();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Voltage
static const char voltage1[] = {
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char voltage2[] = {
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char voltage3[] = {
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char *voltage[] = {voltage1, voltage2, voltage3, voltage2};

void play_voltage_animation(int value)
{
    char msg[32];

    cleanDevKitScreen();
    snprintf(msg, sizeof(msg), "Voltage\r\nis changed to\r\n  %d", value);
    textOutDevKitScreen(0, msg, 1);
    delay(1000);
    textOutDevKitScreen(1, " \r\n \r\n ", 1);

    animate_circular(voltage, 4, 5);

    screen_main();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// IrDA
static const char irda1[] = {
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', 'O', 'O', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char irda2[] = {
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', 'O', 'O', 'O', 'O', 'O', 'O', '.',
    '.', '.', '.', 'O', 'O', '.', '.', '.',
    '.', '.', '.', 'O', 'O', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char irda3[] = {
    '.', '.', '.', '.', '.', '.', '.', '.',
    'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O',
    '.', 'O', 'O', 'O', 'O', 'O', 'O', '.',
    '.', '.', 'O', 'O', 'O', 'O', '.', '.',
    '.', '.', '.', 'O', 'O', '.', '.', '.',
    '.', '.', '.', 'O', 'O', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.'};

static const char *irda[] = {irda1, irda2, irda3, irda2};

void play_irda_animation(int value)
{
    cleanDevKitScreen();
    textOutDevKitScreen(0, "Firing IR beam", 1);
    delay(1000);
    
    unsigned char *buffer = calloc(4 * OLED_SINGLE_FRAME_BUFFER, 1);
    if (buffer)
    {
        for (int i = 0; i < 4; i++)
        {
            render_frame_to_buffer(buffer + (i * OLED_SINGLE_FRAME_BUFFER), irda[i]);
        }

        unsigned char data = 1;
        for (int i = 0; i < 5; i++) 
        {
            transmitIrDA(&data, 1, 100);
            render_frame_to_screen(buffer, 4, OFFSET_X, OFFSET_Y, 50);
        }

        free(buffer);
    }

    screen_main();
}