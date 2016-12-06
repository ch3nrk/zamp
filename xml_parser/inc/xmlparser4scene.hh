#ifndef XMLPARSER4SCENE_HH
#define XMLPARSER4SCENE_HH


#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/Locator.hpp>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

//XERCES_CPP_NAMESPACE_USE
#include <string>
#include "scene.hh"

/*!
 * \brief Implementuje reakcje na napotkane elementu opisu akcji
 *
 * Klasa zawiera zestaw metod, które wywoływane są w trakcie czytania i analizy
 * pliku XML.
 */
class XMLParser4Scene : public xercesc::DefaultHandler {
  public:
   /*!
    * \brief Inicjalizuje obiekt i kojarzy go ze sceną
    */
  XMLParser4Scene(Scene &Scn);

   /*!
    * \brief Wywoływana jest na początku dokumentu
    */
    virtual void startDocument();
   /*!
    * \brief Wywoływana jest na końcu dokumentu
    */
    virtual void endDocument();

   /*!
    * \brief Wywoływana jest po napotkaniu nowego elementu XML
    */
    virtual void startElement(
                    const XMLCh *const pURI, 
                    const XMLCh *const pLocalName, 
                    const XMLCh *const pQNname,
                    const   xercesc::Attributes&     attrs
                  );

   /*!
    * \brief Wywoływana jest po dojściu do końca elementu XML
    */
    virtual  void endElement(
                    const XMLCh *const pURI, 
                    const XMLCh *const pLocalName, 
                    const XMLCh *const pQNname
                   );
   /*!
    * \brief Wywoływana jest gdy napotkany zostanie błąd fatalny
    */
    void fatalError(const xercesc::SAXParseException&);

   /*!
    * \brief Wywoływana jest gdy napotkany zostanie błąd
    */
    void error(const xercesc::SAXParseException &exc);

   /*!
    * \brief Wywoływana jest gdy parser sygnalizuje ostrzeżenie
    */
    void warning(const xercesc::SAXParseException &exc);

    /*!
     * \brief Wykonuje końcowe operacje związane z danym elementem XML
     */
    void WhenEndElement(const std::string& ElemName);

    /*!
     * \brief Wykonuje operacje związane z danym elementem XML
     */
    void WhenStartElement( const std::string&             ElemName,
		           const xercesc::Attributes&     Attrs
                         );
  private:
  /*!
   *  \brief Zawiera wskaźnik do modyfikowanej sceny
   *
   *  Zawiera wskaźnik na obiekt, do którego mają zostać wpisane
   *  informacje z wczytywanego pliku.
   */
   Scene  *_pScn;
};

#endif
