#include <iostream>
#include "Interp4Fly.hh"
#include <unistd.h>
#include <cmath>

using std::cout;
using std::endl;


extern "C" {
 Interp4Command* CreateCmd(void);
}




/*!
 * 
 * Fly odpowiada za lot wykorzystujac predosc wertykalna, predkosc horyzontalna i dlugosc trasy.
 *
 */
Interp4Command* CreateCmd(void)
{
  return Interp4Fly::CreateCmd();
}


/*!
 * konstruktor, ustawiamy zerowe wartosci 
 */
Interp4Fly::Interp4Fly(): _Speed_h_mS(0),_Speed_w_mS(0),_Length_m(0)
{}


/*!
 *wypisujemy bierzace wartosci zapisane we wtyczce 
 */
void Interp4Fly::PrintCmd() const{
  
  cout<<GetCmdName()<<" "<<_Speed_h_mS<<" [m/s]";
  cout<<" "<<_Speed_w_mS<<" [m/s]";
  cout<<" "<<_Length_m<<" [m]" <<endl;
}


/*!
 *wyswietalamy nazwe wtyczki
 */
const char* Interp4Fly::GetCmdName() const
{
  return "Fly";
}


/*!
 *obliczamy i wykonujemy trajektorie dla przechowywanych wartosci 
 */
bool Interp4Fly::ExecCmd( DronPose  *pRobPose,  Visualization *pVis) const{
  
  double alfa,beta,x,y,z,v;
  Wektor3D poczatek=pRobPose->GetPos_m();

  alfa=pRobPose->GetAngle_deg(alfa);
  
  if(_Speed_w_mS*_Speed_h_mS<0) beta=atan(-1*(_Speed_w_mS/_Speed_h_mS));
  else beta=atan(_Speed_w_mS/_Speed_h_mS);
  
  v=_Length_m*cos(beta);

  x=v*sin(M_PI*alfa/180);
  y=v*cos(M_PI*alfa/180);
  z=_Length_m*sin(beta);

  if(_Speed_h_mS<0) y*=-1;
 
  pRobPose->SetPos_m( poczatek[0]+x,poczatek[1]+y,poczatek[2]+z);
  pVis->Draw(pRobPose);
  usleep(800000);
 
  return true;
}


/*!
 *wczytujemy nowe parametry, nieporawne wartosci zwracj± b³ad i wyswietlaj± niepoprawn± komende.
 */
bool Interp4Fly::ReadParams(std::istream& Strm_CmdsList){

  Strm_CmdsList>>_Speed_h_mS>>_Speed_w_mS>>_Length_m;

  if(_Length_m<0){

    cout<<"droga musi byc >=0 -> ";
    PrintCmd();
    return false;
  }  
  else if(_Length_m>0 && (_Speed_h_mS==0 && _Speed_w_mS==0)){
      
      cout<<"droga tez musi byc rozna od 0 -> ";
      PrintCmd();
      return false;
  }
 
  return true;
}


/*!
 *tworzymy nowa wtyczkê 
 */
Interp4Command* Interp4Fly::CreateCmd()
{
  return new Interp4Fly();
}


/*!
 *wyswietlamy parametry danej wtyczki 
 */
void Interp4Fly::PrintSyntax() const
{
  cout << "   Fly  hor_speed[m/s]  ver_speed[m/s]  dist[m]" << endl;
}
