#ifndef __LCDTASK_H__
#define __LCDTASK_H__

#include <includes.h>
#include <hal_lcd.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define FONT_HEIGHT     12 
#define FONT_WIDTH      8

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
static void DrawHLine(int x1, int y1, int x2, unsigned char grayScale);
static void DrawVLine(int x1, int y1, int y2, unsigned char grayScale);
static void DrawText(int x, int y, char string[], unsigned char grayScale);
static void DrawTextCustomSpacing(int x, int y, int spacing, char string[], unsigned char grayScale);
static int DrawTextBlock(int x, int y, int width, char string[], unsigned char grayScale);
static int DrawTextBlockCustomSpacing(int x, int y, int width, int spacing, char string[], unsigned char grayScale);
static void SetPixel(int x, int y, unsigned char grayScale);

static void Render();

#endif