#ifndef FINALS_H_INCLUDED
#define FINALS_H_INCLUDED

#include<vector>
#include<cmath>
#include<iostream>

struct CompressedCommand
{
     float x;
     float y;
     float z;
};
struct OutCommand
{
     unsigned __int8 x;
     unsigned __int8 y;
     unsigned __int8 z;
     unsigned __int8 acc;
};
void SplitLines(int x,int y,int z,unsigned __int8 speed1,unsigned __int8 speed2,unsigned __int8 acc);    /* don't touch this ;D  */


std::vector<OutCommand> step_mov(std::vector <CompressedCommand> v, int xstep, int ystep, int zstep);
//std::vector<OutCommand> step_mov(std::vector <CompressedCommand> v, float xstep, float ystep, float zstep);

  /*this function that you will use it will take
vector of struct "CompressedCommand" and return a vector of struct "OutCommand"
, xstep is the length of the step of the motor on x axis and so on for ystep,zstep */



#endif // FINALS_H_INCLUDED
