#ifndef WEKTOR3D_HH
#define WEKTOR3D_HH


#include <cassert>
#include <iostream>

/*!
 * \brief Symbol definiuje wymiarowi przestrzeni 3D
 *
 * Symbol odpowiada wymiarowi przestrzeni 3D.
 * Celem wprowadzenia definicji jest posiadanie w kodzie 
 * wpisu, który ma czytelną interpretację.
 */
#define WYMIAR3D    3

/*!
 * \brief Wektor z przestrzeni 3D
 */
class Wektor3D {
   double _Wsp[WYMIAR3D];

 public:  
  /*!
   * \brief Inicjalizuje współrzędne wektora wartościami 0.
   */
  Wektor3D() { for (double &Wsp : _Wsp)  Wsp = 0; }
  /*!
   *
   */
  Wektor3D( double xx, double yy, double zz )
               { _Wsp[0] = xx;  _Wsp[1] = yy;  _Wsp[2] = zz; }

  /*!
   * \brief
   */
  Wektor3D& operator += (const Wektor3D& Wek);


  /*!
   * \brief
   */
  Wektor3D operator + (const Wektor3D& Wek) const
                     { return  Wektor3D(*this) += Wek; }

  /*!
   * \brief Udostępnia do odczytu współrzędną o podanym indeksie.
   * 
   * Udostępnia do odczytu współrzędną o podanym indeksie.
   * Metoda nie ma kontroli wartości współrzędnej. Na potrzeby
   * debugowania jest w kodzie tylko asercja.
   *
   * \param[in]  Idx - indeks wybranej współrzędnej.
   *
   * \pre 0 <= Idx && Idx < WYMIAR3D
   */
  double operator [] (int Idx) const 
     { assert(0 <= Idx && Idx < WYMIAR3D); return _Wsp[Idx]; }
  /*!
   * \brief Udostępnia do modyfikacji współrzędną o podanym indeksie.
   * 
   * Udostępnia do modyfikacji współrzędną o podanym indeksie.
   * Metoda nie ma kontroli wartości współrzędnej. Na potrzeby
   * debugowania jest w kodzie tylko asercja.
   *
   * \param[in]  Idx - indeks wybranej współrzędnej.
   *
   * \pre 0 =< Idx && Idx < WYMIAR3D
   */
  double& operator [] (int Idx) { return _Wsp[Idx]; }

  /*!
   * \brief Udostępia do odczytu współrzędną x-ową.
   */
  double x() const { return _Wsp[0]; }
  /*!
   * \brief Udostępia do modyfikacji współrzędną x-ową
   */
  double& x() { return _Wsp[0]; }

  /*!
   * \brief Udostępia do odczytu współrzędną y-ową.
   */
  double y() const { return _Wsp[1]; }
  /*!
   * \brief Udostępia do modyfikacji współrzędną y-ową.
   */
  double& y() { return _Wsp[1]; }

  /*!
   * \brief Udostępia do odczytu współrzędną z-ową.
   */
  double z() const { return _Wsp[2]; }
  /*!
   * \brief Udostępia do modyfikacji współrzędną z-ową.
   */
  double& z() { return _Wsp[2]; }
};


/*!
 * \brief Czyta ze strumienia trzy kolejne współrzędne wektora
 *
 * Czyta ze strumienia tekstowego trzy kolejne współrzędne wektora.
 * Zakłada się, że współrzędne rozdzielone są znakami białymi.
 * \param[in,out]  Strm - strumień wejściowy, z którego odczytane zostają
 *                        współrzędne wektora.
 * \param[in]      W - wektor, do którego zostały wpisane wartości wczytanych
 *                     współrzędnych.
 * \return Referencja do strumienia, z którego odczytane zostały
 *         współrzędne.
 */
std::istream& operator >> (std::istream  &Strm, Wektor3D &W );

/*!
 * \brief Wpisuje do strumienia trzy kolejne współrzędne rozdzielone
 *        pojedynczymi spacjami
 *
 * Wpisuje do strumienia tekstowego trzy kolejne współrzędne rozdzielone
 * pojedynczymi spacjami.
 * \param[in,out]  Strm - strumień wyjściowy, do którego wpisane zostają
 *                        współrzędne wektora.
 * \param[in]      W - wektor, którego współrzędne zostają wpisane do 
 *                     do strumienia.
 * \return Referencja do strumienia, do którego wpisane zostały
 *         współrzędne.
 */
std::ostream& operator << (std::ostream  &Strm, const Wektor3D &W );

#endif
