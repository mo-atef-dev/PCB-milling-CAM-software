#include <iostream>
#include <stdio.h>
#include<string>
#include<string.h>
#include<vector>
#include"mmgparse.h"
using namespace std;


void data(char line[],float*xx,float*yy,float*zz)
{
    float next_x,next_y,next_z,ff;
    char *s = strchr(line,'F');
    if (line[0] == 'G')
    {
        if (line[2] == '0' && s == NULL)
        {
            sscanf(line, "G00 X %f Y %f Z %f", &next_x, &next_y, &next_z);
        }
        else if (line[2] == '1' && s == NULL)
        {
            sscanf(line, "G01 X %f Y %f Z %f", &next_x, &next_y, &next_z);
        }
        else if (line[2] == '0' && s != NULL)
        {
            sscanf(line, "G00 X %f Y %f Z %f F %f", &next_x, &next_y, &next_z,&ff);
        }
        else if (line[2] == '1' && s != NULL)
        {
            sscanf(line, "G01 X %f Y %f Z %f F %f", &next_x, &next_y, &next_z,&ff);
        }
    }
    // cout << next_x << " " << next_y << " " << next_z << endl;
    *xx = next_x;
    *yy = next_y;
    *zz = next_z;



}
vector<CompressedCommand> MMG_PARSE(const char* str)
{

    float x, y, z;
    char line[100];
    vector<CompressedCommand>d;
    int i=0,k=0;
    while (str[i]!=NULL)
    {

     line[k]=str[i];

     if (str[i]=='\n')
        {

        if ( line[0]=='G' && (line[2]=='0'||line[2]=='1'))
        {
            data(line,&x,&y,&z);
            CompressedCommand c;
            c.x=x;
            c.y=y;
            c.z=z;
            d.push_back(c);
        }
        k=0;
    }
    else k++;
    i++;
    }
    return d;
}

