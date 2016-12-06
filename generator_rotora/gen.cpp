#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
using namespace std;

#define VERTEX_COUNT  6
#define RADIUS        1
#define HEIGHT2       1


void WriteVertex( ofstream &Strm, double Angle_rad )
{
  Strm << "0 0 " << HEIGHT2 << endl
       << fixed << setprecision(5)
       << RADIUS*cos(Angle_rad) << " " << RADIUS*sin(Angle_rad) << " " << HEIGHT2 << endl
       << RADIUS*cos(Angle_rad) << " " << RADIUS*sin(Angle_rad) << " " << -HEIGHT2 << endl
       << "0 0 " << -HEIGHT2 << endl
       << "#" << endl
       << endl;
}


int main()
{
  ofstream  Strm("rotor.dat");

  WriteVertex(Strm,0);
  for (int Idx = 1; Idx < VERTEX_COUNT; ++Idx) {
    WriteVertex(Strm,Idx*2*M_PI/VERTEX_COUNT);
  }
  WriteVertex(Strm,0);
}
