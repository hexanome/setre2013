#include "lcd_task.h"

/*******************************************************************************
* The LCD Task.
* Responsible for:
*   Display information about the current recording state.
*   Display the voice recognition result sent by the computer.      
*******************************************************************************/

void LcdTask(void *args)
{
    // Initialize the LCD.
    halLcdInit();
    
    // Initialize the backlight.
    halLcdBackLightInit();
    // halLcdSetBackLight(); <-- Find the right value.
    
    // Set the default contrast level.
    halLcdSetContrast(80);
    
    Clear();
    
    while (1) {
        Draw();
        Render();
        OSTimeDlyHMSM(0, 0, 0, 1);
    }
}

/*******************************************************************************
* Method implementation.
*******************************************************************************/

void Draw()
{
    // Clear the screen.
    //halLcdClearScreen();
    
//    // Draw the title bar.
//    for (int i = 0; i < 20; i++)
//    {
//        halLcdLine(0, i, LCD_COL, i, PIXEL_ON);
//    }
//    
//    // Draw the title.
//    AppState state = GetState();
//    if (state == STATE_IDLE)
//    {
//        halLcdPrintXY("Idle", 10, 4, INVERT_TEXT | OVERWRITE_TEXT);
//    }
//    else if (state == STATE_RECORDING)
//    {
//        halLcdPrintXY("Recording", 10, 4, INVERT_TEXT | OVERWRITE_TEXT);        
//    }
//    else
//    {
//        halLcdPrintXY("Result", 10, 4, INVERT_TEXT | OVERWRITE_TEXT);        
//    }
    
    DrawRect(0, 0, LCD_COL, 20, PIXEL_ON);
    DrawVLine(index, 30, LCD_ROW, PIXEL_ON);
    index++;
}

void Clear()
{
    for (int i = 0; i < LCD_MEM_Size; i++)
    {
        image[i] = 0;
    }
}

void DrawRect(int x, int y, int width, int height, unsigned char grayScale)
{
    for (int iy = y; iy < y + height; iy++)
    {
        DrawHLine(x, iy, x + width - 1, grayScale);
    }
}

void DrawHLine(int x1, int y1, int x2, unsigned char grayScale)
{
    for (int x = x1; x <= x2; x++)
    {
        SetPixel(x, y1, grayScale);
    }
}

void DrawVLine(int x1, int y1, int y2, unsigned char grayScale)
{
    for (int y = y1; y <= y2; y++)
    {
        SetPixel(x1, y, grayScale);        
    }
}

void DrawText(int x, int y, char string[], unsigned char grayScale)
{
    int index = 0;
    int fontRowValue, offset;
    char lookupChar;
    
    while (string[index] != 0)
    {        
        lookupChar = fonts_lookup[string[index]]; 
                
        for (int j = 0; j < FONT_HEIGHT; j++)
        {
            fontRowValue = fonts[lookupChar * 13 + j];
            
            for (int i = 0; i < 13; i++)
            {
                offset = i & 0x07;
                
//                if ( != 0)
//                {
//                    SetPixel(x + i, y + j, grayScale);
//                }                
            }
        }
        
        index++;
    }
}

void SetPixel(int x, int y, unsigned char grayScale)
{
    if (x >= 0 && x < LCD_COL && y >= 0 && y < LCD_ROW)
    {
        // Find the address for this pixel.
        unsigned char offset = x & 0x07;    
        unsigned int index = (y << 4) + (y << 1) + (x >> 3);
        
        // Retrieve the corresponding block from the table.
        int value = image[index];
        
        switch(grayScale)
        {
            case PIXEL_OFF:  
                value &= ~(3 << (offset * 2));          // Both corresponding bits are off.
                break;
            case PIXEL_LIGHT:
                value &= ~(1 << ((offset * 2) + 1));    // Lower bit is on.
                value |= 1 << (offset * 2);             
                break;
            case PIXEL_DARK:
                value &= ~(1 << (offset * 2));          // Lower bit is on.
                value |= (1 << ((offset * 2) + 1));  
                break;
            case PIXEL_ON: 
                value |= (3 << (offset * 2));           // Both bits on.
                break;
        }
        
        // Update the block.
        image[index] = value;
    }    
}

void Render()
{
    halLcdImage(image, 18, LCD_ROW, 0, 0);
}

/*******************************************************************************
* Variable definition.
*******************************************************************************/

OS_STK LcdTaskStack[LCD_TASK_STACK_SIZE];