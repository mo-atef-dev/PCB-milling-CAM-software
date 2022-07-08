#include"maxCupper_loading.h"
#include <stdio.h>
ID2D1Factory* pFactory_ = NULL;
ID2D1RenderTarget* pWicRenderTarget_ = NULL;
IWICImagingFactory* wicFactory_ = NULL;
ID2D1SolidColorBrush* pWicBrush =NULL;
IWICBitmap* pBitmap_ = NULL;    //maxcupper
IWICBitmap* pBitmap_2 = NULL;   //gerber
IWICBitmap* pBitmap_3 = NULL;   //maxcupper and gerber

ID2D1Bitmap* D2DpBitmap = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pWindowBrush =NULL;

MyBitmap bitmapObject = MyBitmap();


UINT w=0;
UINT h=0;

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




void string_to_float(
    D2D1_SIZE_U size,
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    float& x_cord,//output
    float& y_cord,//output
    float scale/* =10*/,
    float Shift_X /*= 0*/,
    float Shift_Y /*= 0*/,
    bool yCoordinatWindow /*=true*/
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

    while (str[index] != '*' && str[index] != 'I' && str[index] != 'J')
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



void string_to_float_dill(
    D2D1_SIZE_U size,
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    bool LZ_TZ,
    float& x_cord,//output
    float& y_cord,//output
    float scale /*= 10*/,
    float Shift_X /*= 0*/,
    float Shift_Y /*= 0*/,
    bool yCoordinatWindow/* = true*/
)
{
    //LZ_TZ = true -> means that the coordinates  is in LZ type
    //LZ_TZ = false -> means that the coordinates  is in TZ type
    if (LZ_TZ == false)
    {
        //get the end of the line
        while (str[index] != '\n')
        {
            index++;
        }
        int index_end_Line = index;

        int i = 0;
        float x = 0;
        float y = 0;
        float num = 0;
        float result = 0;
        index--;

        while (str[index] != '\n')
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
                continue;

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
                continue;
            }

            num = (float)(str[index] - '0') * pow(10, i);
            result = result + num;
            i++;
            index--;
        }
        index = index_end_Line;
    }
    else if (LZ_TZ == true)
    {
        int i = 0;
        float num = 0;
        float result = 0;
        /*go to the start of the line
        while (str[index] != '\n')
        {
            index --;
        }
        index++; //X or Y symbol
        */
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
            while (str[dot_index] != '\n' && str[dot_index] != 'Y' && str[dot_index] != '\r')
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
            while (str[dot_index] != '\n' && str[dot_index] != '\r')
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
    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x, shap_Y), (current_shape->diameter) / 2, (current_shape->diameter) / 2), pWicBrush);
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

        pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(current_x, current_y), current_shape->diameter / 2, current_shape->diameter / 2), pBrush);
        /*if (current_shape->diameter <=0.6)
        {
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush, 1.2f);
        }
        else
        {
            pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush ,current_shape->diameter);
        }*/
        pWicRenderTarget->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pBrush ,current_shape->diameter /*-0.1*/);

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

void draw_Obround(
    float shap_x,
    float shap_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush)
{
    pWicRenderTarget->FillRectangle(D2D1::RectF(
        shap_x - current_shape->x / 2,
        shap_y - current_shape->y / 2,
        shap_x + current_shape->x / 2,
        shap_y + current_shape->y / 2),
        pWicBrush);

    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x - current_shape->x / 2, shap_y), (current_shape->y) / 2, (current_shape->y) / 2), pWicBrush);
    pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(shap_x + current_shape->x / 2, shap_y), (current_shape->y) / 2, (current_shape->y) / 2), pWicBrush);



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
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush
    )
{
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();
    pWicRenderTarget->DrawGeometry(m_pPathGeometry, pWicBrush, 0.0001F);
    pWicRenderTarget->FillGeometry(m_pPathGeometry, pWicBrush);
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
    IWICImagingFactory* wicFactory
)
{
    //ID2D1HwndRenderTarget* pRT = NULL;

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

    pConverter->Initialize(
        pWicBitmap_,
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
    pRT->Clear();
    // Draw a bitmap.
    pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + window_size.width,
            upperLeftCorner.y + window_size.height)

    );
    ///draw in the original size
    /*pRT->DrawBitmap(
    D2DpBitmap,
    D2D1::RectF(
        upperLeftCorner.x,
        upperLeftCorner.y,
        upperLeftCorner.x + w,
        upperLeftCorner.y + h)

    );*/
    pRT->EndDraw();


    return D2DpBitmap;
}



BYTE* lockMeoryBitmap(IWICBitmap* pWicBitmap,IWICBitmapLock* pLock, UINT puiWidth, UINT puiHeight, UINT& cb_Stride, UINT& cb_BufferSize )
{
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

    float MaxX = -999999999999;
    float MinX = 999999999999;
    float MaxY = -999999999999;
    float MinY = 999999999999;
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
        else if ((str[i] == 'D') && (str[i + 1] == '0') && (str[i + 2] == '3'))
        {
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, DrillCenter_X, DrillCenter_Y, scale,Shift_X,Shift_Y, false);
            //pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(DrillCenter_X, DrillCenter_Y), 1.2, 1.2), pWicBrush);
            set_pixel(pv, cb_stride, (int)(DrillCenter_X), (int)(DrillCenter_Y), 0, 0, 0, 255);
            //pWicRenderTarget->DrawLine(D2D1::Point2F(DrillCenter_X, DrillCenter_Y), D2D1::Point2F(DrillCenter_X+1, DrillCenter_Y+1), pWicBrush,1.5);
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
    if(str == NULL)
    {
        std::cout<<"No drill file"<<std::endl;
        return;
    }
    std::cout<<"start drill file"<<std::endl;
    //defult 000.000
    coord_digits_x = 3;
    coord_digits_y = 3;
    bool LZ_TZ =true;

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
        /*else if(comment_flag==true && str[i] !='%' ||
                comment_flag==true && str[i] !='\n' )
        {
            while(str[i] != '\n')
            {
                i++;
            }
            comment_flag = false;
        }*/
        /*else if(comment_flag==true && str[i] == '%' ||
                (comment_flag==true && str[i] =='\n'))
        {
            i++;
            comment_flag = false;
        }*/
        //start of comment
        else if (str[i] == ';')
        {
            while(str[i] != '\n')
            {
                i++;
            }
            //comment_flag = false;

            //i++;
            //comment_flag = true;
        }
        else if (str[i] == 'L' && str[i + 1] == 'Z')
        {
            LZ_TZ = true;
            i=i+2;
        }
        else if (str[i] == 'T' && str[i + 1] == 'Z')
        {
            LZ_TZ = false;
            i=i+2;
        }
        //0000.00
        else if (str[i] == '0' && str[i + 1] == '0' && str[i + 2] == '0' && str[i + 3] == '0' && str[i + 4] == '.' && str[i + 5] == '0' && str[i + 6] == '0')
        {
            std::cout<<"0000.00"<<std::endl;
            if (LZ_TZ == true)
            {
                coord_digits_x = 4;
                coord_digits_y = 4;
            }
            else if (LZ_TZ == false)
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
            if (LZ_TZ == true)
            {
                coord_digits_x = 3;
                coord_digits_y = 3;
            }
            else if (LZ_TZ == false)
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
            if (LZ_TZ == true)
            {
                coord_digits_x = 3;
                coord_digits_y = 3;
            }
            else if (LZ_TZ == false)
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

        /*
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
        */
    }

    //black

    //pWicBrush = NULL;
    //pWicRenderTarget->CreateSolidColorBrush(color, &pWicBrush);
    //const D2D1_COLOR_F color = D2D1::ColorF(0,0,0,1);
    const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Red);
    pWicBrush->SetColor(color);
    //pWicRenderTarget->BeginDraw();
    //pWicRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

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
        // skip the comment
        /*else if((comment_flag==true && str[i] !='%') ||
                (comment_flag==true && str[i] !='\n') )
        {
            i++;
            continue;
        }
        else if((comment_flag==true && str[i] == '%') ||
                (comment_flag==true && str[i] =='\n'))
        {
            comment_flag = false;
            i++;
        }*/
        //start of comment
        else if (str[i] == ';')
        {
            while(str[i] != '\n')
            {
                i++;
            }
            //comment_flag = true;
            //i++;
        }
        //else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        else if(str[i] == 'X' || str[i] == 'Y')
        {
            string_to_float_dill(size, str, i, coord_digits_x, coord_digits_y, LZ_TZ, DrillCenter_X, DrillCenter_Y, scale, Shift_X, Shift_Y, false);
            //string_to_float(size, str, i, coord_digits_x, coord_digits_y, DrillCenter_X, DrillCenter_Y, scale,Shift_X,Shift_Y, false);
            //std::cout<<"coord_digits_x :"<<coord_digits_x<<", coord_digits_y= "<<coord_digits_y<<std::endl;
            //std::cout<<"Shift_X :"<<Shift_X<<", Shift_Y= "<<Shift_Y<<std::endl;
            std::cout<<"DrillCenter_X :"<<DrillCenter_X<<", DrillCenter_Y= "<<DrillCenter_Y<<std::endl;
            //pWicRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(DrillCenter_X, DrillCenter_Y), 2, 2), pWicBrush);
            set_pixel(pv, cb_stride, (int)(DrillCenter_X), (int)(DrillCenter_Y), 0, 0, 0, 255);

            while(str[i]!='\n')
            {
                i++;
            }
        }
        else { i++;}
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
    if(pRT ==NULL)
        return;

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
    pWicRenderTarget->DrawLine(D2D1::Point2F(0, 0), D2D1::Point2F(puiWidth, 0), pWicBrush, 1.0f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(puiWidth, 0), D2D1::Point2F(puiWidth, puiHeight), pWicBrush, 1.0f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(puiWidth, puiHeight), D2D1::Point2F(0, puiHeight), pWicBrush, 1.0f);
    pWicRenderTarget->DrawLine(D2D1::Point2F(0, puiHeight), D2D1::Point2F(0, 0), pWicBrush, 1.0f);
    pWicRenderTarget->EndDraw();


}



/////
//
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
    //scale = 0.1*scale;
    scale = 10;
    // Create a WIC Fact
    if(profileStr ==NULL || str==NULL)
    {
        return NULL;
    }
    if (wicFactory_ == NULL)
    {
        CoCreateInstance(
           CLSID_WICImagingFactory,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_IWICImagingFactory,
           (LPVOID*)&wicFactory_);
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

    float Shift_X=0;
    float Shift_Y = 0;
    //char profileStr[] = "G04 EAGLE Gerber RS-274X export* G75* %MOMM*% %FSLAX34Y34*% %LPD*% %IN*% %IPPOS*% %AMOC8* 5,1,8,0,0,1.08239X$1,22.5*% G01* %ADD10C,0.254000*% D10* X0Y0D02* X1000000Y0D01* X1000000Y800000D01* X0Y800000D01* X0Y0D01* X626110Y339820D02* X626039Y338741D01* X625898Y337669D01* X625687Y336609D01* X625408Y335565D01* X625060Y334541D01* X624646Y333543D01* X624168Y332573D01* X623628Y331637D01* X623027Y330738D01* X622369Y329881D01* X621657Y329068D01* X620892Y328304D01* X620079Y327591D01* X619222Y326933D01* X618323Y326332D01* X617387Y325792D01* X616417Y325314D01* X615419Y324900D01* X614395Y324552D01* X613351Y324273D01* X612291Y324062D01* X611219Y323921D01* X610140Y323850D01* X609060Y323850D01* X607981Y323921D01* X606909Y324062D01* X605849Y324273D01* X604805Y324552D01* X603781Y324900D01* X602783Y325314D01* X601813Y325792D01* X600877Y326332D01* X599978Y326933D01* X599121Y327591D01* X598308Y328304D01* X597544Y329068D01* X596831Y329881D01* X596173Y330738D01* X595572Y331637D01* X595032Y332573D01* X594554Y333543D01* X594140Y334541D01* X593792Y335565D01* X593513Y336609D01* X593302Y337669D01* X593161Y338741D01* X593090Y339820D01* X593090Y340900D01* X593161Y341979D01* X593302Y343051D01* X593513Y344111D01* X593792Y345155D01* X594140Y346179D01* X594554Y347177D01* X595032Y348147D01* X595572Y349083D01* X596173Y349982D01* X596831Y350839D01* X597544Y351652D01* X598308Y352417D01* X599121Y353129D01* X599978Y353787D01* X600877Y354388D01* X601813Y354928D01* X602783Y355406D01* X603781Y355820D01* X604805Y356168D01* X605849Y356447D01* X606909Y356658D01* X607981Y356799D01* X609060Y356870D01* X610140Y356870D01* X611219Y356799D01* X612291Y356658D01* X613351Y356447D01* X614395Y356168D01* X615419Y355820D01* X616417Y355406D01* X617387Y354928D01* X618323Y354388D01* X619222Y353787D01* X620079Y353129D01* X620892Y352417D01* X621657Y351652D01* X622369Y350839D01* X623027Y349982D01* X623628Y349083D01* X624168Y348147D01* X624646Y347177D01* X625060Y346179D01* X625408Y345155D01* X625687Y344111D01* X625898Y343051D01* X626039Y341979D01* X626110Y340900D01* X626110Y339820D01* M02*";
    //char profileStr[] = "%TF.GenerationSoftware,KiCad,Pcbnew,(5.1.12)-1*% %TF.CreationDate,2022-04-23T11:54:43+02:00*% %TF.ProjectId,test,74657374-2e6b-4696-9361-645f70636258,rev?*% %TF.SameCoordinates,Original*% %TF.FileFunction,Profile,NP*% %FSLAX46Y46*% G04 Gerber Fmt 4.6, Leading zero omitted, Abs format (unit mm)* G04 Created by KiCad (PCBNEW (5.1.12)-1) date 2022-04-23 11:54:43* %MOMM*% %LPD*% G01* G04 APERTURE LIST* %TA.AperFunction,Profile*% %ADD10C,0.050000*% %TD*% G04 APERTURE END LIST* D10* X19050000Y-72390000D02* X95250000Y-72390000D01* X19050000Y-13970000D02* X19050000Y-72390000D01* X95250000Y-13970000D02* X19050000Y-13970000D01* X95250000Y-72390000D02* X95250000Y-13970000D01* M02*";
    GetWidthHeight(handeller_to_window, profileStr, size, coord_digits_x, coord_digits_y, scale, puiWidth, puiHeight, Shift_X, Shift_Y);

    w = puiWidth;
    h = puiHeight;

    if (pBitmap_ == NULL)
    {
        WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, formatGUID, WICBitmapCacheOnDemand, &pBitmap_);
    }

    //create factorty
    if (pFactory_ == NULL)
    {
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
    }
    if (pWicRenderTarget_ == NULL)
    {
        pFactory_->CreateWicBitmapRenderTarget(
            pBitmap_,
            D2D1::RenderTargetProperties(),
            &pWicRenderTarget_);
    }


    //create brush
    if (pWicBrush == NULL)
    {
        const D2D1_COLOR_F color = D2D1::ColorF(100, 150, 210);
        pWicRenderTarget_->CreateSolidColorBrush(color, &pWicBrush);
    }
    //End intialization
    //////////////////////////////////////////////////////////////////////////
    DrawBorders(pWicRenderTarget_, pWicBrush, puiWidth, puiHeight);
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
    pWicRenderTarget_->BeginDraw();

    //pWicRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Red));
    //pWicRenderTarget->Clear(D2D1::ColorF(255,0,0,0));
    /*pWicBrush->SetColor(D2D1::ColorF(0, 0, 0,0));
    pWicRenderTarget->FillRectangle(D2D1::RectF(
        0,
        0,
        puiWidth,
        puiHeight),
        pWicBrush);*/
   // pWicBrush->SetColor(D2D1::ColorF(D2D1::ColorF(255,0,0)));

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

    while (1)
    {
        //end of file
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            pWicRenderTarget_->EndDraw();
            break;
        }
        //G36*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '6' && str[i + 3] == '*')
        {
            start_contour =true;
            //contour_x = current_x;
            //contour_y = current_y;
            i=i+3;
        }
        //G01*
        else if(str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '1' && str[i + 3] == '*')
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

        }
        //G37*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '7' && str[i + 3] == '*')
        {
            tempX = current_x;
            tempY = current_y;
            std::cout<<"end_polygon_1"<<std::endl;
            pSink->AddLine(D2D1::Point2F(contour_x, contour_y));
            end_polygon(pSink,m_pPathGeometry,pWicRenderTarget_,pWicBrush);
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
            string_to_float(size, str, i,coord_digits_x,coord_digits_y, shape_x, shape_y, scale,Shift_X,Shift_Y);
            if(start_contour == true)
            {
                //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pWicBrush ,1.0f);
                std::cout<<"draw_line_polygon_1"<<std::endl;
                std::cout<<"shape_x: "<<shape_x<<std::endl;
                std::cout<<"shape_y: "<<shape_y<<std::endl;
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
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, current_x, current_y,scale, Shift_X, Shift_Y);
            i = i + 3;
        }
        //creart flash (replicating) the current aperture
        else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
        {
            //cout<<"D03"<<endl;
            string_to_float(size, str, i, coord_digits_x, coord_digits_y, shape_x, shape_y, scale, Shift_X, Shift_Y);
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

    //DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_,wicFactory_);
    //MessageBoxW(handeller_to_window, L"finish", L"finish", MB_OKCANCEL);
    std::cout<<"finish gerber file"<<std::endl;
    //begin_polygon(0,0,pFactory_,pSink, m_pPathGeometry);
    //pWicRenderTarget_->BeginDraw();
    //ID2D1GeometrySink* pSink =NULL;
    //ID2D1PathGeometry* m_pPathGeometry ;
    //pWicBrush->SetColor(D2D1::ColorF(255, 255, 0));


    /*ID2D1GeometrySink* pSink__ = NULL;
    // Create a path geometry.
    ID2D1PathGeometry* m_pPathGeometry__ =NULL;
    pFactory_->CreatePathGeometry(&m_pPathGeometry);
    // Write to the path geometry using the geometry sink.
    m_pPathGeometry->Open(&pSink);
    pSink->BeginFigure(
        D2D1::Point2F(50, 50),
        D2D1_FIGURE_BEGIN_FILLED
    );
    pSink->AddLine(D2D1::Point2F(100,50));
    pSink->AddLine(D2D1::Point2F(100,100));
    pSink->AddLine(D2D1::Point2F(500,100));
    pSink->AddLine(D2D1::Point2F(50,50));
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();
    //pRenderTarget->DrawGeometry(m_pPathGeometry, pBrush, 1.F);
    //pWicBrush->SetColor(D2D1::ColorF(255, 255, 0));
    pWicRenderTarget_->FillGeometry(m_pPathGeometry, pWicBrush);
    pWicRenderTarget_->EndDraw();*/



    DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_,wicFactory_);
    MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);




    /*
    //Excute Max cupper algorithm the draw it
    MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);
    //start max cupper

    //cb_stride is sent by referance, so the initalization of it is in the fancution
    UINT cb_stride = 0;
    //pv is a pointer to the bitmap bytes
    BYTE* pv = lockMeoryBitmap(pBitmap, puiWidth, puiHeight, cb_stride);
    BYTE* pv = lockMeoryBitmap(pBitmap, puiWidth, puiHeight, cb_stride);
    //start max cupper
    All_algorithm_max_cupper(pv, cb_stride, puiWidth, puiHeight);
    //draw the bitmap after max cupper
    D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
    //end max cupper
    */
    //drills
    ///////////////////
   //char DrillsStr[] = "G04 EAGLE Gerber RS-274X export* G75* %MOMM*% %FSLAX34Y34*% %LPD*% %INSoldermask Bottom*% %IPPOS*% %AMOC8* 5,1,8,0,0,1.08239X$1,22.5*% G01* %ADD10C,1.524000*% %ADD11C,1.879600*% %ADD12C,2.743200*% %ADD13C,3.505200*% D10* X254000Y355600D03* X304800Y355600D03* X609600Y533400D03* X660400Y533400D03* D11* X317500Y431800D03* X393700Y431800D03* D12* X152400Y139700D03* X254000Y139700D03* D10* X152400Y685800D03* X177800Y685800D03* X381000Y533400D03* X508000Y533400D03* X520700Y660400D03* X647700Y660400D03* X381000Y596900D03* X508000Y596900D03* X368300Y685800D03* X495300Y685800D03* X850900Y368300D03* X876300Y393700D03* X850900Y419100D03* D11* X584200Y165100D03* X609600Y165100D03* X635000Y165100D03* D13* X609600Y340360D03* M02*";

    /*if(xln_grb == true)
    {
        DrawDrills_gerber(DrillsStr, size, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale, pWicRenderTarget, pWicBrush);
    }
    else if(xln_grb == false)
    {
        DrawDrills_xln(NULL,0,DrillsStr, size, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale, pWicRenderTarget, pWicBrush);
    }
    D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
    */
    //end draw dills
    //IWICBitmap* pBitmapGerber =NULL;
    //formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
    //wicFactory->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmapGerber);
    //*pBitmapGerber = *pBitmap;

    //ID2D1Bitmap* D2dbitmap = DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapGerber, wicFactory);


    /////////////////

    //start max cupper
    //MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);

    //cb_stride is sent by referance, so the initalization of it is in the fancution
    UINT cb_stride = 0;
    UINT cb_BufferSize = 0;


    //IWICBitmap* pBitmapGerber =NULL;
    //wicFactory->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmapGerber);
    IWICBitmapLock* pLock_maxcupper_with_drills=NULL;
    BYTE* pv_maxcupper_with_drills = lockMeoryBitmap(pBitmap_,pLock_maxcupper_with_drills, puiWidth, puiHeight, cb_stride,cb_BufferSize);



    //pv is a pointer to the bitmap bytes
    //IWICBitmapLock* pLock_maxcupper_with_drills =NULL;
    //BYTE* pv_maxcupper_with_drills = lockMeoryBitmap(pBitmap,pLock_maxcupper_with_drills, puiWidth, puiHeight, cb_stride, cb_BufferSize);

    //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapGerber, wicFactory);
    ////////

    //Excute Max cupper algorithm the draw it
    //MessageBoxW(handeller_to_window, L"start max cupper", L"start max cupper", MB_OKCANCEL);
    //start max cupper
    std::cout<<"start maxcupper"<<std::endl;
    All_algorithm_max_cupper(pv_maxcupper_with_drills, cb_stride, puiWidth, puiHeight);
    //draw the bitmap after max cupper
    //DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    ////end max cupper

    DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    MessageBoxW(handeller_to_window, L"start maxcupper", L"start maxcupper", MB_OKCANCEL);

    std::cout<<"start drills"<<std::endl;
    //MessageBoxW(handeller_to_window, L"start Draw Drills", L"start Draw Drills", MB_OKCANCEL);
    ///draw drills with set_pixel()
    if(xln_grb == true)
    {
        DrawDrills_xln(pv_maxcupper_with_drills,cb_stride,DrillsStr, size, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale, pWicRenderTarget_, pWicBrush);
    }
    else if(xln_grb == false)
    {
        DrawDrills_gerber(pv_maxcupper_with_drills,cb_stride,DrillsStr, size, coord_digits_x, coord_digits_y, Shift_X, Shift_Y, scale, pWicRenderTarget_, pWicBrush);
    }
    DrawBitmapOnWindow(handeller_to_window, pFactory_, pBitmap_, wicFactory_);
    MessageBoxW(handeller_to_window, L"end drills", L"end drills", MB_OKCANCEL);
    std::cout<<"End drill file"<<std::endl;




    //MessageBoxW(handeller_to_window, L"start Bitmap_gerber", L"start Bitmap_gerber", MB_OKCANCEL);
    //start tracing the string

    ///////////
    std::cout<<"start gerber on maxcupper"<<std::endl;
    //IWICBitmap* pBitmap_2=NULL;
    pWicRenderTarget_->Release();
    pWicRenderTarget_ = NULL;
    if (pBitmap_2 == NULL)
    {
        GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_2);
    }
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
    pWicRenderTarget_->FillRectangle(D2D1::RectF(
        0,
        0,
        puiWidth,
        puiHeight),
        pWicBrush);
    //pWicRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    //pWicBrush->SetColor(D2D1::ColorF(255,0,0,1));
    pWicBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    start_contour =false;
    i = 0;
    while (1)
    {
        //end of file
        if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
        {
            pWicRenderTarget_->EndDraw();
            break;
        }
        //G36*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '6' && str[i + 3] == '*')
        {
            start_contour =true;
            //contour_x = current_x;
            //contour_y = current_y;
            i=i+3;
        }
        //G01*
        else if(str[i] == 'G' && str[i + 1] == '0' && str[i + 2] == '1' && str[i + 3] == '*')
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

        }
        //G37*
        else if(str[i] == 'G' && str[i + 1] == '3' && str[i + 2] == '7' && str[i + 3] == '*')
        {
            tempX = current_x;
            tempY = current_y;
            std::cout<<"end_polygon_1"<<std::endl;
            pSink->AddLine(D2D1::Point2F(contour_x, contour_y));
            end_polygon(pSink,m_pPathGeometry,pWicRenderTarget_,pWicBrush);
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
            string_to_float(size, str, i,coord_digits_x,coord_digits_y, shape_x, shape_y, scale,Shift_X,Shift_Y);
            if(start_contour == true)
            {
                //pWicRenderTarget_->DrawLine(D2D1::Point2F(current_x, current_y), D2D1::Point2F(shape_x, shape_y), pWicBrush ,1.0f);
                std::cout<<"draw_line_polygon_1"<<std::endl;
                std::cout<<"shape_x: "<<shape_x<<std::endl;
                std::cout<<"shape_y: "<<shape_y<<std::endl;
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

    //Draw the original bitmap
    //DrawBitmapOnWindow(handeller_to_window,pFactory, pBitmap_2,wicFactory);
    //DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_2,wicFactory_);
    IWICBitmapLock* pLock_gerber = NULL;
    BYTE* pv_gerber = lockMeoryBitmap(pBitmap_2,pLock_gerber, puiWidth, puiHeight, cb_stride,cb_BufferSize);



    //MessageBoxW(handeller_to_window, L"draw total", L"draw total", MB_OKCANCEL);
    //IWICBitmap* pBitmap_3 =NULL;
    wicFactory_->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmap_3);
    IWICBitmapLock* pLock_total=NULL;
    BYTE* pv_total = lockMeoryBitmap(pBitmap_3,pLock_total, puiWidth, puiHeight, cb_stride,cb_BufferSize);
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

    //DrawBitmapOnWindow(handeller_to_window,pFactory_, pBitmap_3,wicFactory_);
    //MessageBoxW(handeller_to_window, L"Done", L"Done", MB_OKCANCEL);


        //IWICBitmap* pBitmapDrills =NULL;
        //formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        //wicFactory->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmapDrills);
        //pBitmapDrills = pBitmap;
        //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapDrills, wicFactory);

        //MessageBoxW(handeller_to_window, L"draw the gerber", L"draw the gerber", MB_OKCANCEL);
        //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapDrills, wicFactory);

        //MessageBoxW(handeller_to_window, L"draw the gerber", L"draw the gerber", MB_OKCANCEL);
        //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapGerber, wicFactory);


        /*////convert the red drills to black
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
        */
        //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
        ////
        //MessageBoxW(handeller_to_window, L"draw the gerber", L"draw the gerber", MB_OKCANCEL);
        //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapGerber, wicFactory);

        //Draw the Gerber on the bitmab
        //IWICBitmap* pBitmapMuxcupper_Gerber =NULL;
        //formatGUID = GUID_WICPixelFormat32bppPBGRA;//format of bitmap
        //wicFactory->CreateBitmap(puiWidth, puiHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &pBitmapMuxcupper_Gerber);
        /*BYTE* pv_muxcupper_with_gerber = lockMeoryBitmap(pBitmapMuxcupper_Gerber, puiWidth, puiHeight, cb_stride, cb_BufferSize);
        for(int k =0;k<cb_BufferSize;k++)
        {
            *(pv_muxcupper_with_gerber+k) = *(pv_maxcupper_with_drills+k);
        }*/



        ////End Draw Gerber again
        //DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmapGerber, wicFactory);



        //initalize the bitmap object
        bitmapObject.SetSpecifications(pv_maxcupper_with_drills, cb_BufferSize, cb_stride, puiWidth, puiHeight);
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
        wicFactory_->CreateStream(&piStream);
        piStream->InitializeFromFilename(L"testOut.bmp", GENERIC_WRITE);
        wicFactory_->CreateEncoder(GUID_ContainerFormatBmp, NULL, &piEncoder);
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
        //save the maxcupper bmp
        //piBitmapFrame->WriteSource(pBitmap_, NULL);
        piBitmapFrame->WriteSource(pBitmap_3, NULL);

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

    return /*D2dbitmap*/NULL;

    /*
    UINT cb_stride=0;
    BYTE* pv = lockMeoryBitmap(pBitmap, 800, 800, cb_stride);
    All_algorithm_max_cupper(pv,cb_stride, 800, 800);

    DrawBitmapOnWindow(handeller_to_window, pFactory, pBitmap, wicFactory);
    */



}


void Resize(HWND handeller_to_window,ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* D2DpBitmap)
{
    if(D2DpBitmap ==NULL)
        return;

    RECT rc;
    GetClientRect(handeller_to_window, &rc);
    D2D1_SIZE_U window_size = D2D1::SizeU(rc.right, rc.bottom);
    pRT->Resize(window_size);
    InvalidateRect(handeller_to_window, NULL, FALSE);


    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(0.f, 0.f);
    pRT->BeginDraw();
    pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
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
    pRT->DrawBitmap(
        D2DpBitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + w,
            upperLeftCorner.y + h)

    );
    pRT->EndDraw();

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
