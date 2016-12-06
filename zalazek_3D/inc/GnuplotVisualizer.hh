#ifndef GNUPLOTVISULIZER_HH
#define GNUPLOTVISULIZER_HH

#include "Visualization.hh"
#include "lacze_do_gnuplota.hh"


/*!
 * \brief Nazwa pliku zawierającego współrzędne trajektorii drona
 *
 * Symbol definiuje nazwę pliku, do którego dopisywane są współrzędne
 * kolejnych punktów trajektrii. Na podstawie zawartości tego pliku
 * rysowana jest łamana reprezentująca trajektorię drona.
 */
#define FILE_NAME__TRAJECTORY         "trajektoria_drona.dat"
/*!
 * \brief Nazwa pliku zawierającego współrzędne korpusu drona
 *
 * Symbol definiuje nazwę pliku, do którego wpisywane są współrzędne
 * wierzchołków korpusu drona po każdorazowej zmianie jego pozycji.
 * Plik ten jest wykorzystywany do odczytu przez program gnuplot.
 * Na podstawi danych w nim zawartych.
 */
#define FILE_NAME__DRON_BODY_VIEW     "widok_drona.dat"
/*!
 * \brief Nazwa pliku z opisem były wzorcowej modelującej korpus drona
 *
 * Symbol definiuje nazwę pliku, w którym zapisane są współrzędne
 * wierzchołków bryły wzorcowej, która modeluje korpus drona.
 * Współrzędne są podane względem lokalnego układu współrzędnych,
 * w którym współrzędne środka to (0,0,0). Aby więc otrzymać obrys
 * bryły w aktualnej pozycji należy je rotować o kąt odpowiadający
 * aktualnej orientacji drona, a następnie dodać do współrzędnych
 * poszczególnych wierzchołków, współrzędne akutalne pozycji.
 */
#define FILE_NAME__DRON_BODY_TEMPLATE "elementarny_szescian.dat"


/*!
 * \brief Umożliwia wykorzystanie programu \p gnuplot do wizualizacji
 *        ruchu drona
 */
class GnuplotVisualizer: public Visualization {
  public:
  /*!
   * \brief Pośredniczy w uruchomieniu i przekazywaniu poleceń
   *        do programu \p gnuplot
   */
   PzG::LaczeDoGNUPlota   Plotter;
  /*!
   * \brief Kasuje zawartość plików danych i dodaje je do łącza do gnuplota
   */ 
  GnuplotVisualizer();
  /*!
   *
   */
   virtual void Draw( const DronPose *pPose );


  /*!
   * \brief Metoda czytania opis sceny z pliku XML
   */
  virtual bool ReadScene(const char* FileName_XML);
  /*!
   * \brief Tę metodę należy reimplementować
   *
   * Tę metodę należy reimplementować. Kod tej metody jest tylko po to, 
   * aby ta klasa nie była klasą abstrakcyjną.
   */
  virtual const Cuboid* GetObstacle(unsigned int Idx) const { return nullptr; }

  //--------------------------------------------------------------
  // W dalszej części zdefinowane są przykładowe pomocnicze metody
  //

  /*!
   * \brief Dodaje nazwy plików opisów obiektów reprezentujących drona
   */
  void AddDronFileNames4Gnuplot();

  
};


#endif
