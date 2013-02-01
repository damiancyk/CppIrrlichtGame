#include "CProgressBar.h"

IProgressBar::IProgressBar(IGUIEnvironment * guienv,const core::rect<s32>& rectangle,s32 id,IGUIElement * parent) : IGUIElement(EGUIET_ELEMENT,guienv,parent,id,rectangle)
{
   total = rectangle.LowerRightCorner.X - rectangle.UpperLeftCorner.X;
   gui = guienv;
   bar = rectangle;

   if(parent == 0)
      guienv->getRootGUIElement()->addChild(this); //Ensure that draw method is called
   vdriver = this->gui->getVideoDriver();
   fillcolor.set(255,255,255,255);
   emptycolor.set(255,0,0,0);
   border = bar;

IGUIFont* fontSimple=gui->getFont ( "textures/fonts/fontSimple.xml" );
tt=gui->addStaticText ( str.c_str(), rect<s32 > ( position2di ( rectangle.getCenter().X-130, rectangle.getCenter().Y-rectangle.getHeight()-20 ), dimension2di ( 260, 20 ) ), false, false, 0 );
tt->setOverrideColor(video::SColor(250,250,250,250));
tt->setDrawBorder(true);
tt->setBackgroundColor(video::SColor(250,0,0,0));
tt->setDrawBackground(true);
tt->setOverrideFont(fontSimple);

this->setProgress(0,"");
}
void IProgressBar::setColors(irr::video::SColor progress,irr::video::SColor filling)
{
   fillcolor = progress;
   emptycolor = filling;
}
void IProgressBar::addBorder(irr::s32 size,irr::video::SColor color)
{
   bordercolor = color;
   border = bar;
   border.UpperLeftCorner.X -= size;
   border.UpperLeftCorner.Y -= size;
   border.LowerRightCorner.X += size;
    border.LowerRightCorner.Y += size;
}

void IProgressBar::setProgress(irr::u32 progress, core::stringw infoProgress)
{
   if(progress > 100)
      progress = 0;

   u32 xpercentage;
   xpercentage = (progress * total)/100; //Reducing to the bar size
   tofill.UpperLeftCorner.set(bar.UpperLeftCorner.X,bar.UpperLeftCorner.Y);
   tofill.LowerRightCorner.set(bar.UpperLeftCorner.X+xpercentage,bar.LowerRightCorner.Y);
   empty.UpperLeftCorner.set(tofill.LowerRightCorner.X,tofill.UpperLeftCorner.Y);
   empty.LowerRightCorner.set(bar.LowerRightCorner.X,bar.LowerRightCorner.Y);
      tt->setText(infoProgress.c_str());




gui->drawAll();
gui->getVideoDriver()->endScene();
gui->getVideoDriver()->beginScene ( true, true, video::SColor ( 0, 0, 0, 0 ) );
}

void IProgressBar::draw()
{
   if(this->IsVisible == false)
      return;

   vdriver->draw2DRectangle(bordercolor,border);
   vdriver->draw2DRectangle(fillcolor,tofill);
   vdriver->draw2DRectangle(emptycolor,empty);
}
