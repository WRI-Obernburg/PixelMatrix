#pragma once
#include <Adafruit_NeoPixel.h>
class MatrixManager
{
public:
    MatrixManager(Adafruit_NeoPixel *pixels)
    {
        this->pixels = pixels;
    }

    void set(int x, int y, uint32_t color,bool ignoreOutOfRange = false)
    {
        this->set(x, y, color >> 16, color >> 8 & 0xFF, color & 0xFF,ignoreOutOfRange);
    }

    void off(int x, int y)
    {
        this->set(x, y, 0, 0, 0);
    }

    void set(int x, int y, int r, int g, int b,bool ignoreOutOfRange = false)
    {
        if (x < 0 || x > 11 || y < 0 || y > 11)
        {
            if(!ignoreOutOfRange) {
                Serial.println("Out of range\n");
            }
            return;
        }
        int pixel = this->calculate_strip_pixel(x, y);
        pixels->setPixelColor(pixel, r, g, b);
    }

    void set_string(int n, uint32_t color)
    {
        this->set_string(n, color >> 16, color >> 8 & 0xFF, color & 0xFF);
    }

    void set_string(int n, int r, int g, int b)
    {
        if (n > 143)
        {
            Serial.println("Out of range\n");
            return;
        }
        int pixel = n;
        pixels->setPixelColor(pixel, r, g, b);
    }

    void fill(uint32_t color)
    {
        this->fill(color >> 16, color >> 8 & 0xFF, color & 0xFF);
    }

    void fill(int r, int g, int b)
    {
        for (int i = 0; i < 144; i++)
        {
            pixels->setPixelColor(i, r, g, b);
        }
    }

    void clear()
    {
        for (int i = 0; i < 144; i++)
        {
            pixels->setPixelColor(i, 0, 0, 0);
        }
    }

    void line(int x1, int y1, int x2, int y2, uint32_t color)
    {
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy, e2; /* error value e_xy */

        for (;;)
        {
            this->set(x1, y1, color);
            if (x1 == x2 && y1 == y2)
                break;
            e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x1 += sx;
            } /* e_xy+e_x > 0 */
            if (e2 <= dx)
            {
                err += dx;
                y1 += sy;
            } /* e_xy+e_y < 0 */
        }
    }

    void line(int x1, int y1, int x2, int y2, int r, int g, int b)
    {
        this->line(x1, y1, x2, y2, Color(r, g, b));
    }

    // origin is bottom left
    void number(int x, int y, unsigned int n, uint32_t color, int gap = 1)
    {
        int amount_of_digits = this->count_digits(n);
        int needed_space = amount_of_digits * 3 + (amount_of_digits - 1) * gap;
        if ((needed_space + x) > 12)
        {
            Serial.println("Not enough space available");
            return;
        }

        if (n == 0)
        { 
            digit(x,y,n,color);
        }
        int pos = needed_space-3;
        while (n > 0)
        { 
            digit(x+pos,y,n%10,color);
            n /= 10;
            pos-=3+gap;
        }
    }

    void digit(int x, int y, int n, int r, int g, int b)
    {
        this->digit(x, y, n, Color(r, g, b));
    }

    // origin is bottom left
    void digit(int x, int y, int n, uint32_t color)
    {
        switch (n)
        {
        case 0:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 3, color);
            this->number_segment(x, y, 4, color);
            this->number_segment(x, y, 5, color);
            break;
        case 1:
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            break;
        case 2:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 3, color);
            this->number_segment(x, y, 4, color);
            this->number_segment(x, y, 6, color);
            break;
        case 3:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 3, color);
            this->number_segment(x, y, 6, color);
            break;
        case 4:
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 5, color);
            this->number_segment(x, y, 6, color);
            break;
        case 5:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 3, color);
            this->number_segment(x, y, 5, color);
            this->number_segment(x, y, 6, color);
            break;
        case 6:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 3, color);
            this->number_segment(x, y, 4, color);
            this->number_segment(x, y, 5, color);
            this->number_segment(x, y, 6, color);
            break;
        case 7:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            break;
        case 8:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 3, color);
            this->number_segment(x, y, 4, color);
            this->number_segment(x, y, 5, color);
            this->number_segment(x, y, 6, color);
            break;
        case 9:
            this->number_segment(x, y, 0, color);
            this->number_segment(x, y, 1, color);
            this->number_segment(x, y, 2, color);
            this->number_segment(x, y, 5, color);
            this->number_segment(x, y, 6, color);
            break;
        default:
            Serial.println("Invalid digit");
            break;
        }
    }

    void number_segment(int x, int y, int s, uint32_t color)
    {
        switch (s)
        {
        case 0:
            set(x, y + 4, color);
            set(x + 1, y + 4, color);
            set(x + 2, y + 4, color);
            break;
        case 1:
            set(x + 2, y + 4, color);
            set(x + 2, y + 3, color);
            set(x + 2, y + 2, color);
            break;
        case 2:
            set(x + 2, y + 2, color);
            set(x + 2, y + 1, color);
            set(x + 2, y, color);
            break;
        case 3:
            set(x, y, color);
            set(x + 1, y, color);
            set(x + 2, y, color);
            break;
        case 4:
            set(x, y, color);
            set(x, y + 1, color);
            set(x, y + 2, color);
            break;
        case 5:
            set(x, y + 2, color);
            set(x, y + 3, color);
            set(x, y + 4, color);
            break;
        case 6:
            set(x, y + 2, color);
            set(x + 1, y + 2, color);
            set(x + 2, y + 2, color);
            break;
        default:
            Serial.println("Invalid Segment");
            break;
        }
    }

    void rect(int x, int y, int width, int height, uint32_t color, bool filled = false)
    {
        width--;
        height--;
        if (filled)
        {
            for (int i = 0; i < width; i++)
            {
                this->line(x + i, y, x + i, y + height, color);
            }
            for (int i = 0; i < height; i++)
            {
                this->line(x, y + i, x + width, y + i, color);
            }
        }
        this->line(x, y, x + width, y, color);
        this->line(x, y + height, x + width, y + height, color);
        this->line(x, y, x, y + height, color);
        this->line(x + width, y, x + width, y + height, color);
    }

    void rect(int x, int y, int width, int height, int r, int g, int b, bool filled = false)
    {
        this->rect(x, y, width, height, Color(r, g, b), filled);
    }

    void circle(int x, int y, int radius, uint32_t color, bool filled = true, int u = 5)
    {
        for (int i = 0; i < 360; i += u)
        {
            if (i == 0)
                continue;
            this->set(floor(cos(i) * radius + x), floor(sin(i) * radius + y), color,true);
            if (filled)
            {
                // TODO
            }
        }
    }

    void circle(int x, int y, int radius, int r, int g, int b, bool filled = false, int u = 15)
    {
        this->circle(x, y, radius, Color(r, g, b), filled, u);
    }

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    void set_tps(float tps)
    {
        if (tps > 200)
        {
            tps = 200;
        }

        if (tps < 0)
        {
            tps = 0;
        }

        this->currentTPS = tps;
    }

    float get_current_tps()
    {
        return currentTPS;
    }

  

private:
    Adafruit_NeoPixel *pixels;
    float currentTPS = 0;
 

    int calculate_strip_pixel(int x, int y)
    {
        if (x < 0 || x > 11 || y < 0 || y > 11)
        {
            Serial.println("Out of range\n");
            return -1;
        }

        int bottom_row = floor((x + 1) / 2) * 23 + floor(x / 2);
        int height_correction = -((x % 2) * 2 - 1) * y;
        return bottom_row + height_correction;
    }

    unsigned count_digits(unsigned i)
    {
        return i > 0 ? (int)log10((double)i) + 1 : 1;
    }
};