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

//#include <readline/readline.h>
//#include <readline/history.h>

using namespace std;
using namespace xercesc;


bool wczytaj_cmd(string nazwaPliku, stringstream &sekwencja);
bool wykonaj(stringstream &sekwencja, map<string,Interp4Command*> &wtyczki);
bool wczytaj(char *argv[]);
bool ReadFile(const char* sFileName, Scene& Scn);


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

  Scene  Scn;

  if (!ReadFile("scene.xml",Scn)) return 1;
  stringstream sekwencja;
  char polecenie;
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

    cout<<"\tTwoj wybor (?-menu): ";
    cin>>polecenie;
    
    switch(polecenie){
      
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
  }
  while (polecenie != 'k');
  }
 
  else if(argc==2){
    cout << "Nacisnij ENTER, aby rozpoczac ... " << flush;
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

/*!
 * \brief wczytuje sekwencje
 *wczytujemy plik, nastepnie za pomaca funkcji popen wykonujemy skrypt pre_cmd.sh ktory przetwarza nasz plik o nazwie \param[in] nazwaPiku z komendami funkcja "cpp -P"
 *tworzy sie nowy plik ktorego zawartosc wczytywana jest do \param[in] sekwencja
 */
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

/*!
 *\brief wyswietla sekwencje gnuplotem
 *\param[in] wtyczki - przekazuje nam obecnie wczytane wtyczki dla ktorych zostanie wykonana trajektoria.
 *param[in] sekwencja - stream z  poleceniami ktore maja zostac przedstawione, polecenia bez wczytanej wtyczki zostana pominiete.
 */
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
  DPose.SetPos_m(0,0,0);
  PlotVis.Draw(&DPose);

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

/*
bool wczytaj( char *argv[]){

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
*/
bool ReadFile(const char* sFileName, Scene& Scn)
{
   try {
            XMLPlatformUtils::Initialize();
   }
   catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Error during initialization! :\n";
            cerr << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
            return 1;
   }

   SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

   DefaultHandler* pHandler = new XMLParser4Scene(Scn);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

   try {
     
     if (!pParser->loadGrammar("grammar/scene.xsd",
                              xercesc::Grammar::SchemaGrammarType,true)) {
       cerr << "!!! Plik grammar/scene.xsd, '" << endl
            << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
            << endl;
       return false;
     }
     pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
     pParser->parse(sFileName);
   }
   catch (const XMLException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            cerr << "Informacja o wyjatku: \n"
                 << "   " << sMessage << "\n";
            XMLString::release(&sMessage);
            return false;
   }
   catch (const SAXParseException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

            cerr << "Blad! " << endl
                 << "    Plik:  " << sSystemId << endl
                 << "   Linia: " << Exception.getLineNumber() << endl
                 << " Kolumna: " << Exception.getColumnNumber() << endl
                 << " Informacja: " << sMessage 
                 << endl;

            XMLString::release(&sMessage);
            XMLString::release(&sSystemId);
            return false;
   }
   catch (...) {
            cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
            return false;
   }

   delete pParser;
   delete pHandler;
   return true;
}

