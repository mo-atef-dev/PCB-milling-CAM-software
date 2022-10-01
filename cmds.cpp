#include <iostream>
#include <fstream>
#include<cmath>
#include "cmds.h"
#include<string>
#include<bitset>
#include<string.h>
#include<vector>

using namespace std;
int msp=7; // this parameter is contains the maximum speed value this parameter is set by the function "SetMaxSpeed(int x)"
int StepPerSpeed  = 10;
vector<OutCommand> vout;
void SplitLines(int x,int y,int z,unsigned __int8 speed1,unsigned __int8 speed2,unsigned __int8 acc);
void SetMaxSpeed(int x)
{
    msp=x;
    return;
}

struct link //one element of list
{
    __int8 speed1,speed2;
    __int8 acc;
    int diststep;
    int stepx,stepy,stepz;
    link* next;
    link* prev;

};
class chspeed
{
private:
    link *head;
    link *tail;
public:

    chspeed();
    void Add_Line(__int8 speed,int stepx,int stepy,int stepz,__int8 acc);
    void checkspeed();
    void Split_and_Writ();
    void pr();

};
chspeed::chspeed()
{
    head = new link;
    tail = new link;

    head->next=tail;
    head->acc=0;
    head->diststep=0;
    head->speed1=0;
    head->speed2=0;
    head->stepx=0;
    head->stepy=0;
    head->stepz=0;

    tail->prev=head;
    tail->acc=0;
    tail->diststep=0;
    tail->speed1=0;
    tail->speed2=0;
    tail->stepx=0;
    tail->stepy=0;
    tail->stepz=0;
    tail->next = NULL;
}

void chspeed::Add_Line(__int8 speed,int stepx,int stepy,int stepz,__int8 acc)    //add line at the back
{

    link* nline = new link;

    if (head->next==tail&&tail->prev==head )
    {
        nline->next=tail;
        tail->prev=nline;
        head->next=nline;
        nline->prev=head;
    }
    else
    {
        nline->next = tail;
        tail->prev->next=nline;
        nline->prev = tail->prev;
        tail->prev = nline;

    }

    nline->stepx = stepx;
    nline->stepy = stepy;
    nline->stepz = stepz;
    nline->speed1 = speed;
     nline->speed2 = 0;
    nline->prev->speed2 = nline->speed1;
    nline->acc = acc;

    nline->diststep = sqrt(pow(stepx,2)+ pow(stepy,2)+ pow(stepz,2));

}


void chspeed::pr()
{

    int i=1;
    bitset<8> ac ;
    link*crr = head->next;
    while(crr!=tail)
    {
        cout <<"command no.: "<<i<<endl;

        cout<<"3D distance: "<<crr->diststep<<" "<<"sp1: "<<(int)crr->speed1<<" "<<"sp2: "<<(int)crr->speed2<<endl;
        cout<<"stepx: "<<crr->stepx<<" "<<"stepy: "<<crr->stepy<<" "<<"stepz: "<<crr->stepz<<endl;
        ac = crr->acc;
        cout<<ac<<endl;
        cout<<endl;

/*        dfile <<"command no.: "<<i<<endl;
        dfile<<"3D distance: "<<crr->diststep<<" "<<"sp1: "<<(int)crr->speed1<<" "<<"sp2: "<<(int)crr->speed2<<endl;
        dfile<<"stepx: "<<crr->stepx<<" "<<"stepy: "<<crr->stepy<<" "<<"stepz: "<<crr->stepz<<endl;

        dfile<<ac<<endl;
        dfile<<endl;
*/


        crr = crr->next;
        i++;
    }
    cout <<"no. of commands: "<<i<<endl;
}


void chspeed::checkspeed()
{

    float steps_per_speed = StepPerSpeed;
    link *crr = head->next;

    int crrIndex = 1;


    while(crr!=tail)
    {

        int maxstep = max(crr->stepx,max(crr->stepy,crr->stepz));
        int premaxstep = max(crr->prev->stepx,max(crr->prev->stepy,crr->prev->stepz));
        while ( maxstep < abs(crr->speed2 - crr->speed1)*steps_per_speed )
        {
         //   cout <<"cs >>>>>>>>>>>>>>>>>>>>>>>>> "<<crrIndex<<endl;
            maxstep = max(crr->stepx,max(crr->stepy,crr->stepz));
            premaxstep = max(crr->prev->stepx,max(crr->prev->stepy,crr->prev->stepz));
            if (crr->speed2 > crr->speed1 && (crr->speed1 >= 0) )
            {
                crr->speed2 -=1;
                crr->next->speed1=crr->speed2;
            }
            else if(crr->speed2 < crr->speed1 && (crr->speed2 >= 0))
            {
                crr->speed1 -=1;
                crr->prev->speed2 = crr->speed1;
            }

        }
            if (premaxstep < abs(crr->prev->speed2 - crr->prev->speed1)*steps_per_speed )
            {
                if (crr!=head)
                {
                crr = crr->prev;
                //cout<<"back"<<endl;
                 crrIndex--;
                }


            }
            else{
            crr = crr->next;
      //  cout<<"front"<<endl;
           crrIndex++;
           }

    }


//cout<<"end while check speed"<<endl;

}



void chspeed::Split_and_Writ()
{
    int i = 1;
    link *crr = head->next;
    while (crr!=tail)
    {

        SplitLines(crr->stepx, crr->stepy, crr->stepz, crr->speed1, crr->speed2, crr->acc);
        crr = crr->next;
        i++;
    }

}


void AddToTextFileB2(unsigned __int8 x,unsigned __int8 y,unsigned __int8 z,unsigned __int8 acc)
{




    bitset<8> ac;
    ac = acc;
   // cout<<(int)x<<" "<<(int)y<<" "<<(int)z<<" "<<ac<<endl;
    OutCommand com;
    com.x = x;
    com.y = y;
    com.z = z;
    com.acc = acc;
    vout.push_back(com);
}


void SplitLines(int x,int y,int z,unsigned __int8 speed1,unsigned __int8 speed2,unsigned __int8 acc)
{
    if ((x==0)&&(y==0)&&(z==0)){return;}

    int const sps = StepPerSpeed ;
    int X[10000]={0},Y[10000]={0},Z[10000]={0},S[10000]={0},x_c=0,y_c=0,z_c=0,acc2=0,x_s=0,y_s=0,z_s=0;
    int x_r=0,y_r=0,z_r=0;
    int x_p=0,y_p=0,z_p=0;
    int maxstep= max(z,max(x,y)),maxspeed=msp,sts=0,bos=0,ens=0;
    int upn=0,bodyn=0,downn=0,n=0,i=0,r=0;
    // cout<<"maxstep"<< maxstep<<endl;



    if(maxstep<=sps)
    {
  //      cout<<"con1"<<endl;

        X[0]=x;
        Y[0]=y;
        Z[0]=z;
        S[0]=speed2;
        n=1;
    }
    else if((maxstep>=(abs(speed1-speed2)*sps))&&(maxstep<((abs(speed1-speed2)*sps)+(sps*2))))   // up only or down only
    {
       // cout<<"con2 0"<<endl;
       n=abs(speed1-speed2);
       x_p = round(sps *((float)x/(float)maxstep));
       y_p = round(sps *((float)y/(float)maxstep));
       z_p = round(sps *((float)z/(float)maxstep));
       for(i=0;i<n;i++)     /////////////split
       {
           X[i]=x_p;
           Y[i]=y_p;
           Z[i]=z_p;
           if(speed1<speed2)
            S[i]=speed1+i;
           else
            S[i]=speed1-(i);
       }

       if(speed1==speed2)
       {
            X[0]=x;
            Y[0]=y;
            Z[0]=z;
            S[0]=speed2;
            n++;
       }

     // cout<<"con2 2"<<endl;
       for(int k=0;k<n;k++)    /////////////cheack
       {
           x_c+=X[k];
           y_c+=Y[k];
           z_c+=Z[k];
       }
       //cout<<"con2 3"<<endl;
       if(x_c<x || y_c<y || z_c<z)     ///////////res
       {
           x_r=abs(x-x_c);
           y_r=abs(y-y_c);
           z_r=abs(z-z_c);
           r=0;
           for(int k=0;k<x_r;k++)
           {
               X[r]+=1;
               if(r==n-1)
                    r=0;
                else
                    r++;
           }
           r=0;
           for(int k=0;k<y_r;k++)
           {
               Y[r]+=1;
               if(r==n-1)
                    r=0;
                else
                    r++;
           }
           r=0;
           for(int k=0;k<z_r;k++)
           {
               Z[r]+=1;
               if(r==n-1)
                    r=0;
                else
                    r++;
           }
       }
       // cout<<"con2 4"<<endl;


    }
    else if((maxstep<=((abs(speed1-msp)+abs(speed2-msp))*sps)&&(maxstep>=((abs(speed1-speed2)*sps)+(sps*2)))))
    {
        //cout<<"con3"<<endl;
        sts = ((maxspeed-speed1))*sps;
        ens = ((maxspeed-speed2))*sps;
        while (maxstep < sts + ens)
        {
            maxspeed--;
            sts = ((maxspeed-speed1))*sps;
            ens = ((maxspeed-speed2))*sps;
            if(maxspeed==speed2)
            {
                break;
            }
        }
        upn=sts/sps;
        downn=ens/sps;
       x_p = (sps *((float)x/(float)maxstep));
       y_p = (sps *((float)y/(float)maxstep));
       z_p = (sps *((float)z/(float)maxstep));
        for(i=0;i<upn;i++)     /////////////split up
       {
           X[i]=x_p;
           Y[i]=y_p;
           Z[i]=z_p;
           S[i]=speed1+(i+1);
       }
       for(int k=0;k<downn;k++)     /////////////split down
       {
           X[i]=x_p;
           Y[i]=y_p;
           Z[i]=z_p;
           S[i]=maxspeed-k-1;
           i++;
       }
       n=upn+downn;
       for(int k=0;k<n;k++)    /////////////cheack
       {
           x_c+=X[k];
           y_c+=Y[k];
           z_c+=Z[k];
       }
       if(x_c<x || y_c<y || z_c<z)     ///////////res
       {
               x_r=abs(x-x_c);
               y_r=abs(y-y_c);
               z_r=abs(z-z_c);
               r=0;
               for(int k=0;k<x_r;k++)
               {
                   X[r]+=1;
                   if(r==n-1)
                        r=0;
                    else
                        r++;
               }
               r=0;
               for(int k=0;k<y_r;k++)
               {
                   Y[r]+=1;
                   if(r==n-1)
                        r=0;
                    else
                        r++;
               }
               r=0;
               for(int k=0;k<z_r;k++)
               {
                   Z[r]+=1;
                   if(r==n-1)
                        r=0;
                    else
                        r++;
               }
       }

    }
    else if((maxstep>((abs(speed1-msp)+abs(speed2-msp))*sps)))
    {
       // cout<<"con4"<<endl;
        sts = ((msp-speed1))*sps;
        ens = ((msp-speed2))*sps;
        bos = maxstep-sts-ens;

        upn=sts/sps;
        downn=ens/sps;
        bodyn=ceil(bos/255.0);
        n=upn+downn+bodyn;
        x_p = (sps *((float)x/(float)maxstep));
        y_p = (sps *((float)y/(float)maxstep));
        z_p = (sps *((float)z/(float)maxstep));
        for(i=0;i<upn;i++)     /////////////split up
        {
            if(upn==0)
                break;

            X[i]=x_p;
            Y[i]=y_p;
            Z[i]=z_p;
            S[i]=speed1+(i+1);
        }
        if(i!=0)
        i--;
         //cout<<"con4 test"<<downn<<endl;
        for(;i<upn+bodyn;i++)     /////////////split body
        {
            if(bos>255)
            {
                x_p = (255 *((float)x/(float)maxstep));
                y_p = (255 *((float)y/(float)maxstep));
                z_p = (255 *((float)z/(float)maxstep));
            }
            else
            {
                x_p = (bos *((float)x/(float)maxstep));
                y_p = (bos *((float)y/(float)maxstep));
                z_p = (bos *((float)z/(float)maxstep));
            }
            X[i]=x_p;
            Y[i]=y_p;
            Z[i]=z_p;
            S[i]=msp;
            bos-=255;
            if(bos<=0)
                break;

        }
        i++;
        x_p = (sps *((float)x/(float)maxstep));
        y_p = (sps *((float)y/(float)maxstep));
        z_p = (sps *((float)z/(float)maxstep));
        for(int k=0;k<downn;k++)     /////////////split down
       {
           if(downn==0)
                break;
           X[i]=x_p;
           Y[i]=y_p;
           Z[i]=z_p;
           S[i]=msp-k-1;
           i++;
       }
 for(int k=0;k<n;k++)    /////////////cheack
       {
           x_c+=X[k];
           y_c+=Y[k];
           z_c+=Z[k];
       }

       if(x_c<x || y_c<y || z_c<z)     ///////////res
       {
               x_r=abs(x-x_c);
               y_r=abs(y-y_c);
               z_r=abs(z-z_c);
               r=0;
               for(int k=0;k<x_r;k++)
               {
                   while(X[r]==255)
                   {
                       r++;
                   }
                   X[r]+=1;
                   if(r==n-2)
                        r=0;
                    else
                        r++;
               }
               r=0;
               for(int k=0;k<y_r;k++)
               {
                   while(Y[r]==255)
                   {
                       r++;
                   }
                   Y[r]+=1;
                   if(r==n-2)
                        r=0;
                    else
                        r++;
               }
               r=0;
               for(int k=0;k<z_r;k++)
               {
                   while(Z[r]==255)
                   {
                       r++;
                   }
                   Z[r]+=1;
                   if(r==n-2)
                        r=0;
                    else
                        r++;
               }
       }


    }
    else
    {
        cout<<"error       >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    }
    x_c=0;
    y_c=0;
    z_c=0;
    for(int k=0;k<n;k++)
       {
           acc2 = acc | S[k];
           if (X[k]|Y[k]|Z[k]|acc2)
            AddToTextFileB2(X[k],Y[k],Z[k],acc2);
           x_c+=X[k];
           y_c+=Y[k];
           z_c+=Z[k];
           //cout<<"k "<< k<<"X-->"<<X[k]<<" Y-->"<<Y[k]<<" Z-->"<<Z[k]<<" speed-->"<<S[k]<<endl;
       }
    //cout<<"-x-"<<x<<"-y-"<<y<<"-z-"<<z<<endl;

    //cout<<"............ "<<endl;
    if(x_c!=x || y_c!=y || z_c!=z)
    cout<<"X_c--> "<<x_c<<"-x-"<<x<<" Y_c--> "<<y_c<<"-y-"<<y<<" Z_c--> "<<z_c<<"-z-"<<z<<"sp1 "<<(int)speed1<<"sp2 "<<(int)speed2 <<endl;
}


__int8 GetSpeed3D(float x1,float x2,float x3,float y1,float y2,float y3,float z1,float z2,float z3)
{
    float Dir_x1 = x2-x1;
    float Dir_y1 = y2-y1;
    float Dir_z1 = z2-z1;

    float Dir_x2 = x3-x2;
    float Dir_y2 = y3-y2;
    float Dir_z2 = z3-z2;

    float Vector1_abs = pow(Dir_x1,2) + pow(Dir_y1,2) +pow(Dir_z1,2);
    float Vector2_abs = pow(Dir_x2,2) + pow(Dir_y2,2) +pow(Dir_z2,2);

    Vector1_abs = sqrt(Vector1_abs);
    Vector2_abs = sqrt(Vector2_abs);

    float total = Vector1_abs * Vector2_abs;
    float total3D = Dir_x1*Dir_x2 + Dir_y1*Dir_y2 + Dir_z1*Dir_z2;

    float angle = acos(abs(total3D)/total) * 180/3.14;
    angle = 180 - angle;

    __int8 speed=0;


    if (angle <= 90)
    {
        speed = 0;
    }
    else
    {
        speed=round((angle-90)/(90.0/(float)msp));

    }

    return speed;
}

vector<OutCommand> step_mov(vector <CompressedCommand> v,float xstep, float ystep, float zstep)
{

    float priv_x, priv_y, priv_z, currnt_x, currnt_y, currnt_z, next_x, next_y, next_z;
    __int8 currnt_speed;

    int step_x, step_y, step_z;


        vout.clear();

    chspeed Dlist;

    __int8 acc = 0x00;

    currnt_speed=0;

    priv_x = 0.0;
    priv_y = 0.0;
    priv_z = 0.0;
    currnt_x = 0.0;
    currnt_y = 0.0;
    currnt_z = 0.0;
    /*
     geting data for next x,y,z

    */



    for (long long unsigned int i = 0; i < v.size(); ++i)
    {
        /*extracting the data from the MMG file*/



            //data(line,&next_x,&next_y,&next_z);

            next_x =v[i].x;
            next_y =v[i].y;
            next_z =v[i].z;

            step_x = round(abs(next_x - currnt_x) / xstep);
            step_y = round(abs(next_y - currnt_y) / ystep);
            step_z = round(abs(next_z - currnt_z) / zstep);
            // cout << step_x << " " << step_y << " " << step_z << endl;

            acc = 0x00;

            /*Directions*/

            if (currnt_x <= next_x)
            {
                acc |= 0x00;
            }
            else if (currnt_x > next_x)
            {
                acc |= 0x80;
            }

            if (currnt_y > next_y)
            {
                acc |= 0x40;
            }
            else if (currnt_y <= next_y)
            {
                acc |= 0x00;
            }
            if (currnt_z <= next_z)
            {
                acc |= 0x00;
            }
            else if (currnt_z > next_z)
            {
                acc |= 0x20;
            }
			if (msp>0)
			{currnt_speed = GetSpeed3D(priv_x,currnt_x,next_x,priv_y,currnt_y,next_y,priv_z,currnt_z,next_z);}






            //  cout<<currnt_speed<<endl;
            Dlist.Add_Line(currnt_speed,step_x,step_y,step_z,acc);



            priv_x = currnt_x;
            priv_y = currnt_y;
            priv_z = currnt_z;
            currnt_x = next_x;
            currnt_y = next_y;
            currnt_z = next_z;



    }



  Dlist.checkspeed();


  Dlist.Split_and_Writ();




for(auto i=vout.begin();i!=vout.end();i++)
{
    if (((*i).acc==0)&&((*i).x==0)&&((*i).y==0)&&((*i).z==0))
    {
            vout.erase(i);
            i--;
    }
}


AddToTextFileB2(0,0,0,0);
return vout;






}




