#ifndef __LCDTASK_H__
#define __LCDTASK_H__

#include <includes.h>
#include <hal_lcd.h>
#include <hal_lcd_fonts.h>

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK LcdTaskStack[LCD_TASK_STACK_SIZE];

// Private.
static unsigned int image[18 * 110];
static int index = 0;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void LcdTask(void *args);

// Private.
static void Draw();

static void Clear();
static void DrawRect(int x, int y, int width, int height, unsigned char grayScale);
static void DrawHLine(int x1, int y1, int x2, unsigned char grayScale);
static void DrawVLine(int x1, int y1, int y2, unsigned char grayScale);
static void DrawText(int x, int y, char string[], unsigned char grayScale);
static void SetPixel(int x, int y, unsigned char grayScale);

static void Render();

#endif