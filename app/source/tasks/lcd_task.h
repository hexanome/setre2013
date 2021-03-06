#ifndef __LCDTASK_H__
#define __LCDTASK_H__

#include <includes.h>
#include <hal_lcd.h>
#include "rx_task.h"

/*******************************************************************************
* Defines.
*******************************************************************************/

#define FONT_HEIGHT     12 
#define FONT_WIDTH      8
#define SQRT_2          1.4142
#define COS_45          SQRT_2/2

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK LcdTaskStack[LCD_TASK_STACK_SIZE];

// External variables.
extern const unsigned int fonts[];
extern const unsigned char fonts_lookup[];

// Private.
static unsigned int image[18 * 110];
static char lineBuffer[20];
static int lastScrollPosition = 0;
static int localScrollPosition = 0;
static int lastTextHeight = 10000;
static int stickGrey =0;
static int secLoading =0;
/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void LcdTask(void *args);

// Private.
static void Draw();

static void Clear();
static void ClearRect(int x, int y, int width, int height);
static void DrawRect(int x, int y, int width, int height, unsigned char grayScale);
static void Draw8PointsCircleStick(int cx, int cy,int r,int l, int stick);
static void Draw8PointsCircle(int cx, int cy,int r,int stick);
static void Draw1PointCircle(int cx, int cy,int r,int stick,int level);
static void DrawHLine(int x1, int y1, int x2, unsigned char grayScale);
static void DrawVLine(int x1, int y1, int y2, unsigned char grayScale);
static void DrawText(int x, int y, char string[], unsigned char grayScale);
static void DrawTextCustomSpacing(int x, int y, int spacing, char string[], unsigned char grayScale);
static int DrawTextBlock(int x, int y, int width, char string[], unsigned char grayScale);
static int DrawTextBlockCustomSpacing(int x, int y, int width, int spacing, char string[], unsigned char grayScale);
static void SetPixel(int x, int y, unsigned char grayScale);

static void Render();

#endif