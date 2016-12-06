#ifndef  DRONPOSE_HH
#define  DRONPOSE_HH

#include <iostream>
#include "Wektor3D.hh"

/*!
 * \file
 * \brief Definicja klasy DronPose
 *
 * Plik zawiera definicję klasy DronPose
 */

/*!
 * \brief Modeluje zestaw informacji określający pozę robota
 *
 *  Klasa modeluje zestaw informacji określających pozę robota.
 *  Ponadto udostępnia metody, które pozwalają aktualizowanie
 *  współrzędnych pozy robota, jak też pobieranie tych informacji.
 */
class DronPose {
  public:
   /*!
    * \brief Zeruje pozę robota
    *
    *  Inicjalizuje wartościami 0 zarówno pozycję robota,
    *  jak też jego kąt orientacji.
    */
   DronPose(): _Angle_deg(0) {}


   /*!
    * \brief Ustawia wartości współrzędnych pozy robota
    *
    *  Ustawia wartości współrzędnych kartezjańskich pozycji
    *  oraz kąt orientacji robota.
    *  \param[in] x_m - usawiana wartość współrzędnej \e x-owej
    *                    wyrażonej w metrach,
    *  \param[in] y_m - usawiana wartość współrzędnej \e y-owej
    *                    wyrażonej w metrach,
    *  \param[in] z_m - usawiana wartość współrzędnej \e z-owej
    *                    wyrażonej w metrach,
    */
   void SetPos_m( double x_m, double y_m, double z_m )
    {
      _Wsp_m.x() = x_m;
      _Wsp_m.y() = y_m;
      _Wsp_m.z() = z_m;
    }
   /*!
    * \brief Ustawia wartości współrzędnych pozycji robota
    *
    *  Ustawia wartości współrzędnych kartezjańskich pozycji
    *  robota.
    *  \param[in] rWsp_m - nowe wartości współrzędnych pozycji
    *                    wyrażone w metrach,
    */
   void SetPos_m( const Wektor3D &  rWsp_m )
    {
      _Wsp_m = rWsp_m;
    }
   /*!
    * \brief Ustawia wartości kąta orientacji robota
    *
    *  Ustawia wartości kąta orientacji robota.
    *  \param[in] Angle_deg - ustawiana wartość kąta orientacji 
    *                   robota wyrażona w stopniach.
    */
   void SetAngle_deg( double Angle_deg )
    {
      _Angle_deg = Angle_deg;
    }
   /*!
    * \brief Udostępnia wartości współrzędnych pozy robota
    *
    *  Udostępnia wartości współrzędnych kartezjańskich pozycji
    *  oraz kąt orientacji robota.
    *  \return Bieżące współrzędne położenia drona.
    */
   const Wektor3D & GetPos_m() const { return _Wsp_m; }

  /*!
   * \brief Udostępnia kąt orientacji drona.
   *
   * Udostępnia kąt orientacji drona. Kąt wyrażony jest w stopniach.
   */
   double GetAngle_deg( double angle_deg ) const
    {
      return _Angle_deg;
    }

  private:
    /*!
     * \brief  Współrzędne drona w globalnym układzie współrzędnych
     *
     * Współrzędne pozycji drona w globalnym układzie współrzędnych.
     * Wartość współrzędnej wyrażona jest w metrach.
     */
    Wektor3D    _Wsp_m;

    /*!
     * \brief Kąt orientacji robota w globalnym układzie współrzednych
     *
     *  Kąt orientacji robota w globalnym układzie współrzednych.
     *  Wartość kąta wyrażona jest w stopniach.
     */
    double    _Angle_deg;
};




#endif
