#ifndef VISUALIZATION_HH
#define VISUALIZATION_HH


#include "DronPose.hh"
#include "Cuboid.hh"

/*!
 * \brief
 */
class Visualization {

  public:
  /*!
   * \brief Destruktor wirtualny, aby zapewnić poprawną destrukcję
   *        obiektu klasy pochodnej
   */
   virtual ~Visualization() {}
  /*!
   * \brief Powoduje odrysowanie aktualnej sceny z uwzglęnieniem bieżącej
   *        pozycji drona
   */
  virtual void Draw( const DronPose *pPose ) {}
  /*!
   * \brief Metoda ma czytać opis sceny z pliku XML
   *
   *  Metoda ma czytać opis sceny z pliku XML, który zawiera informację
   *  o rozmieszczeniu przeszkód. Każda z przeszkód reprezentowana jest
   *  poprzez prostopadłościan. Dla ułatwienia przyjmujemy, że 
   *  ścianki prostopadłościanów są albo prostopadle, albo też równoległe
   *  do płaszczyzn OXY, OXZ oraz OYZ.
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
  virtual bool ReadScene(const char* FileName_XML) = 0;
  /*!
   * \brief Udostępnia wskaźnik na przeszkodę o podanym indeksie
   *
   * Zakłada się, że kolejne przeszkody są modelowane przez prostopadłościany.
   * Metoda zwraca wskaźnik na obiekt klasy bazowej, który reprezentuje przeszkodę.
   * Zakłada się, że kolejne przeszkody dostępne są poprzez indeksy, które są
   * z zakresu od 0 do wartości określającej ilość przeszkód minus 1.
   *
   * \param[in] Idx - indeks określający, który kolejny obiekt reprezentujący
   *                  przeszkodę ma być udostępniony.
   *
   * \return Zwraca wskaźnik na obiekt reprezentujący przeszkodę, o ile podana
   *         wartość indeksu mieści się w przedziale od 0 do ilości przeszkód minus 1.
   *         Jeśli indeks jest spoza tego przedziału, to zwracany jest 
   *         wskaźnik nullptr.
   */
  virtual const Cuboid* GetObstacle(unsigned int Idx) const = 0;
};






#endif
