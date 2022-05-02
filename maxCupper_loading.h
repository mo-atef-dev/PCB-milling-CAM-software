//max capper algorithm and load the image
#include<wincodec.h>
#include<objbase.h>
#include<combaseapi.h>
//#include<d2d1_3.h>
#pragma comment(lib, "windowscodecs.lib")
#include<winuser.h>
#include<windef.h>
#include<d2d1.h>
#pragma comment(lib, "d2d1")
#include<d2d1helper.h>//for colorF
#include<shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <math.h>//for power (pow)
#include <stdio.h>//for sprintf

#include"BMP.h"

ID2D1Bitmap* D2DpBitmap = NULL;
ID2D1HwndRenderTarget* pRT = NULL;

/// <summary>
/// Class that represent a bitmap and handle with it to change the pixel individually
/// </summary>
class MyBitmap
{
private:
    BYTE* pv;
    UINT cb_BufferSize;
    UINT cb_Stride;
    UINT width;
    UINT height;
public:
    /// <summary>
    /// Constructor that initialize the image by its total size and its width
    /// </summary>
    /// <param name="pv">Pointer to array of bytes that has the pixels</param>
    /// <param name="cbBufferSize">total size of the image in bytes</param>
    /// <param name="cbStride">the width of the image</param>
    MyBitmap()
    {
        pv = NULL;
        cb_BufferSize = 0;
        cb_Stride = 0;
    }
    MyBitmap(BYTE* ArrayBytes, UINT cbBufferSize, UINT cbStride)
    {
        pv = new BYTE[cbBufferSize];
        //copy bitmap to pv pointer
        for (UINT i = 0; i < cbBufferSize; i++)
        {
            *(pv + i) = *(ArrayBytes + i);
        }

        cb_BufferSize = cbBufferSize;
        cb_Stride = cbStride;

    }
    MyBitmap(BYTE* ArrayBytes, UINT cbBufferSize, UINT cbStride,UINT w,UINT h)
    {
        pv = new BYTE[cbBufferSize];
        //copy bitmap to pv pointer
        for (UINT i = 0; i < cbBufferSize; i++)
        {
            *(pv + i) = *(ArrayBytes + i);
        }

        cb_BufferSize = cbBufferSize;
        cb_Stride = cbStride;
        width = w;
        height = h;

    }
    //set all specifications of the bitmap
    void SetSpecifications(BYTE* ArrayBytes, UINT cbBufferSize, UINT cbStride, UINT w, UINT h)
    {
        pv = new BYTE[cbBufferSize];
        //copy bitmap to pv pointer
        for (UINT i = 0; i < cbBufferSize; i++)
        {
            *(pv + i) = *(ArrayBytes + i);
        }

        cb_BufferSize = cbBufferSize;
        cb_Stride = cbStride;
        width = w;
        height = h;
    }
    //set size of the bitmap
    void SetSize(UINT cbBufferSize)
    {
        cb_BufferSize = cbBufferSize;
    }
    //Set stride of the bitmap
    void SetStride(UINT cbStride)
    {
        cb_Stride = cbStride;
    }
    //get the width of the BitmapObject
    UINT GetWidth()
    {
        return width;
    }
    //get the width of the BitmapObject
    UINT GetHeight()
    {
        return height;
    }
    //Set pixel array using pointer to te first byte(array of bytes)
    void SetpixelArray(BYTE* ArrayBytes)
    {
        pv = new BYTE[cb_BufferSize];
        //copy bitmap to pv pointer
        for (UINT i = 0; i < cb_BufferSize; i++)
        {
            *(pv + i) = *(ArrayBytes + i);
        }
    }
    /// <summary>
    /// Set pixel in the image by using bytes that represent the color of RGBA
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    /// <param name="B">Blue</param>
    /// <param name="G">Green</param>
    /// <param name="R">RED</param>
    /// <param name="A">Alpha</param>
    void set_pixel(UINT x0, UINT y0, BYTE B, BYTE G, BYTE R, BYTE A) {
        pv[(y0 * cb_Stride + x0 * 4) + 0] = B;
        pv[(y0 * cb_Stride + x0 * 4) + 1] = G;
        pv[(y0 * cb_Stride + x0 * 4) + 2] = R;
        pv[(y0 * cb_Stride + x0 * 4) + 3] = A;
    }
    /// <summary>
    /// Set pixel by using D2D1_COLOR_F struct
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    /// <param name="pixelColor">D2D1_COLOR_F struct that definr the color(RGB)</param>
    void set_pixel(UINT x0, UINT y0, D2D1_COLOR_F pixelColor) {
        pv[(y0 * cb_Stride + x0 * 4) + 0] = pixelColor.b;
        pv[(y0 * cb_Stride + x0 * 4) + 1] = pixelColor.g;
        pv[(y0 * cb_Stride + x0 * 4) + 2] = pixelColor.r;
        pv[(y0 * cb_Stride + x0 * 4) + 3] = pixelColor.a;
    }
    /// <summary>
    /// Set the  pixel to white
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    void set_pixel_white(UINT x0, UINT y0) {
        pv[(y0 * cb_Stride + x0 * 4) + 0] = 255;
        pv[(y0 * cb_Stride + x0 * 4) + 1] = 255;
        pv[(y0 * cb_Stride + x0 * 4) + 2] = 255;
        pv[(y0 * cb_Stride + x0 * 4) + 3] = 255;
    }
    /// <summary>
    /// Set pixel to black
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    void set_pixel_black(UINT x0, UINT y0) {
        pv[(y0 * cb_Stride + x0 * 4) + 0] = 0;
        pv[(y0 * cb_Stride + x0 * 4) + 1] = 0;
        pv[(y0 * cb_Stride + x0 * 4) + 2] = 0;
        pv[(y0 * cb_Stride + x0 * 4) + 3] = 0;
    }
    /// <summary>
    /// Get the pixel in the image by using byte-variables that represent the color of RGBA
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    /// <param name="B">Blue</param>
    /// <param name="G">Green</param>
    /// <param name="R">RED</param>
    /// <param name="A">Alpha</param>
    void get_pixel(UINT x0, UINT y0, BYTE& B, BYTE& G, BYTE& R, BYTE& A) {
        B = pv[(y0 * cb_Stride + x0 * 4) + 0];
        G = pv[(y0 * cb_Stride + x0 * 4) + 1];
        R = pv[(y0 * cb_Stride + x0 * 4) + 2];
        A = pv[(y0 * cb_Stride + x0 * 4) + 3];
    }
    /// <summary>
    /// get pixel by using D2D1_COLOR_F struct&
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    /// <param name="pixelColor">D2D1_COLOR_F struct that definr the color(RGB)</param>
    void get_pixel(UINT x0, UINT y0, D2D1_COLOR_F& pixelColor) {
        pixelColor.b = pv[(y0 * cb_Stride + x0 * 4) + 0];
        pixelColor.g = pv[(y0 * cb_Stride + x0 * 4) + 1];
        pixelColor.r = pv[(y0 * cb_Stride + x0 * 4) + 2];
        pixelColor.a = pv[(y0 * cb_Stride + x0 * 4) + 3];

    }
    /// <summary>
    /// Test if the pixel is black or not. Return true if black
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    /// <returns>true if the pixel is black and false if not black</returns>
    bool isBlack(UINT x0, UINT y0)
    {
        BYTE B = pv[(y0 * cb_Stride + x0 * 4) + 0];
        BYTE G = pv[(y0 * cb_Stride + x0 * 4) + 1];
        BYTE R = pv[(y0 * cb_Stride + x0 * 4) + 2];
        BYTE A = pv[(y0 * cb_Stride + x0 * 4) + 3];
        if (B == 0 && G == 0 && R == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /// <summary>
    /// Test if the pixel is white or not.
    /// </summary>
    /// <param name="x0">The x coordinate of the pixel</param>
    /// <param name="y0">The y coordinate of the pixel</param>
    /// <returns>true if the pixel is white</returns>
    bool isWhite(UINT x0, UINT y0)
    {
        BYTE B = pv[(y0 * cb_Stride + x0 * 4) + 0];
        BYTE G = pv[(y0 * cb_Stride + x0 * 4) + 1];
        BYTE R = pv[(y0 * cb_Stride + x0 * 4) + 2];
        BYTE A = pv[(y0 * cb_Stride + x0 * 4) + 3];
        if (B == 255 && G == 255 && R == 255)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

//object of tracing algorithm
MyBitmap bitmapObject = MyBitmap();

//////////
//set pixel from array of pixel
void set_pixel(BYTE* pv, UINT cb_Stride, UINT x0, UINT y0, UINT B, UINT G, UINT R, UINT A) {
    pv[(y0 * cb_Stride + x0 * 4) + 0] = B;
    pv[(y0 * cb_Stride + x0 * 4) + 1] = G;
    pv[(y0 * cb_Stride + x0 * 4) + 2] = R;
    pv[(y0 * cb_Stride + x0 * 4) + 3] = A;
}
//get pixel from array of pixel
void get_pixel(BYTE* pv, UINT cb_Stride, UINT x0, UINT y0, UINT& B, UINT& G, UINT& R, UINT& A) {
    B = pv[(y0 * cb_Stride + x0 * 4) + 0];
    G = pv[(y0 * cb_Stride + x0 * 4) + 1];
    R = pv[(y0 * cb_Stride + x0 * 4) + 2];
    A = pv[(y0 * cb_Stride + x0 * 4) + 3];
}
//coloring all pixel to the same color (color_1)
void same_color(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight, UINT color_1)
{

    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
            UINT BB = 0;
            UINT GG = 0;
            UINT RR = 0;
            UINT A = 0;
            get_pixel(pv, cb_Stride, j, i, BB, GG, RR, A);
            /*if (BB != 0 && RR != 0 && GG != 0)
            {
                set_pixel(pv, cb_Stride, j, i, color_1, color_1, color_1, 255);
            }*/
            if ((BB == 0 && RR == 0 && GG == 0)||
                (BB == 0 && RR == 255 && GG == 0))
            {

            }
            else
            {
                set_pixel(pv, cb_Stride, j, i, color_1, color_1, color_1, 255);
            }

        }
    }
}
//max cupper using different colors
void max_cupper(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight, UINT color_1, UINT color_2, UINT cupper)
{
    /*
    number of pixels used in algorithm
            +___+
          20| 2 | 13
        +___+___+____+
      16| 9 | 1 | 10 | 15
    +___+___+__ +____+__+
    | 6 | 5 | 0 | 7 | 8 |
    +___+___+___+___+___+
      17|11 | 3 | 12| 14
        +___+___+___+
          19| 4 | 18
            +___+
    */

    //max cupper
    UINT black = 0;
    UINT counter = -1;
    UINT A = 0;
    int c = 0;

    UINT BB = 0;
    UINT GG = 0;
    UINT RR = 0;

    UINT B1 = 0;
    UINT G1 = 0;
    UINT R1 = 0;

    UINT B2 = 0;
    UINT G2 = 0;
    UINT R2 = 0;

    UINT B3 = 0;
    UINT G3 = 0;
    UINT R3 = 0;

    UINT B4 = 0;
    UINT G4 = 0;
    UINT R4 = 0;

    UINT B5 = 0;
    UINT G5 = 0;
    UINT R5 = 0;

    UINT B6 = 0;
    UINT G6 = 0;
    UINT R6 = 0;

    UINT B7 = 0;
    UINT G7 = 0;
    UINT R7 = 0;

    UINT B8 = 0;
    UINT G8 = 0;
    UINT R8 = 0;

    UINT B9 = 0;
    UINT G9 = 0;
    UINT R9 = 0;

    UINT B10 = 0;
    UINT G10 = 0;
    UINT R10 = 0;

    UINT B11 = 0;
    UINT G11 = 0;
    UINT R11 = 0;

    UINT B12 = 0;
    UINT G12 = 0;
    UINT R12 = 0;

    UINT B13 = 0;
    UINT G13 = 0;
    UINT R13 = 0;

    UINT B14 = 0;
    UINT G14 = 0;
    UINT R14 = 0;

    UINT B15 = 0;
    UINT G15 = 0;
    UINT R15 = 0;

    UINT B16 = 0;
    UINT G16 = 0;
    UINT R16 = 0;

    UINT B17 = 0;
    UINT G17 = 0;
    UINT R17 = 0;

    UINT B18 = 0;
    UINT G18 = 0;
    UINT R18 = 0;

    UINT B19 = 0;
    UINT G19 = 0;
    UINT R19 = 0;

    UINT B20 = 0;
    UINT G20 = 0;
    UINT R20 = 0;

    //while(c<10)
    while (counter != 0)
    {
        c++;
        counter = 0;
        for (UINT i = 0 + 2; i < puiHeight - 2; i++)
        {
            for (UINT j = 0 + 2; j < puiWidth - 2; j++)
            {
                bool same_pixel_flage = true;
                while (same_pixel_flage == true)
                {
                    same_pixel_flage = false;

                    get_pixel(pv, cb_Stride, j, i, BB, GG, RR, A);
                    //if current pixel is cupper -> start the algorithm
                    if (BB == color_1 && GG == color_1 && RR == color_1)
                    {
                        get_pixel(pv, cb_Stride, j, i + 1, B1, G1, R1, A);
                        get_pixel(pv, cb_Stride, j, i + 2, B2, G2, R2, A);
                        get_pixel(pv, cb_Stride, j, i - 1, B3, G3, R3, A);
                        get_pixel(pv, cb_Stride, j, i - 2, B4, G4, R4, A);
                        get_pixel(pv, cb_Stride, j - 1, i, B5, G5, R5, A);
                        get_pixel(pv, cb_Stride, j - 2, i, B6, G6, R6, A);
                        get_pixel(pv, cb_Stride, j + 1, i, B7, G7, R7, A);
                        get_pixel(pv, cb_Stride, j + 2, i, B8, G8, R8, A);
                        get_pixel(pv, cb_Stride, j - 1, i + 1, B9, G9, R9, A);
                        get_pixel(pv, cb_Stride, j + 1, i + 1, B10, G10, R10, A);
                        get_pixel(pv, cb_Stride, j - 1, i - 1, B11, G11, R11, A);
                        get_pixel(pv, cb_Stride, j + 1, i - 1, B12, G12, R12, A);
                        get_pixel(pv, cb_Stride, j + 1, i + 2, B13, G13, R13, A);
                        get_pixel(pv, cb_Stride, j + 2, i - 1, B14, G14, R14, A);
                        get_pixel(pv, cb_Stride, j + 2, i + 1, B15, G15, R15, A);
                        get_pixel(pv, cb_Stride, j - 2, i + 1, B16, G16, R16, A);
                        get_pixel(pv, cb_Stride, j - 2, i - 1, B17, G17, R17, A);
                        get_pixel(pv, cb_Stride, j + 1, i - 2, B18, G18, R18, A);
                        get_pixel(pv, cb_Stride, j - 1, i - 2, B19, G19, R19, A);
                        get_pixel(pv, cb_Stride, j - 1, i + 2, B20, G20, R20, A);

                        //fill up (fill 1)
                        if (B1 == black && G1 == black && R1 == black && B2 == black && G2 == black && R2 == black)
                        {

                            bool cond1 = false;
                            bool cond2 = false;

                            //cond1:
                            //( 5_cupper || (5_black and 9_black) )
                            if (B5 == black && G5 == black && R5 == black && B9 == black && G9 == black && R9 == black)
                            {
                                cond1 = true;
                            }
                            else if (B5 != black && G5 != black && R5 != black)
                            {
                                cond1 = true;
                            }

                            //cond2:
                            //( 7_cupper || (7_black and 10_black) )
                            if (B7 == black && G7 == black && R7 == black && B10 == black && G10 == black && R10 == black)
                            {
                                cond2 = true;
                            }
                            else if (B7 != black && G7 != black && R7 != black)
                            {
                                cond2 = true;
                            }
                            //if two condition are true -> fill 1
                            if (cond1 == true && cond2 == true)
                            {
                                set_pixel(pv, cb_Stride, j, i + 1, color_2, color_2, color_2, 255);
                                counter++;
                                same_pixel_flage = true;
                            }

                        }
                        //condition for U line
                        else if (B1 == black && G1 == black && R1 == black && B2 != black && G2 != black && R2 != black)
                        {
                            if (B7 != black && G7 != black && R7 != black &&
                                B10 != black && G10 != black && R10 != black &&
                                B13 != black && G13 != black && R13 != black)
                            {
                                if ((B5 != black && G5 != black && R5 != black) ||
                                    (B20 != black && G20 != black && R20 != black))
                                {
                                    set_pixel(pv, cb_Stride, j, i + 1, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }
                            else if (B5 != black && G5 != black && R5 != black &&
                                B9 != black && G9 != black && R9 != black &&
                                B20 != black && G20 != black && R20 != black)
                            {
                                if ((B7 != black && G7 != black && R7 != black) ||
                                    (B13 != black && G13 != black && R13 != black))
                                {
                                    set_pixel(pv, cb_Stride, j, i + 1, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }

                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                        //fill down (fill 3)
                        if (B3 == black && G3 == black && R3 == black && B4 == black && G4 == black && R4 == black)
                        {
                            bool cond1 = false;
                            bool cond2 = false;

                            //( 5_cupper || (5_black and 11_black) )
                            if (B5 == black && G5 == black && R5 == black && B11 == black && G11 == black && R11 == black)
                            {
                                cond1 = true;
                            }
                            //OR
                            //else if(B5==color_1 &&G5==color_1 && R5==color_1)
                            else if (B5 != black && G5 != black && R5 != black)
                            {
                                cond1 = true;
                            }

                            //( 7_cupper || (7_black and 12_black) )
                            if (B7 == black && G7 == black && R7 == black && B12 == black && G12 == black && R12 == black)
                            {
                                cond2 = true;
                            }
                            //OR
                            else if (B7 != black && G7 != black && R7 != black)
                            {
                                cond2 = true;
                            }

                            //if two condition are true -> fill 3
                            if (cond1 == true && cond2 == true)
                            {
                                set_pixel(pv, cb_Stride, j, i - 1, color_2, color_2, color_2, 255);
                                counter++;
                                same_pixel_flage = true;
                            }

                        }
                        //condition for U line
                        else if (B3 == black && G3 == black && R3 == black && B4 != black && G4 != black && R4 != black)
                        {
                            //pixels in left

                            //condition for U line connected from left
                            if (B5 != black && G5 != black && R5 != black &&
                                B11 != black && G11 != black && R11 != black &&
                                B19 != black && G19 != black && R19 != black)
                            {
                                if ((B7 != black && G7 != black && R7 != black) ||
                                    (B18 != black && G18 != black && R18 != black))
                                {
                                    set_pixel(pv, cb_Stride, j, i - 1, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }
                            //condition for U line connected from right
                            else if (B7 != black && G7 != black && R7 != black &&
                                B12 != black && G12 != black && R12 != black &&
                                B18 != black && G18 != black && R18 != black)
                            {
                                if ((B5 != black && G5 != black && R5 != black) ||
                                    (B19 != black && G19 != black && R19 != black))
                                {
                                    set_pixel(pv, cb_Stride, j, i - 1, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                        //fill left (fill 5)
                        if (B5 == black && G5 == black && R5 == black && B6 == black && G6 == black && R6 == black)
                        {
                            bool cond1 = false;
                            bool cond2 = false;

                            //( 1_cupper || (1_black and 9_black) )
                            if (B1 == black && G1 == black && R1 == black && B9 == black && G9 == black && R9 == black)
                            {
                                cond1 = true;
                            }
                            //OR
                            //else if(B1==color_1 &&G1==color_1 && R1==color_1)
                            else if (B1 != black && G1 != black && R1 != black)
                            {
                                cond1 = true;
                            }

                            //( 3_cupper || (3_black and 11_black) )
                            if (B3 == black && G3 == black && R3 == black && B11 == black && G11 == black && R11 == black)
                            {
                                cond2 = true;
                            }
                            //OR
                            //else if(B3==color_1 &&G3==color_1 && R3==color_1)
                            else if (B3 != black && G3 != black && R3 != black)
                            {
                                cond2 = true;
                            }

                            //if two condition are true -> fill 5
                            if (cond1 == true && cond2 == true)
                            {
                                set_pixel(pv, cb_Stride, j - 1, i, color_2, color_2, color_2, A);
                                counter++;
                                same_pixel_flage = true;
                            }

                        }
                        //condition for U line
                        else if (B5 == black && G5 == black && R5 == black && B6 != black && G6 != black && R6 != black)
                        {


                            //condition for U line connected from bottom
                            if (B3 != black && G3 != black && R3 != black &&
                                B11 != black && G11 != black && R11 != black &&
                                B17 != black && G17 != black && R17 != black)
                            {
                                if ((B1 != black && G1 != black && R1 != black) ||
                                    (B16 != black && G16 != black && R16 != black))
                                {
                                    set_pixel(pv, cb_Stride, j - 1, i, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }
                            //condition for U line connected from upper
                            else if (B1 != black && G1 != black && R1 != black &&
                                B9 != black && G9 != black && R9 != black &&
                                B16 != black && G16 != black && R16 != black)
                            {
                                if ((B3 != black && G3 != black && R3 != black) ||
                                    (B17 != black && G17 != black && R17 != black))
                                {
                                    set_pixel(pv, cb_Stride, j - 1, i, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }

                            }
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                        //fill right (fill 7)
                        if (B7 == black && G7 == black && R7 == black && B8 == black && G8 == black && R8 == black)
                        {
                            bool cond1 = false;
                            bool cond2 = false;

                            //( 1_cupper || (1_black and 10_black) )
                            if (B1 == black && G1 == black && R1 == black && B10 == black && G10 == black && R10 == black)
                            {
                                cond1 = true;
                            }
                            //OR
                            //else if(B1==color_1 &&G1==color_1 && R1==color_1)
                            else if (B1 != black && G1 != black && R1 != black)
                            {
                                cond1 = true;
                            }

                            //( 3_cupper || (3_black and 12_black) )
                            if (B3 == black && G3 == black && R3 == black && B12 == black && G12 == black && R12 == black)
                            {
                                cond2 = true;
                            }
                            //OR
                            //else if(B3==color_1 &&G3==color_1 && R3==color_1)
                            else if (B3 != black && G3 != black && R3 != black)
                            {
                                cond2 = true;
                            }

                            //if two condition are true -> fill 7
                            if (cond1 == true && cond2 == true)
                            {
                                set_pixel(pv, cb_Stride, j + 1, i, color_2, color_2, color_2, 255);
                                counter++;
                                same_pixel_flage = true;
                            }

                        }
                        //condition for U line
                        else if (B7 == black && G7 == black && R7 == black && B8 != black && G8 != black && R8 != black)
                        {

                            //condition for U line connected from bottom
                            if (B3 != black && G3 != black && R3 != black &&
                                B12 != black && G12 != black && R12 != black &&
                                B14 != black && G14 != black && R14 != black)
                            {
                                if ((B1 != black && G1 != black && R1 != black) ||
                                    (B15 != black && G15 != black && R15 != black))
                                {
                                    set_pixel(pv, cb_Stride, j + 1, i, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }
                            //condition for U line connected from upper
                            else if (B1 != black && G1 != black && R1 != black &&
                                B10 != black && G10 != black && R10 != black &&
                                B15 != black && G15 != black && R15 != black)
                            {
                                if ((B3 != black && G3 != black && R3 != black) ||
                                    (B14 != black && G14 != black && R14 != black))
                                {
                                    set_pixel(pv, cb_Stride, j + 1, i, color_2, color_2, color_2, 255);
                                    counter++;
                                    same_pixel_flage = true;
                                }
                            }

                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }


                }

                if (BB == color_1 && GG == color_1 && RR == color_1)
                {
                    set_pixel(pv, cb_Stride, j, i, cupper, cupper, cupper, 255);
                }



            }
        }
        UINT temp = color_1;
        color_1 = color_2;
        color_2 = temp;

    }


}

///Algorithm max cupper
void All_algorithm_max_cupper(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight)
{
    UINT color_1 = 200;
    UINT color_2 = 100;
    UINT cupper = 255;//white
    //temp
    UINT A;
    //make all cupper in the same color
    same_color(pv, cb_Stride, puiWidth, puiHeight, color_1);
    //max_cupper
    max_cupper(pv, cb_Stride, puiWidth, puiHeight, color_1, color_2, cupper);
}



//Data structure for Standard Aperture Templates:
struct aperture
{
    /*aperture type:
        1 for Circle
        2 for Rectangle
        3 for Obround
        4 for Polygon
    */
    int type;
    //number of aperture in gerber file
    int num_aperture;
    /*
        C Circle	Diameter[, Hole diameter]
        R Rectangle X size, Y size[, Hole diameter]
        O Obround	X size, Y size[, Hole diameter]
        P Polygon	Outer diameter, # vertices[, Rotation[, Hole diameter]]
    */
    //for Circle or Polygon
    float diameter;
    //for Rectangle or Obround
    float x;
    float y;
    //for Polygon
    int num_vertices;
    //for all shapes (optional)
    float hole_diameter;
    //for next node in linked list
    aperture* next;
};

class all_standerd_aperture
{
private:
    aperture* first;
public:
    ////no-argument constructor
    all_standerd_aperture()
    {
        first = NULL;
    }
    //add from the beginning(last node added is the first node in the linked list)
    void add_aperture(int t, int n, float d, float xx, float yy, int num_v, float h_d)
    {
        aperture* new_aperture = new aperture;
        new_aperture->type = t;
        new_aperture->num_aperture = n;
        new_aperture->diameter = d;
        new_aperture->x = xx;
        new_aperture->y = yy;
        new_aperture->num_vertices = num_v;
        new_aperture->hole_diameter = h_d;
        new_aperture->next = first;
        first = new_aperture;
    }
    aperture* get_aperture(int num)
    {
        aperture* current = first;
        while (current != NULL)
        {
            if (current->num_aperture == num)
            {
                return current;
            }
            current = current->next;
        }
    }

};


void string_to_float(
    D2D1_SIZE_U size,
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    float& x_cord,//output
    float& y_cord,//output
    float scale =10,
    float Shift_X = 0,
    float Shift_Y = 0,
    bool yCoordinatWindow = true
)
{
    //Dots per inch
    //DPI and Device - Independent Pixels need to understand
    int i = 0;
    float x = 0;
    float y = 0;
    float num = 0;
    float result = 0;
    index--;

    while (str[index] != '*')
    {
        num = (float)(str[index] - '0') * pow(10, i);
        result = result + num;
        i++;
        index--;
        if (str[index] == '-')
        {
            result = result * -1;
            index--;
        }

        if (str[index] == 'Y')
        {
            result = result * scale;
            y_cord = result * pow(10, -coord_digits_y);
            y_cord = y_cord + Shift_Y;
            num = 0;
            i = 0;
            result = 0;
            index--;

            if (yCoordinatWindow == true)
            {
                //get the window cordinates
                //y_cord = size.height - y_cord;
            }

        }
        else if (str[index] == 'X')
        {
            result = result * scale;
            x_cord = result * pow(10, -coord_digits_x);
            x_cord = x_cord + Shift_X;
            num = 0;
            i = 0;
            result = 0;
            index--;
        }

    }

}

void Draw_circle(
    float shap_x,
    float shap_Y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush
)
{
    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x, shap_Y), (current_shape->diameter) / 2, (current_shape->diameter) / 2), pWicBrush);

    //current_x = shape_x;
    //current_y = shape_y;
}

void draw_line(
    HWND handeller_to_window,
    float& current_x,
    float& current_y,
    float shape_x,
    float shape_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pBrush
)
{

        pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(current_x, current_y), current_shape->diameter / 2, current_shape->diameter / 2), pBrush);
        if (current_shape->diameter == 0)
        {
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush, .5f);
        }
        else
        {
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush ,current_shape->diameter);
        }

        //draw a circule on the end of the line
        pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shape_x, shape_y), current_shape->diameter / 2, current_shape->diameter / 2), pBrush);


        //change the current shape coordinate to the new shape has just drawn
        current_x = shape_x;
        current_y = shape_y;
}


void draw_rectangle(
    float shape_x,
    float shape_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush)
{
    pWicRenderTarget->FillRectangle(D2D1::RectF(
        shape_x - current_shape->x / 2,
        shape_y - current_shape->y / 2,
        shape_x + current_shape->x / 2,
        shape_y + current_shape->y / 2),
        pWicBrush);

    //change the current shape to new shape that has just drawn
    //current_x = shape_x;
    //current_y = shape_y;
}


void draw_polygon(
    float shape_x,
    float shape_y,
    ID2D1Factory* pFactory,
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush,
    aperture* current_shape
    )
{
    ID2D1GeometrySink* pSink = NULL;
    // Create a path geometry.
    ID2D1PathGeometry* m_pPathGeometry;
    pFactory->CreatePathGeometry(&m_pPathGeometry);
    // Write to the path geometry using the geometry sink.
    m_pPathGeometry->Open(&pSink);
    double pi = 3.14159265;
    pSink->BeginFigure(
        D2D1::Point2F(shape_x + (current_shape->diameter / 2) * cos(pi / 4 * 0), shape_y + (current_shape->diameter / 2) * sin(pi / 4 * 0)),
        D2D1_FIGURE_BEGIN_FILLED
    );

    //(x,y) = (x+r*cos(pi/4*n) , y+r*sin(pi/4*n)); n = 0,1,2,3,4,5,6,7 for 8-polygon
    for (int i = 0; i < current_shape->num_vertices; i++)
    {
        pSink->AddLine(D2D1::Point2F(shape_x + (current_shape->diameter / 2) * cos(pi / 4 * i), shape_y + (current_shape->diameter / 2) * sin(pi / 4 * i)));

    }
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();
    //pRenderTarget->DrawGeometry(m_pPathGeometry, pBrush, 1.F);
    pWicRenderTarget->FillGeometry(m_pPathGeometry, pWicBrush);
    //SafeRelease(&pSink);

    //current_x = shape_x;
    //current_y = shape_y;

}


ID2D1Bitmap* DrawBitmapOnWindow(
    HWND handeller_to_window,
    ID2D1Factory* pFactory,
    IWICBitmap* pWicBitmap,
    IWICImagingFactory* wicFactory
)
{
    //ID2D1HwndRenderTarget* pRT = NULL;

    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U window_size = D2D1::SizeU(rc.right, rc.bottom);

    if (pRT == NULL)
    {
        pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(handeller_to_window, window_size),
            &pRT);
     }

    //convert WicBitmap to D2dBitmap
    IWICFormatConverter* pConverter = NULL;
    wicFactory->CreateFormatConverter(&pConverter);

    pConverter->Initialize(
        pWicBitmap,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );

    //ID2D1Bitmap* D2DpBitmap = NULL;

    // Create a Direct2D bitmap from the WIC bitmap.
    pRT->CreateBitmapFromWicBitmap(
        pConverter,
        NULL,
        &D2DpBitmap
    );

    //D2D1_SIZE_F size = pBitmap->GetSize();
    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(0.f, 0.f);
    pRT->BeginDraw();
    // Draw a bitmap.
    pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + window_size.width,
            upperLeftCorner.y + window_size.height)

    );
    pRT->EndDraw();


    return D2DpBitmap;
}



BYTE* lockMeoryBitmap(IWICBitmap* pWicBitmap, UINT puiWidth, UINT puiHeight, UINT& cb_Stride, UINT& cb_BufferSize )
{
    WICRect rcLock = { 0,0, puiWidth, puiHeight };
    IWICBitmapLock* pLock = NULL;
    pWicBitmap->Lock(&rcLock, WICBitmapLockWrite && WICBitmapLockRead, &pLock);


    BYTE* pv = NULL;

    pLock->GetStride(&cb_Stride);

    pLock->GetDataPointer(&cb_BufferSize, &pv);


    return pv;
}

//get the borders from the profile gerber
void GetWidthHeight(
    HWND handeller_to_window,
    char* str,
    D2D1_SIZE_U size,
    int coord_digits_x,
    int coord_digits_y,
    float scale,
    UINT& puiWidth, //output
    UINT& puiHeight, //output
    float& Shift_X,
    float& Shift_Y
)
{

    //Get the coordenate scale
    int i = 0;
    while (1)
    {
        if (str[i] == 'F' && str[i + 1] == 'S' && str[i + 2] == 'L' && str[i + 3] == 'A')
        {
            coord_digits_x = (int)(str[i + 6] - '0');
            coord_digits_y = (int)(str[i + 9] - '0');
            break;
        }
        else if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            break;
        }
        else
        {
            i++;
        }
    }

    float MaxX = 0;
    float MinX = 0;
    float MaxY = 0;
    float MinY = 0;
    float x;
    float y;
    /*
    i = 0;
    while (1)
    {
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            break;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, x, y, scale,true);
            if (x > MaxWidith)
            {
                MaxWidith = x;
            }
            if (y > MaxHeight)
            {
                MaxHeight = y;
            }
            i = i + 3;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, x, y, scale,true);
            if (x > MaxWidith)
            {
                MaxWidith = x;
            }
            if (y > MaxHeight)
            {
                MaxHeight = y;
            }
            i = i + 3;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, x, y, scale, true);
            if (x > MaxWidith)
            {
                MaxWidith = x;
            }
            if (y > MaxHeight)
            {
                MaxHeight = y;
            }
            i = i + 3;
        }
        else
        {
            i++;
        }
    }
   */

    i = 0;
    while (1)
    {
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            break;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, x, y, scale, false);
            if(x>MaxX)
            {
                MaxX = x;
            }
            if(y>MaxY)
            {
                MaxY = y;
            }
            if(x<MinX)
            {
                MinX = x;
            }
            if(y<MinY)
            {
                MinY = y;
            }

            i = i + 3;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, x, y, scale, false);
            if (x > MaxX)
            {
                MaxX = x;
            }
            if (y > MaxY)
            {
                MaxY = y;
            }
            if (x < MinX)
            {
                MinX = x;
            }
            if (y < MinY)
            {
                MinY = y;
            }

            i = i + 3;
        }
        else { i++; }
    }
    //float Shift_X = 0;
    //float Shift_Y = 0;

    /*if (MinX <= 0 && MaxX <= 0)
    {
        //any x must be increased by -MinX
        Shift_X = -MinX;
    }
    else if (MinX <= 0 && MaxX >= 0)
    {
        //any x must be encreased by -MinX
        Shift_X = -MinX;
    }
    else if(MinX>=0&&MaxX>=0)
    {
        //any x decreased by MinX
        Shift_X = -MinX;
    }*/
    Shift_X = -MinX;
    MaxX = MaxX - MinX;
    //MinX = 0;

    Shift_Y = -MinY;
    MaxY = MaxY - MinY;
    //MinY = 0;

    puiWidth = MaxX;
    puiHeight = MaxY;
    /*
    char xx[100];
    sprintf_s(xx, 99, "\nxx%d", puiWidth);
    MessageBoxWA(handeller_to_window, xx, "xx", MB_OK);


    char yy[100];
    sprintf_s(yy, 99, "\nyy%d", puiHeight);
    MessageBoxWA(handeller_to_window, yy, "yy", MB_OK);*/

}



//DrawDrills
void DrawDrills(
    char* str,
    D2D1_SIZE_U size,
    int coord_digits_x,
    int coord_digits_y,
    float Shift_X,
    float Shift_Y,
    float scale,
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush
)
{
    int i = 0;
    while (1)
    {
        if (str[i] == 'F' && str[i + 1] == 'S' && str[i + 2] == 'L' && str[i + 3] == 'A')
        {
            coord_digits_x = (int)(str[i + 6] - '0');
            coord_digits_y = (int)(str[i + 9] - '0');
            break;
        }
        else if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            break;
        }
        else
        {
            i++;
        }
    }

    //black
    const D2D1_COLOR_F color = D2D1::ColorF(255, 0, 0);
    //pWicBrush = NULL;
    //pWicRenderTarget->CreateSolidColorBrush(color, &pWicBrush);
    pWicBrush->SetColor(color);
    pWicRenderTarget->BeginDraw();

    float DrillCenter_X=0;
    float DrillCenter_Y=0;
    i = 0;
    while (1)
    {
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            pWicRenderTarget->EndDraw();
            break;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, DrillCenter_X, DrillCenter_Y, scale,Shift_X,Shift_Y, false);
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(DrillCenter_X, DrillCenter_Y), 1.2, 1.2), pWicBrush);
            //pWicRenderTarget->DrawLine(D2D1::Point2F(DrillCenter_X, DrillCenter_Y), D2D1::Point2F(DrillCenter_X+1, DrillCenter_Y+1), pWicBrush,1.5);
            i = i + 3;
        }
        else { i++; }
    }


}



void scale_translation(
    HWND handeller_to_window,
    ID2D1HwndRenderTarget* pRT,
    float scale_factor,
    float offset_x,
    float offset_y
)
{
    //get the size of the window
    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
    //scale the shapes from the center of window
    pRT->SetTransform(
        D2D1::Matrix3x2F::Scale(scale_factor,
            scale_factor,
            D2D1::Point2F(size.width / 2, size.height / 2)) * D2D1::Matrix3x2F::Translation(offset_x, offset_y)
    );

}




//////////
void SaveBitmapToFile(BYTE* pBitmapBits,
    LONG lWidth,
    LONG lHeight,
    WORD wBitsPerPixel,
    const unsigned long& padding_size,
    LPCTSTR lpszFileName)
{
    // Some basic bitmap parameters
    unsigned long headers_size = sizeof(BITMAPFILEHEADER) +
        sizeof(BITMAPINFOHEADER);

    unsigned long pixel_data_size = lHeight * ((lWidth * (wBitsPerPixel / 8)) + padding_size);

    BITMAPINFOHEADER bmpInfoHeader = { 0 };

    // Set the size
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);

    // Bit count
    bmpInfoHeader.biBitCount = wBitsPerPixel;

    // Use all colors
    bmpInfoHeader.biClrImportant = 0;

    // Use as many colors according to bits per pixel
    bmpInfoHeader.biClrUsed = 0;

    // Store as un Compressed
    bmpInfoHeader.biCompression = BI_RGB;

    // Set the height in pixels
    bmpInfoHeader.biHeight = lHeight;

    // Width of the Image in pixels
    bmpInfoHeader.biWidth = lWidth;

    // Default number of planes
    bmpInfoHeader.biPlanes = 1;

    // Calculate the image size in bytes
    bmpInfoHeader.biSizeImage = pixel_data_size;

    BITMAPFILEHEADER bfh = { 0 };

    // This value should be values of BM letters i.e 0x4D42
    // 0x4D = M 0×42 = B storing in reverse order to match with endian
    bfh.bfType = 0x4D42;
    //bfh.bfType = 'B'+('M' << 8);

    // <<8 used to shift ‘M’ to end  */

    // Offset to the RGBQUAD
    bfh.bfOffBits = headers_size;

    // Total size of image including size of headers
    bfh.bfSize = headers_size + pixel_data_size;

    // Create the file in disk to write
    HANDLE hFile = CreateFile(lpszFileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    // Return if error opening file
    if (!hFile) return;

    DWORD dwWritten = 0;

    // Write the File header
    WriteFile(hFile,
        &bfh,
        sizeof(bfh),
        &dwWritten,
        NULL);

    // Write the bitmap info header
    WriteFile(hFile,
        &bmpInfoHeader,
        sizeof(bmpInfoHeader),
        &dwWritten,
        NULL);

    // Write the RGB Data
    WriteFile(hFile,
        pBitmapBits,
        bmpInfoHeader.biSizeImage,
        &dwWritten,
        NULL);

    // Close the file handle
    CloseHandle(hFile);
}

void SaveBitmapToFileColor(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCTSTR lpszFileName)
{

    BITMAPINFOHEADER bmpInfoHeader = { 0 };
    // Set the size
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    // Bit count
    bmpInfoHeader.biBitCount = wBitsPerPixel;
    // Use all colors
    bmpInfoHeader.biClrImportant = 0;
    // Use as many colors according to bits per pixel
    bmpInfoHeader.biClrUsed = 0;
    // Store as un Compressed
    bmpInfoHeader.biCompression = BI_RGB;
    // Set the height in pixels
    bmpInfoHeader.biHeight = lHeight;
    // Width of the Image in pixels
    bmpInfoHeader.biWidth = lWidth;
    // Default number of planes
    bmpInfoHeader.biPlanes = 1;
    // Calculate the image size in bytes
    bmpInfoHeader.biSizeImage = lWidth * lHeight * (wBitsPerPixel / 8);

    BITMAPFILEHEADER bfh = { 0 };
    // This value should be values of BM letters i.e 0x4D42
    // 0x4D = M 0×42 = B storing in reverse order to match with endian

    bfh.bfType = 'B' + ('M' << 8);
    // <<8 used to shift ‘M’ to end

    // Offset to the RGBQUAD
    bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    // Total size of image including size of headers
    bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
    // Create the file in disk to write
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (!hFile) // return if error opening file
    {
        return;
    }

    DWORD dwWritten = 0;
    // Write the File header
    WriteFile(hFile, &bfh, sizeof(bfh), &dwWritten, NULL);
    // Write the bitmap info header
    WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
    // Write the palette
    //WriteFile( hFile, &palette[0], sizeof(RGBQUAD) * 256, &dwWritten, NULL );
    // Write the RGB Data
    if (lWidth % 4 == 0)
    {
        WriteFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
    }
    else
    {
        char* empty = new char[4 - lWidth % 4];
        for (int i = 0; i < lHeight; ++i)
        {
            WriteFile(hFile, &pBitmapBits[i * lWidth], lWidth, &dwWritten, NULL);
            WriteFile(hFile, empty, 4 - lWidth % 4, &dwWritten, NULL);
        }
    }
    // Close the file handle
    CloseHandle(hFile);
}

void DrawBorders(
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush,
    UINT puiWidth,
    UINT puiHeight
)
{
    const D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0);
    pWicBrush->SetColor(color);
    pWicRenderTarget->BeginDraw();
    pWicRenderTarget->DrawLine(D2D1::Point2F(0, 0), D2D1::Point2F(puiWidth, 0), pWicBrush, .5f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(puiWidth, 0), D2D1::Point2F(puiWidth, puiHeight), pWicBrush, .5f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(puiWidth, puiHeight), D2D1::Point2F(0, puiHeight), pWicBrush, .5f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(0, puiHeight), D2D1::Point2F(0, 0), pWicBrush, .5f);
    pWicRenderTarget->EndDraw();


}



/////
//
ID2D1Bitmap* DrawGerberOnBitmab(
    HWND handeller_to_window,
    char* str,
    char* profileStr,
    char* DrillsStr,
    ID2D1Factory* pFactory,
    ID2D1RenderTarget* pWicRenderTarget,
    IWICImagingFactory* wicFactory,
    IWICBitmap* pBitmap,
    ID2D1SolidColorBrush* pWicBrush)
{

    // Create a WIC Factory
    if (wicFactory == NULL)
    {
        CoCreateInstance(
           CLSID_WICImagingFactory,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_IWICImagingFactory,
           (LPVOID*)&wicFactory);
    }

    //get the window cordinates
    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    UINT puiWidth=0;
    UINT puiHeight=0;
    //assume 10^-6 mm mode
    int coord_digits_x = 4;
    int coord_digits_y = 4;
    //assume inatial value of scaling
    float scale = 10;
    float Shift_X=0;
    float Shift_Y = 0;
    //char profileStr[] = "G04 EAGLE Gerber RS-274X export* G75* %MOMM*% %FSLAX34Y34*% %LPD*% %IN*% %IPPOS*% %AMOC8* 5,1,8,0,0,1.08239X$1,22.5*% G01* %ADD10C,0.254000*% D10* X0Y0D02* X1000000Y0D01* X1000000Y800000D01* X0Y800000D01* X0Y0D01* X626110Y339820D02* X626039Y338741D01* X625898Y337669D01* X625687Y336609D01* X625408Y335565D01* X625060Y334541D01* X624646Y333543D01* X624168Y332573D01* X623628Y331637D01* X623027Y330738D01* X622369Y329881D01* X621657Y329068D01* X620892Y328304D01* X620079Y327591D01* X619222Y326933D01* X618323Y326332D01* X617387Y325792D01* X616417Y325314D01* X615419Y324900D01* X614395Y324552D01* X613351Y324273D01* X612291Y324062D01* X611219Y323921D01* X610140Y323850D01* X609060Y323850D01* X607981Y323921D01* X606909Y324062D01* X605849Y324273D01* X604805Y324552D01* X603781Y324900D01* X602783Y325314D01* X601813Y325792D01* X600877Y326332D01* X599978Y326933D01* X599121Y327591D01* X598308Y328304D01* X597544Y329068D01* X596831Y329881D01* X596173Y330738D01* X595572Y331637D01* X595032Y332573D01* X594554Y333543D01* X594140Y334541D01* X593792Y335565D01* X593513Y336609D01* X593302Y337669D01* X593161Y338741D01* X593090Y339820D01* X593090Y340900D01* X593161Y341979D01* X593302Y343051D01* X593513Y344111D01* X593792Y345155D01* X594140Y346179D01* X594554Y347177D01* X595032Y348147D01* X595572Y349083D01* X596173Y349982D01* X596831Y350839D01* X597544Y351652D01* X598308Y352417D01* X599121Y353129D01* X599978Y353787D01* X600877Y354388D01* X601813Y354928D01* X602783Y355406D01* X603781Y355820D01* X604805Y356168D01* X605849Y356447D01* X606909Y356658D01* X607981Y356799D01* X609060Y356870D01* X610140Y356870D01* X611219Y356799D01* X612291Y356658D01* X613351Y356447D01* X614395Y356168D01* X615419Y355820D01* X616417Y355406D01* X617387Y354928D01* X618323Y354388D01* X619222Y353787D01* X620079Y353129D01* X620892Y352417D01* X621657Y351652D01* X622369Y350839D01* X623027Y349982D01* X623628Y349083D01* X624168Y348147D01* X624646Y347177D01* X625060Y346179D01* X625408Y345155D01* X625687Y344111D01* X625898Y343051D01* X626039Y341979D01* X626110Y340900D01* X626110Y339820D01* M02*";
    //char profileStr[] = "%TF.GenerationSoftware,KiCad,Pcbnew,(5.1.12)-1*% %TF.CreationDate,2022-04-23T11:54:43+02:00*% %TF.ProjectId,test,74657374-2e6b-4696-9361-645f70636258,rev?*% %TF.SameCoordinates,Original*% %TF.FileFunction,Profile,NP*% %FSLAX46Y46*% G04 Gerber Fmt 4.6, Leading zero omitted, Abs format (unit mm)* G04 Created by KiCad (PCBNEW (5.1.12)-1) date 2022-04-23 11:54:43* %MOMM*% %LPD*% G01* G04 APERTURE LIST* %TA.AperFunction,Profile*% %ADD10C,0.050000*% %TD*% G04 APERTURE END LIST* D10* X19050000Y-72390000D02* X95250000Y-72390000D01* X19050000Y-13970000D02* X19050000Y-72390000D01* X95250000Y-13970000D02* X19050000Y-13970000D01* X95250000Y-72390000D02* X95250000Y-13970000D01* M02*";
    GetWidthHeight(handeller_to_window, profileStr, size, coord_digits_x, coord_digits_y, scale, puiWidth, puiHeight, Shift_X, Shift_Y);

    if (pBitmap == NULL)
    {
        WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory->CreateBitmap(puiWidth, puiHeight, formatGUID, WICBitmapCacheOnDemand, &pBitmap);
    }

    //create factorty
    if (pFactory == NULL)
    {
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
    }
    if (pWicRenderTarget == NULL)
    {
        pFactory->CreateWicBitmapRenderTarget(
            pBitmap,
            D2D1::RenderTargetProperties(),
            &pWicRenderTarget);
    }

    //create brush
    if (pWicBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210   );
        pWicRenderTarget->CreateSolidColorBrush(color, &pWicBrush);
    }
    //End intialization
    //////////////////////////////////////////////////////////////////////////
    DrawBorders(pWicRenderTarget, pWicBrush, puiWidth, puiHeight);
    const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
    pWicBrush->SetColor(color);

    //start tracing and draw on wicBitmap

    //object for all shapes
    all_standerd_aperture all_shapes;
    //struct for current shape
    aperture* current_shape=NULL;

    //(x,y) of the current shape
    float current_x;
    float current_y;
    //(x,y) of the new shape
    float shape_x;
    float shape_y;
    //let num = 3 as defult value
    int num_integers_in_coordinate_x = 3;
    int num_integers_in_coordinate_y = 3;

    // Draw a black bitmap
    pWicRenderTarget->BeginDraw();
    pWicRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    //start tracing the string
    int i = 0;
    while (1)
    {
        //end of file
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            pWicRenderTarget->EndDraw();
            break;
        }
        //draw line or arch
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            string_to_float(size, str, i,coord_digits_x,coord_digits_y, shape_x, shape_y, scale,Shift_X,Shift_Y);
            draw_line(handeller_to_window, current_x, current_y, shape_x,shape_y,pWicRenderTarget, current_shape, pWicBrush);
            i = i + 3;
        }
        //only move to a certain point
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            i = i + 3;
        }
        //creart flash (replicating) the current aperture
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
            //for Circle
            if (current_shape->type == 1)
            {
                Draw_circle(shape_x, shape_y, pWicRenderTarget, current_shape, pWicBrush);
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            //for Rectangle
            else if (current_shape->type == 2)
            {
                string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                draw_rectangle(shape_x, shape_y, pWicRenderTarget,current_shape,pWicBrush);
            }
            //for Obround
            else if (current_shape->type == 3)
            {
                string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                Draw_circle(shape_x, shape_y, pWicRenderTarget, current_shape, pWicBrush);
                //draw_Obround(handeller_to_window);
            }
            //for Polygon
            else if (current_shape->type == 4)
            {
                draw_polygon(shape_x, shape_y, pFactory, pWicRenderTarget, pWicBrush, current_shape);
                // draw polygon as circle
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            i = i + 3;
        }
        //create shape (create only not draw .....assume all shapes name start with D..
        else if (str[i] == 'A' && str[i + 1] == 'D' && str[i + 2] == 'D')
        {
            //all shapes start from D10 to D99
            int num_shape = (int)(str[i + 3] - '0') * 10 + (int)(str[i + 4] - '0') * 1;

            //aperture* shape = all_shapes->get_aperture(num_shape);
            //create circle
            if (str[i + 5] == 'C')
            {
                float diameter = 0;
                float hole_diameter = 0;
                int j = i + 7;
                while (str[j] != '.')
                {
                    j++;
                }
                int dot_index = j;
                float num = 0;
                float result = 0;
                int exponent = 0;
                j--;
                while (str[j] != ',')
                {
                    num = (float)(str[j] - '0') * pow(10, exponent);
                    result = result + num;
                    j--;
                    exponent++;
                }
                exponent = -1;
                j = 1;
                while (str[dot_index + j] != '*' && str[dot_index + j] != 'X')
                {
                    num = (float)(str[dot_index + j] - '0') * pow(10, exponent);
                    result = result + num;
                    j++;
                    exponent--;
                }
                //scale the raduis
                result = result * scale;
                diameter = result;
                //diameter = result * pow(10, -coord_digits_x);

                //let the diameter has one digit
                i = i + 9;

                all_shapes.add_aperture(1, num_shape, diameter, 0, 0, 0, hole_diameter);

            }
            //for other shapes .....neglict them now
            else if (str[i + 5] == 'R')
            {
                float x = 0;
                float y = 0;
                float hole_diameter = 0;

                int j = i + 7;
                //first number (x)
                while (str[j] != '.')
                {
                    j++;
                }
                int dot_index = j;
                float num = 0;
                float result = 0;
                int exponent = 0;
                j--;
                while (str[j] != ',')
                {
                    num = (float)(str[j] - '0') * pow(10, exponent);
                    result = result + num;
                    j--;
                    exponent++;
                }
                exponent = -1;
                j = 1;
                int index_y = 0;
                while (str[dot_index + j] != 'X')
                {
                    num = (float)(str[dot_index + j] - '0') * pow(10, exponent);
                    result = result + num;
                    j++;
                    exponent--;

                }
                //start of y
                index_y = dot_index + j + 1;
                result = result * scale;
                x = result;
                //x = result * pow(10, -coord_digits_x);

                //second number (y)
                dot_index = 0;
                while (str[index_y] != '.')
                {
                    index_y++;
                }
                dot_index = index_y;
                num = 0;
                result = 0;
                exponent = 0;
                index_y = index_y - 1;
                while (str[index_y] != 'X')
                {
                    num = (float)(str[index_y] - '0') * pow(10, exponent);
                    result = result + num;
                    index_y--;
                    exponent++;
                }
                exponent = -1;
                index_y = 1;
                while (str[dot_index + index_y] != '*' && str[dot_index + index_y] != 'X')
                {
                    num = (float)(str[dot_index + index_y] - '0') * pow(10, exponent);
                    result = result + num;
                    index_y++;
                    exponent--;
                }
                result = result * scale;
                y = result;

                //let the diameter has one digit for x and one digit for y
                i = i + 11;

                all_shapes.add_aperture(2, num_shape, 0, x, y, 0, hole_diameter);
            }
            else if (str[i + 5] == 'O')
            {
            float x = 0;
            float y = 0;
            float hole_diameter = 0;

            int j = i + 7;
            //first number (x)
            while (str[j] != '.')
            {
                j++;
            }
            int dot_index = j;
            float num = 0;
            float result = 0;
            int exponent = 0;
            j--;
            while (str[j] != ',')
            {
                num = (float)(str[j] - '0') * pow(10, exponent);
                result = result + num;
                j--;
                exponent++;
            }
            exponent = -1;
            j = 1;
            int index_y = 0;
            while (str[dot_index + j] != 'X')
            {
                num = (float)(str[dot_index + j] - '0') * pow(10, exponent);
                result = result + num;
                j++;
                exponent--;

            }
            //start of y
            index_y = dot_index + j + 1;
            result = result * scale;
            x = result;
            //x = result * pow(10, -coord_digits_x);

            //second number (y)
            dot_index = 0;
            while (str[index_y] != '.')
            {
                index_y++;
            }
            dot_index = index_y;
            num = 0;
            result = 0;
            exponent = 0;
            index_y = index_y - 1;
            while (str[index_y] != 'X')
            {
                num = (float)(str[index_y] - '0') * pow(10, exponent);
                result = result + num;
                index_y--;
                exponent++;
            }
            exponent = -1;
            index_y = 1;
            while (str[dot_index + index_y] != '*' && str[dot_index + index_y] != 'X')
            {
                num = (float)(str[dot_index + index_y] - '0') * pow(10, exponent);
                result = result + num;
                index_y++;
                exponent--;
            }
            result = result * scale;
            y = result;

                //let the diameter has one digit for x and one digit for y
                i = i + 11;
                all_shapes.add_aperture(3, num_shape, x, 0, 0, 0, hole_diameter);
            }
            //need to create
            else if (str[i + 5] == 'P')
            {
                float diameter = 0;
                float vertices = 0;
                float hole_diameter = 0;

                int j = i + 7;
                //first number (diameter)
                while (str[j] != '.')
                {
                    j++;
                }
                int dot_index = j;
                float num = 0;
                float result = 0;
                int exponent = 0;
                j--;
                while (str[j] != ',')
                {
                    num = (float)(str[j] - '0') * pow(10, exponent);
                    result = result + num;
                    j--;
                    exponent++;
                }
                exponent = -1;
                j = 1;
                int index_y = 0;
                while (str[dot_index + j] != 'X')
                {
                    num = (float)(str[dot_index + j] - '0') * pow(10, exponent);
                    result = result + num;
                    j++;
                    exponent--;
                }
                //start of y
                index_y = dot_index + j + 1;
                result = result * scale;
                diameter = result;

                //second number (vertices)
                if (str[index_y + 1] != 'X')
                {
                    vertices = (float)(str[index_y] - '0') * 10 + (float)(str[index_y + 1] - '0') * 1;
                }
                else if (str[index_y + 1] == 'X')
                {
                    vertices = (float)(str[index_y] - '0') * 1;
                }
                /*while (str[index_y] != '.')
                {
                    index_y++;
                }
                dot_index = index_y;
                num = 0;
                result = 0;
                exponent = 0;
                index_y = index_y - 1;
                while (str[index_y] != ',')
                {
                    num = (float)(str[index_y] - '0') * pow(10, exponent);
                    result = result + num;
                    index_y--;
                    exponent++;
                }
                exponent = -1;
                index_y = 1;
                while (str[dot_index + index_y] != '*' && str[dot_index + index_y] != 'X')
                {
                    num = (float)(str[dot_index + index_y] - '0') * pow(10, exponent);
                    result = result + num;
                    index_y++;
                    exponent--;
                }*/

                //let the diameter has one digit for x and one digit for y

                all_shapes.add_aperture(4, num_shape, diameter, 0, 0, vertices, hole_diameter);
                i = i + 12;

            }
        }
        else if (str[i] == 'D' && ((int)(str[i + 1])) >= 1 && str[i + 3] == '*')
        {
            //all shapes start from D10 to D99
            int num_shape = (int)(str[i + 1] - '0') * 10 + (int)(str[i + 2] - '0') * 1;

            current_shape = all_shapes.get_aperture(num_shape);
            i = i + 3;
        }
        else if (str[i] == 'F' && str[i + 1] == 'S' && str[i + 2] == 'L' && str[i + 3] == 'A')
        {
            num_integers_in_coordinate_x = (int)(str[i + 5] - '0');
            coord_digits_x = (int)(str[i + 6] - '0');

            num_integers_in_coordinate_y = (int)(str[i + 8] - '0');
            coord_digits_y = (int)(str[i + 9] - '0');
            i = i + 11;
        }
        else
        {
            i++;
        }
    }

    //Draw the original bitmap
    ID2D1Bitmap* D2dbitmap = DrawBitmapOnWindow(handeller_to_window,pFactory, pBitmap,wicFactory);

    /*
        //Excute Max cupper algorithm the draw it
        MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);
        //start max cupper

        //cb_stride is sent by referance, so the initalization of it is in the fancution
        UINT cb_stride = 0;
        //pv is a pointer to the bitmap bytes
        BYTE* pv = lockMeoryBitmap(pBitmap, puiWidth, puiHeight, cb_stride);
        //start max cupper
        All_algorithm_max_cupper(pv, cb_stride, puiWidth, puiHeight);
        //draw the bitmap after max cupper
        D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
        //end max cupper
        */
        //drills
        ///////////////////
        MessageBoxW(handeller_to_window, L"start Draw Drills", L"start Draw Drills", MB_OKCANCEL);
        //char DrillsStr[] = "G04 EAGLE Gerber RS-274X export* G75* %MOMM*% %FSLAX34Y34*% %LPD*% %INSoldermask Bottom*% %IPPOS*% %AMOC8* 5,1,8,0,0,1.08239X$1,22.5*% G01* %ADD10C,1.524000*% %ADD11C,1.879600*% %ADD12C,2.743200*% %ADD13C,3.505200*% D10* X254000Y355600D03* X304800Y355600D03* X609600Y533400D03* X660400Y533400D03* D11* X317500Y431800D03* X393700Y431800D03* D12* X152400Y139700D03* X254000Y139700D03* D10* X152400Y685800D03* X177800Y685800D03* X381000Y533400D03* X508000Y533400D03* X520700Y660400D03* X647700Y660400D03* X381000Y596900D03* X508000Y596900D03* X368300Y685800D03* X495300Y685800D03* X850900Y368300D03* X876300Y393700D03* X850900Y419100D03* D11* X584200Y165100D03* X609600Y165100D03* X635000Y165100D03* D13* X609600Y340360D03* M02*";
        DrawDrills(DrillsStr, size, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale, pWicRenderTarget, pWicBrush);
        D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
        //end draw dills

        /////////////////

         //Excute Max cupper algorithm the draw it
        MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);

        //start max cupper

        //cb_stride is sent by referance, so the initalization of it is in the fancution
        UINT cb_stride = 0;
        UINT cb_BufferSize = 0;
        //pv is a pointer to the bitmap bytes
        BYTE* pv = lockMeoryBitmap(pBitmap, puiWidth, puiHeight, cb_stride, cb_BufferSize);
        //start max cupper
        All_algorithm_max_cupper(pv, cb_stride, puiWidth, puiHeight);
        //draw the bitmap after max cupper
        D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
        ////end max cupper

        ////convert the red drills to black
        for (UINT i = 0; i < puiHeight; i++)
        {
            for (UINT j = 0; j < puiWidth; j++)
            {
                UINT BB = 0;
                UINT GG = 0;
                UINT RR = 0;
                UINT A = 0;
                get_pixel(pv, cb_stride, j, i, BB, GG, RR, A);

                if (BB == 0 && RR == 255 && GG == 0)
                {
                    set_pixel(pv, cb_stride, j, i, 0, 0, 0, A);
                }
            }
        }
        D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
        ////

        //initalize the bitmap object
        bitmapObject.SetSpecifications(pv, cb_BufferSize, cb_stride, puiWidth, puiHeight);
        ////

        //SaveBitmapToFile(pv, puiWidth, puiHeight, 4, 0, L"test1.bmp");
        //SaveBitmapToFileColor(pv, puiWidth, puiHeight, 4, L"test2");
        /*GpuiHeight = puiHeight;
        GpuiWidth = puiWidth;
        Gpv = pv;*/


        /*
        BMP image(puiWidth, puiHeight, true);
        int k = 0;
        for (UINT i = 0 + 1; i < puiHeight - 1; i++)
        {
            for (UINT j = 0 + 1; j < puiWidth - 1; j++)
            {
                image.set_pixel(j, i, *(pv + k), *(pv + k + 1), *(pv + k + 2), 255);
                k=k+4;
            }
        }

        image.write("o.bmp");
        */




        //Export the Bitmap
        IWICStream* piStream = NULL;
        IWICBitmapEncoder* piEncoder = NULL;
        IWICBitmapFrameEncode* piBitmapFrame = NULL;
        wicFactory->CreateStream(&piStream);
        piStream->InitializeFromFilename(L"testOut.bmp", GENERIC_WRITE);
        wicFactory->CreateEncoder(GUID_ContainerFormatBmp, NULL, &piEncoder);
        piEncoder->Initialize(piStream, WICBitmapEncoderNoCache);
        piEncoder->CreateNewFrame(&piBitmapFrame, NULL);
        piBitmapFrame->Initialize(NULL);

        /*
        IWICImagingFactory2*  m_wicFactory =NULL;
        CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_wicFactory)
        );
        IWICImageEncoder* imageEncoder = NULL;
        m_wicFactory->CreateImageEncoder(D2dbitmap,
            &imageEncoder
        );
        imageEncoder->WriteFrame(D2dbitmap,
            piBitmapFrame,
            nullptr);
        */


        WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
        piBitmapFrame->SetSize(puiWidth, puiHeight);
        piBitmapFrame->SetPixelFormat(&formatGUID);
        //UINT cb_BufferSize = puiWidth * puiHeight * 4;
        piBitmapFrame->WriteSource(pBitmap, NULL);

        piBitmapFrame->Commit();
        piEncoder->Commit();

        //piBitmapFrame->WritePixels(puiHeight, cb_stride, cb_BufferSize, pv);
        //piStream->Write(pv, cb_BufferSize,NULL);
        /*
        IStream* pIStream = NULL;
        SHCreateStreamOnFileEx(
            L"Output.bmp",
            STGM_READ | STGM_SHARE_DENY_NONE | STGM_DELETEONRELEASE,
            FILE_ATTRIBUTE_NORMAL,
            FALSE,
            NULL,
            &pIStream);
        pIStream->Write(pv, cb_BufferSize, NULL);
        pIStream->Commit(STGC_DEFAULT);
        */


        /*
        if(SUCCEEDED(hr))
        {
            MessageBoxW(handeller_to_window, L"Done Writting", L"Done Writting", MB_OKCANCEL);
        }*/
        ///////////////////////////////////////////

    return D2dbitmap;

    /*
    UINT cb_stride=0;
    BYTE* pv = lockMeoryBitmap(pBitmap, 800, 800, cb_stride);
    All_algorithm_max_cupper(pv,cb_stride, 800, 800);

    DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
    */



}


void Resize(HWND handeller_to_window,ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* D2DpBitmap)
{
    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U window_size = D2D1::SizeU(rc.right, rc.bottom);
    pRT->Resize(window_size);
    InvalidateRect(handeller_to_window, NULL, FALSE);


    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(0.f, 0.f);
    pRT->BeginDraw();
    pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
    // Draw a bitmap.
    pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + window_size.width,
            upperLeftCorner.y + window_size.height)

    );
    pRT->EndDraw();

}

/*
void Resize(HWND handeller_to_window, char* szFileBuffer, float scale_factor, float  offset_x, float offset_y)
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(handeller_to_window, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        pRenderTarget->Resize(size);
        //calculate_layout();
        InvalidateRect(handeller_to_window, NULL, FALSE);
    }

    if (szFileBuffer)
        DrawGerberOnBitmab(handeller_to_window, szFileBuffer,NULL,NULL,NULL,NULL,NULL, scale_factor, offset_x, offset_y);

}
*/
