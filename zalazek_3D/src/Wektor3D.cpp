#include "Wektor3D.hh"

using namespace std;


/*!
 *
 */
Wektor3D& Wektor3D::operator +=  (const Wektor3D& Wek)
{
 for (int Idx = 0; Idx < WYMIAR3D; ++Idx) {
                        _Wsp[Idx] += Wek[Idx];
 }
 return *this;
}


/*!
 * \brief 
 */
std::istream& operator >> (std::istream  &Strm, Wektor3D &W )
{
  return Strm >> W.x() >> W.y() >> W.z();
}


/*!
 * \brief 
 */
std::ostream& operator << (std::ostream  &Strm, const Wektor3D &W )
{
  return Strm << W.x() << " " << W.y() << " " << W.z();
}
