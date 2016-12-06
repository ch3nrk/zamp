#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <iomanip>

#include "Interp4Command.hh"
#include "GnuplotVisualizer.hh"

using namespace std;



int main()
{
  void *pLibHnd_Fly = dlopen("Interp4Fly.so",RTLD_LAZY);
  Interp4Command *(*pCreateCmd_Fly)(void);
  void *pFun;

  if (!pLibHnd_Fly) {
    cerr << "!!! Brak biblioteki: Interp4Fly.so" << endl;
    return 1;
  }


  pFun = dlsym(pLibHnd_Fly,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Fly = *reinterpret_cast<Interp4Command* (**)(void)>(&pFun);


  Interp4Command *pCmd = pCreateCmd_Fly();


  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;

  DronPose            DPose;
  GnuplotVisualizer   PlotVis;

  DPose.SetPos_m(0,0,0);
  PlotVis.ReadScene("scena.xml"); // Ta metoda w tej chwili tylko udaje, że czyta
  PlotVis.Draw(&DPose);

  cout << "Aby rozpoczac lot, nacisniecie klawisza ENTER" << endl;
  cin >> skipws;
  cin.ignore(100000,'\n');

  pCmd->ExecCmd(&DPose,&PlotVis); // Wykonanie polecenia wraz z jego wizualizacją

  cout << "Aby zakonczyc, nacisniecie klawisza ENTER" << endl;
  cin.ignore(100000,'\n');

  delete pCmd;

  dlclose(pLibHnd_Fly);
}
