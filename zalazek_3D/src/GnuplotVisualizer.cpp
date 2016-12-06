#include "GnuplotVisualizer.hh"
#include <fstream>
#include <iostream>
#include <cmath>
#include "Wektor3D.hh"

using namespace std;

#define FILE_NAME__ROTOR_TEMPLATE "wzor_rotora.dat"
#define FILE_NAME__ROTOR1          "rotor1.dat"
#define FILE_NAME__ROTOR2          "rotor2.dat"
#define FILE_NAME__ROTOR3          "rotor3.dat"
#define FILE_NAME__ROTOR4          "rotor4.dat"
#define FILE_NAME__OBSTACLE1          "przeszkoda1.dat"
#define FILE_NAME__OBSTACLE2          "przeszkoda2.dat"
#define FILE_NAME__OBSTACLE3          "przeszkoda3.dat"

/*!
 * Konstruktor inicjalizuje pracę z programem gnuplot.
 * Na potrzeby tej inicjalizacji wykonane są następujące operacje:
 * 
 * \li Kasowanie zawartości plików, których nazwy zdefiniowane są poprzez stałe
 *   \link GnuplotVisualizer.hh::FILE_NAME__TRAJECTORY FILE_NAME__TRAJECTORY\endlink
 *   oraz 
 *   \link GnuplotVisualizer.hh::FILE_NAME__DRON_BODY_VIEW FILE_NAME__DRON_BODY_VIEW\endlink.
 *   W pierwszym z plików zapisywane są współrzędne wierzchołków łamanej modelującej
 *   ścieżkę przelotu drona. W drugim natomiast są zapisywane współrzędne aktualnego
 *   widoku bryły reprezentującej korpus drona.
 *
 * \li Inicjacja pracy programu gnuplot.
 *
 * \li Dodanie nazw plików, z których będą czytane dane przy każdorazowym odrysowaniu
 *     rysunku.
 *
 * \li Ustawienie trybu rysowania jako rysowanie przestrzenne 3D.
 *
 * \li Wstępne ustawienie zakresu współrzędnych rysowanej sceny.
 *     Te zakresy należy dostosować do potrzeb własnego programu.
 *
 * \li Wstępne ustawienie kierunku, z którego widziana jest scena.
 *     W trakcie pracy aplikacji kierunek ten można modyfikować
 *     wykorzystując kursor myszki.
 */
GnuplotVisualizer::GnuplotVisualizer()
{
  ofstream  Strm;
  Strm.open(FILE_NAME__TRAJECTORY);      Strm.close();  // Kasujemy poprzednia
  Strm.open(FILE_NAME__DRON_BODY_VIEW);  Strm.close();  // zawartość

  Plotter.Inicjalizuj(); 

  AddDronFileNames4Gnuplot();

  Plotter.ZmienTrybRys(PzG::TR_3D);
  Plotter.UstawZakresX(-100, 250);
  Plotter.UstawZakresY(-100, 250);
  Plotter.UstawZakresZ(-100, 200);
  //
  // Lepiej nie wymuszać ustawienia widoku. To pozwoli zmieniać widok za pomocą
  // myszki w trakcie wykonywania rysowania.
  //
  //  Plotter.UstawRotacjeXZ(40,60); 
}


/*
 * Moduł LaczeDoGnuplota posiada listę plików, z których gnuplot będzie brał
 * dane od rysowania brył.
 * Niniejsza metoda dodaje do tej listy nazwy plików, w których będzie obrysł
 * brył reprezentujących poszczególne elementy drona.
 *
 * Metodę tę można rozbudować wg własnych potrzeb.
 */
void GnuplotVisualizer::AddDronFileNames4Gnuplot()
{
    // Parametr 1 na końcu odpowiada rysowaniu kolorem czerwonym
  Plotter.DodajNazwePliku(FILE_NAME__TRAJECTORY,PzG::RR_Ciagly,2,1);

    // Parametr 1 na końcu odpowiada rysowaniu kolorem czarnym
  Plotter.DodajNazwePliku(FILE_NAME__DRON_BODY_VIEW,PzG::RR_Ciagly,1,7);
  Plotter.DodajNazwePliku(FILE_NAME__ROTOR1,PzG::RR_Ciagly,1,7);
  Plotter.DodajNazwePliku(FILE_NAME__ROTOR2,PzG::RR_Ciagly,1,7);
  Plotter.DodajNazwePliku(FILE_NAME__ROTOR3,PzG::RR_Ciagly,1,7);
  Plotter.DodajNazwePliku(FILE_NAME__ROTOR4,PzG::RR_Ciagly,1,7);
}


/*!
 * \brief Dodaje punkt do ścieżki ruchu
 *
 * Dodaje do pliku, którego nazwa jest zdefinowana
 * przez stałą 
 * \link Visualization.hh::FILE_NAME__DRON_BODY_VIEW
 *                                     FILE_NAME__DRON_BODY_VIEW\endlink,
 * współrzędne następnego punktu ścieżki ruchu drona.
 */
bool AddTrajectoryPoint( const DronPose *pPose )
{
  ofstream  OuStrm(FILE_NAME__TRAJECTORY,ios::app); // Plik otwarty w trybie dopisywania
  if (!OuStrm.is_open()) {
    cerr << " Blad otwarcia do zapisu pliku \"" FILE_NAME__TRAJECTORY "\""
	 << endl;
    return false;
  }
  OuStrm << pPose->GetPos_m() << endl;
  return true;
}


/*!
 * \brief Rotacja i skalowanie współrzędnych punktu
 *
 * Dokonuje rotacji i skalowania współrzędnych zadanego punktu.
 * \param[in] sn - sinus kąta rotacji,
 * \param[in] cn - cosinus kąta rotacji,
 * \param[in] rPnt   - współrzędne punktu, które mają być podane rotacji i skalowaniu,
 * \param[in] rScale - współczynniki skali wzdłuż każdej z osi.
 * \return Zwaraca przetransformowane współrzędne punktu.
 */
Wektor3D RotateXY( double sn, double cn, const Wektor3D & rPnt, const Wektor3D & rScale)
{
  return Wektor3D( rScale.x()*(rPnt.x()*cn - rPnt.y()*sn), 
                   rScale.y()*(rPnt.x()*sn + rPnt.y()*cn), 
                   rScale.z()*rPnt.z()
                  );
}

/*
 * Czyta plik, który zawiera współrzędne bryły wzorocej. Przeczytane współrzędne
 * wierzchołków są transformowane, tzn. podane są:
 *   \li rotacji,
 *   \li skalowaniu,
 *   \li translacji.
 * Następnie zapisywane są do pliku, który będzie czytał gnuplot. Ważne jest,
 * że funkcja ta zachowuje budowę pliku wzorcowego, tzn. w odpowiednim miejscu
 * znajdzie się wolna linia między poszczególnymi zestawami współrzędnych
 * wierzchołków.
 *  
 * \param[in] sFile_Template - nazwa pliku zawierającego współrzędne wierzchołków bryły
 *                             wzorcowej. Z tego pliku wspomniane współrzędne są czytane.
 * \param[in] sFile_Target - nazwa pliku, do którego zostaną zapisane przetransformowane
 *                           współrzędne.
 * \param[in] rTrans_m     - wektor translacji, którego współrzędne wyrażone są w metrach.
 * \param[in] RotAngle_rad - kąt rotacji wyrażony w radianach.
 *
 * \retval true - gdy operacja otwarcia i zapisu odpowiednich plików zakończyła się
 *                powodzeniem,
 * \retval false - w przypadku przeciwnym.
 */
bool TransformGeom(
		   const char      *sFile_Template,
		   const char      *sFile_Target,
                   const Wektor3D  &rTrans_m,
                   double            RotAngle_rad,
                   const Wektor3D  &rScale
    	          )
{
  ofstream  OuStrm(sFile_Target);
  ifstream  InStrm(sFile_Template);
  Wektor3D  Pos;

  if (!OuStrm.is_open()) {
    cerr << " Blad otwarcia do zapisu pliku \"" << sFile_Target << "\""
	 << endl;
    return false;
  }
  if (!InStrm.is_open()) {
    cerr << " Blad otwarcia do odczytu pliku \"" << sFile_Template << "\""
	 << endl;
    return false;
  }

  char      Separator;
  double    sn = sin(RotAngle_rad),  cn = cos(RotAngle_rad);
 
  while (!InStrm.eof()) {
    if (InStrm >> Pos) {
        // Najpierw dokonujemy rotacji
      Pos = RotateXY(sn,cn,Pos,rScale);
        // Następnie translacja
      Pos += rTrans_m;
      OuStrm << Pos << endl;
      continue;
    }
    InStrm.clear();
    if (!(InStrm >> Separator)) break;
    if (Separator != '#') {
      cerr << " Blad w pliku \"" << sFile_Target <<  "\"" << endl
           << " oczekiwano znaku #. Zamiast tego odebrano znak " << Separator
	   << endl;
      return false;
    }
    OuStrm << "#\n\n";
  }
  return true;
}




/*!
 * Zapisanie pliku z zaktualnymi współrzędnymi wierzchołków bryły
 * opisującej drona.
 * Ze względu na specyfikę sposobu rysowania powierzchni przez program gnuplot,
 * format wejściowy i wyjściowy pliku musi zostać zachowany, tzn. muszą być
 * zachwane przerwy w postaci wolnych linii między kolejnymi zbiorami wierzchołków,
 * które stanowią tworzącą rysowanej siatki danej powierzchni.
 * Ta funkcja to zapewnia.
 * \param[in] pPose - wskaźnik na obiekt zawierający dane dotyczące aktualnej pozy
 *                    drona.
 *
 * \warning W tej funkcji brakuje uwzględnienia rotacji, która pozwoli odzwierciedlić
 *          aktualną orientację drona. W kodzie funkcji jest zaznaczone miejsce,
 *          w którym należy wpisać odpowiedni kod.
 *
 */
bool WriteCurrDonPose( const DronPose *pPose )
{
  double DronAngle_rad = 0;
  static double RotorAngle_rad = 0;
  Wektor3D      RotorScale(18,18,10);
  Wektor3D      DronScale(20,20,10);

   //
   // Wektory translacji rotorów względem korpusu drona
   //
  Wektor3D      Trans_Rot1(DronScale.x(),DronScale.y(),DronScale.z());
  Wektor3D      Trans_Rot2(-DronScale.x(),DronScale.y(),DronScale.z());
  Wektor3D      Trans_Rot3(DronScale.x(),-DronScale.y(),DronScale.z());
  Wektor3D      Trans_Rot4(-DronScale.x(),-DronScale.y(),DronScale.z());

  RotorAngle_rad += 5*M_PI/180;

  if (!TransformGeom(FILE_NAME__DRON_BODY_TEMPLATE,FILE_NAME__DRON_BODY_VIEW,
                     pPose->GetPos_m(), DronAngle_rad, DronScale)) return false;


   //
   // Rotory po przekątnej tworzą pary. Rotory z tej samej pary obracają się w tę 
   // samą stronę. Jednakże każda z par rotorów musi obracać w kierunku przeciwnym
   // do tego, w jakim obracają się rotory z drugiej pary.
   //
   //  Pierwsza para rotorów
   //
  if (!TransformGeom(FILE_NAME__ROTOR_TEMPLATE, FILE_NAME__ROTOR1,
                     pPose->GetPos_m()+Trans_Rot1, 
                     -RotorAngle_rad + DronAngle_rad, RotorScale)) return false;

  if (!TransformGeom(FILE_NAME__ROTOR_TEMPLATE,FILE_NAME__ROTOR4,
                     pPose->GetPos_m()+Trans_Rot4, 
                     -RotorAngle_rad + DronAngle_rad, RotorScale)) return false;


   //
   //  Druga para rotorów
   //
  if (!TransformGeom(FILE_NAME__ROTOR_TEMPLATE,FILE_NAME__ROTOR2,
                     pPose->GetPos_m()+Trans_Rot2, 
                     RotorAngle_rad + DronAngle_rad, RotorScale)) return false;

  if (!TransformGeom(FILE_NAME__ROTOR_TEMPLATE,FILE_NAME__ROTOR3,
                     pPose->GetPos_m()+Trans_Rot3, 
                     RotorAngle_rad + DronAngle_rad, RotorScale)) return false;
  return true;
}



/*!
 * Powoduje dopisanie aktualnej współrzędnej drona do pliku,
 * w którym zawarta jest sekwencja wcześniejszych położeń.
 * W ten sposób otrzymana łamana reprentuje trajektorię ścieżkę
 * lotu drona.
 * Ponadto na podstawie pliku wzorcowego opisującego korpus drona,
 * utworzony zostaje plik ze współrzędnymi wierzchołków w jego
 * nowej pozycji. Po tej pozycji następuje odrysowanie drona
 * w jego aktulanej pozycji.
 *
 * \param[in] pPose - wskaźnik na obiekt zawierający dane dotyczące aktualnej pozy
 *                    drona.
 */
void GnuplotVisualizer::Draw( const DronPose *pPose )
{
  AddTrajectoryPoint(pPose);
  WriteCurrDonPose(pPose);
  Plotter.Rysuj();
}



/*!
 *  Metoda ma czytać opis sceny z pliku XML, który zawiera informację
 *  o rozmieszczeniu przeszkód. Każda z przeszkód reprezentowana jest
 *  poprzez prostopadłościan. Dla ułatwienia przyjmujemy, że 
 *  ścianki prostopadłościanów są albo prostopadle, albo też równoległe
 *  do płaszczyzn OXY, OXZ oraz OYZ.
 *
 *
 *  W tej chwili metoda ta jedynie symuluje czytanie z pliku.
 *  Wstawia przeszkody na sztywno.
 *
 *  \param[in] FileName_XML - nazwa pliku w formacie XML, w którym zawarty
 *                            jest opis rozmieszczenia przeszkód na scenie.
 *
 *  \retval true - gdy operacja zakończyła się powodzeniem,
 *  \retval false - w przypadku przeciwnym.
 *
 *  \post  Jeżeli operacja zakończyła się powodzeniem, to zostają usunięte
 *         stare przeszkody, zaś na ich miejsce wstawiane są nowe.
 */
bool GnuplotVisualizer::ReadScene(const char* FileName_XML)
{
   // 1. Powinniśmy sprawdzić, czy można otworzyć plik o podanej nazwie,
   //    jeśli nie, to kończymy działanie.
   //
   // 2. Usuwamy dotychczasowe obiekty 

   // 3. Następnie usuwamy wszystkie nazwy plików związne ze starymi
   //    obiektami.
  Plotter. UsunWszystkieNazwyPlikow();

   // 4. Pozostawiamy nazwy plików, które wiążą się z elementami konstrukcji drona
  AddDronFileNames4Gnuplot();

   // 5. Tu powinniśmy rozpocząć czytanie plików i dodać przeszkody.
   //    W tej metodzie udajemy, że czytamy i wstawiamy przeszkody 
   //    Tworzymi pliki z obrysem prostopadłościanów modelujących przeszkody
   //    i przekazujemy ich nazwy do modułu lacze_do_gnuplota. 

   // Tu tworzymy nowy plik z obrysem przeszkody transformując sześcian o boku 1
   // do prostopadłościanu i przesuwając go w odpowiednie miejsce.
  if (!TransformGeom(FILE_NAME__DRON_BODY_TEMPLATE, FILE_NAME__OBSTACLE1,
                     Wektor3D(10,80,10), 0, Wektor3D(40,20,10))) return false;
   //  Parametr 12 na końcu odpowiada rysowaniu kolorem niebieskim
  Plotter.DodajNazwePliku(FILE_NAME__OBSTACLE1,PzG::RR_Ciagly,1,12);


   // To samo powtarzamy z następną przeszkodą
  if (!TransformGeom(FILE_NAME__DRON_BODY_TEMPLATE,FILE_NAME__OBSTACLE2,
                     Wektor3D(150,20,20), 0, Wektor3D(30,20,20))) return false;
  Plotter.DodajNazwePliku(FILE_NAME__OBSTACLE2,PzG::RR_Ciagly,1,12); 


  if (!TransformGeom(FILE_NAME__DRON_BODY_TEMPLATE,FILE_NAME__OBSTACLE3,
                     Wektor3D(150,60,60), 0, Wektor3D(10,10,120))) return false;
  Plotter.DodajNazwePliku(FILE_NAME__OBSTACLE3,PzG::RR_Ciagly,1,12);

  return true;
}

