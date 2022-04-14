#include "Header.h"

///////////////////////////////////////////
// globle variables

//for graphics:
ID2D1Factory* pFactory = NULL;
ID2D1HwndRenderTarget* pRenderTarget;
ID2D1SolidColorBrush* pBrush;

D2D1_ELLIPSE ellipse;	//has two radius and point(x,y)
D2D1_ELLIPSE ellipse_array[1000];
int ellipse_counter = 0;

float current_x = 0;
float current_y = 0;
float shape_x;
float shape_y;
//let num = 3 as defult value
int num_integers_in_coordinate_x = 3;
int num_integers_in_coordinate_y = 3;

//assume 10^-6 mm mode
int coord_digits_x = 6;
int coord_digits_y = 6;
//assume inatial value of scaling = 10
float scale = 10;
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

all_standerd_aperture all_shapes;

aperture* current_shape;

//////////////////////////////////////////

int creat_factory()
{
	if (FAILED(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	{
		return -1;
	}
}


HRESULT create_graphics_resources(HWND handeller_to_window)
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(handeller_to_window, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(handeller_to_window, size),
			&pRenderTarget);
		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		}
	}
	return hr;
}

void on_paint(HWND handeller_to_window, float x, float y, float radius)
{
	HRESULT hr = create_graphics_resources(handeller_to_window);
	if (SUCCEEDED(hr))
	{
		/*PAINTSTRUCT ps;
		BeginPaint(handeller_to_window, &ps);*/
		///pRenderTarget->BeginDraw();

		//for background
		//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

		//need to edite this
		//ellipse_array[ellipse_counter] = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
		//pRenderTarget->FillEllipse(ellipse_array[ellipse_counter], pBrush);
		//ellipse_counter++;

		pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius),pBrush);
		///hr = pRenderTarget->EndDraw();
        /*
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			pRenderTarget->Release();
			pBrush->Release();
		}
		EndPaint(handeller_to_window, &ps);*/

		current_x = shape_x;
		current_y = shape_y;
	}
}

//Resize
void Resize(HWND handeller_to_window,char* szFileBuffer,float scale_factor,float  offset_x,float offset_y)
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

	if(szFileBuffer)
        draw(handeller_to_window, szFileBuffer, scale_factor,  offset_x, offset_y);

}

void draw_circle(HWND handeller_to_window, float x, float y, float radius)
{
	//creat_factory();
	on_paint(handeller_to_window, x, y, radius);
}

void draw_line(HWND handeller_to_window, float start_x, float start_y, float end_x, float end_y)
{
	//creat_factory();
	HRESULT hr = S_OK;
	//create_graphics_resources function
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(handeller_to_window, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(handeller_to_window, size),
			&pRenderTarget);
		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		}
	}
	//on paint function
	if (SUCCEEDED(hr))
	{
		/*PAINTSTRUCT ps;
		BeginPaint(handeller_to_window, &ps);*/
		///pRenderTarget->BeginDraw();

		//for background
		//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		start_x = current_x;
		start_y = current_y;
		end_x = shape_x;
		end_y = shape_y;


		//D2D1_POINT_2F start_point = D2D1::Point2F(start_x, start_y);
		//D2D1_POINT_2F end_point = D2D1::Point2F(end_x, end_y);
		//line_width = diameter of current shape
		if (current_shape->diameter == 0.0)
		{
			//pRenderTarget->DrawLine(start_point, end_point, pBrush, 0.5F);
			pRenderTarget->DrawLine(D2D1::Point2F(start_x, start_y), D2D1::Point2F(end_x, end_y), pBrush, .5F);
		}
		else
		{
			//pRenderTarget->DrawLine(start_point, end_point, pBrush, current_shape->diameter);
			pRenderTarget->DrawLine(D2D1::Point2F(start_x, start_y), D2D1::Point2F(end_x, end_y), pBrush, current_shape->diameter);

		}
		///hr = pRenderTarget->EndDraw();
        /*
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			pRenderTarget->Release();
			pBrush->Release();
		}
		EndPaint(handeller_to_window, &ps);*/

		current_x = shape_x;
		current_y = shape_y;

	}
}

void draw_rectangle(HWND handeller_to_window)
{
	//creat_factory();
	//no need for x,y,radius
	float x = 0;
	float y = 0;
	float radius = 0;

	HRESULT hr = create_graphics_resources(handeller_to_window);
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(handeller_to_window, &ps);
		pRenderTarget->BeginDraw();
		//for background
		//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

    char arr[100];
	sprintf_s(arr, 99, "\ncurrent_shape->y / 2:%f ", current_shape->y / 2);
	MessageBoxA(handeller_to_window, arr, " current_shape->y / 2", MB_OK);
	 char arrr[100];
	sprintf_s(arr, 99, "\nshape_y: %f", shape_y );
	MessageBoxA(handeller_to_window, arrr, "shape_y ", MB_OK);

		pRenderTarget->DrawRectangle(D2D1::RectF(
			shape_x - current_shape->x / 2,
			shape_y - current_shape->y / 2,
			shape_x + current_shape->x / 2,
			shape_y + current_shape->y / 2),
			pBrush);
		hr = pRenderTarget->EndDraw();

		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			pRenderTarget->Release();
			pBrush->Release();
		}
		EndPaint(handeller_to_window, &ps);
		current_x = shape_x;
		current_y = shape_y;
	}
}

void draw_polygon()
{/*
	ID2D1GeometrySink* pSink = NULL;
	//creat_factory();
	// Create a path geometry.
	HRESULT hr = S_OK;
	ID2D1PathGeometry* m_pPathGeometry;
	if (SUCCEEDED(hr))
	{
		creat_factory();

		hr = pFactory->CreatePathGeometry(&m_pPathGeometry);

		if (SUCCEEDED(hr))
		{
			// Write to the path geometry using the geometry sink.
			hr = m_pPathGeometry->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(
					D2D1::Point2F(shape_x + current_shape->diameter / 2, shape_y),
					D2D1_FIGURE_BEGIN_FILLED
				);

				//(x,y) = (x+r*cos(pi/4*n) , y+r*sin(pi/4*n)); n = 0,1,2,3,4,5,6,7
				//pSink->AddLine(D2D1::Point2F(shape_x + current_shape->diameter / 2, shape_y));
				pSink->AddLine(D2D1::Point2F(shape_x + (current_shape->diameter / 2) / sqrt(2), (shape_y + current_shape->diameter / 2) / sqrt(2)));
				pSink->AddLine(D2D1::Point2F(shape_x, shape_y + current_shape->diameter / 2));
				pSink->AddLine(D2D1::Point2F(shape_x - (current_shape->diameter / 2) / sqrt(2), (shape_y + current_shape->diameter / 2) / sqrt(2)));
				pSink->AddLine(D2D1::Point2F(shape_x - current_shape->diameter / 2, shape_y));
				pSink->AddLine(D2D1::Point2F(shape_x - (current_shape->diameter / 2) / sqrt(2), (shape_y - current_shape->diameter / 2) / sqrt(2)));
				pSink->AddLine(D2D1::Point2F(shape_x, shape_y - current_shape->diameter / 2));
				pSink->AddLine(D2D1::Point2F(shape_x + (current_shape->diameter / 2) / sqrt(2), (shape_y - current_shape->diameter / 2) / sqrt(2)));

				pSink->AddBezier(
					D2D1::BezierSegment(
						D2D1::Point2F(shape_x, shape_y - current_shape->diameter / 2),
						D2D1::Point2F(shape_x + (current_shape->diameter / 2) / sqrt(2), (shape_y + current_shape->diameter / 2) / sqrt(2)),
						D2D1::Point2F(shape_x + (current_shape->diameter / 2) / sqrt(2), (shape_y - current_shape->diameter / 2) / sqrt(2)) )
				);

				//pSink->AddLine(D2D1::Point2F(0, 200));

				pSink->AddBezier(
					D2D1::BezierSegment(
						D2D1::Point2F(50, 150),
						D2D1::Point2F(50, 50),
						D2D1::Point2F(0, 0))
				);

				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				hr = pSink->Close();
				pRenderTarget->DrawGeometry(m_pPathGeometry, pBrush, 10.F);
			}


			//SafeRelease(&pSink);
		}
	}

*/}

void string_to_float(HWND handeller_to_window,char* str, int index, float& x_cord, float& y_cord)
{
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
		if(str[index]=='-')
        {
            result =result * -1;
            index--;
        }

		if (str[index] == 'Y')
		{
			result = result * scale;
			y_cord = result * pow(10, -coord_digits_y);
			num = 0;
			i = 0;
			result = 0;
			index--;

			//get the window cordinates
			RECT rc;
			GetClientRect(handeller_to_window, &rc);
			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
			y_cord = size.height - y_cord;
			//y_cord = size.height/2 - y_cord;
		}
		else if (str[index] == 'X')
		{
			result = result * scale;
			x_cord = result * pow(10, -coord_digits_x);
			num = 0;
			i = 0;
			result = 0;
			index--;

			//get the window cordinates
			RECT rc;
			GetClientRect(handeller_to_window, &rc);
			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            //x_cord = size.width/2 + x_cord;

		}

	}

}

void scale_translation(HWND handeller_to_window,float scale_factor,float offset_x,float offset_y)
{
	//get the size of the window
	RECT rc;
	GetClientRect(handeller_to_window, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
	//scale the shapes from the center of window
	pRenderTarget->SetTransform(
		D2D1::Matrix3x2F::Scale(scale_factor,
                          scale_factor,
			D2D1::Point2F(rc.right/2-offset_x, rc.bottom/2-offset_y) )*D2D1::Matrix3x2F::Translation(offset_x, offset_y)
	);

}

void draw(HWND handeller_to_window, char* str, float scale_factor, float offset_x,float offset_y)
{
    //create factory called in main program
    //for drawing
    create_graphics_resources(handeller_to_window);
    //scal&translation
	scale_translation(handeller_to_window, scale_factor, offset_x, offset_y);

	// Draw a black window
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	//pRenderTarget->EndDraw();

    ///
    HRESULT hr = S_OK;
	/*PAINTSTRUCT ps;
    BeginPaint(handeller_to_window, &ps);*/
    //pRenderTarget->BeginDraw();

	int i = 0;
	while (1)
	{
		//end of file
		if (str[i] == 'M' && str[i + 1] == '0' && str[i + 2] == '2')
		{
		    hr = pRenderTarget->EndDraw();
			return;
		}
		//draw line or arch
		else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '1')
		{
			string_to_float(handeller_to_window,str, i, shape_x, shape_y);
			draw_line(handeller_to_window, current_x, current_y, shape_x, shape_y);
			i = i + 3;
		}
		//only move to a certain point
		else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '2')
		{
			string_to_float(handeller_to_window,str, i, current_x, current_y);
			i = i + 3;
		}
		//creart flash (replicating) the current aperture
		else if (str[i] == 'D' && str[i + 1] == '0' && str[i + 2] == '3')
		{
			string_to_float(handeller_to_window,str, i, shape_x, shape_y);
			//for Circle
			if (current_shape->type == 1)
			{
				draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
			}
			//for Rectangle
			else if (current_shape->type == 2)
			{
				//MessageBox(handeller_to_window, L"test", L"test", MB_OK);
				string_to_float(handeller_to_window,str, i, shape_x, shape_y);
				draw_rectangle(handeller_to_window);
			}
			//for Obround
			else if (current_shape->type == 3)
			{

			}
			//for Polygon
			else if (current_shape->type == 4)
			{
				/*char arrr[100];
				sprintf_s(arrr, 99, "\nX:%f", current_shape->diameter);
				MessageBoxA(handeller_to_window, arrr, "current_shape->diameter", MB_OK);*/
				//draw_polygon();
				// draw polygon as circle
				draw_circle(handeller_to_window, shape_x, shape_y, (current_shape->diameter) / 2);
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
				dot_index=0;
				while (str[index_y] != '.')
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
				}
				result = result * scale;
				y = result;

				//let the diameter has one digit for x and one digit for y
				i = i + 11;

                char arrr[100];
				sprintf_s(arrr, 99, "\ny : %f", y);
				MessageBoxA(handeller_to_window, arrr, "y", MB_OK);
                //X30000000Y15000000D03

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
				int num = 0;
				float result = 0;
				int exponent = 0;
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
					//start of y
					index_y = dot_index + j + 1;
				}
				result = result * scale;
				x = result;
				//x = result * pow(10, -coord_digits_x);

				//second number (y)
				while (str[index_y] != '.')
				{
					index_y++;
				}
				dot_index = index_y;
				num = 0;
				result = 0;
				exponent = 0;
				while (str[j] != ',')
				{
					num = (float)(str[j] - '0') * pow(10, exponent);
					result = result + num;
					index_y--;
					exponent++;
				}
				exponent = -1;
				j = 1;
				while (str[dot_index + index_y] != '*' && str[dot_index + index_y] != 'X')
				{
					num = (float)(str[dot_index + index_y] - '0') * pow(10, exponent);
					result = result + num;
					index_y++;
					exponent--;
				}
				result = result * scale;
				y = result;
				//y = result * pow(10, -coord_digits_y);

				//let the diameter has one digit for x and one digit for y
				i = i + 11;
				all_shapes.add_aperture(3, num_shape, 0, x, y, 0, hole_diameter);
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
					vertices=(float)(str[index_y] - '0') * 10 + (float)(str[index_y+1] - '0') * 1;
				}
				else if(str[index_y +1] == 'X')
				{
					vertices=(float)(str[index_y] - '0') * 1;
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

				/*
				char arr[100];
				sprintf_s(arr, 99, "\ndiameter:%f", diameter);
				MessageBoxA(handeller_to_window, arr, "End of function diameter", MB_OK);
				char arrr[100];
				sprintf_s(arrr, 99, "\nvertices:%f", vertices);
				MessageBoxA(handeller_to_window, arrr, "End of function vertices", MB_OK);
				*/
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

    ///


   /* if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
        pRenderTarget->Release();
        pBrush->Release();
    }
    EndPaint(handeller_to_window, &ps);*/
    pRenderTarget->EndDraw();

}

/*char arr[100];
	sprintf_s(arr, 99, "\nY:%f", y_cord);
	MessageBoxA(handeller_to_window, arr, "End of function Y", MB_OK);
	char arrr[100];
	sprintf_s(arrr, 99, "\nX:%f", x_cord);
	MessageBoxA(handeller_to_window, arrr, "End of function X", MB_OK);*/
