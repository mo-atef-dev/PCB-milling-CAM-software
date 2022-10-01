#include"maxCupper_loading.h"
#include <stdio.h>
ID2D1Factory* pFactory_ = NULL;
ID2D1RenderTarget* pWicRenderTarget_ = NULL;
IWICImagingFactory* wicFactory_ = NULL;
ID2D1SolidColorBrush* pWicBrush =NULL;
IWICBitmap* pBitmap_ = NULL;    //maxcupper
IWICBitmap* pBitmap_flip = NULL;    //maxcupper
IWICBitmap* pBitmap_2 = NULL;   //gerber
IWICBitmap* pBitmap_2_flip = NULL;   //gerber
IWICBitmap* pBitmap_3 = NULL;   //maxcupper and gerber
IWICBitmap* pBitmap_3_flip = NULL;   //maxcupper and gerber
IWICBitmap* pBitmap_4 = NULL;   //mmg
IWICBitmap* pBitmap_4_flip = NULL;   //mmg

IWICBitmap* pBitmap_main = NULL;   //all

ID2D1Bitmap* D2DpBitmap = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pWindowBrush =NULL;

MyBitmap bitmapObject = MyBitmap();


UINT w=0;
UINT h=0;
UINT w_MMG =0;
UINT h_MMG =0;
bool MMG_or_Maxcupper = true;  //true for MMG , false for maxcupper
bool flip_or_not =true;

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
            /*if ((BB == 0 && RR == 0 && GG == 0)||
                (BB == 0 && RR == 255 && GG == 0))
            {

            }*/
            /*else*/ if(BB == 0 && RR >= 250 && GG ==0 )
            {
                set_pixel(pv, cb_Stride, j, i, color_1, color_1, color_1, 255);
            }
            else
            {
                set_pixel(pv, cb_Stride, j, i, 0, 0, 0, 255);
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
    std::cout<<"loading maxcupper...";
    //while(c<0)
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


void inhanceMaxcupper(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight)
{

    UINT B = 0;
    UINT G = 0;
    UINT R = 0;
    UINT A = 0;

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

    UINT counter =1;
    bool flag_detect_one = false;
    int c =0;
    std::cout<<"loading maxcupper...";
    //while (counter != 0)
    while (c != 1)
    {
        c++;
        counter = 0;
        for (UINT i = 0 + 2; i < puiHeight - 2; i++)
        {
            for (UINT j = 0 + 2; j < puiWidth - 2; j++)
            {
                flag_detect_one = false;
                int xNew=j;
                int yNew=i;
                int x=0;
                int y=0;
                get_pixel(pv, cb_Stride, j, i, B, G, R, A);
                //if current pixel is cupper -> start the algorithm
                if (B == 0 && G == 0 && R == 0)
                {
                    x= j;
                    y= i;
                    while(1)
                    {
                        get_pixel(pv, cb_Stride, x-1, y-1, B1, G1, R1, A);
                        if(B1 == 0 && G1 == 0 && R1 ==0)
                        {
                            flag_detect_one = true;
                            xNew = x-1;
                            yNew = y-1;
                        }
                        get_pixel(pv, cb_Stride, x, y-1, B2, G2, R2, A);
                        if(B2 == 0 && G2 == 0 && R2 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x;
                                yNew = y-1;
                            }

                        }
                        get_pixel(pv, cb_Stride, x+1, y-1, B3, G3, R3, A);
                        if(B3 == 0 && G3 == 0 && R3 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x+1;
                                yNew = y-1;
                            }
                        }
                        get_pixel(pv, cb_Stride, x-1, y, B4, G4, R4, A);
                        if(B4 == 0 && G4 == 0 && R4 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x-1;
                                yNew = y;
                            }
                        }
                        get_pixel(pv, cb_Stride, x+1, y, B5, G5, R5, A);
                        if(B5 == 0 && G5 == 0 && R5 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x+1;
                                yNew = y;
                            }
                        }
                        get_pixel(pv, cb_Stride, x-1, y+1, B6, G6, R6, A);
                        if(B6 == 0 && G6 == 0 && R6 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x-1;
                                yNew = y+1;
                            }
                        }
                        get_pixel(pv, cb_Stride, x, y+1, B7, G7, R7, A);
                        if(B7 == 0 && G7 == 0 && R7 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x;
                                yNew = y+1;
                            }
                        }
                        get_pixel(pv, cb_Stride, x+1, y+1, B8, G8, R8, A);
                        if(B8 == 0 && G8 == 0 && R8 ==0)
                        {
                            if(flag_detect_one == true)
                                break;
                            else
                            {
                                flag_detect_one = true;
                                xNew = x+1;
                                yNew = y+1;
                            }
                        }

                        set_pixel(pv,cb_Stride,x,y,255,255,255,255);
                        counter++;
                        x = xNew;
                        y = yNew;
                        if(flag_detect_one ==false)
                            break;
                        else
                            flag_detect_one = false;

                    }//end while(1)
                }
            }
        }
    }
}

//All Algorithm max cupper functions
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
    max_cupper(pv,cb_Stride, puiWidth, puiHeight, color_1, color_2, cupper);

    inhanceMaxcupper(pv,cb_Stride,puiWidth,puiHeight);
}

void BlackAroundTracks(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight)
{
    //same color
    for (UINT i = 0 + 2; i < puiHeight - 2; i++)
    {
        for (UINT j = 0 + 2; j < puiWidth - 2; j++)
        {
            UINT B = 0;
            UINT G = 0;
            UINT R = 0;
            UINT A = 0;

            get_pixel(pv, cb_Stride, j, i, B, G, R, A);
            if(B == 0 && G == 0 && R >=250)
            {
                //red
                set_pixel(pv,cb_Stride,j,i,0,0,255,A);
            }
            else if(B == 0 && G == 0 && R ==0)
            {
                continue;
            }
            else
            {
                //not red
                set_pixel(pv,cb_Stride,j,i,255,255,255,A);
            }
        }
    }


    UINT B = 0;
    UINT G = 0;
    UINT R = 0;
    UINT A = 0;

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

    UINT counter =1;
    bool flag_detect_one = false;
    int c =0;
    std::cout<<"loading Without maxcupper...";
    //while (counter != 0)
    while (c != 1)
    {
        //std::cout<<c<<std::endl;
        c++;
        counter = 0;
        for (UINT i = 0 + 2; i < puiHeight - 2; i++)
        {
            for (UINT j = 0 + 2; j < puiWidth - 2; j++)
            {
                get_pixel(pv, cb_Stride, j, i, B, G, R, A);
                if(B == 0 && G == 0 && R == 0)
                {
                    //std::cout<<"3"<<std::endl;
                    continue;
                }
                else if(B == 255 && G == 255 && R == 255)
                {
                    //std::cout<<"2"<<std::endl;
                    continue;
                }
                else if (B == 0 && G == 0 && R == 255)
                {
                    std::cout<<"1111"<<std::endl;
                    /*get_pixel(pv, cb_Stride, j-1, i-1, B1, G1, R1, A);
                    //if(B1 ==255 && G1 ==255 && R1==255)
                    if(!(B1 ==0 && G1 ==0 && R1==255))
                    {
                        set_pixel(pv,cb_Stride,j-1,i-1,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }*/
                    get_pixel(pv, cb_Stride, j, i-1, B2, G2, R2, A);
                    //if(B2 ==255 && G2 ==255 && R2==255)
                    if(!(B2 ==0 && G2 ==0 && R2==255))
                    {
                        set_pixel(pv,cb_Stride,j,i-1,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }
                    /*get_pixel(pv, cb_Stride, j+1, i-1, B3, G3, R3, A);
                    //if(B3 ==255 && G3 ==255 && R3==255)
                    if(!(B3 ==0 && G3 ==0 && R3==255))
                    {
                        set_pixel(pv,cb_Stride,j+1,i-1,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }*/
                    get_pixel(pv, cb_Stride, j-1, i, B4, G4, R4, A);
                    //if(B3 ==255 && G3 ==255 && R3==255)
                    if(!(B4 ==0 && G4 ==0 && R4==255))
                    {
                        set_pixel(pv,cb_Stride,j-1,i,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }
                    get_pixel(pv, cb_Stride, j+1, i, B5, G5, R5, A);
                    //if(B3 ==255 && G3 ==255 && R3==255)
                    if(!(B5 ==0 && G5 ==0 && R5==255))
                    {
                        set_pixel(pv,cb_Stride,j+1,i,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }
                    /*get_pixel(pv, cb_Stride, j-1, i+1, B6, G6, R6, A);
                    //if(B3 ==255 && G3 ==255 && R3==255)
                    if(!(B6 ==0 && G6 ==0 && R6==255))
                    {
                        set_pixel(pv,cb_Stride,j-1,i+1,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }*/
                    get_pixel(pv, cb_Stride, j, i+1, B7, G7, R7, A);
                    //if(B3 ==255 && G3 ==255 && R3==255)
                    if(!(B7 ==0 && G7 ==0 && R7==255))
                    {
                        set_pixel(pv,cb_Stride,j,i+1,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }
                    /*get_pixel(pv, cb_Stride, j+1, i+1, B8, G8, R8, A);
                    //if(B3 ==255 && G3 ==255 && R3==255)
                    if(!(B8 ==0 && G8 ==0 && R8==255))
                    {
                        set_pixel(pv,cb_Stride,j+1,i+1,0,0,0,A);
                        //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                    }*/
                    //set_pixel(pv,cb_Stride,j,i,255,255,255,A);
                }
            }
        }
    }


    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
            UINT BB = 0;
            UINT GG = 0;
            UINT RR = 0;
            UINT A = 0;
            get_pixel(pv, cb_Stride, j, i, BB, GG, RR, A);

            if(BB == 0 && RR == 255 && GG ==0 )
            {
                set_pixel(pv, cb_Stride, j, i, 255, 255, 255, 255);
            }

        }
    }
}


void flipBitmap(BYTE* pv_maxcupper_with_drills, BYTE* pv_gerber, BYTE* pv_total,UINT cb_BufferSize,UINT cb_stride, UINT puiHeight, UINT puiWidth)
{
    BYTE* pvTemp_ = new BYTE[cb_BufferSize];
    BYTE* pvTemp_2 = new BYTE[cb_BufferSize];
    BYTE* pvTemp_3 = new BYTE[cb_BufferSize];
    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
            UINT BB = 0;
            UINT GG = 0;
            UINT RR = 0;
            UINT A = 0;
            get_pixel(pv_maxcupper_with_drills, cb_stride, j, i, BB, GG, RR, A);
            set_pixel(pvTemp_, cb_stride, j, puiHeight-i-1, BB, GG, RR,A);

            get_pixel(pv_gerber, cb_stride, j, i, BB, GG, RR, A);
            set_pixel(pvTemp_2, cb_stride, j, puiHeight-i-1, BB, GG, RR,A);

            get_pixel(pv_total, cb_stride, j, i, BB, GG, RR, A);
            set_pixel(pvTemp_3, cb_stride, j, puiHeight-i-1, BB, GG, RR,A);

        }
    }

    for(int k=0; k<cb_BufferSize;k++)
    {
        *(pv_total + k) = *(pvTemp_3 + k);

        *(pv_maxcupper_with_drills + k) = *(pvTemp_ + k);

        *(pv_gerber + k) = *(pvTemp_2 + k);
    }
}



void string_to_float(
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    float& x_cord,//output
    float& y_cord,//output
    float scale/* =10*/,
    float Shift_X /*= 0*/,
    float Shift_Y /*= 0*/
)
{
    int i = 0;
    float x = 0;
    float y = 0;
    float num = 0;
    float result = 0;
    index--;

    while (str[index] != '*' && str[index] != 'I' && str[index] != 'J')
    {
        if(!((float)(str[index] - '0') <= 9 || (float)(str[index] - '0') >= 0))
        {
            //an error occures
            return;
        }
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



void string_to_float_dill(
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    int LZ_TZ_dec,
    float& x_cord,//output
    float& y_cord,//output
    float scale /*= 10*/,
    float Shift_X /*= 0*/,
    float Shift_Y /*= 0*/
)
{

    /*
         0: decimal format
         1: LZ format
         2: TZ format
    */
    if(LZ_TZ_dec == 0)
    {
        if(str[index]=='X')
        {
            bool minus_num = false;
            if(str[index+1]=='-')
            {
                minus_num = true;
            }
            while(str[index] != '.')
            {
                index++;
            }
            int dot_index = index;
            index--;
            float result =0;
            float num=0;
            int i=0;
            while (str[index] != 'X' && str[index] != '-')
            {
                num = (float)(str[index] - '0') * pow(10, i);
                result = result + num;
                index--;
                i++;
            }
            i=-1;
            dot_index++;
            while(str[dot_index]!= 'Y' && str[dot_index]!= '\n' &&str[dot_index]!= '\r')
            {
                num = (float)(str[dot_index] - '0') * pow(10, i);
                result = result + num;
                dot_index++;
                i--;
            }
            if(minus_num == true)
            {
                result = result * -1;
            }
            x_cord = result;
            x_cord = x_cord * scale;
            x_cord = x_cord +Shift_X;
            index = dot_index;
        }

        if(str[index]=='Y')
        {
            bool minus_num=false;
            if(str[index+1]=='-')
            {
                minus_num = true;
            }
            while(str[index] != '.')
            {
                index++;
            }
            int dot_index = index;
            index--;
            float result =0;
            float num=0;
            int i=0;
            while (str[index] != 'Y' && str[index] != '-')
            {
                num = (float)(str[index] - '0') * pow(10, i);
                result = result + num;
                index--;
                i++;
            }
            i=-1;
            dot_index++;
            while(str[dot_index]!= '\n' &&str[dot_index]!= '\r')
            {
                num = (float)(str[dot_index] - '0') * pow(10, i);
                result = result + num;
                dot_index++;
                i--;
            }
            if(minus_num == true)
            {
                result = result*-1;
            }
            y_cord = result;
            y_cord =y_cord * scale;
            y_cord = y_cord +Shift_Y;
        }
    }
    else if (LZ_TZ_dec == 2)
    {
        //get the end of the line
        while (str[index] != '\n' && str[index] != 'G' && str[index] != '\r')
        {
            index++;
        }
        int index_end_Line = index;

        int i = 0;
        float x = 0;
        float y = 0;
        double num = 0;
        double result = 0;
        index--;

        while (str[index] != '\n' )
        {
            if (str[index] == '-')
            {
                result = result * -1;
                index--;
                continue;
            }

            if (str[index] == 'Y')
            {

                result = result * scale;
                std::cout<<"result: "<<result<<std::endl;
                y_cord = result * pow(10, -coord_digits_y);
                std::cout<<"Y_cord: "<<y_cord<<std::endl;
                y_cord = y_cord + Shift_Y;
                std::cout<<"y_cord-shift: "<<y_cord<<std::endl;
                num = 0;
                i = 0;
                result = 0;
                index--;

                continue;

            }
            else if (str[index] == 'X')
            {

                result = result * scale;
                std::cout<<"result: "<<result<<std::endl;
                x_cord = result * pow(10, -coord_digits_x);
                std::cout<<"x_cord: "<<x_cord<<std::endl;
                x_cord = x_cord + Shift_X;
                std::cout<<"x_cord-shift: "<<x_cord<<std::endl;
                num = 0;
                i = 0;
                result = 0;
                index--;
                continue;
            }

            num = (double)(str[index] - '0') * pow(10, i);
            result = result + num;
            i++;
            index--;
        }
        index = index_end_Line;
    }
    else if (LZ_TZ_dec == 1)
    {
        int i = 0;
        float num = 0;
        float result = 0;
        if (str[index] == 'X')
        {
            bool minus_result = false;
            if (str[index + 1] == '-')
            {
                minus_result = true;
                index++;    // - symbol
            }

            int dot_index = index;
            for (i = 0; i < coord_digits_x; i++)
            {
                dot_index++;
            }
            index = dot_index;
            dot_index++;
            //integer part
            i = 0;
            while (str[index] != 'X' && str[index] != '-')
            {
                num = (float)(str[index] - '0') * pow(10, i);
                result = result + num;
                index--;
                i++;
            }
            //fraction part
            i = -1;
            while (str[dot_index] != '\n' && str[dot_index] != 'Y' && str[dot_index] != '\r'  && str[dot_index] != 'G')
            {
                num = (str[dot_index] - '0') * pow(10, i);
                result = result + num;
                dot_index++;
                i--;
            }
            if (minus_result == true)
            {
                result = result * -1;
            }
            result = result * scale;
            x_cord = result;
            x_cord = x_cord + Shift_X;
            index = dot_index;


        }



        if (str[index] == 'Y')
        {
            result = 0;
            i=0;
            num=0;
            bool minus_result = false;
            if (str[index + 1] == '-')
            {
                minus_result = true;
                index++;    // - symbol
            }

            int dot_index = index;
            for (i = 0; i < coord_digits_y; i++)
            {
                dot_index++;
            }
            index = dot_index;
            dot_index++;
            //integer part
            i = 0;
            while (str[index] != 'Y' && str[index] != '-')
            {
                num = (str[index] - '0') * pow(10, i);
                result = result + num;
                index--;
                i++;
            }
            //fraction part
            i = -1;
            while (str[dot_index] != '\n' && str[dot_index] != '\r'  && str[dot_index] != 'G')
            {
                num = (float)(str[dot_index] - '0') * pow(10, i);
                result = result + num;
                dot_index++;
                i--;
            }
            if (minus_result == true)
            {
                result = result * -1;
            }
            result = result * scale;
            y_cord=result;
            y_cord = y_cord + Shift_Y;

            index = dot_index;

        }


    }

    std::cout<<"drillx: "<<x_cord<<"Drilly: "<<y_cord<<std::endl;
}



int testFormatDrills(char* str)
{
    /*
        return 0: decimal format
        return 1: LZ format
        return 2: TZ format
    */
    int i=0;
    while(1)
    {
       if(str[i] == 'X')
       {
            while(str[i]!= 'Y' && str[i]!= '\n' && str[i]!= '\r' && str[i]!= 'G')
            {
                if(str[i] == 'X' && str[i+1] == '0')
                {
                    return 1;//LZ
                }
                else if(str[i] == 'X' && str[i+1] == '-' && str[i+2] == '0')
                {
                    return 1;//LZ
                }
                else if(str[i]=='.')
                {
                    return 0;//decimal point
                }
                i++;
            }
        }
        else if(str[i] == 'Y')
        {
            while(str[i]!= '\n' && str[i]!= '\r' && str[i]!= 'G')
            {
                if(str[i] == 'Y' && str[i+1] == '0')
                {
                    std::cout<<"type: LZ"<<std::endl;
                    return 1;//LZ
                }
                else if(str[i] == 'Y' && str[i+1] == '-' && str[i+2] == '0')
                {
                    std::cout<<"type: LZ"<<std::endl;
                    return 1;//LZ
                }
                else if(str[i] == '.')
                {
                    std::cout<<"type: decimal point"<<std::endl;
                    return 0;//decimal point
                }
                i++;
            }
        }
        else if(str[i]=='M' && str[i+1] == '3' && str[i] == '0')
        {
            std::cout<<"type: TZ"<<std::endl;
            return 2;//TZ
        }
        else
        {
            i++;
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
    //draw circule as circule
    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x, shap_Y), (current_shape->diameter) / 2 , (current_shape->diameter) / 2 ), pWicBrush);
    //Draw a circule as a line
    //pWicRenderTarget->DrawLine(D2D1::Point2F(shap_x-(current_shape->diameter) / 2, shap_Y), D2D1::Point2F(shap_x+current_shape->diameter / 2, shap_Y),pWicBrush ,current_shape->diameter/2);

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
        /*if(current_shape->diameter > 0.5)
        {
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(current_x, current_y), current_shape->diameter / 2-fraction_to_reduce, current_shape->diameter / 2-fraction_to_reduce), pBrush);
            //0.2 for sefty
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush ,current_shape->diameter - 0.4 );
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shape_x, shape_y), current_shape->diameter / 2 - fraction_to_reduce, current_shape->diameter / 2 - fraction_to_reduce), pBrush);
        }*/
        //else if(current_shape->diameter <= 0.2)
        {
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(current_x, current_y), current_shape->diameter / 2, current_shape->diameter / 2), pBrush);
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush ,current_shape->diameter);
            //draw a circule on the end of the line
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shape_x, shape_y), current_shape->diameter / 2 , current_shape->diameter / 2), pBrush);
        }
        /*else if(current_shape->diameter > 0.2 && current_shape->diameter <= 0.4)
        {
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(current_x, current_y), current_shape->diameter / 2 - fraction_to_reduce, current_shape->diameter / 2 - fraction_to_reduce), pBrush);
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush ,current_shape->diameter - fraction_to_reduce);
            //draw a circule on the end of the line
            pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shape_x, shape_y), current_shape->diameter / 2 - fraction_to_reduce , current_shape->diameter / 2 - fraction_to_reduce), pBrush);
        }*/

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

void draw_Obround(
    float shap_x,
    float shap_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush)
{
    pWicRenderTarget->FillRectangle(D2D1::RectF(
        shap_x - current_shape->x / 2 +(current_shape->y) / 2,
        shap_y - current_shape->y / 2  ,
        shap_x + current_shape->x / 2 -(current_shape->y) / 2,
        shap_y + current_shape->y / 2 ),
        pWicBrush);

    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x - current_shape->x / 2+(current_shape->y) / 2  , shap_y), (current_shape->y) / 2 , (current_shape->y) / 2 ), pWicBrush);
    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x + current_shape->x / 2 -(current_shape->y) / 2 , shap_y), (current_shape->y) / 2 , (current_shape->y) / 2 ), pWicBrush);



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
    pWicRenderTarget->DrawGeometry(m_pPathGeometry, pWicBrush ,0.0001F);
    pWicRenderTarget->FillGeometry(m_pPathGeometry, pWicBrush);
    //SafeRelease(&pSink);

    //current_x = shape_x;
    //current_y = shape_y;

}


void begin_polygon(
    float shape_x,
    float shape_y,
    ID2D1Factory* pFactory,
    ID2D1GeometrySink* &pSink,
    ID2D1PathGeometry* &m_pPathGeometry
    )
{
    // Write to the path geometry using the geometry sink.
    pFactory_->CreatePathGeometry(&m_pPathGeometry);
    m_pPathGeometry->Open(&pSink);
    pSink->BeginFigure(
    D2D1::Point2F(shape_x , shape_y),
    D2D1_FIGURE_BEGIN_FILLED
    );
}

void draw_line_polygon(
    float shape_x,
    float shape_y,
    ID2D1GeometrySink* &pSink
    )
{
       std::cout<<"AddLine_1"<<std::endl;
       pSink->AddLine(D2D1::Point2F(shape_x, shape_y));
       std::cout<<"AddLine_1"<<std::endl;
}

void end_polygon(
    ID2D1GeometrySink* &pSink,
    ID2D1PathGeometry* &m_pPathGeometry,
    bool LPC_LPD,
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush
    )
{
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();
    if(LPC_LPD == true)
    {
        pWicRenderTarget->DrawGeometry(m_pPathGeometry, pWicBrush, 0.1F);
    }
    else if(LPC_LPD == false)
    {
        pWicRenderTarget->FillGeometry(m_pPathGeometry, pWicBrush);
    }

    pSink->Release();
    m_pPathGeometry->Release();

    //SafeRelease(&pSink);

    //current_x = shape_x;
    //current_y = shape_y;
}




ID2D1Bitmap* DrawBitmapOnWindow(
    HWND handeller_to_window,
    ID2D1Factory* pFactory,
    IWICBitmap* pWicBitmap_,
    IWICImagingFactory* wicFactory,
    bool flip
)
{
    //ID2D1HwndRenderTarget* pRT = NULL;
    if(pWicBitmap_== NULL)
        return NULL;

    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U window_size = D2D1::SizeU(rc.right, rc.bottom);
    //D2D1_SIZE_U window_size = D2D1::SizeU(w,h);

    //if (pRT == NULL)
    //{
        pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(handeller_to_window, window_size),
            &pRT);
     //}

    //convert WicBitmap to D2dBitmap
    IWICFormatConverter* pConverter = NULL;
    wicFactory->CreateFormatConverter(&pConverter);

    if(flip == true)
    {
        if(pBitmap_main == pBitmap_)
        {
            if(flip_or_not == false)
            {
                pConverter->Initialize(
                pBitmap_flip,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = true;
            }
            else
            {
                pConverter->Initialize(
                pBitmap_,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = false;
            }
        }
        else if(pBitmap_main == pBitmap_2)
        {
            if(flip_or_not == false)
            {

                pConverter->Initialize(
                pBitmap_2_flip,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = true;
            }
            else
            {
                pConverter->Initialize(
                pBitmap_2,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = false;
            }
        }
        else if(pBitmap_main == pBitmap_3)
        {
            if(flip_or_not == false)
            {
                pConverter->Initialize(
                pBitmap_3_flip,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = true;
            }
            else
            {
                pConverter->Initialize(
                pBitmap_3,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = false;
            }
        }
        else if(pBitmap_main == pBitmap_4)
        {
            if(flip_or_not == false)
            {
                std::cout<<"flip_or_not false"<<std::endl;
                pConverter->Initialize(
                pBitmap_4_flip,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = true;
            }
            else
            {
                std::cout<<"flip_or_not true"<<std::endl;
                pConverter->Initialize(
                pBitmap_4,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
                flip_or_not = false;
            }
        }
    }
    else
    {
        pConverter->Initialize(
        pWicBitmap_,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.f,
        WICBitmapPaletteTypeMedianCut
        );
        flip_or_not = false;
    }




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


    ///draw in the original size
    if(/*pWicBitmap_ == pBitmap_4*/pBitmap_main == pBitmap_4 )//MMG
    {
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
        pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
        upperLeftCorner.x,
        upperLeftCorner.y,
        upperLeftCorner.x + w_MMG,
        upperLeftCorner.y + h_MMG)
        );
        //std::cout<<"draw not flipped"<<std::endl;
        MMG_or_Maxcupper = true;
    }
    else     //max cupper
    {
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
        upperLeftCorner.x,
        upperLeftCorner.y,
        upperLeftCorner.x + w,
        upperLeftCorner.y + h)
        );

        MMG_or_Maxcupper = false;
    }

    pBitmap_main = pWicBitmap_;

    if(/*pWicBitmap_ == pBitmap_4*/pBitmap_main == pBitmap_4 )//MMG
    {
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
        pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
        upperLeftCorner.x,
        upperLeftCorner.y,
        upperLeftCorner.x + w_MMG,
        upperLeftCorner.y + h_MMG)
        );
        //std::cout<<"draw not flipped"<<std::endl;
        MMG_or_Maxcupper = true;
    }
    else     //max cupper
    {
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
        upperLeftCorner.x,
        upperLeftCorner.y,
        upperLeftCorner.x + w,
        upperLeftCorner.y + h)
        );

        MMG_or_Maxcupper = false;
    }

    pRT->EndDraw();

    pBitmap_main = pWicBitmap_;

    return D2DpBitmap;
}


BYTE* lockMeoryBitmap(IWICBitmap* pWicBitmap, UINT puiWidth, UINT puiHeight, UINT& cb_Stride, UINT& cb_BufferSize )
{
    IWICBitmapLock* pLock =NULL;
    WICRect rcLock = { 0,0, puiWidth, puiHeight };
    pWicBitmap->Lock(&rcLock,WICBitmapLockWrite && WICBitmapLockRead, &pLock);


    BYTE* pv = NULL;

    pLock->GetStride(&cb_Stride);

    pLock->GetDataPointer(&cb_BufferSize, &pv);


    return pv;
}

//get the borders from the profile gerber
void GetWidthHeight(
    HWND handeller_to_window,
    char* str,
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

    float MaxX = -999999999999;
    float MinX = 999999999999;
    float MaxY = -999999999999;
    float MinY = 999999999999;
    float x;
    float y;

    i = 0;
    while (1)
    {
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            break;
        }
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            string_to_float(str, i, coord_digits_x, coord_digits_y, x, y, scale, false);
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
            string_to_float(str, i, coord_digits_x, coord_digits_y, x, y, scale);
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

    std::cout<<"puiWidth"<<puiWidth<<std::endl;
    std::cout<<"puiHeight"<<puiHeight<<std::endl;
    std::cout<<"shift_x: "<<Shift_X<<std::endl;
    std::cout<<"Shift_Y: "<<Shift_Y<<std::endl;

}



//DrawDrills
void DrawDrills_gerber(
    BYTE* pv,
    UINT cb_stride,
    char* str,
    int coord_digits_x,
    int coord_digits_y,
    float Shift_X,
    float Shift_Y,
    float scale

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

    float DrillCenter_X=0;
    float DrillCenter_Y=0;
    i = 0;
    while (1)
    {
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
        }
        else if ((str[i] == 'D') && (str[i + 1] == '0') && (str[i + 2] == '3'))
        {
            string_to_float(str, i, coord_digits_x, coord_digits_y, DrillCenter_X, DrillCenter_Y, scale,Shift_X,Shift_Y);
            if(DrillCenter_X > w || DrillCenter_Y > h)
            {
                i = i + 3;
                continue;
            }
            set_pixel(pv, cb_stride, (int)(DrillCenter_X), (int)(DrillCenter_Y), 0, 0, 0, 255);
            i = i + 3;
        }
        else { i++; }
    }


}

//DrawDrills
void DrawDrills_xln(
    BYTE* pv,
    UINT cb_stride,
    char* str,
    int coord_digits_x,
    int coord_digits_y,
    float Shift_X,
    float Shift_Y,
    float scale
)
{
    if(str == NULL)
    {
        std::cout<<"No drill file"<<std::endl;
        return;
    }
    std::cout<<"start drill file"<<std::endl;
    //defult 000.000
    coord_digits_x = 3;
    coord_digits_y =3;

    /*
        return 0: decimal format
        return 1: LZ format
        return 2: TZ format
    */
    int LZ_TZ_dec = 0;

    LZ_TZ_dec = testFormatDrills(str);

    int i = 0;
    bool comment_flag = false;
    while (1)
    {
        if ((str[i] == 'M' && str[i + 1] == '3' && str[i + 2] == '0') /*||
            (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '0') ||
            (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '6') ||
            (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '9') ||
            (str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '4')*/)
        {
            break;
        }
        //start of comment
        else if (str[i] == ';')
        {
            while(str[i] != '\n')
            {
                i++;
            }
        }
        else if (str[i] == 'L' && str[i + 1] == 'Z')
        {
            LZ_TZ_dec = 1;
            std::cout<<"LZ_TZ_dec = LZ"<<std::endl;
            i=i+2;
        }
        else if (str[i] == 'T' && str[i + 1] == 'Z')
        {
            LZ_TZ_dec = 2;
            std::cout<<"LZ_TZ_dec = TZ"<<std::endl;
            i=i+2;
        }
        //0000.00
        else if (str[i] == '0' && str[i + 1] == '0' && str[i + 2] == '0' && str[i + 3] == '0' && str[i + 4] == '.' && str[i + 5] == '0' && str[i + 6] == '0')
        {
            std::cout<<"0000.00"<<std::endl;
            if (LZ_TZ_dec == 1)
            {
                coord_digits_x = 4;
                coord_digits_y = 4;
            }
            else if (LZ_TZ_dec == 2)
            {
                coord_digits_x = 2;
                coord_digits_y = 2;
            }
            i = i+7;
        }
        //000.000
        else if (str[i] == '0' && str[i + 1] == '0' && str[i + 2] == '0' && str[i + 3] == '.' && str[i + 4] == '0' && str[i + 5] == '0' && str[i + 6] == '0')
        {
            std::cout<<"000.000"<<std::endl;
            if (LZ_TZ_dec == 1)
            {
                coord_digits_x = 3;
                coord_digits_y = 3;
            }
            else if (LZ_TZ_dec == 2)
            {
                coord_digits_x = 3;
                coord_digits_y = 3;
            }
            i = i+7;
        }
        //000.00
        else if(str[i] == '0' && str[i + 1] == '0' && str[i+2] == '0' && str[i + 3] == '.' && str[i + 4] == '0' && str[i + 5] == '0')
        {
            std::cout<<"000.00"<<std::endl;
            if (LZ_TZ_dec == 1)
            {
                coord_digits_x = 3;
                coord_digits_y = 3;
            }
            else if (LZ_TZ_dec == 2)
            {
                coord_digits_x = 2;
                coord_digits_y = 2;
            }
            i=i+6;
        }
        else
        {
            i++;
        }
    }

    std::cout<<"LZ_TZ_dec: "<<LZ_TZ_dec<<std::endl;
    float DrillCenter_X=0;
    float DrillCenter_Y=0;
    i = 0;
    comment_flag = false;

    while (1)
    {
        //if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        if((str[i] == 'M'&& str[i+1] == '3'&& str[i+2] == '0')/* ||
           (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '0') ||
           (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '6') ||
           (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '9') ||
           (str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '4')*/)
        {
            //pWicRenderTarget->EndDraw();
            break;
        }
        //start of comment
        else if (str[i] == ';')
        {
            while(str[i] != '\n')
            {
                i++;
            }
        }
        else if(str[i] == 'X' || str[i] == 'Y')
        {
            string_to_float_dill(str, i, coord_digits_x, coord_digits_y, LZ_TZ_dec, DrillCenter_X, DrillCenter_Y, scale, Shift_X, Shift_Y);
            if(DrillCenter_X > w || DrillCenter_Y > h)
            {
                //error ocures dont draw this drill
            }
            else
            {
                std::cout<<"DrillCenter_X :"<<DrillCenter_X<<", DrillCenter_Y= "<<DrillCenter_Y<<std::endl;
                set_pixel(pv, cb_stride, (int)(DrillCenter_X), (int)(DrillCenter_Y), 0, 0, 0, 255);
            }

            while(str[i]!='\n')
            {
                i++;
            }
        }
        else { i++;}
    }


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
    pWicRenderTarget->DrawLine(D2D1::Point2F(0, 0), D2D1::Point2F(puiWidth, 0), pWicBrush, 1.0f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(puiWidth, 0), D2D1::Point2F(puiWidth, puiHeight), pWicBrush, 1.0f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(puiWidth, puiHeight), D2D1::Point2F(0, puiHeight), pWicBrush, 1.0f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(0, puiHeight), D2D1::Point2F(0, 0), pWicBrush, 1.0f);
    pWicRenderTarget->EndDraw();


}

void DrawBordersAtEnd(
    BYTE* pv,
    UINT cb_stride,
    UINT puiWidth,
    UINT puiHeight
)
{
    for(int i=0; i<puiHeight-2; i++)
    {
        set_pixel(pv,cb_stride,0,i,0,0,0,255);
        set_pixel(pv,cb_stride,puiWidth-1,i,0,0,0,255);
    }
    for(int i=0; i<puiWidth-2; i++)
    {
        set_pixel(pv,cb_stride,i,0,0,0,0,255);
        set_pixel(pv,cb_stride,i,puiHeight-1,0,0,0,255);
    }

    //convert corner to white
    set_pixel(pv,cb_stride,1,1,255,255,255,255);
    set_pixel(pv,cb_stride,puiWidth-2,1,255,255,255,255);
    set_pixel(pv,cb_stride,1,puiHeight-2,255,255,255,255);
    set_pixel(pv,cb_stride,puiWidth-2,puiHeight-2,255,255,255,255);

}


void initializeDrawing(HWND handeller_to_window)
{
    if (wicFactory_ == NULL)
    {
        CoCreateInstance(
           CLSID_WICImagingFactory,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_IWICImagingFactory,
           (LPVOID*)&wicFactory_);
    }

    //create factorty
    if (pFactory_ == NULL)
    {
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
    }
    if(pRT ==NULL)
    {
        RECT rc;
        GetClientRect(handeller_to_window, &rc);
        D2D1_SIZE_U window_size = D2D1::SizeU(rc.right, rc.bottom);
        pFactory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(handeller_to_window, window_size),
                &pRT);
    }

    /*if (pWicRenderTarget_ == NULL)
    {
        pFactory_->CreateWicBitmapRenderTarget(
            pBitmap_,
            D2D1::RenderTargetProperties(),
            &pWicRenderTarget_);
    }*/


    //create brush
    /*if (pWicBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
        pWicRenderTarget_->CreateSolidColorBrush(color, &pWicBrush);
    }*/

    //create brush
    if (pWindowBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(255, 0, 0);
        pRT->CreateSolidColorBrush(color, &pWindowBrush);
    }

}


ID2D1Bitmap* DrawGerberOnBitmab(
    HWND handeller_to_window,
    char* str,
    char* profileStr,
    char* DrillsStr,
    float scale ,//each pixel have scale mm
    /*ID2D1Factory* pFactory_,
    ID2D1RenderTarget* pWicRenderTarget_,
    IWICImagingFactory* wicFactory_,
    IWICBitmap* pBitmap_,
    ID2D1SolidColorBrush* pWicBrush,*/
    bool xln_grb /*= true*/
    )
{
    //scale=10;
    scale = 1/scale;
    // Create a WIC Fact
    if(profileStr ==NULL || str==NULL)
    {
        return NULL;
    }

    pBitmap_= NULL;
    pBitmap_2= NULL;
    pBitmap_3= NULL;
    //pBitmap_4= NULL;
    pBitmap_flip= NULL;
    pBitmap_2_flip= NULL;
    pBitmap_3_flip= NULL;
    //pBitmap_4_flip= NULL;






    if (wicFactory_ == NULL)
    {
        CoCreateInstance(
           CLSID_WICImagingFactory,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_IWICImagingFactory,
           (LPVOID*)&wicFactory_);
    }

    UINT puiWidth=0;
    UINT puiHeight=0;
    //assume 10^-6 mm mode
    int coord_digits_x = 4;
    int coord_digits_y = 4;

    float Shift_X=0;
    float Shift_Y = 0;
    GetWidthHeight(handeller_to_window, profileStr, coord_digits_x, coord_digits_y, scale, puiWidth, puiHeight, Shift_X, Shift_Y);

    w = puiWidth;
    h = puiHeight;

    /*if (pBitmap_ == NULL)
    {*/
       // WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_);
    //}

    //create factorty
    if (pFactory_ == NULL)
    {
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
    }
    /*if (pWicRenderTarget_ == NULL)
    {*/
        pFactory_->CreateWicBitmapRenderTarget(
            pBitmap_,
            D2D1::RenderTargetProperties(),
            &pWicRenderTarget_);
    //}


    //create brush
    if (pWicBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
        pWicRenderTarget_->CreateSolidColorBrush(color, &pWicBrush);
    }
    //End intialization
    //////////////////////////////////////////////////////////////////////////
    DrawBorders(pWicRenderTarget_, pWicBrush, puiWidth, puiHeight);
    const D2D1_COLOR_F color = D2D1::ColorF(255, 0, 0);
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
    pWicRenderTarget_->BeginDraw();


    pWicRenderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    std::cout<<"start draw the gerber"<<std::endl;
    //start tracing the string
    int i = 0;
    float contour_x=0;
    float contour_y=0;
    float tempX =0;
    float tempY =0;
    bool start_contour = false;
    ID2D1GeometrySink* pSink =NULL;
    ID2D1PathGeometry* m_pPathGeometry ;
    bool LPC_LPD = true;
    while (1)
    {
        //end of file
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            pWicRenderTarget_->EndDraw();
            break;
        }
        if(str[i] == 'L' && str[i+1] == 'P' && str[i+2] == 'C' && str[i+3] == '*')
        {
            LPC_LPD = true;
            i = i+3;
        }
        else if(str[i] == 'L' && str[i+1] == 'P' && str[i+2] == 'D' && str[i+3] == '*')
        {
            LPC_LPD = false;
            i = i+3;
        }
        //G36*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '6' && str[i + 3] == '*')
        {
            start_contour =true;
            while(str[i] != 'D' && str[i + 1] != '0' && str[i + 2] != '2')
            {
                i++;
            }
            string_to_float(str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            begin_polygon(current_x,current_y,pFactory_,pSink, m_pPathGeometry);
            std::cout<<"G36 D02 "<<"current_x "<<current_x<<" current_y "<<current_y<<std::endl;
            contour_x = current_x;
            contour_y = current_y;
            //contour_x = current_x;
            //contour_y = current_y;
            i=i+3;
        }
        //G01*
        /*else if(str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '1' && str[i + 3] == '*')
        {
            if(start_contour == true)
            {
                begin_polygon(current_x,current_y,pFactory_,pSink, m_pPathGeometry);
                std::cout<<"current_x"<<current_x<<std::endl;
                std::cout<<"current_y"<<current_y<<std::endl;
                contour_x = current_x;
                contour_y = current_y;
                //start_contour = false;
            }
            i=i+3;

        }*/
        //G37*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '7' && str[i + 3] == '*')
        {
            tempX = current_x;
            tempY = current_y;
            std::cout<<"end_polygon_1"<<std::endl;
            pSink->AddLine(D2D1::Point2F(contour_x, contour_y));
            end_polygon(pSink,m_pPathGeometry,LPC_LPD,pWicRenderTarget_,pWicBrush);
            //std::cout<<"end_polygon_2"<<std::endl;
            //pSink->Release();
            //pSink = NULL;
            //m_pPathGeometry->Release();
            //m_pPathGeometry = NULL;
            //draw_line(handeller_to_window, current_x, current_y, contour_x,contour_y,pWicRenderTarget_, current_shape, pWicBrush);
            //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(contour_x, contour_y), pWicBrush ,1.0f);
            current_x = tempX;
            current_y = tempY;
            //std::cout<<"current_x"<<current_x<<std::endl;
            //std::cout<<"current_y"<<current_y<<std::endl;
            std::cout<<"G37"<<std::endl;
            start_contour = false;
            i=i+3;

        }
        //draw line
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            string_to_float(str, i,coord_digits_x,coord_digits_y, shape_x, shape_y, scale,Shift_X,Shift_Y);
            if(start_contour == true)
            {
                //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pWicBrush ,1.0f);
                std::cout<<"draw_line_polygon_1"<<std::endl;
                //std::cout<<"shape_x: "<<shape_x<<std::endl;
                //std::cout<<"shape_y: "<<shape_y<<std::endl;
                //draw_line_polygon(shape_x,shape_y,pSink);
                pSink->AddLine(D2D1::Point2F(shape_x, shape_y));
                std::cout<<"draw_line_polygon_2"<<std::endl;
                current_x = shape_x;
                current_y = shape_y;
            }
            else
                {draw_line(handeller_to_window, current_x, current_y, shape_x,shape_y,pWicRenderTarget_, current_shape, pWicBrush);}
            i = i + 3;
        }
        //only move to a certain point
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            string_to_float(str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            i = i + 3;
        }
        //creart flash (replicating) the current aperture
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            //cout<<"D03"<<endl;
            string_to_float(str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
            //for Circle
            if (current_shape->type == 1)
            {
                Draw_circle(shape_x, shape_y, pWicRenderTarget_, current_shape, pWicBrush);
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            //for Rectangle
            else if (current_shape->type == 2)
            {
                //string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                draw_rectangle(shape_x, shape_y, pWicRenderTarget_,current_shape,pWicBrush);
            }
            //for Obround
            else if (current_shape->type == 3)
            {
                //string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                //Draw_circle(shape_x, shape_y, pWicRenderTarget, current_shape, pWicBrush);
                draw_Obround(shape_x, shape_y, pWicRenderTarget_,current_shape,pWicBrush);
            }
            //for Polygon
            else if (current_shape->type == 4)
            {
                draw_polygon(shape_x, shape_y, pFactory_, pWicRenderTarget_, pWicBrush, current_shape);
                // draw polygon as circle
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            current_x = shape_x;
            current_y = shape_y;
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

                float diameter = 0/*x*/;
                //let the diameter has one digit for x and one digit for y
                i = i + 11;

                all_shapes.add_aperture(2, num_shape, diameter, x, y, 0, hole_diameter);
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

            float diameter = x;

                //let the diameter has one digit for x and one digit for y
                i = i + 11;
                all_shapes.add_aperture(3, num_shape, diameter, x, y, 0, hole_diameter);
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

    std::cout<<"finish gerber file"<<std::endl;

    //DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_,wicFactory_);
    //MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);

    //cb_stride is sent by referance, so the initalization of it is in the fancution
    UINT cb_stride = 0;
    UINT cb_BufferSize = 0;

    BYTE* pv_maxcupper_with_drills = lockMeoryBitmap(pBitmap_, puiWidth, puiHeight, cb_stride,cb_BufferSize);


    //Excute Max cupper algorithm then draw it
    std::cout<<"start maxcupper"<<std::endl;
    All_algorithm_max_cupper(pv_maxcupper_with_drills, cb_stride, puiWidth, puiHeight);
    //DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    //MessageBoxW(handeller_to_window, L"start drills", L"start drills", MB_OKCANCEL);

    std::cout<<"start drills"<<std::endl;
    //draw drills with set_pixel()
    if(xln_grb == true)
    {
        DrawDrills_xln(pv_maxcupper_with_drills,cb_stride,DrillsStr, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale);
    }
    else if(xln_grb == false)
    {
        DrawDrills_gerber(pv_maxcupper_with_drills,cb_stride,DrillsStr, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale);
    }
    //DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    //MessageBoxW(handeller_to_window, L"end drills", L"end drills", MB_OKCANCEL);
    std::cout<<"End drill file"<<std::endl;

    std::cout<<"Draw Borders"<<std::endl;
    //borders black
    DrawBordersAtEnd(pv_maxcupper_with_drills,cb_stride,puiWidth,puiHeight);


    //MessageBoxW(handeller_to_window, L"start Bitmap_gerber and maxcupper", L"start Bitmap_gerber and maxcupper", MB_OKCANCEL);
    std::cout<<"start gerber on maxcupper"<<std::endl;
    //IWICBitmap* pBitmap_2=NULL;
    pWicRenderTarget_->Release();
    pWicRenderTarget_ = NULL;
    /*if (pBitmap_2 == NULL)
    {*/
        GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_2);
    //}
    pFactory_->CreateWicBitmapRenderTarget(
        pBitmap_2,
        D2D1::RenderTargetProperties(),
        &pWicRenderTarget_);

    ////////////
    current_x = 0;
    current_y =0;
    shape_x =0;
    shape_y =0;
    pWicRenderTarget_->BeginDraw();
    pWicBrush->SetColor(D2D1::ColorF(0, 0, 0,0));
    /*pWicRenderTarget_->FillRectangle(D2D1::RectF(
        0,
        0,
        puiWidth,
        puiHeight),
        pWicBrush);
    */
    pWicRenderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::Black));
    pWicBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    start_contour = false;
    i = 0;
    LPC_LPD = true;
    while (1)
    {
        //end of file
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            std::cout<<"M02"<<std::endl;
            pWicRenderTarget_->EndDraw();
            break;
        }
        if(str[i] == 'L' && str[i+1] == 'P' && str[i+2] == 'C' && str[i+3] == '*')
        {
            LPC_LPD = true;
            i = i+3;
        }
        else if(str[i] == 'L' && str[i+1] == 'P' && str[i+2] == 'D' && str[i+3] == '*')
        {
            LPC_LPD = false;
            i = i+3;
        }
        //G36*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '6' && str[i + 3] == '*')
        {
            start_contour =true;
            //contour_x = current_x;
            //contour_y = current_y;
            i=i+3;
            while(str[i] != 'D' && str[i + 1] != '0' && str[i + 2] != '2')
            {
                i++;
            }
            string_to_float(str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            begin_polygon(current_x,current_y,pFactory_,pSink, m_pPathGeometry);
            std::cout<<"G36 D02 "<<"current_x "<<current_x<<" current_y "<<current_y<<std::endl;
            contour_x = current_x;
            contour_y = current_y;
            //i = i + 3;

        }
        //G01*
        /*else if(str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '1' && str[i + 3] == '*')
        {
            if(start_contour == true)
            {
                begin_polygon(current_x,current_y,pFactory_,pSink, m_pPathGeometry);
                std::cout<<"current_x"<<current_x<<std::endl;
                std::cout<<"current_y"<<current_y<<std::endl;
                contour_x = current_x;
                contour_y = current_y;
                //start_contour = false;

            }
            i=i+3;

        }*/
        //G37*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '7' && str[i + 3] == '*')
        {
            tempX = current_x;
            tempY = current_y;
            std::cout<<"end_polygon_1"<<std::endl;
            pSink->AddLine(D2D1::Point2F(contour_x, contour_y));
            end_polygon(pSink,m_pPathGeometry,LPC_LPD,pWicRenderTarget_,pWicBrush);
            //std::cout<<"end_polygon_2"<<std::endl;
            //pSink->Release();
            //pSink = NULL;
            //m_pPathGeometry->Release();
            //m_pPathGeometry = NULL;
            //draw_line(handeller_to_window, current_x, current_y, contour_x,contour_y,pWicRenderTarget_, current_shape, pWicBrush);
            //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(contour_x, contour_y), pWicBrush ,1.0f);
            current_x = tempX;
            current_y = tempY;
            //std::cout<<"current_x"<<current_x<<std::endl;
            //std::cout<<"current_y"<<current_y<<std::endl;
            std::cout<<"G37"<<std::endl;
            start_contour = false;
            i=i+3;

        }
        //draw line
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            std::cout<<"D01"<<std::endl;
            string_to_float(str, i,coord_digits_x,coord_digits_y, shape_x, shape_y, scale,Shift_X,Shift_Y);
            if(start_contour == true)
            {
                //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pWicBrush ,1.0f);
                std::cout<<"draw_line_polygon_1"<<std::endl;
                //std::cout<<"shape_x: "<<shape_x<<std::endl;
                //std::cout<<"shape_y: "<<shape_y<<std::endl;
                //draw_line_polygon(shape_x,shape_y,pSink);
                pSink->AddLine(D2D1::Point2F(shape_x, shape_y));
                std::cout<<"draw_line_polygon_2"<<std::endl;
                current_x = shape_x;
                current_y = shape_y;
            }
            else
                {draw_line(handeller_to_window, current_x, current_y, shape_x,shape_y,pWicRenderTarget_, current_shape, pWicBrush);}
            i = i + 3;
        }
        //only move to a certain point
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            std::cout<<"D02"<<std::endl;
            string_to_float(str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            i = i + 3;
        }
        //creart flash (replicating) the current aperture
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            std::cout<<"D03"<<std::endl;
            string_to_float(str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
            //for Circle
            if (current_shape->type == 1)
            {
                Draw_circle(shape_x, shape_y, pWicRenderTarget_, current_shape, pWicBrush);
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            //for Rectangle
            else if (current_shape->type == 2)
            {
                //string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                draw_rectangle(shape_x, shape_y, pWicRenderTarget_,current_shape,pWicBrush);
            }
            //for Obround
            else if (current_shape->type == 3)
            {
                //string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                //Draw_circle(shape_x, shape_y, pWicRenderTarget, current_shape, pWicBrush);
                draw_Obround(shape_x, shape_y, pWicRenderTarget_,current_shape,pWicBrush);
            }
            //for Polygon
            else if (current_shape->type == 4)
            {
                draw_polygon(shape_x, shape_y, pFactory_, pWicRenderTarget_, pWicBrush, current_shape);
                // draw polygon as circle
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            current_x = shape_x;
            current_y = shape_y;
            i = i + 3;
        }
        else if (str[i] == 'D' && ((int)(str[i + 1])) >= 1 && str[i + 3] == '*')
        {
            //all shapes start from D10 to D99
            int num_shape = (int)(str[i + 1] - '0') * 10 + (int)(str[i + 2] - '0') * 1;

            current_shape = all_shapes.get_aperture(num_shape);
            i = i + 3;
        }
        else
        {
            i++;
        }
    }

    std::cout<<"finish"<<std::endl;
    BYTE* pv_gerber = lockMeoryBitmap(pBitmap_2, puiWidth, puiHeight, cb_stride,cb_BufferSize);

    wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_3);
    BYTE* pv_total = lockMeoryBitmap(pBitmap_3, puiWidth, puiHeight, cb_stride,cb_BufferSize);
    //put maxcupper in pv_total(pBitmap_3)
    for(int k=0; k<cb_BufferSize;k++)
    {
        *(pv_total + k) = *(pv_maxcupper_with_drills + k);
    }
    //put gerber in pv_total(pBitmap_3)
    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
            UINT BB = 0;
            UINT GG = 0;
            UINT RR = 0;
            UINT A = 0;
            get_pixel(pv_gerber, cb_stride, j, i, BB, GG, RR, A);

            if (BB == 0 && RR == 255 && GG == 0)
            {
                set_pixel(pv_total, cb_stride, j, i, 0, 0, 255,255);
            }
        }
    }

    DrawDrills_xln(pv_total,cb_stride,DrillsStr, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale);

    std::cout<<"start flip"<<std::endl;

    /*if (pBitmap_flip == NULL)
    {*/
      //  WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_flip);
    //}
    /*if (pBitmap_2_flip == NULL)
    {*/
       // WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_2_flip);
    //}
    /*if (pBitmap_3_flip == NULL)
    {*/
      //  WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_3_flip);
    //}
    /*if (pBitmap_4_flip == NULL)
    {*/
       // WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        //wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_4_flip);
    //}
std::cout<<"start flip"<<std::endl;
    BYTE* pv_filp = lockMeoryBitmap(pBitmap_flip, puiWidth, puiHeight, cb_stride,cb_BufferSize);
    BYTE* pv_2_filp = lockMeoryBitmap(pBitmap_2_flip, puiWidth, puiHeight, cb_stride,cb_BufferSize);
    BYTE* pv_3_filp = lockMeoryBitmap(pBitmap_3_flip, puiWidth, puiHeight, cb_stride,cb_BufferSize);
   // BYTE* pv_4_filp = lockMeoryBitmap(pBitmap_4_flip, puiWidth, puiHeight, cb_stride,cb_BufferSize);
    std::cout<<"start flip"<<std::endl;
    for(int k=0; k<cb_BufferSize;k++)
    {
        *(pv_filp + k) = *(pv_maxcupper_with_drills + k);
        *(pv_2_filp + k) = *(pv_gerber + k);
        *(pv_3_filp + k) = *(pv_total + k);
        //*(pv_4_filp + k) = *(pv_maxcupper_with_drills + k);
    }



    flipBitmap(pv_maxcupper_with_drills,pv_gerber,pv_total,cb_BufferSize,cb_stride,puiHeight,puiWidth);

    //pBitmap_main = pBitmap_;

    //DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_3,wicFactory_);
    //MessageBoxW(handeller_to_window, L"Done", L"Done", MB_OKCANCEL);

    std::cout<<"finish gerber on maxcupper"<<std::endl;
    //initalize the bitmap object
    //bitmapObject.SetSpecifications(pv_maxcupper_with_drills, cb_BufferSize, cb_stride, puiWidth, puiHeight);
    bitmapObject.SetSpecifications(pv_total, cb_BufferSize, cb_stride, puiWidth, puiHeight);
    ////

    //Export the Bitmap
    IWICStream* piStream = NULL;
    IWICBitmapEncoder* piEncoder = NULL;
    IWICBitmapFrameEncode* piBitmapFrame = NULL;
    wicFactory_->CreateStream(&piStream);

    // Section to set the output path
    WCHAR szTempPath[MAX_PATH];
    wcscpy_s(szTempPath, szGerberPath);
    PathRemoveFileSpecW(szTempPath);
    PathAppendW(szTempPath, L"MaxCupper.bmp");

    piStream->InitializeFromFilename(szTempPath, GENERIC_WRITE);
    wicFactory_->CreateEncoder(GUID_ContainerFormatBmp, NULL, &piEncoder);
    piEncoder->Initialize(piStream, WICBitmapEncoderNoCache);
    piEncoder->CreateNewFrame(&piBitmapFrame, NULL);
    piBitmapFrame->Initialize(NULL);

    WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
    piBitmapFrame->SetSize(puiWidth, puiHeight);
    piBitmapFrame->SetPixelFormat(&formatGUID);
    //save the maxcupper bmp
    //piBitmapFrame->WriteSource(pBitmap_, NULL);
    piBitmapFrame->WriteSource(pBitmap_3, NULL);

    piBitmapFrame->Commit();
    piEncoder->Commit();

    //MessageBoxW(handeller_to_window, L"Done Writting", L"Done",MB_OK);

    return /*D2dbitmap*/NULL;
}




//Without maxcupper

ID2D1Bitmap* DrawGerberOnBitmabWithoutMaxCupper(
    HWND handeller_to_window,
    char* str,
    char* profileStr,
    char* DrillsStr,
    float scale ,//each pixel have scale mm
    /*ID2D1Factory* pFactory_,
    ID2D1RenderTarget* pWicRenderTarget_,
    IWICImagingFactory* wicFactory_,
    IWICBitmap* pBitmap_,
    ID2D1SolidColorBrush* pWicBrush,*/
    bool xln_grb /*= true*/
    )
{
    //scale=10;
    scale = 1/scale;
    // Create a WIC Fact
    if(profileStr ==NULL || str==NULL)
    {
        return NULL;
    }

    pBitmap_= NULL;
    pBitmap_2= NULL;
    pBitmap_3= NULL;
    //pBitmap_4= NULL;
    pBitmap_flip= NULL;
    pBitmap_2_flip= NULL;
    pBitmap_3_flip= NULL;
    //pBitmap_4_flip= NULL;


    if (wicFactory_ == NULL)
    {
        CoCreateInstance(
           CLSID_WICImagingFactory,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_IWICImagingFactory,
           (LPVOID*)&wicFactory_);
    }

    UINT puiWidth=0;
    UINT puiHeight=0;
    //assume 10^-6 mm mode
    int coord_digits_x = 4;
    int coord_digits_y = 4;

    float Shift_X=0;
    float Shift_Y = 0;
    GetWidthHeight(handeller_to_window, profileStr, coord_digits_x, coord_digits_y, scale, puiWidth, puiHeight, Shift_X, Shift_Y);

    w = puiWidth;
    h = puiHeight;

    /*if (pBitmap_ == NULL)
    {*/
       // WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_);
    //}

    //create factorty
    if (pFactory_ == NULL)
    {
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
    }
    /*if (pWicRenderTarget_ == NULL)
    {*/
        pFactory_->CreateWicBitmapRenderTarget(
            pBitmap_,
            D2D1::RenderTargetProperties(),
            &pWicRenderTarget_);
    //}


    //create brush
    if (pWicBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
        pWicRenderTarget_->CreateSolidColorBrush(color, &pWicBrush);
    }
    //End intialization
    //////////////////////////////////////////////////////////////////////////
    DrawBorders(pWicRenderTarget_, pWicBrush, puiWidth, puiHeight);
    const D2D1_COLOR_F color = D2D1::ColorF(255, 0, 0);
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
    pWicRenderTarget_->BeginDraw();


    pWicRenderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::White));

    std::cout<<"start draw the gerber"<<std::endl;
    //start tracing the string
    int i = 0;
    float contour_x=0;
    float contour_y=0;
    float tempX =0;
    float tempY =0;
    bool start_contour = false;
    ID2D1GeometrySink* pSink =NULL;
    ID2D1PathGeometry* m_pPathGeometry ;
    bool LPC_LPD = true;
    while (1)
    {
        //end of file
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            pWicRenderTarget_->EndDraw();
            break;
        }
        if(str[i] == 'L' && str[i+1] == 'P' && str[i+2] == 'C' && str[i+3] == '*')
        {
            LPC_LPD = true;
            i = i+3;
        }
        else if(str[i] == 'L' && str[i+1] == 'P' && str[i+2] == 'D' && str[i+3] == '*')
        {
            LPC_LPD = false;
            i = i+3;
        }
        //G36*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '6' && str[i + 3] == '*')
        {
            start_contour =true;
            while(str[i] != 'D' && str[i + 1] != '0' && str[i + 2] != '2')
            {
                i++;
            }
            string_to_float(str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            begin_polygon(current_x,current_y,pFactory_,pSink, m_pPathGeometry);
            std::cout<<"G36 D02 "<<"current_x "<<current_x<<" current_y "<<current_y<<std::endl;
            contour_x = current_x;
            contour_y = current_y;
            //contour_x = current_x;
            //contour_y = current_y;
            i=i+3;
        }
        //G01*
        /*else if(str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '1' && str[i + 3] == '*')
        {
            if(start_contour == true)
            {
                begin_polygon(current_x,current_y,pFactory_,pSink, m_pPathGeometry);
                std::cout<<"current_x"<<current_x<<std::endl;
                std::cout<<"current_y"<<current_y<<std::endl;
                contour_x = current_x;
                contour_y = current_y;
                //start_contour = false;
            }
            i=i+3;

        }*/
        //G37*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '7' && str[i + 3] == '*')
        {
            tempX = current_x;
            tempY = current_y;
            std::cout<<"end_polygon_1"<<std::endl;
            pSink->AddLine(D2D1::Point2F(contour_x, contour_y));
            end_polygon(pSink,m_pPathGeometry,LPC_LPD,pWicRenderTarget_,pWicBrush);
            //std::cout<<"end_polygon_2"<<std::endl;
            //pSink->Release();
            //pSink = NULL;
            //m_pPathGeometry->Release();
            //m_pPathGeometry = NULL;
            //draw_line(handeller_to_window, current_x, current_y, contour_x,contour_y,pWicRenderTarget_, current_shape, pWicBrush);
            //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(contour_x, contour_y), pWicBrush ,1.0f);
            current_x = tempX;
            current_y = tempY;
            //std::cout<<"current_x"<<current_x<<std::endl;
            //std::cout<<"current_y"<<current_y<<std::endl;
            std::cout<<"G37"<<std::endl;
            start_contour = false;
            i=i+3;

        }
        //draw line
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
        {
            string_to_float(str, i,coord_digits_x,coord_digits_y, shape_x, shape_y, scale,Shift_X,Shift_Y);
            if(start_contour == true)
            {
                //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pWicBrush ,1.0f);
                std::cout<<"draw_line_polygon_1"<<std::endl;
                //std::cout<<"shape_x: "<<shape_x<<std::endl;
                //std::cout<<"shape_y: "<<shape_y<<std::endl;
                //draw_line_polygon(shape_x,shape_y,pSink);
                pSink->AddLine(D2D1::Point2F(shape_x, shape_y));
                std::cout<<"draw_line_polygon_2"<<std::endl;
                current_x = shape_x;
                current_y = shape_y;
            }
            else
                {draw_line(handeller_to_window, current_x, current_y, shape_x,shape_y,pWicRenderTarget_, current_shape, pWicBrush);}
            i = i + 3;
        }
        //only move to a certain point
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            string_to_float(str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            i = i + 3;
        }
        //creart flash (replicating) the current aperture
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            //cout<<"D03"<<endl;
            string_to_float(str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
            //for Circle
            if (current_shape->type == 1)
            {
                Draw_circle(shape_x, shape_y, pWicRenderTarget_, current_shape, pWicBrush);
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            //for Rectangle
            else if (current_shape->type == 2)
            {
                //string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                draw_rectangle(shape_x, shape_y, pWicRenderTarget_,current_shape,pWicBrush);
            }
            //for Obround
            else if (current_shape->type == 3)
            {
                //string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
                //Draw_circle(shape_x, shape_y, pWicRenderTarget, current_shape, pWicBrush);
                draw_Obround(shape_x, shape_y, pWicRenderTarget_,current_shape,pWicBrush);
            }
            //for Polygon
            else if (current_shape->type == 4)
            {
                draw_polygon(shape_x, shape_y, pFactory_, pWicRenderTarget_, pWicBrush, current_shape);
                // draw polygon as circle
                //draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
            }
            current_x = shape_x;
            current_y = shape_y;
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

                float diameter = 0/*x*/;
                //let the diameter has one digit for x and one digit for y
                i = i + 11;

                all_shapes.add_aperture(2, num_shape, diameter, x, y, 0, hole_diameter);
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

            float diameter = x;

                //let the diameter has one digit for x and one digit for y
                i = i + 11;
                all_shapes.add_aperture(3, num_shape, diameter, x, y, 0, hole_diameter);
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

    std::cout<<"finish gerber file"<<std::endl;

    //DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_,wicFactory_);
    //MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);

    //cb_stride is sent by referance, so the initalization of it is in the fancution
    UINT cb_stride = 0;
    UINT cb_BufferSize = 0;

    BYTE* pv_maxcupper_with_drills = lockMeoryBitmap(pBitmap_, puiWidth, puiHeight, cb_stride,cb_BufferSize);


    //Excute Max cupper algorithm then draw it
    std::cout<<"start maxcupper"<<std::endl;
    //All_algorithm_max_cupper(pv_maxcupper_with_drills, cb_stride, puiWidth, puiHeight);
    //DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    //MessageBoxW(handeller_to_window, L"start drills", L"start drills", MB_OKCANCEL);

    std::cout<<"start drills"<<std::endl;
    //draw drills with set_pixel()
    if(xln_grb == true)
    {
        DrawDrills_xln(pv_maxcupper_with_drills,cb_stride,DrillsStr, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale);
    }
    else if(xln_grb == false)
    {
        DrawDrills_gerber(pv_maxcupper_with_drills,cb_stride,DrillsStr, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale);
    }
    //DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    //MessageBoxW(handeller_to_window, L"end drills", L"end drills", MB_OKCANCEL);
    std::cout<<"End drill file"<<std::endl;

    std::cout<<"Draw Borders"<<std::endl;
    //borders black
    DrawBordersAtEnd(pv_maxcupper_with_drills,cb_stride,puiWidth,puiHeight);


    wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_2);
    BYTE* pv_2 = lockMeoryBitmap(pBitmap_2, puiWidth, puiHeight, cb_stride,cb_BufferSize);


    wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_2_flip);
    BYTE* pv_2_filp = lockMeoryBitmap(pBitmap_2_flip, puiWidth, puiHeight, cb_stride,cb_BufferSize);

    UINT BB = 0;
    UINT GG = 0;
    UINT RR = 0;
    UINT A = 0;
    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
             get_pixel(pv_maxcupper_with_drills, cb_stride, j, i, BB, GG, RR, A);
             set_pixel(pv_2_filp, cb_stride, j, i, BB, GG, RR,A);
        }
    }


    ///
    //add filp circuit
    //BYTE* pvTemp_ = new BYTE[cb_BufferSize];

    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
             get_pixel(pv_2_filp, cb_stride, j, i, BB, GG, RR, A);
             set_pixel(pv_2, cb_stride, j, puiHeight-i-1, BB, GG, RR,A);
        }
    }

    BlackAroundTracks(pv_maxcupper_with_drills, cb_stride, puiWidth, puiHeight);

    std::cout<<"start flip"<<std::endl;
    wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_flip);

    BYTE* pv_filp = lockMeoryBitmap(pBitmap_flip, puiWidth, puiHeight, cb_stride,cb_BufferSize);
    ///
    //add filp circuit
    //BYTE* pvTemp_ = new BYTE[cb_BufferSize];

    for (UINT i = 0; i < puiHeight; i++)
    {
        for (UINT j = 0; j < puiWidth; j++)
        {
             get_pixel(pv_maxcupper_with_drills, cb_stride, j, i, BB, GG, RR, A);
             set_pixel(pv_filp, cb_stride, j, puiHeight-i-1, BB, GG, RR,A);
        }
    }

    BYTE* temp =new BYTE[cb_BufferSize];
    for(int k =0;k<cb_BufferSize;k++)
    {
        *(temp + k) = *(pv_maxcupper_with_drills + k);
    }
    for(int k =0;k<cb_BufferSize;k++)
    {
        *(pv_maxcupper_with_drills + k) = *(pv_filp + k);
    }
    for(int k =0;k<cb_BufferSize;k++)
    {
        *(pv_filp + k) = *(temp + k);
    }



    ///
    //initalize the bitmap object
    //bitmapObject.SetSpecifications(pv_maxcupper_with_drills, cb_BufferSize, cb_stride, puiWidth, puiHeight);
    bitmapObject.SetSpecifications(pv_maxcupper_with_drills, cb_BufferSize, cb_stride, puiWidth, puiHeight);
    ////

    //Export the Bitmap
    IWICStream* piStream = NULL;
    IWICBitmapEncoder* piEncoder = NULL;
    IWICBitmapFrameEncode* piBitmapFrame = NULL;
    wicFactory_->CreateStream(&piStream);

    // Section to set the output path
    WCHAR szTempPath[MAX_PATH];
    wcscpy_s(szTempPath, szGerberPath);
    PathRemoveFileSpecW(szTempPath);
    PathAppendW(szTempPath, L"Circuit.bmp");

    piStream->InitializeFromFilename(szTempPath, GENERIC_WRITE);
    wicFactory_->CreateEncoder(GUID_ContainerFormatBmp, NULL, &piEncoder);
    piEncoder->Initialize(piStream, WICBitmapEncoderNoCache);
    piEncoder->CreateNewFrame(&piBitmapFrame, NULL);
    piBitmapFrame->Initialize(NULL);

    WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
    piBitmapFrame->SetSize(puiWidth, puiHeight);
    piBitmapFrame->SetPixelFormat(&formatGUID);
    //save the maxcupper bmp
    //piBitmapFrame->WriteSource(pBitmap_, NULL);
    piBitmapFrame->WriteSource(pBitmap_, NULL);

    piBitmapFrame->Commit();
    piEncoder->Commit();

    //MessageBoxW(handeller_to_window, L"Done Writting", L"Done",MB_OK);

    return /*D2dbitmap*/NULL;
}










void Resize(HWND handeller_to_window,ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* D2DpBitmap)
{
    if(D2DpBitmap == NULL || pRT == NULL)
        return;

    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U window_size = D2D1::SizeU(rc.right, rc.bottom);
    pRT->Resize(window_size);
    InvalidateRect(handeller_to_window, NULL, FALSE);


    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(0.f, 0.f);
    pRT->BeginDraw();

    // Draw a bitmap.
    /*pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + window_size.width,
            upperLeftCorner.y + window_size.height)

    );*/
    ///the original scale
    if(MMG_or_Maxcupper == true)
    {
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
        pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + w_MMG,
            upperLeftCorner.y + h_MMG)

        );
    }
    else
    {
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + w,
            upperLeftCorner.y + h)

        );
    }

    pRT->EndDraw();

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

    D2D1_SIZE_F scale_size = D2D1::SizeF(scale_factor, scale_factor);
    pRT->SetTransform(D2D1::Matrix3x2F::Scale(scale_size, D2D1::Point2F(size.width / 2 - offset_x, size.height / 2 - offset_y)) * D2D1::Matrix3x2F::Translation(offset_x, offset_y));
    //pRT->SetTransform(D2D1::Matrix3x2F::Translation(offset_x, offset_y));

}


void scaleResize(
    HWND handeller_to_window,
    ID2D1HwndRenderTarget* pRT,
    float scale_factor,
    float offset_x,
    float offset_y,
    ID2D1Bitmap* D2DpBitmap)
{
    if(pBitmap_ == NULL && pBitmap_2 == NULL && pBitmap_3 == NULL && pBitmap_4 == NULL)
        return;

    scale_translation(handeller_to_window, pRT, scale_factor, offset_x, offset_y);
    Resize(handeller_to_window,pRT,D2DpBitmap);
}


void DrawLineOnWindow(float xStart, float yStart, float xEnd, float yEnd, float red, float green, float blue, float width)
{
    if(pWindowBrush==NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
        pRT->CreateSolidColorBrush(color, &pWindowBrush);
    }
    pWindowBrush->SetColor(D2D1::ColorF(red,green, blue,1.0));
    pRT->DrawLine(D2D1::Point2F(xStart, yStart), D2D1::Point2F(xEnd, yEnd), pWindowBrush, width);
}


void DrawCirculeOnWindow(float xStart, float yStart, float radius, float red, float green, float blue)
{
    if(pWindowBrush==NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
        pRT->CreateSolidColorBrush(color, &pWindowBrush);
    }
    pWindowBrush->SetColor(D2D1::ColorF(red,green, blue,1.0));
    pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(xStart, yStart), radius, radius), pWindowBrush);
}

void BeginDrawOnWindow()
{
    pRT->BeginDraw();
}

void EndDrawOnWindow()
{
    pRT->EndDraw();
}



void GetWidthHeightMMG(
    std::vector <CompressedCommand> commands,
    UINT& puiWidth, //output
    UINT& puiHeight, //output
    float& Shift_X,
    float& Shift_Y,
    float& high_level_Z,
    float& low_level_Z,
    float scale=10)
{

    float MaxX = -999999999999;
    float MinX = 999999999999;
    float MaxY = -999999999999;
    float MinY = 999999999999;
    float MaxZ = -999999999999;
    float MinZ = 999999999999;
    float x;
    float y;
    float z;


    for(int i =0; i<commands.size(); i++)
    {
        x = commands[i].x;
        y = commands[i].y;
        z = commands[i].z;

        if (x > MaxX)
        {
            MaxX = x;
        }
        if (y > MaxY)
        {
            MaxY = y;
        }
        if(z > MaxZ)
        {
            MaxZ = z;
        }


        if (x < MinX)
        {
            MinX = x;
        }
        if (y < MinY)
        {
            MinY = y;
        }
        if (z < MinZ)
        {
            MinZ = z;
        }


    }

    Shift_X = -MinX*scale;
    MaxX = MaxX - MinX;
    MaxX=MaxX*scale;
    //MinX = 0;

    Shift_Y = -MinY*scale;
    MaxY = MaxY - MinY;
    MaxY =MaxY*scale;
    //MinY = 0;

    high_level_Z = MaxZ;
    low_level_Z = MinZ;

    puiWidth = MaxX + 3;//3 for safty
    puiHeight = MaxY+ 3;//3 for safty


}

void DrawMMG(std::vector <CompressedCommand> commands, float scale)
{

    pBitmap_4 = NULL;
    pBitmap_4_flip = NULL;
    scale = 1.0/scale;
    //scale =10;
    //scale = 1;
    std::cout<<"start MMG"<<std::endl;

    UINT width=0;
    UINT height=0;
    float shift_x=0;
    float shift_y=0;
    float high_level_z = 0;
    float low_level_z = 0;

    std::cout<<"GetWidthHeightMMG"<<std::endl;
    GetWidthHeightMMG(commands,width,height,shift_x,shift_y,high_level_z,low_level_z,scale);
    std::cout<<"width: "<<width<<std::endl;
    std::cout<<"height: "<<height<<std::endl;
    w_MMG = width;
    h_MMG = height;
    std::cout<<"GetWidthHeightMMG"<<std::endl;
    /* if (pBitmap_4 == NULL)
    {*/
        GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_4);
    //}
    pFactory_->CreateWicBitmapRenderTarget(
        pBitmap_4,
        D2D1::RenderTargetProperties(),
        &pWicRenderTarget_);

    std::cout<<"start draw"<<std::endl;
    pWicRenderTarget_->BeginDraw();

    //pWicRenderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::White));
    pWicRenderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::White));


    if (pWicBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(255, 0, 0);
        pWicRenderTarget_->CreateSolidColorBrush(color, &pWicBrush);
    }
    D2D1_COLOR_F color = D2D1::ColorF(255,0, 0);
    pWicBrush->SetColor(color);

    float previousX=0;
    float previousY=0;
    float previousZ=0;

    float currentX=0;
    float currentY=0;
    float currentZ=0;
    currentX = commands[0].x * scale + shift_x +1;//1 for safty
    currentY = commands[0].y * scale + shift_y +1;//1 for safty
    currentZ = commands[0].z;

    for(int i =0; i<commands.size(); i++)
    {
        previousX = currentX;
        previousY = currentY;
        previousZ = currentZ;

        currentX = commands[i].x * scale + shift_x +1 ;//1 for safty
        currentY = commands[i].y * scale + shift_y +1 ;//1 for safty
        currentZ = commands[i].z;
        if(currentZ <= 0)
        {
            pWicRenderTarget_->DrawLine(D2D1::Point2F(previousX, /*h_MMG-*/previousY), D2D1::Point2F(currentX, /*h_MMG-*/currentY), pWicBrush, 1.0);
            //std::cout<<"line"<<std::endl;
        }
        else
        {
            D2D1_COLOR_F color = D2D1::ColorF(0,0,255);
            pWicBrush->SetColor(color);
            pWicRenderTarget_->DrawLine(D2D1::Point2F(previousX, /*h_MMG-*/previousY), D2D1::Point2F(currentX, /*h_MMG-*/currentY), pWicBrush, 1.0);
            //std::cout<<"air"<<std::endl;
            color = D2D1::ColorF(255,0,0);
            pWicBrush->SetColor(color);
        }

    }
    std::cout<<"End draw MMG"<<std::endl;
    pWicRenderTarget_->EndDraw();

    UINT cb_stride=0;
    UINT cb_BufferSize = 0;
    wicFactory_->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_4_flip);
    BYTE* pv_4_flip = lockMeoryBitmap(pBitmap_4_flip, width, height, cb_stride,cb_BufferSize);

    BYTE* pv_4 = lockMeoryBitmap(pBitmap_4, width, height, cb_stride,cb_BufferSize);


    for(int k=0; k<cb_BufferSize;k++)
    {
        *(pv_4_flip + k) = *(pv_4 + k);
    }


    BYTE* pvTemp_4 = new BYTE[cb_BufferSize];
    for (UINT i = 0; i < height; i++)
    {
        for (UINT j = 0; j < width; j++)
        {
            UINT BB = 0;
            UINT GG = 0;
            UINT RR = 0;
            UINT A = 0;
            get_pixel(pv_4, cb_stride, j, i, BB, GG, RR, A);
            set_pixel(pvTemp_4, cb_stride, j, height-i-1, BB, GG, RR,A);
        }
    }

    for(int k=0; k<cb_BufferSize;k++)
    {
      //  if(*(pvTemp_4 + k) != 255)
           // std::cout<<"not white"<<std::endl;
        *(pv_4 + k) = *(pvTemp_4 + k);
    }

    //pBitmap_main = pBitmap_4;
}



void DrawAceleration(std::vector <OutCommand> commands)
{
    char previousX=0;
    char previousY=0;
    char previousZ=0;
    char currentX = 0 /* * scale + shift_x*/;
    char currentY = 0 /* * scale + shift_y*/;
    char currentZ = 0;

    D2D1_COLOR_F colorB = D2D1::ColorF(D2D1::ColorF::Black);
    D2D1_COLOR_F colorP = D2D1::ColorF(D2D1::ColorF::Purple);
    D2D1_COLOR_F colorN = D2D1::ColorF(D2D1::ColorF::Blue);
    D2D1_COLOR_F colorCB = D2D1::ColorF(D2D1::ColorF::CadetBlue);
    D2D1_COLOR_F colorG = D2D1::ColorF(D2D1::ColorF::Green);
    D2D1_COLOR_F colorY = D2D1::ColorF(D2D1::ColorF::Yellow);
    D2D1_COLOR_F colorO = D2D1::ColorF(D2D1::ColorF::Orange);
    D2D1_COLOR_F colorR = D2D1::ColorF(D2D1::ColorF::Red);

    //pWicRenderTarget_ now will draw on Bitmap_4 (MMG bitmap)
    pWicRenderTarget_->BeginDraw();
    for(int i =0; i<commands.size(); i++)
    {
        //steps
        previousX = currentX;
        previousY = currentY;
        previousZ = currentZ;
        std::cout<<"Draw Steps with acc: "<<"previousX: "<<previousX<<"previousY: "<<previousY;
        //0b10000000 -> -x
        if(commands[i].acc &0b10000000 == 0b10000000)
        {
            currentX = previousX - commands[i].x /* * scale + shift_x*/;
        }
        else
        {
            currentX = previousX + commands[i].x /* * scale + shift_x*/;
        }
        ////0b01000000 -> -y
        if(commands[i].acc &0b01000000 == 0b01000000)
        {
            currentY = previousX - commands[i].y /* * scale + shift_y*/;
        }
        else
        {
            currentY = previousX + commands[i].y /* * scale + shift_y*/;
        }
        ////0b00100000 -> -z
        if(commands[i].acc &0b00100000 == 0b00100000)
        {
            currentZ = previousX - commands[i].z;
        }
        else
        {
            currentZ = previousX + commands[i].z;
        }
        /////////////

        //Acc
        if(commands[i].acc & 0b00000111 == 0b00000111)
        {
            pWicBrush->SetColor(colorR);
        }
        else if(commands[i].acc &0b00000111 ==0b00000110)
        {
            pWicBrush->SetColor(colorO);
        }
        else if(commands[i].acc &0b00000111 ==0b00000101)
        {
            pWicBrush->SetColor(colorY);
        }
        else if(commands[i].acc &0b00000111 ==0b00000100)
        {
            pWicBrush->SetColor(colorG);
        }
        else if(commands[i].acc &0b00000111 ==0b00000011)
        {
            pWicBrush->SetColor(colorCB);
        }
        else if(commands[i].acc &0b00000111 ==0b00000010)
        {
            pWicBrush->SetColor(colorN);
        }
        else if(commands[i].acc &0b00000111 ==0b00000001)
        {
            pWicBrush->SetColor(colorP);
        }
        else if(commands[i].acc &0b00000111 ==0b00000000)
        {
            pWicBrush->SetColor(colorB);
        }

        pWicRenderTarget_->DrawLine(D2D1::Point2F(previousX, previousY), D2D1::Point2F(currentX, currentY), pWicBrush, 0.7);
        std::cout<<"->"<<"currentX: "<<currentX<<"currentX: "<<currentY<<std::endl;

    }
    pWicRenderTarget_->EndDraw();
}
