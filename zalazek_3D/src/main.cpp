#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <iomanip>

#include "Interp4Command.hh"
#include "GnuplotVisualizer.hh"

#include <list>
#include "xmlparser4scene.hh"
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include "scene.hh"

#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;
using namespace xercesc;

/*!
 * \brief wczytuje sekwencje
 *wczytujemy plik, nastepnie za pomaca funkcji popen wykonujemy skrypt pre_cmd.sh 
 * \param[in] nazwaPiku nazwa pliku który zostanie przetworzony funkcja "cpp -P"
 * \param[in] sekwencja stream polecen wczytanych z pliku
 */
bool wczytaj_cmd(string nazwaPliku, stringstream &sekwencja);
/*!
 * \brief wyswietla sekwencje gnuplotem
 * \param[in] wtyczki  przekazuje nam obecnie wczytane wtyczki dla ktorych zostanie wykonana trajektoria.
 * \param[in] sekwencja - stream z  poleceniami ktore maja zostac przedstawione, polecenia bez wczytanej wtyczki zostana pominiete.
 */
bool wykonaj(stringstream &sekwencja, map<string,Interp4Command*> &wtyczki);
/*!
 * \brief funkcja wywolywana przy podania pliku polecen podczas uruchomienia programu
 * \param[in] *argv wskaznik do tablicy nazw parametrow wczytanych podczas uruchomienia 
 * \param[in] wtyczki przechowuje wszystkie dostepne wtyczki 
 */
bool wczytaj(char *argv[], map<string,Interp4Command*> &wtyczki);

/*!
   * \brief Sprawdza czy nast±pi³a kolizja
   *  Algorytm testuj±cy kolizje walca wraz z prostopad³o¶cianem.
   * \retval true  - jesli wyst±pi³a kolizja miedzy elementami
   * \retval false - je¶li nie wyst±pi³a
 */
bool Colision(DronPose &Dpose,Scene &Scn);


int main(int argc, char *argv[]){

  void *pLibHnd_Fly = dlopen("libs/Interp4Fly.so",RTLD_LAZY);
  void *pLibHnd_Rotate = dlopen("libs/Interp4Rotate.so",RTLD_LAZY);
  void *pLibHnd_Turn = dlopen("libs/Interp4Turn.so",RTLD_LAZY);
  void *pLibHnd_Pause = dlopen("libs/Interp4Pause.so",RTLD_LAZY);
  void *pLibHnd_ReadScene = dlopen("libs/Interp4ReadScene.so",RTLD_LAZY);
  Interp4Command *(*pCreateCmd_Fly)(void);
  Interp4Command *(*pCreateCmd_Rotate)(void);
  Interp4Command *(*pCreateCmd_Turn)(void);
  Interp4Command *(*pCreateCmd_Pause)(void);
  Interp4Command *(*pCreateCmd_ReadScene)(void);
  void *pFun1;
  void *pFun2;
  void *pFun3;
  void *pFun4;
  void *pFun5;
 
  if (!pLibHnd_Fly) {
    cerr << "!!! Brak biblioteki: Interp4Fly.so" << endl;
    return 1;
  }
  if (!pLibHnd_Rotate) {
    cerr << "!!! Brak biblioteki: Interp4Rotate.so" << endl;
    return 1;
  } 
  if (!pLibHnd_Turn) {
    cerr << "!!! Brak biblioteki: Interp4Turn.so" << endl;
    return 1;
  }
  if (!pLibHnd_Pause) {
    cerr << "!!! Brak biblioteki: Interp4Pause.so" << endl;
    return 1;
  }
  if (!pLibHnd_Pause) {
    cerr << "!!! Brak biblioteki: Interp4ReadScene.so" << endl;
    return 1;
  }
  pFun1 = dlsym(pLibHnd_Fly,"CreateCmd");
  pFun2 = dlsym(pLibHnd_Turn,"CreateCmd"); 
  pFun3 = dlsym(pLibHnd_Rotate,"CreateCmd");
  pFun4 = dlsym(pLibHnd_Pause,"CreateCmd");
  pFun5 = dlsym(pLibHnd_ReadScene,"CreateCmd");
 
  if (!pFun1) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  if (!pFun2) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  if (!pFun3) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  if (!pFun4) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  if (!pFun5) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
 
  pCreateCmd_Fly = *reinterpret_cast<Interp4Command* (**)(void)>(&pFun1);
  pCreateCmd_Turn = *reinterpret_cast<Interp4Command* (**)(void)>(&pFun2);
  pCreateCmd_Rotate = *reinterpret_cast<Interp4Command* (**)(void)>(&pFun3);
  pCreateCmd_Pause = *reinterpret_cast<Interp4Command* (**)(void)>(&pFun4);
  pCreateCmd_ReadScene = *reinterpret_cast<Interp4Command* (**)(void)>(&pFun5);
 
  Interp4Command *pCmd_Fly=pCreateCmd_Fly();
  Interp4Command *pCmd_Rotate=pCreateCmd_Rotate();
  Interp4Command *pCmd_Turn=pCreateCmd_Turn();
  Interp4Command *pCmd_Pause=pCreateCmd_Pause();
  Interp4Command *pCmd_ReadScene=pCreateCmd_ReadScene();
  
  map<string,Interp4Command*> wtyczki;
  wtyczki["Fly"]=pCmd_Fly;
  wtyczki["Rotate"]=pCmd_Rotate; 
  wtyczki["Turn"]=pCmd_Turn;
  wtyczki["Pause"]=pCmd_Pause;
  wtyczki["ReadScene"]=pCmd_ReadScene;

  //Scene  Scn;  if (!ReadFile("scene.xml",Scn)) return 1;

  rl_bind_key('\t',rl_complete);
  const char *szacheta="\tTwoj wybor (?-menu): ";
  char * polecenie;

  stringstream sekwencja;
  string menu=
	
    string("w - wczytanie nazwy pliku sekwencji instrukcji dla drona\n")+
    string("p - pokaz sekwencje instrukcji\n")+
    string("i - pokaz dostepne instrukcje/wtyczki\n")+
    string("s - start wykonywania sekwencji instrukcji\n")+
    string("a - dodaj nowa wtyczke\n")+
    string("d - usun wtyczke\n")+
    string("? - menu\n\n")+
    string("k - koniec dzialania programu\n\n");

 
  if(argc==1){

    cout<<menu;
    do{
      
      polecenie=readline(szacheta);
      if(!polecenie) *polecenie='k'; 
      add_history(polecenie);
     
      switch(*polecenie){
      
      case 'w':{
     	     	
	cout<<"Podaj nazwe pliku: ";
	string nazwa;
	cin>>nazwa;
	cout<<wczytaj_cmd(nazwa,sekwencja)<<endl<<endl;
	
	break;
      }
	
      case 'p':{
	
	stringstream temp;
	string tmp;
	
	while(!sekwencja.eof()){
	  
	  tmp.clear();
	  sekwencja>>tmp;
	  if(tmp=="Fly"||tmp=="Rotate"||tmp=="Turn"||tmp=="Pause"||tmp=="ReadScene") cout<<endl;
	  cout<<tmp<<" ";
	  temp<<tmp<<" ";
	}
	
	cout<<endl;
	sekwencja.clear();
	
	while(!temp.eof()){
	  
	  tmp.clear();
	  temp>>tmp;
	  sekwencja<<tmp<<" ";
	}
	break;
      }
	
      case 'i':{
	
	for(auto i=wtyczki.cbegin();i!=wtyczki.cend();++i)
	  i->second->PrintSyntax();
	
	break;
      }
	
      case 's':{
	
	wykonaj(sekwencja,wtyczki);
	
	break;
      }	      
	
      case 'a':{
	
	cout<<"nazwa: ";
	string nazwa;
	cin>>nazwa;
	auto search=wtyczki.find(nazwa);
	if(search != wtyczki.end()) cout<<"juz istnieje\n";
	else if(nazwa=="Fly"){ wtyczki[nazwa]=pCmd_Fly; cout<<"dodano\n";}
	else if(nazwa=="Rotate"){ wtyczki[nazwa]=pCmd_Rotate; cout<<"dodano\n";}
	else if(nazwa=="Turn"){ wtyczki[nazwa]=pCmd_Turn; cout<<"dodano\n";}
	else if(nazwa=="Pause"){ wtyczki[nazwa]=pCmd_Turn; cout<<"dodano\n";}
	else if(nazwa=="ReadScene"){ wtyczki[nazwa]=pCmd_Turn; cout<<"dodano\n";}
	else cout<<"zla nazwa\n";

	break;
      }
      
      case 'd':{
      
	cout<<"nazwa: ";
	string nazwa;
	cin>>nazwa;
	
	auto search=wtyczki.find(nazwa);
	if(search == wtyczki.end()) cout<<"nie ma takiej\n";
	else{
	  wtyczki.erase(search);
	  cout<<"usunieto\n";
	}
	break;
      }
	
      case '?':{
	
	cout<<menu;
	break;
      }
	
      }

     

    }while (*polecenie != 'k');
  }
  
  else if(argc==2){

    cout << "Aby rozpoczac lot, nacisniecie klawisza ENTER" << endl;
    cin >> skipws;
    cin.ignore(100000,'\n');
    
    wczytaj(argv,wtyczki);
  }

  delete pCmd_Fly;
  delete pCmd_Rotate;
  delete pCmd_Turn;
  delete pCmd_Pause;
  delete pCmd_ReadScene;

  dlclose(pLibHnd_Fly);
  dlclose(pLibHnd_Rotate);
  dlclose(pLibHnd_Turn);
  dlclose(pLibHnd_Pause);
  dlclose(pLibHnd_ReadScene);
 
  
  cout<<"\nkoniec\n";
  
  return 0;
}


bool wczytaj_cmd(string nazwaPliku,stringstream &sekwencja){
  
  string tmp=string("./pre_cmd.sh ")+nazwaPliku;
  const char *cc=tmp.c_str();
  FILE *p= popen(cc,"r");
  pclose(p);
  nazwaPliku+=string("_pre"); 

  ifstream plik;

  plik.open( nazwaPliku.c_str() );
  if( !plik.good() ){ cerr<<"blad pliku\n"; return false;}

  string pozycja;     
  sekwencja.clear();
  
  while( !plik.eof() ){

    pozycja.clear();
    plik>>pozycja;
    sekwencja<<pozycja<<" ";
  }

  plik.close();
  return true;
}


bool wykonaj(stringstream &sekwencja, map<string,Interp4Command*> &wtyczki){
  
  if(sekwencja.eof()){ cerr<<" brak sekwencji\n"; return false;}
  if(wtyczki.size()<3){ 
    cout<<" nie wczytales wszystkich  wtyczek,\n dane sekwencje zostana pominiete,\n chcesz kontynuowac [T/N] \n";
    string odp;
    cin>>odp;
    if(odp=="N") return false;
  }
 
  ofstream plik;

  string nazwaPliku="wynik.txt";
  plik.open( nazwaPliku.c_str() );
  if( !plik.good() ){ cerr<<"blad pliku\n"; return false;}

  DronPose            DPose;
  GnuplotVisualizer   PlotVis;
 
  DPose.SetPos_m(0,0,-100);
  PlotVis.Draw(&DPose);
  Scene Scn;

  PlotVis.VisualizerScene(Scn);
  Wektor3D tmp;
  string nazwa;

  while(!sekwencja.eof()){
    nazwa.clear();
    sekwencja>>nazwa;

    auto search=wtyczki.find(nazwa);
    if(search != wtyczki.end()){
      
      if(search->second->ReadParams(sekwencja)){

	tmp=DPose.GetPos_m();
	plik<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<endl;
	search->second->ExecCmd(&DPose,&PlotVis);
      }
    }
    
  }
  plik.close();
   
  return true;
}

#define DRONE_Z 50
#define DRONE_R 50

bool Colision(DronPose &Dpose,Scene &Scn){

  Wektor3D center,size,pose,test;

 Scn.get_parameters(center,size,0);
 pose=Dpose.GetPos_m();

 test[0]=abs(pose[0]-center[0]);
 test[1]=abs(pose[1]-center[1]);
 test[2]=abs(pose[2]-center[2]);

 if(test[0] > (size[0]/2+DRONE_R)) return false;
 else if(test[1] > (size[1]/2+DRONE_R)) return false;

 if(test[0] <= (size[0]/2)){

   if(test[2] <= (size[2]/2)) return true;
 }

 if(test[1] <= (size[1]/2)){

   if(test[2] <= (size[2]/2)) return true;
 }
 
 return true;
}


bool wczytaj(char *argv[], map<string,Interp4Command*> &wtyczki){

  char tmp[20]="./pre_cmd.sh ";
  strcat(tmp,argv[1]);
  const char *cc=tmp;
 
  FILE *p= popen(cc,"r");
  pclose(p);

  char  nazwaPliku[20];
  strcpy(nazwaPliku,argv[1]);
  strcat(nazwaPliku,"_pre"); 
  ifstream plik;

  plik.open( nazwaPliku );
  if( !plik.good() ){ cerr<<"blad pliku\n"; return false;}

  string pozycja; 
  stringstream sekwencja;
 
  while( !plik.eof() ){

    pozycja.clear();
    plik>>pozycja;
    sekwencja<<pozycja<<" ";
  }

  plik.close();

  if(sekwencja.eof()){ cerr<<" brak sekwencji\n"; return false;}
  if(wtyczki.size()<3){ 
    cout<<" nie wczytales wszystkich  wtyczek,\n dane sekwencje zostana pominiete,\n chcesz kontynuowac [T/N] \n";
    string odp;
    cin>>odp;
    if(odp=="N") return false;
  }
 
  ofstream plik_w;

  string nazwa_pliku="wynik.txt";
  plik_w.open( nazwa_pliku.c_str() );
  if( !plik_w.good() ){ cerr<<"blad pliku\n"; return false;}

  DronPose            DPose;
  GnuplotVisualizer   PlotVis;
  Scene Scn;

  PlotVis.VisualizerScene(Scn);
  DPose.SetPos_m(0,0,-100);
  PlotVis.Draw(&DPose);

  Wektor3D temp;
  string nazwa;
  
  sekwencja>>nazwa;

  auto search=wtyczki.find(nazwa);
  if(search != wtyczki.end()){
    
    if(search->second->ReadParams(sekwencja))
      search->second->ExecCmd(&DPose,&PlotVis);
  }

  while(!sekwencja.eof()){

    nazwa.clear();
    sekwencja>>nazwa;

    search=wtyczki.find(nazwa);
    if(search != wtyczki.end()){
      
      if(search->second->ReadParams(sekwencja)){

	temp=DPose.GetPos_m();
	plik_w<<temp[0]<<" "<<temp[1]<<" "<<temp[2]<<endl;

	search->second->ExecCmd(&DPose,&PlotVis);

	if(Colision(DPose,Scn)==true){

	  cout<<endl<<"trafiono w przeszkode!!"<<endl;

	  cout << "Aby zakonczyc, nacisniecie klawisza ENTER" << endl;
	  cin.ignore(100000,'\n');

	  return false;
	}
      }
    } 
  }
  plik_w.close();

  cout << "Aby zakonczyc, nacisniecie klawisza ENTER" << endl;
  cin.ignore(100000,'\n');

  return true;
}

