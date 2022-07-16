#include "tracecheck.h"

int BriefCheck(vector<Command> commands, bitmap_image img)
{
    return DetailedCheck(commands, img).code;
}

CheckResult DetailedCheck(vector<Command> commands, bitmap_image img)
{
    if(!img)
        return {-1, 0, 0, 0, 0, bitmap_image(1, 1)};

    bitmap_image img2(img.width(), img.height());
    img2.set_all_channels(255);

    CheckResult r = {0, 0, 0, 0, 0, img2};

    int x = 0;
    int y = 0;
    int z = 0;

    // This loop creates the new image based on the commands
    for(vector<Command>::iterator i = commands.begin(); i != commands.end(); ++i)
    {
        #if defined(DEBUG)
        cout << "Started command: " << CommandString((*i)) << endl;
        #endif

        if((*i).GetType() == MOVE)
        {
            x = (*i).GetX();
            y = (*i).GetY();
            z = (*i).GetZ();
            r.nGoTo ++;
        }
        else if((*i).GetType() == LINE)
        {
            int dx = (*i).GetX() - x;
            int dy = (*i).GetY() - y;
            int xi = x;
            int yi = y;

            #if defined(DEBUG)
            ///----------------DEBUG---------------------

            cout << "dx = " << dx << ", dy = " << dy << endl;

            ///------------------------------------------
            #endif

            while(x != (*i).GetX() || y != (*i).GetY() || z != (*i).GetZ())
            {
                if(dx == 0 || dy == 0)
                {
                    x += GET_DIR((*i).GetX(), x);
                    y += GET_DIR((*i).GetY(), y);
                    z += GET_DIR((*i).GetZ(), z);
                    if(z < 0)
                    {
                        rgb_t white = {255, 255, 255};
                        if(r.img.get_pixel(x, y) != white)
                        {
                            r.img.set_pixel(x, y, 0, 255, 0);

                            #if defined(DEBUG)
                            cout << "Interleaved pixel\n";
                            #endif
                        }
                        else
                            r.img.set_pixel(x, y, (r.nLineTo%2)*255, 0, 0);
                    }

                    continue;
                }

                // In case dy and dx are not zero
                if(abs(dx) > abs(dy))
                {
                    x += GET_DIR((*i).GetX(), x);
                    y = yi + dy*(x-xi)/dx;
                }
                else
                {
                    y += GET_DIR((*i).GetY(), y);
                    x = xi + dx*(y-yi)/dy;
                }

                z += GET_DIR((*i).GetZ(), z);
                if(z < 0)
                {
                    rgb_t white = {255, 255, 255};
                    if(r.img.get_pixel(x, y) != white)
                    {
                        r.img.set_pixel(x, y, 0, 255, 0);
                        #if defined(DEBUG)
                        cout << "Interleaved pixel\n";
                        #endif
                    }
                    else
                        r.img.set_pixel(x, y, (r.nLineTo%2)*255, 0, 0);
                }
            }

            /*
            while(x != (*i).GetX() || y != (*i).GetY() || z != (*i).GetZ())
            {
                x += GET_DIR((*i).GetX(), x);
                y += GET_DIR((*i).GetY(), y);
                z += GET_DIR((*i).GetZ(), z);
                if(z == END_DEPTH)
                    r.img.set_pixel(x, y, 0, 0, 0);
            }
            */
            r.nLineTo ++;
        }
    }

    // This loop checks the equality of the two images
    for(int i = 0; i < img.width(); i++)
    {
        for(int j = 0; j < img.height(); j++)
        {
            if(img.get_pixel(i, j) != r.img.get_pixel(i, j))
            {
                r.nErrors ++;
                r.code = -2;
            }
        }
    }

    r.nTotal = r.nGoTo + r.nLineTo;

    return r;
}
