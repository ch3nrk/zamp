#ifndef CUBOID_HH
#define CUBOID_HH


#include "Wektor3D.hh"

/*!
 * \brief Klasa bazowa dla modelowania prostopadłościanów
 *
 * Jest to abstrakcyjna klasa bazowa dla modeli prostopadłościanów reprezentujących
 * przeszkody. W definicji klasy zawarte są jedynie podstawowe elementu interfejsu
 * tej klasy.
 *
 * Zakłada się, że podstawowymi atrybutami, których wartości mają być udostępniane,
 * to położenie środka prostopadłościanu oraz długość poszczególnych boków.
 */
class Cuboid {
  public:
  /*!
   * \brief Destruktor wirtualny dla poprawnej destrukcji obiektu
   *
   * Destruktor jest zdefiniowany tylko po to, aby móc bezpiecznie usunąć
   * obiekt posługując się wskaźnikiem klasy bazowej.
   */
  virtual ~Cuboid() {}

  /*!
   * \brief Udostępnia współrzędne środka prostopadłościanu
   *
   *  Udostępnia przestrzenne współrzędne geometrycznego 
   *  środka prostopadłościanu.
   */
   virtual const Wektor3D &GetCenter() const = 0;

  /*!
   * \brief Udostępnia długości boków prostopadłościanu
   *
   *  Udostępnia długości boków prostopadłościanu.
   *  Zakłada się, że prostopadłościany są tak ustawione, że 
   *  jego ścianki są równoległe do jednej z powierzchni OXY, OXZ lub OYZ.
   *  Każda współrzędna wektora zawiera długość boku, który jest równoległy
   *  do danej osi. Tak więc np. współrzędna x-owa zawiera długość boku,
   *  który równoległy jest do osi OX.
   */
   virtual const Wektor3D &GetSize() const = 0;
};


#endif
