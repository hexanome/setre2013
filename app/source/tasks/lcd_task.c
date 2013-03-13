#include "lcd_task.h"

/*******************************************************************************
* The LCD Task.
* Responsible for:
*   Display information about the current recording state.
*   Display the voice recognition result sent by the computer.      
*******************************************************************************/

void LcdTask(void *args)
{    
    // Set the contrast level and backlight.
    halLcdSetContrast(80);
    halLcdSetBackLight(16); 
    
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
    // Clear the center region of the screen.
    ClearRect(0, 21, LCD_COL, LCD_ROW - 21);
    
    AppState state = GetState();
    char *title;    
    
    if (state == STATE_IDLE)
    {
        title = "Idle";
    }
    else if (state == STATE_RECORDING)
    {
        title = "Recording";     
       
        // Draw the time interval since starting to record.
        INT32U time = OSTimeGet();
        INT32U timeDiff = time - recordStartTime;

        INT32U sTime = timeDiff / OS_TICKS_PER_SEC;
        
        unsigned char sSeconds = sTime % 60;
        unsigned char sMinutes = (sTime % 3600) / 60;
                
        char sTimeString[5];
        sprintf(sTimeString, "%.2d:%.2d", sMinutes, sSeconds);
        DrawText(48, 55, sTimeString, PIXEL_ON);
    }
    else
    {
        title = "Result";        
    }
    
    // Draw the title.
    DrawRect(0, 0, LCD_COL, 21, PIXEL_ON);
    DrawText(8, 5, title, PIXEL_OFF);
}

void Clear()
{
    for (int i = 0; i < LCD_MEM_Size; i++)
    {
        image[i] = 0;
    }
}

void ClearRect(int x, int y, int width, int height)
{
    DrawRect(x, y, width, height, PIXEL_OFF);
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
    int fontRowValue;
    char lookupChar;
    unsigned char pixelValue;
    
    while (string[index] != 0)
    {        
        lookupChar = fonts_lookup[string[index]]; 
                
        for (int j = 0; j < FONT_HEIGHT; j++)
        {
            fontRowValue = fonts[lookupChar * 13 + j];
            
            for (int i = 0; i < 8; i++)
            {
                pixelValue = (fontRowValue >> (i * 2)) & (BIT1 | BIT0);
                
                // Extract corresponding pixel in FontRow.
                if (pixelValue != 0)
                {
                    SetPixel(x + i, y + j, grayScale);
                }              
            }
        }
        
        x += 8;
        
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
                value |= (3 << (offset * 2));            // Both bits on.
                break;
        }
        
        // Update the block.
        image[index] = value;
    }    
}

/*******************************************************************************
* Method GetPixel
* Return an unsigned char with the 2 LSBs corresponding to the greyscale value
* of the (x,y) pixel inside the frame
*******************************************************************************/
unsigned char GetPixel(int x, int y)
{
    if (x >= 0 && x < LCD_COL && y >= 0 && y < LCD_ROW)
    {
        // Find the address for this pixel.
        unsigned char offset = x & 0x07;
        // index = y*18 + x >> 3
        unsigned int index = (y << 4) + (y << 1) + (x >> 3);
        
        // Retrieve the corresponding pixel from the image.
        // As we have 8 pixels in an integer, in order to retrieve the n th one,
        // we have to shift right by 2*n (there are 2 bits per pixel)
        return (image[index] >> 2*offset) & (BIT0|BIT1);
        
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