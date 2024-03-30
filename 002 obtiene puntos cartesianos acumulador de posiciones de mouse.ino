//OBTIENE PUNTOS CARTESIANOS DE UNA LINEA O DE FORMAS
//SIGUE EL MOVIMIENTO DEL MOUSE EN COORDENADAS CARTESIANAS

//desarrollado para Arduino Day 2024
//Modificado por Americo Alvarez Surci
// 29-03-2024

#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
int Xmouse=0;
int Ymouse=0;
int inicio=0;
int ONmovimiento=0;
long XX=0;
long YY=0;
long X_1=0;
long Y_1=0;
int valorX=0;
int valorY=0;

class MouseRptParser : public MouseReportParser
{
protected:
  void OnMouseMove  (MOUSEINFO *mi);
  void OnLeftButtonUp (MOUSEINFO *mi);
  void OnLeftButtonDown (MOUSEINFO *mi);
  void OnRightButtonUp  (MOUSEINFO *mi);
  void OnRightButtonDown  (MOUSEINFO *mi);
  void OnMiddleButtonUp (MOUSEINFO *mi);
  void OnMiddleButtonDown (MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    ONmovimiento=1;
    Xmouse=mi->dX;
    Ymouse=mi->dY;
    
};
void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi)
{
    Serial.println("L Butt Up");
    inicio=2;
};
void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{
    Serial.println("L Butt Dn");
    Serial.println("Xmouse Ymouse valorX ValorY");
    inicio=1;
};
void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi)
{
    Serial.println("R Butt Up");
};
void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi)
{
    Serial.println("R Butt Dn");
};
void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{
    Serial.println("M Butt Up");
};
void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{
    Serial.println("M Butt Dn");
};

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser                               Prs;

void setup()
{
    Serial.begin( 115200 );
#if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay( 200 );

    HidMouse.SetReportParser(0, &Prs);
}

void loop()
{
  ONmovimiento=0;
  Usb.Task();
  if (inicio==1){
    if(ONmovimiento==1){
      XX=XX+Xmouse;
      YY=YY+Ymouse;
      
      //COORDENADAS CARTESIANAS
      //ajuste del valor X,Y a menor resolucion (1)
      valorX=XX/1;
      valorY=YY/1;
      if(valorX != X_1 || valorY != Y_1){
        Serial.print(Xmouse);
        Serial.print(" ");
        Serial.print(Ymouse);
        Serial.print(" ");
        Serial.print(valorX);
        Serial.print(" ");
        Serial.println(valorY);
        X_1=valorX;
        Y_1=valorY;
      }
      
//      Serial.print(Xmouse);
//      Serial.print(" ");
//      Serial.print(Ymouse);
//      Serial.print(" ");
//      Serial.print(XX);
//      Serial.print(" ");
//      Serial.println(YY);
    }
  }
  if (inicio==2){
    inicio=0;
    XX=0;
    YY=0;
  }
}
