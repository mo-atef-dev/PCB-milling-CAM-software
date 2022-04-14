#pragma once
#include <shobjidl.h>
//for graphics Direct2D
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <math.h>
#include <stdio.h>

int creat_factory();
HRESULT create_graphics_resources(HWND handeller_to_window, float x, float y, float radius);
void on_paint(HWND handeller_to_window, float x, float y, float radius);
void draw_circle(HWND handeller_to_window, float x, float y, float radius);
void draw_line(HWND handeller_to_window, float start_x, float start_y, float end_x, float end_y);
void draw_rectangle(HWND handeller_to_window);
void string_to_float(HWND handeller_to_window, int index, float& x_cord, float& y_cord);
void draw(HWND handeller_to_window, char* str, float scale_factor, float offset_x,float offset_y);
//need to create
void draw_polygon();
void Resize(HWND handeller_to_window,char* szFileBuffer,float scale_factor,float  offset_x,float offset_y);
void scale_translation(HWND handeller_to_window, float scale_factor, float offset_x,float offset_y);

