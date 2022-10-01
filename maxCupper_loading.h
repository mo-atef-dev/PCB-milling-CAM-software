#ifndef MAXCOPPER_H_INCLUDED
#define MAXCOPPER_H_INCLUDED

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


#include"cmds.h"

extern WCHAR szGerberPath[];

extern ID2D1Factory* pFactory_ /*= NULL*/;
extern ID2D1RenderTarget* pWicRenderTarget_ /*= NULL*/;
extern IWICImagingFactory* wicFactory_ /*= NULL*/;
extern ID2D1SolidColorBrush* pWicBrush /*=NULL*/;
extern IWICBitmap* pBitmap_ /*= NULL*/;    //maxcupper
extern IWICBitmap* pBitmap_2 /*= NULL*/;   //gerber
extern IWICBitmap* pBitmap_3 /*= NULL*/;   //maxcupper and gerber
extern IWICBitmap* pBitmap_4 /*= NULL*/;   //mmg



extern IWICBitmap* pBitmap_main/* = NULL*/;   //all

extern ID2D1Bitmap* D2DpBitmap /*= NULL*/;
extern ID2D1HwndRenderTarget* pRT /*= NULL*/;
extern ID2D1SolidColorBrush* pWindowBrush /*=NULL*/;

//width and height to scale_transition
extern UINT w;
extern UINT h;
extern UINT w_MMG;
extern UINT h_MMG;
extern float fraction_to_reduce /* = 0.05*/;
extern bool flip_or_not /*=false*/;

/// <summary>
/// Initialize render target and brush
/// </summary>
/// <param name="handeller_to_window">: Handler to the originale window that will paint on</param>
void initializeDrawing(HWND handeller_to_window);

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
    /// Constructor that initialize the bitmap
    /// </summary>
    /// <param name="pv">Pointer to array of bytes that has the pixels</param>
    /// <param name="cbBufferSize">Total size of the image in bytes</param>
    /// <param name="cbStride">the width of the image  in bytes</param>
    MyBitmap()
    {
        pv = NULL;
        cb_BufferSize = 0;
        cb_Stride = 0;
    }
    /// <summary>
    /// Constructor that initialize the image by array of bytes and the size and the stride
    /// </summary>
    /// <param name="ArrayBytes">: The array of bytes that have the pexil</param>
    /// <param name="cbBufferSize">: Total size of the image in bytes</param>
    /// <param name="cbStride">: The width of the image in bytes</param>
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
    /// <summary>
    ///  Constructor that initialize the image by array of bytes and the size and the stride
    /// </summary>
    /// <param name="ArrayBytes">: The array of bytes that have the pexil</param>
    /// <param name="cbBufferSize">: Total size of the image in bytes</param>
    /// <param name="cbStride">: The width of the image in bytes</param>
    /// <param name="w"><: The width of the bitmap/param>
    /// <param name="h">: The height of the bitmap</param>
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
    /// <summary>
    /// Set specifications of the bitmap if the object made without setting the specifications
    /// </summary>
    /// <param name="ArrayBytes">: The array of bytes that have the pexil</param>
    /// <param name="cbBufferSize">: Total size of the image in bytes</param>
    /// <param name="cbStride">: The width of the image in bytes</param>
    /// <param name="w"><: The width of the bitmap/param>
    /// <param name="h">: The height of the bitmap</param>
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
    /// <summary>
    /// Set the size of the bitmap
    /// </summary>
    /// <param name="cbBufferSize">: Total size of the image in bytes</param>
    void SetSize(UINT cbBufferSize)
    {
        cb_BufferSize = cbBufferSize;
    }
    /// <summary>
    /// Set the stride of the bitmap
    /// </summary>
    /// <param name="cbStride">: The width of the image in bytes</param>
    void SetStride(UINT cbStride)
    {
        cb_Stride = cbStride;
    }
    /// <summary>
    /// Get the width of the BitmapObject
    /// </summary>
    /// <returns>The width of the bitmap</returns>
    UINT GetWidth()
    {
        return width;
    }
    /// <summary>
    /// Get the width of the BitmapObject
    /// </summary>
    /// <returns></returns>
    UINT GetHeight()
    {
        return height;
    }
    /// <summary>
    /// Set pixel array using pointer to te first byte(array of bytes)
    /// </summary>
    /// <param name="ArrayBytes">: The array of bytes that have the pexil</param>
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


//Global object for tracing algorithm
extern MyBitmap bitmapObject /*= MyBitmap()*/;




/// <summary>
/// Set one pixel to a certain color. No return
/// </summary>
/// <param name="pv">pointr to the array that represent a bitmap</param>
/// <param name="cb_Stride">The width of the bitmap(the row length)</param>
/// <param name="x0">The x coordinate of the pixel that will be changed</param>
/// <param name="y0">The y coordinate of the pixel that will be changed</param>
/// <param name="B">Blue value (0 - 255) </param>
/// <param name="G">Green value (0 - 255)</param>
/// <param name="R">Red value (0 - 255)</param>
/// <param name="A">Alpha value (0 - 1): 1 for no transparent and 0 to make the pixel transparent </param>
void set_pixel(BYTE* pv, UINT cb_Stride, UINT x0, UINT y0, UINT B, UINT G, UINT R, UINT A);

/// <summary>
/// get the color of one pixel. No return
/// </summary>
/// <param name="pv">: pointr to the array that represent a bitmap</param>
/// <param name="cb_Stride">: The width of the bitmap(the row length)</param>
/// <param name="x0">: The x coordinate of the pixel that will be changed</param>
/// <param name="y0">: The y coordinate of the pixel that will be changed</param>
/// <param name="B">:Return by value parameter. Blue value (0 - 255) </param>
/// <param name="G">:Return by value parameter. Green value (0 - 255)</param>
/// <param name="R">:Return by value parameter. Red value (0 - 255)</param>
/// <param name="A">:Return by value parameter. Alpha value (0 - 1): 1 for no transparent and 0 to make the pixel transparent </param>
void get_pixel(BYTE* pv, UINT cb_Stride, UINT x0, UINT y0, UINT& B, UINT& G, UINT& R, UINT& A);

/// <summary>
/// Put all cupper in the bitmap to a certain color
/// </summary>
/// <param name="pv">: pointr to the array that represent a bitmap</param>
/// <param name="cb_Stride">: The width of the bitmap(the row length)</param>
/// <param name="puiWidth">: the width of the bitmap</param>
/// <param name="puiHeight">: the height of the bitmap</param>
/// <param name="color_1">the color of the cupper of the circuit</param>
void same_color(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight, UINT color_1);

/// <summary>
/// Making the max cupper algorithm to the bitmap which represent a electric circuit
/// </summary>
/// <param name="pv">: pointr to the array that represent a bitmap</param>
/// <param name="cb_Stride">: The width of the bitmap(the row length)</param>
/// <param name="puiWidth">: The width of the bitmap</param>
/// <param name="puiHeight">: The height of the bitmap</param>
/// <param name="color_1">The first color to use in the max cupper</param>
/// <param name="color_2">The second color to use in the max cupper</param>
/// <param name="cupper">The thired color to use in the max cupper which represent the finle color of the cupper</param>
void max_cupper(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight, UINT color_1, UINT color_2, UINT cupper);

/// <summary>
/// Inhance the max cupper algorithm to the bitmap which represent a electric circuit
/// </summary>
/// <param name="pv">: pointr to the array that represent a bitmap</param>
/// <param name="cb_Stride">: The width of the bitmap(the row length)</param>
/// <param name="puiWidth">: The width of the bitmap</param>
/// <param name="puiHeight">: The height of the bitmap</param>
void inhanceMaxcupper(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight);

/// <summary>
/// Make the same color function and the max cupper function
/// </summary>
/// <param name="pv">: pointr to the array that represent a bitmap</param>
/// <param name="cb_Stride">: The width of the bitmap(the row length)</param>
/// <param name="puiWidth">: The width of the bitmap</param>
/// <param name="puiHeight">: The height of the bitmap</param>
void All_algorithm_max_cupper(BYTE* pv, UINT cb_Stride, UINT puiWidth, UINT puiHeight);

//Data structure for Standard Aperture Templates:
struct aperture
{
    /*aperture type:
        1 for Circle
        2 for Rectangle
        3 for Obround
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

/// <summary>
/// Test the format of the drills file it would be(TZ or LZ or Dec)
/// </summary>
/// <param name="str">: String represented the drill files</param>
/// <returns>return 0:if decimal format, return 1 if LZ format, return 2 if TZ format</returns>
int testFormatDrills(char* str);

/// <summary>
/// Convert the string to float starting from the index of the least segneficant digit and back to the X and Y symbols
/// </summary>
/// <param name="str">: The string that have the x and y points (XxxxxYxxxx)</param>
/// <param name="index">: Index of the last digit in the string</param>
/// <param name="coord_digits_x">: The number of the fraction digits of the X point</param>
/// <param name="coord_digits_y">: The number of the fraction digits of the Y point</param>
/// <param name="x_cord">: variable that will have the result of the X (send by referance)</param>
/// <param name="y_cord">: variable that will have the result of the Y (send by referance)</param>
/// <param name="scale">: The scale which represent the number of pixels for mm</param>
/// <param name="Shift_X">: The x value for shifting to draw the circut in the first quarter of the coordinates</param>
/// <param name="Shift_Y">: The y value for shifting to draw the circut in the first quarter of the coordinates</param>
void string_to_float(
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    float& x_cord,//output
    float& y_cord,//output
    float scale =10,
    float Shift_X = 0,
    float Shift_Y = 0
);


/// <summary>
/// Convert the string to float starting for the drill file from the index of the least segneficant digit and back to the X and Y symbols
/// </summary>
/// <param name="str">: The string that have the x and y points (XxxxxYxxxx)</param>
/// <param name="index">: Index of the last digit in the string</param>
/// <param name="coord_digits_x">: The number of the fraction digits of the X point</param>
/// <param name="coord_digits_y">: The number of the fraction digits of the Y point</param>
/// <param name="LZ_TZ">: Chose if the format is leading zeros or traling zeros</param>
/// <param name="x_cord">: variable that will have the result of the X (send by referance)</param>
/// <param name="y_cord">: variable that will have the result of the Y (send by referance)</param>
/// <param name="scale">: The scale which represent the number of pixels for mm</param>
/// <param name="Shift_X">: The x value for shifting to draw the circut in the first quarter of the coordinates</param>
/// <param name="Shift_Y">: The y value for shifting to draw the circut in the first quarter of the coordinates</param>
void string_to_float_dill(
    char* str,
    int index,
    int coord_digits_x,
    int coord_digits_y,
    int LZ_TZ_dec,
    float& x_cord,//output
    float& y_cord,//output
    float scale = 10,
    float Shift_X = 0,
    float Shift_Y = 0
);

/// <summary>
/// Draw a circle which represent a drill
/// </summary>
/// <param name="shap_x">: The x coordinat of the center</param>
/// <param name="shap_Y">: The y coordinat of the center</param>
/// <param name="pWicRenderTarget">: RenderTarget to draw in</param>
/// <param name="current_shape">: Pointer to the shape(circle) to draw</param>
/// <param name="pWicBrush">: pointer to the brush that has the color</param>
void Draw_circle(
    float shap_x,
    float shap_Y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush
);

/// <summary>
/// Draw a line which repersent a cupper track
/// </summary>
/// <param name="handeller_to_window">: The handler to the window that will draw on</param>
/// <param name="current_x">: The x coordinat of the start of the line</param>
/// <param name="current_y">: The x coordinat of the start of the line</param>
/// <param name="shape_x">: The x coordinat of the end of the line</param>
/// <param name="shape_y">: The y coordinat of the center</param>
/// <param name="pWicRenderTarget">: RenderTarget to draw in</param>
/// <param name="current_shape">: Pointer to the shape(circle) that its diameter is the width of the line</param>
/// <param name="pBrush">: pointer to the brush that has the color</param>
void draw_line(
    HWND handeller_to_window,
    float& current_x,
    float& current_y,
    float shape_x,
    float shape_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pBrush
);

/// <summary>
/// Draw a rectangle
/// </summary>
/// <param name="shape_x">: The x coordinat of the center of the rectangle</param>
/// <param name="shape_y">: The y coordinat of the center of the rectangle</param>
/// <param name="pWicRenderTarget">: RenderTarget to draw in</param>
/// <param name="current_shape">: Pointer to the shape(rectangle) that have the width and height specification of the rectangle</param>
/// <param name="pWicBrush">: pointer to the brush that has the color</param>
void draw_rectangle(
    float shape_x,
    float shape_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush
);

/// <summary>
/// Draw a obround
/// </summary>
/// <param name="shap_x">: The x coordinat of the center of the rectangle</param>
/// <param name="shap_y">: The y coordinat of the center of the rectangle</param>
/// <param name="pWicRenderTarget">: RenderTarget to draw in</param>
/// <param name="current_shape">: Pointer to the shape(obround) that have the width and height specification of the obround</param>
/// <param name="pWicBrush">: pointer to the brush that has the color</param>
void draw_Obround(
    float shap_x,
    float shap_y,
    ID2D1RenderTarget* pWicRenderTarget,
    aperture* current_shape,
    ID2D1SolidColorBrush* pWicBrush
);

/// <summary>
/// Draw polygon(8 edges)
/// </summary>
/// <param name="shape_x">: The x coordinat of the center of the polygon</param>
/// <param name="shape_y">: The y coordinat of the center of the polygon</param>
/// <param name="pFactory">: pointer to factory that used to draw</param>
/// <param name="pWicRenderTarget">: RenderTarget to draw in</param>
/// <param name="pWicBrush">: pointer to the brush that has the color</param>
/// <param name="current_shape">: Pointer to the shape(poltgon) that have the specification of the polygon</param>
void draw_polygon(
    float shape_x,
    float shape_y,
    ID2D1Factory* pFactory,
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush,
    aperture* current_shape
);

/// <summary>
/// Draw a WicBitmap on the window
/// </summary>
/// <param name="handeller_to_window">: The handler to the window which the bitmap will draw on</param>
/// <param name="pFactory">: The factory of drawing</param>
/// <param name="pWicBitmap_">: WicBitmap which must be one of:(pBitmap_, pBitmap_2, pBitmap_3, pBitmap_4)</param>
/// <param name="wicFactory">: The WicFactory of drawing</param>
/// <returns></returns>
ID2D1Bitmap* DrawBitmapOnWindow(
    HWND handeller_to_window,
    ID2D1Factory* pFactory,
    IWICBitmap* pWicBitmap_,
    IWICImagingFactory* wicFactory,
    bool flip = false
);

/// <summary>
/// Get a pointer of an array which represent the bitmap to manually modify it
/// </summary>
/// <param name="pWicBitmap">: The WicBitmap which neeed to get its pointer array</param>
/// <param name="pLock">: Pointer to IWICBitmapLock to lock the Bitmap</param>
/// <param name="puiWidth">: Width of the WicBitmap</param>
/// <param name="puiHeight">: Height of the WicBitmap</param>
/// <param name="cb_Stride">: The stride of the bitmap(the width)</param>
/// <param name="cb_BufferSize">: The size of the bitmap</param>
/// <returns>: pointer to an array which represent a WicBitmap</returns>
BYTE* lockMeoryBitmap(IWICBitmap* pWicBitmap, UINT puiWidth, UINT puiHeight, UINT& cb_Stride, UINT& cb_BufferSize );


/// <summary>
/// Get the width and the height of the gerber file(the width and the height of circuit)
/// </summary>
/// <param name="handeller_to_window">: The handeller to the window</param>
/// <param name="str">: String represent a gerber file</param>
/// <param name="coord_digits_x">: number of fraction part of x of the number in the gerber file</param>
/// <param name="coord_digits_y">: number of fraction part of y of the number in the gerber file</param>
/// <param name="scale">: The inverse of the mm per pixel</param>
/// <param name="puiWidth">: Retern by referance(The output):The width of the gerber file(the width of the circuit)</param>
/// <param name="puiHeight">: Retern by referance(The output): The height of the gerber file(the width of the circuit)</param>
/// <param name="Shift_X">: Retern by referance(The output): The value of x axis that shift all the gerber values to the first quarter</param>
/// <param name="Shift_Y">: Retern by referance(The output): The value of y axis that shift all the gerber values to the first quarter</param>
void GetWidthHeight(
    HWND handeller_to_window,
    char* str,
    int coord_digits_x,
    int coord_digits_y,
    float scale,
    UINT& puiWidth,
    UINT& puiHeight,
    float& Shift_X,
    float& Shift_Y
);


//Old function to draw drill...now use DrawDrills_xln()
void DrawDrills_gerber(
    BYTE* pv,
    UINT cb_stride,
    char* str,
    int coord_digits_x,
    int coord_digits_y,
    float Shift_X,
    float Shift_Y,
    float scale
);

/// <summary>
/// Draw drills on the maxcupper as one pixel for drill
/// </summary>
/// <param name="pv">: Pointer to the array that represent a circuit</param>
/// <param name="cb_stride">: The stride of the bitmap that represent a circuit</param>
/// <param name="str">: String represent a drills Xln file </param>
/// <param name="coord_digits_x">: number of fraction part of x of the number in the gerber file</param>
/// <param name="coord_digits_y">: number of fraction part of y of the number in the gerber file</param>
/// <param name="Shift_X">: Retern by referance(The output): The value of x axis that shift all the gerber values to the first quarter</param>
/// <param name="Shift_Y">: Retern by referance(The output): The value of y axis that shift all the gerber values to the first quarter</param>
/// <param name="scale">: The inverse of the mm per pixel</param>
void DrawDrills_xln(
    BYTE* pv,
    UINT cb_stride,
    char* str,
    int coord_digits_x,
    int coord_digits_y,
    float Shift_X,
    float Shift_Y,
    float scale
);

/// <summary>
/// Scale and shift the drawing
/// </summary>
/// <param name="handeller_to_window">: Handeler to a window that refer to the window that have the drawing</param>
/// <param name="pRT">: Pointer to rendertarget that will be scalling</param>
/// <param name="scale_factor">: the factor that will be use to scall the draing</param>
/// <param name="offset_x">: The value of the drawing shifting in x axis</param>
/// <param name="offset_y">: The value of the drawing shifting in x axis</param>
void scale_translation(
    HWND handeller_to_window,
    ID2D1HwndRenderTarget* pRT,
    float scale_factor,
    float offset_x,
    float offset_y
);

/// <summary>
/// Repaint the image when resizing
/// </summary>
/// <param name="handeller_to_window">: Handeler to a window that refer to the window that have the drawing</param>
/// <param name="pRT">: Pointer to rendertarget that will be scalling</param>
/// <param name="D2DpBitmap">: Pointer to D2DpBitmap that will resize</param>
void Resize(HWND handeller_to_window,ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* D2DpBitmap);


/// <summary>
/// Mearge: scale_translation() and Resize()
/// </summary>
/// <param name="handeller_to_window">: Handeler to a window that refer to the window that have the drawing</param>
/// <param name="pRT">: Pointer to rendertarget that will be scalling</param>
/// <param name="scale_factor">: the factor that will be use to scall the draing</param>
/// <param name="offset_x">: The value of the drawing shifting in x axis</param>
/// <param name="offset_y">: The value of the drawing shifting in x axis</param>
/// <param name="D2DpBitmap">: pointer to D2DpBitmap that will resize</param>
void scaleResize(
    HWND handeller_to_window,
    ID2D1HwndRenderTarget* pRT,
    float scale_factor,
    float offset_x,
    float offset_y,
    ID2D1Bitmap* D2DpBitmap
);

/// <summary>
/// Draw borders before drawing the gerber files
/// </summary>
/// <param name="pWicRenderTarget">: Pointer to render target that will draw on wicBitmap</param>
/// <param name="pWicBrush">: pointer to the brush that has the color</param>
/// <param name="puiWidth">: Width of the bitmap</param>
/// <param name="puiHeight">: height of the bitmap</param>
void DrawBorders(
    ID2D1RenderTarget* pWicRenderTarget,
    ID2D1SolidColorBrush* pWicBrush,
    UINT puiWidth,
    UINT puiHeight
);

/// <summary>
/// Main function that parth the gerber filles and drills and draw thim on the wicBitmap
/// </summary>
/// <param name="handeller_to_window">: handeler to a window that refer to the window that have the drawing</param>
/// <param name="str">: String that have the gerber fille</param>
/// <param name="profileStr">: String that have the border fille</param>
/// <param name="DrillsStr">: String that have the drills fille</param>
/// <param name="scale">: The scale that represent the mmPerpexil</param>
/// <param name="xln_grb">Drills file format chose true to select xln format and false to select gerber format</param>
/// <returns></returns>
ID2D1Bitmap* DrawGerberOnBitmab(
    HWND handeller_to_window,
    char* str,
    char* profileStr,
    char* DrillsStr,
    float scale,
    /*ID2D1Factory* pFactory_,
    ID2D1RenderTarget* pWicRenderTarget_,
    IWICImagingFactory* wicFactory_,
    IWICBitmap* pBitmap_,
    ID2D1SolidColorBrush* pWicBrush,*/
    bool xln_grb = true
);


///without Maxcupper
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
    );



/// <summary>
/// Get the width and the height of the MMG file(the width and the height of circuit) and the shift x and y to draw the file in the first quarter
/// </summary>
/// <param name="commands">: Vector has the commands of MMG to get the maximum number to initialize the width and the height</param>
/// <param name="puiWidth">: Retern by referance(The output):width</param>
/// <param name="puiHeight">: Retern by referance(The output):heighth</param>
/// <param name="Shift_X">: Retern by referance(The output): The value of x axis that shift all the gerber values to the first quarter</param>
/// <param name="Shift_Y">: Retern by referance(The output): The value of y axis that shift all the gerber values to the first quarter</param>
/// <param name="high_level_Z">: Retern by referance(The output): The large level of Z</param>
/// <param name="low_level_Z">: Retern by referance(The output): The low level of Z</param>
/// <param name="scale">: The scale that represent the mmPerpexil</param>
void GetWidthHeightMMG(
    std::vector <CompressedCommand> commands,
    UINT& puiWidth, //output
    UINT& puiHeight, //output
    float& Shift_X,
    float& Shift_Y,
    float& high_level_Z,
    float& low_level_Z,
    float scale);

/// <summary>
/// Draw MMG commands on wicBitmap
/// </summary>
/// <param name="commands">: Vector has the commands of MMG to draw</param>
/// <param name="scale">: The scale that represent the mmPerpexil</param>
void DrawMMG(std::vector <CompressedCommand> commands, float scale = 0.1);

//Not test and not used
void DrawAceleration(std::vector <OutCommand> commands);

//tested but not used
void DrawLineOnWindow(float xStart, float yStart, float xEnd, float yEnd, float red, float green, float blue, float width);
//tested but not used
void DrawCirculeOnWindow(float xStart, float yStart, float radius, float red, float green, float blue);
//tested but not used
void BeginDrawOnWindow();
//tested but not used
void EndDrawOnWindow();

#endif // MAXCOPPER_H_INCLUDED
