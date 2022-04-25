#ifndef TRACEBMP_H_INCLUDED
#define TRACEBMP_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>

#include "bitmap_image.hpp"

using namespace std;

#warning "This file is missing some documentation, please add it when you can."
#warning "Depth preprocesser constant is defined arbitrarly and should be changed."
#warning "PixelMatrix class is missing house keeping functions."

/// Legacy constants to indicate start and end z positions. Currently, they are replaced by arguments to the functions TracePixelMatrix and TracePixel
#define END_DEPTH -10
#define START_DEPTH 100

enum TriState{COPPER, ISOLATE, CHECKED};
enum CmdType{MOVE, LINE, TURNSPINDLE};

class PixelMatrix
{
private:
    TriState** matrix;
    int w, h;
public:
    int GetWidth();
    int GetHeight();

    TriState GetPixelState(int x, int y) const;
    int SetPixelState(int x, int y, TriState state);

    PixelMatrix(int width, int height);
    PixelMatrix(bitmap_image image);
    PixelMatrix(PixelMatrix& pm);
    ~PixelMatrix();
};

class Command
{
private:
    CmdType type;
    int x, y, z;    // Absolute coordinates
public:
    Command(CmdType type, int x, int y, int z) : type(type), x(x), y(y), z(z)
    {

    }
    int GetX()
    {
        return x;
    }
    int GetY()
    {
        return y;
    }
    int GetZ()
    {
        return z;
    }
    CmdType GetType()
    {
        return type;
    }
};

class Direction
{
public:
    signed char x;
    signed char y;

    Direction() : x(0), y(0)
    {}

    Direction(signed char xx, signed char yy) : x(xx), y(yy)
    {}

    Direction(Command first, Command second);

    void DirectionFromCommands(Command first, Command second);

    bool Zero()
    {
        return (x == 0) && (y == 0);
    }
};

/** \brief This function returns a command array that traces the input pixel matrix.
 *
 * \param matrix PixelMatrix* The input pixel matrix to be traced
 * \return vector<Command> The resulting command array.
 *
 */
vector<Command> TracePixelMatrix(PixelMatrix* matrix, int zTop = 100, int zBottom = -10);

/** \brief This function returns a command array that traces a path of connected pixels
 * starting at the pixel identified by the input.
 *
 * \param matrix PixelMatrix* The pixel matrix that includes the pixel path.
 * \param x int The x coordinate of the starting pixel in the path.
 * \param y int The y coordinate of the starting pixel in the path.
 * \return vector<Command> The resulting command array.
 *
 */
vector<Command> TracePixel(PixelMatrix* matrix, int x, int y, int zTop = 100, int zBottom = -10);

vector<Command> SimplifyCommandsXY(vector<Command> commands);

/** \brief Traces a pixel path to one of its ends and returns end coordinates.
 *
 * \param matrix PixelMatrix* The pixel matrix that includes the pixel path.
 * \param x int The x coordinate of a pixel in the path.
 * \param y int The y coordinate of a pixel in the path.
 * \param xx int* Integer to store the resulting position x coordinate.
 * \param yy int* Integer to store the resulting position y coordinate.
 * \return int Status code indicating the result of the operation (0 is successful).
 *
 */
int TraceStart(PixelMatrix* matrix, int x, int y, int* xx, int* yy);

/** \brief Obtains the direction to a connected path pixel from a given pixel. Priority
 * starts with the top direction and rotates 45 degrees an a clockwise fashion.
 *
 * \param matrix const PixelMatrix* The pixel matrix that includes the pixel path.
 * \param x int The x coordinate of the source pixel.
 * \param y int The y coordinate of the source pixel.
 * \return Direction The direction to the next pixel in the path (if exists). Returns
 * (0, 0) if no connected pixels exist.
 *
 */
Direction InitDirection(const PixelMatrix* matrix, int x, int y);

int RadialScan(PixelMatrix* matrix, int x, int y, int* xx, int* yy);

int Clearance(PixelMatrix* matrix, int x, int y);

string CommandsString(vector<Command> commands, float scale = 0.1f, bool negativeY = false);

string CommandString(Command commands, float scale = 0.1f, bool negativeY = false);

void PrintCommands(vector<Command> commands, float scale = 0.1f);

void PrintCommand(Command command, float scale = 0.1f);

#endif // TRACEBMP_H_INCLUDED
