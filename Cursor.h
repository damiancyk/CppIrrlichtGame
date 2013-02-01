#ifndef CLASS_CURSOR_H_INC
#define CLASS_CURSOR_H_INC

#include <irrlicht.h>

class CCursor
{
private :

  irr::IrrlichtDevice      *device;
  irr::video::IVideoDriver *driver;
  irr::video::ITexture     *cursorTex;
  irr::u32 picSizeX;
  irr::u32 picSizeY;

public :

  CCursor(irr::IrrlichtDevice *pDevice, const irr::io::path &filename)
  {
    device = pDevice;
    driver = device->getVideoDriver();
    cursorTex = driver->getTexture(filename);
    device->getCursorControl()->setVisible(false);
    picSizeX = cursorTex->getSize().Width;
    picSizeY = cursorTex->getSize().Height;
  }

  void drawCursor()
  {
    driver->draw2DImage(cursorTex,
                        device->getCursorControl()->getPosition(),
                        recti(0,0, picSizeX,picSizeY),
                        0,
                        SColor(255, 255,255,255),
                        true);
  }
};

#endif
