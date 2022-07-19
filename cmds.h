#ifndef FINALS_H_INCLUDED
#define FINALS_H_INCLUDED
#include<vector>

struct CompressedCommand       // this struct is to hold the input commands
{
     float x;
     float y;
     float z;
};
struct OutCommand      //// this struct is to hold the output commands
{
  unsigned __int8 x;
  unsigned __int8 y;
  unsigned __int8 z;
  unsigned __int8 acc;
};


 void SetMaxSpeed(int x); //this function for setting the maximum speed will the machine work at acceleration an deacceleration
 // it is must the value between (0 --> 7)




std::vector<OutCommand> step_mov(std::vector <CompressedCommand> v,float xstep, float ystep, float zstep);

  /*this function that you will use it will take
vector of struct "CompressedCommand" and return a vector of struct "OutCommand"
, xstep is the length of the step of the motor on x axis and so on for ystep,zstep */



#endif // FINALS_H_INCLUDED
