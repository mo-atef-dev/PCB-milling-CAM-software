#include "tracebmp.h"

#warning "TO DOs in this file are still present"

int PixelMatrix::GetWidth()
{
    return w;
}

int PixelMatrix::GetHeight()
{
    return h;
}

TriState PixelMatrix::GetPixelState(int x, int y) const
{
    if(x > w-1 || y > h-1 || x < 0 || y < 0)
        return COPPER;
    return matrix[y][x];
}

int PixelMatrix::SetPixelState(int x, int y, TriState state)
{
    if(x > w-1 || y > h-1 || x < 0 || y < 0)
        return 1;
    matrix[y][x] = state;
    return 0;
}

PixelMatrix::PixelMatrix(int width, int height)
{
    h = height;
    w = width;
    matrix = new TriState*[h];
    for(int i = 0; i < h; i++)
    {
        matrix[i] = new TriState[w];
        for(int j = 0; j < w; j++)
        {
            matrix[i][j] = COPPER;
        }
    }
}

PixelMatrix::PixelMatrix(PixelMatrix& pm)
{
    w = pm.GetWidth();
    h = pm.GetHeight();
    matrix = new TriState*[h];
    for(int i = 0; i < h; i++)
    {
        matrix[i] = new TriState[w];
        for(int j = 0; j < w; j++)
        {
            matrix[i][j] = pm.GetPixelState(j, i);
        }
    }
}

PixelMatrix::PixelMatrix(bitmap_image img)
{
    w = img.width();
    h = img.height();
    const bitmap_image::rgb_t RGB_BLACK = {0, 0, 0};
    matrix = new TriState*[h];
    for(int i = 0; i < h; i++)
    {
        matrix[i] = new TriState[w];
        for(int j = 0; j < w; j++)
        {
            if(img.get_pixel(j, i) == RGB_BLACK)
                matrix[i][j] = ISOLATE;
        }
    }
}

PixelMatrix::~PixelMatrix()
{
    for(int i = 0; i < h; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

vector<Command> TracePixelMatrix(PixelMatrix* matrix, int zTop, int zBottom)
{
    vector<Command> commands;

    int curX = 0;
    int curY = 0;

    int tempX = 0;
    int tempY = 0;

    // TO DO: Add mill turning and feed commands

    // Add ascending command
    commands.push_back(Command(MOVE, 0, 0, zTop));

    while(RadialScan(matrix, curX, curY, &tempX, &tempY))
    {
        curX = tempX; curY = tempY;
        int result = TraceStart(matrix, curX, curY, &tempX, &tempY);

        if(result != 0)
        {
            cout << "Error in TraceStart function with code: " << result << endl;
        }

        // Go to command to initial position
        commands.push_back(Command(MOVE, tempX, tempY, zTop));
        matrix->SetPixelState(tempX, tempY, CHECKED);

        // Descend with the mill
        commands.push_back(Command(LINE, tempX, tempY, zBottom));

        // Create a path carving command set
        vector<Command> commandsTmp = TracePixel(matrix, tempX, tempY);
        commands.insert(commands.end(), commandsTmp.begin(), commandsTmp.end());

        // Set the current coordinates to the coordinates of the last command
        curX = ( *(--commands.end()) ).GetX();
        curY = ( *(--commands.end()) ).GetY();

        // NOTE: Ascending with the mill is now included in the TracePixel function to support legacy TracePixelMatrix function
    }

    return commands;
}

int TraceStart(PixelMatrix* matrix, int x, int y, int* xx, int* yy)
{
    // TO DO: fix bad practices in this function

    cout << "Tracing the start of a path......" << endl;

    Direction curDir;
    Direction prevDir;

    // Preserve initial coordinates
    int startX = x;
    int startY = y;

    // Create a copy of the input matrix to prevent alteration
    PixelMatrix pm1(*matrix);

    // Store x and y coordinates of each trace iteration
    vector<int> endX;
    vector<int> endY;

    // Loop which checks the starting pixel repeatedly for paths in each direction.
    // When the end of a path is reached, the coordinates of the end is stored in the above vectors.
    while(InitDirection(&pm1, startX, startY).Zero() == false)
    {
        while(1)
        {
            // Get first direction
            curDir = InitDirection(&pm1, x, y);

            if(curDir.Zero())
                break;

            // Loop until a change of direction or the end of the line
            TriState curState = pm1.GetPixelState(x, y);
            TriState nextState = pm1.GetPixelState(x+curDir.x, y+curDir.y);
            while(nextState == ISOLATE || (nextState == CHECKED && curState == ISOLATE))
            {
                x += curDir.x; y+= curDir.y;
                pm1.SetPixelState(x, y, CHECKED);
                curState = nextState;
                nextState = pm1.GetPixelState(x+curDir.x, y+curDir.y);
            }
        }

        // Store the end point
        endX.push_back(x);
        endY.push_back(y);

        // Restore preserved starting coordinates
        x = startX;
        y = startY;
    }

    // Calculate the minimum distance between the starting point and each calculated end
    if(endX.size() == 0)
    {
        cout << "### Error in tracing a path start: no end points are detected." << endl;
        return 1;
    }

    int disX = endX[0] - x;
    int disY = endY[0] - y;
    int minSqDis = disX*disX + disY*disY;
    *xx = endX[0]; *yy = endY[0];

    for(int i = 0; i < endX.size(); i++)
    {
        if((endX[i]*endX[i] + endY[i]*endY[i]) < minSqDis)
        {
            int disX = endX[i] - x;
            int disY = endY[i] - y;
            minSqDis = disX*disX + disY*disY;
            *xx = endX[i]; *yy = endY[i];
        }
    }

    cout << "Finished tracing the start" << endl;

    cout << "*xx = "<< (*xx) << endl;
    cout << "*yy = "<< (*yy) << endl;

    return 0;
}

int RadialScan(PixelMatrix* matrix, int x, int y, int* xx, int* yy)
{
    // Determine the max radius based on the distance to the edge
    int pdx = matrix->GetWidth() - x;
    int ndx = x;
    int pdy = matrix->GetHeight() - y;
    int ndy = y;

    cout << "Starting a radial scan" << endl;

    int maxR = pdx;

    if(ndx > maxR)
        maxR = ndx;
    if(pdy > maxR)
        maxR = pdy;
    if(ndy > maxR)
        maxR = ndy;

    // Start searching in a rectangular fashion and return once a pixel is found
    int curR;
    for(curR = 0; curR < maxR; curR++)
    {
        //cout << "curR = " << curR << ", maxR = " << maxR << endl;
        int i = x - curR;
        int j = y - curR;

        for(; i <= (int)x + curR; i++)
        {
            if(matrix->GetPixelState(i, j) == ISOLATE)
            {
                *xx = i;
                *yy = j;
                return 1;
            }
        }

        for(; j <= (int)y + curR; j++)
        {
            if(matrix->GetPixelState(i, j) == ISOLATE)
            {
                *xx = i;
                *yy = j;
                return 1;
            }
        }

        for(; i >= (int)x - curR; i--)
        {
            if(matrix->GetPixelState(i, j) == ISOLATE)
            {
                *xx = i;
                *yy = j;
                return 1;
            }
        }

        for(; j >= (int)y - curR; j--)
        {
            if(matrix->GetPixelState(i, j) == ISOLATE)
            {
                *xx = i;
                *yy = j;
                return 1;
            }
        }
    }
    return 0;
}

int Clearance(PixelMatrix* matrix, int x, int y)
{

}

vector<Command> TracePixel(PixelMatrix* matrix, int x, int y, int zTop, int zBottom)
{
    // TO DO: fix bad practices in this function

    Direction curDir;
    Direction prevDir;

    vector<Command> commands;

    while(1)
    {
        // Get first direction
        curDir = InitDirection(matrix, x, y);

        if(curDir.Zero())
            break;

        // Loop until a change of direction or the end of the line
        TriState curState = matrix->GetPixelState(x, y);
        TriState nextState = matrix->GetPixelState(x+curDir.x, y+curDir.y);
        while(nextState == ISOLATE || (nextState == CHECKED && curState == ISOLATE))
        {
            x += curDir.x; y+= curDir.y;
            matrix->SetPixelState(x, y, CHECKED);
            curState = nextState;
            nextState = matrix->GetPixelState(x+curDir.x, y+curDir.y);
        }

        // Add the "line to" command once the direction change is detected
        commands.push_back(Command(LINE, x, y, zBottom));
    }

    // Ascend with the mill
    commands.push_back(Command(LINE, x, y, zTop));

    return commands;
}

Direction InitDirection(const PixelMatrix* matrix, int x, int y)
{
    int dx = 1;
    int dy = 0;


    // This section basically checks the 8 surrounding pixels in a clockwise fashion and
    // returns the direction of the first detected correct path.
    // TO DO: Replace this section with a loop algorithm to make the code more compact.
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = 1; dy = -1;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = 0; dy = -1;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = -1; dy = -1;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = -1; dy = 0;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = -1; dy = 1;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = 0; dy = 1;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    dx = 1; dy = 1;
    if(matrix->GetPixelState(x+dx, y+dy) == ISOLATE)
        return Direction(dx, dy);

    return Direction();
}

vector<Command> SimplifyCommandsXY(vector<Command> commands)
{
    #warning "This function is missing critical functionality"

    if(commands.size() < 4)
    {
        return commands;
    }

    vector<Command> simpCommands;
    simpCommands.push_back(commands[0]);

    // For checking the one pixel condition
    signed char prevDeltaX = 0;
    signed char prevDeltaY = 0;

    // True during a routine of simplification
    bool simplifying = false;

    int i = 0;
    while(i+3 < commands.size())
    {
        // Set starting X and Y
        int startX = commands[i].GetX();
        int startY = commands[i].GetY();

        // Read command type
        CmdType type = commands[i].GetType();

        Direction dir3;
        Direction dir2;
        Direction dir1;

        while(i+3 < commands.size())
        {
            // Context for understanding comments
            // L1 is line from command i to i+1
            // L2 is line from command i+1 to i+2
            // L3 is line from command i+2 to i+3

            dir3.DirectionFromCommands(commands[i+2], commands[i+3]);
            dir2.DirectionFromCommands(commands[i+1], commands[i+2]);
            dir1.DirectionFromCommands(commands[i], commands[i+1]);

            // Check L1, L2, L3 command types
            bool condType = true;
            if(commands[i+1].GetType() != type || commands[i+2].GetType() != type || commands[i+3].GetType() != type)
            {
                condType = false;
                cout << "Condition failed: Same type" << endl;
            }

            // Check Commands i to i+3 have the same z
            bool condZ = true;
            if(commands[i].GetZ() != commands[i+1].GetZ() || commands[i].GetZ() != commands[i+2].GetZ() || commands[i].GetZ() != commands[i+3].GetZ())
            {
                condZ = false;
                cout << "Condition failed: Same Z" << endl;
            }


            // Check L2 having only a single direction change
            bool condSingleDirection = true;
            signed char deltaDirX = dir2.x - dir1.x;
            signed char deltaDirY = dir2.y - dir1.y;

            if((deltaDirX && deltaDirY) || (!deltaDirX && !deltaDirY))
            {
                condSingleDirection = false;
                cout << "Condition failed: Single direction" << endl;
            }

            // Check L2 having only a 1 pixel long movement
            /// Final program should check either L2 having 1 pixel length or both L1 and L3
            bool condOnePixel = true;
            int deltaX = commands[i+2].GetX() - commands[i+1].GetX();
            int deltaY = commands[i+2].GetY() - commands[i+1].GetY();
            if(deltaX > 1 || deltaY > 1 || deltaX < -1 || deltaY < -1)
            {
                condOnePixel = false;
                cout << "Condition failed: One pixel" << endl;
            }

            if(simplifying)
            {
                if(prevDeltaX != deltaX || prevDeltaY != deltaY)
                {
                    condOnePixel = false;
                }
            }
            else
            {
                prevDeltaX = deltaX;
                prevDeltaY = deltaY;
            }

            // Check L1 and L3 having the same direction
            bool condSameDir = true;
            if(dir3.x != dir1.x || dir3.y != dir1.y)
            {
                condSameDir = false;
                cout << "Condition failed: Same direction" << endl;
            }

            // Check L1 and L3 having the same length
            /// At the current state this condition makes the same direction condition redundant
            bool condSameLength = true;

            int deltaX1 = commands[i+1].GetX() - commands[i].GetX();
            int deltaY1 = commands[i+1].GetY() - commands[i].GetY();

            int deltaX2 = commands[i+3].GetX() - commands[i+2].GetX();
            int deltaY2 = commands[i+3].GetY() - commands[i+2].GetY();

            if(deltaX1 != deltaX2 || deltaY1 != deltaY2)
            {
                condSameLength = false;
            }

            // If all conditions are true:
            // Step to the next 3 lines where new L1 = old L3
            if(condOnePixel && condSameDir && condSingleDirection && condType && condZ && condSameLength)
            {
                i+=2;
                simplifying = true;
            }


            // Else:
            // Make a command of the same type from starting X and Y to X and Y of L1
            else
            {
                simpCommands.push_back(commands[i+1]);
                cout << "Added a command to the simplified list" << endl;
                i++;
                simplifying = false;
                break;
            }
        }
    }

    for(; i+1 < commands.size(); i++)
    {
        simpCommands.push_back(commands[i+1]);
    }

    return simpCommands;
}

string CommandsString(vector<Command> commands, float scale, bool negativeY)
{
    string cmdstring;
    for(vector<Command>::iterator i = commands.begin(); i != commands.end(); ++i)
    {
        cmdstring += CommandString(*i, scale, negativeY);
    }
    return cmdstring;
}

string CommandString(Command command, float scale, bool negativeY)
{
    string s;
    switch(command.GetType())
    {
    case MOVE:
        s += "G00 ";
        break;
    case LINE:
        s += "G01 ";
        break;
    case TURNSPINDLE:
        s += "M03 ";
        break;
    }
    s += "X "; s += to_string(command.GetX()*scale);
    s += " Y "; s += to_string(command.GetY()*scale*(1 - 2*negativeY));
    s += " Z "; s += to_string(command.GetZ()*scale); s+= '\n';
    return s;
}

#warning "Add debug compiler switches to these functions and their required includes"

void PrintCommands(vector<Command> commands, float scale)
{
    for(vector<Command>::iterator i = commands.begin(); i != commands.end(); ++i)
    {
        PrintCommand(*i, scale);
    }
}

void PrintCommand(Command command, float scale)
{
    switch(command.GetType())
    {
    case MOVE:
        cout << "G00 ";
        break;
    case LINE:
        cout << "G01 ";
        break;
    case TURNSPINDLE:
        cout << "M03 ";
        break;
    }
    cout << "X " << command.GetX()*scale << " Y " << command.GetY()*scale << " Z " << command.GetZ()*scale << endl;
}

Direction::Direction(Command first, Command second)
{
    x = 0 + (second.GetX() > first.GetX()) - (second.GetX() < first.GetX());
    y = 0 + (second.GetY() > first.GetY()) - (second.GetY() < first.GetY());
}

void Direction::DirectionFromCommands(Command first, Command second)
{
    x = 0 + (second.GetX() > first.GetX()) - (second.GetX() < first.GetX());
    y = 0 + (second.GetY() > first.GetY()) - (second.GetY() < first.GetY());
}
