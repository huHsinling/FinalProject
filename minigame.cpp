#include<iostream>
#include<string>
#include<ctime> 
using namespace std;

void makecopy(int answer[], int Anscopy[])
{
    for(int i = 0; i < 4; i++)
        Anscopy[i] = answer[i];
}
void strToInt(int keyIn[], string numstr)
{
    string a;
    for(int i = 0; i < 4; i++)
    {
       a = numstr[i];
       keyIn[i] = stoi(a);
    }
}
void makerand(int answer[])
{
    //srand(time(NULL));
    //for(int i = 0; i < 4; i++)
        //answer[i] = rand()%10;
}
int findA(int Anscopy[], int keyIn[])
{
   int A = 0;
   for(int i = 0; i < 4; i++)
   {
      if(Anscopy[i] == keyIn[i])
      {
         A++;
         Anscopy[i] = -1;
         keyIn[i] = -2;
      }
   }
   return A;
}
int findB(int Anscopy[], int keyIn[])
{
    int B = 0;
    for(int i = 0 ; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            if(Anscopy[i] == keyIn[j])
            {
                B++;
                keyIn[j] = -2;
                Anscopy[i] = -1;
            }
        }
    }
    return B;
}