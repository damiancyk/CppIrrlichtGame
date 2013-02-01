
#include "object.h"

Object::Object ( )
{
    gameSettings.resolutionX = 800;
    gameSettings.resolutionY = 600;
    gameSettings.buttonX = 128;
    gameSettings.buttonY = 40;
    gameSettings.isSound = true;
}

void Object::loadMenuGUI ( Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, eTypeOfMainMenu typeMainMenu)
{
    //smgr->getMeshCache()->clear();
    //driver->removeAllHardwareBuffers();
    //driver->removeAllTextures();
    //smgr->getVideoDriver()->setMaterial(SMaterial());
    //smgr->getVideoDriver()->removeAllTextures();
    gui->clear();

    gui->getSkin()->setColor ( EGDC_BUTTON_TEXT, video::SColor(250,250,250,250) );
    gui->getSkin()->setColor ( EGDC_3D_HIGH_LIGHT, video::SColor(0,110,110,110) );
    gui->getSkin()->setColor ( EGDC_3D_FACE, video::SColor(0,110,110,110) );
    gui->getSkin()->setColor ( EGDC_WINDOW, video::SColor(240,150,150,150) );

    switch ( typeMainMenu )
    {
    case GUI_MENU_TYPE_MAIN:
        device->getCursorControl()->setVisible ( false );

        for ( u32 i = 0; i < EGDC_COUNT; ++i )
        {
            SColor col = gui->getSkin()->getColor ( ( EGUI_DEFAULT_COLOR ) i );
            col.setAlpha ( 250 );
            gui->getSkin()->setColor ( ( EGUI_DEFAULT_COLOR ) i, col );
        }

        //zaladowanie czcionek
        sGui.font1 = gui->getFont ( "textures/fonts/fonthaettenschweiler.bmp" );
        fontDefault=device->getGUIEnvironment()->getBuiltInFont();
        sGui.skin = gui->getSkin();

        if ( sGui.font1 )
        {
            sGui.skin->setFont ( sGui.font1 );
        }

        if ( sGui.font1 )
        {
            sGui.skin->setFont ( sGui.font1, EGDF_TOOLTIP );
        }

        if(sSet->resolution.Height==600)sGui.iBackgroundRight = driver->getTexture ( "textures/gui/iBackgroundRight.jpg" );
        else sGui.iBackgroundRight = driver->getTexture ( "textures/gui/iBackgroundRightBig.jpg" );

        sGui.iGuiBackgroundRight = gui->addImage ( sGui.iBackgroundRight, core::position2d<s32 > ( sSet->resolution.Width - 200, 0 ), true, 0, -1, L"iGuiBackgroundRight" );

        //sGui.iGuiBackgroundRight->setImageExtend(16, 34, 16, 20);
        sGui.iGuiBackgroundRight->setScaleImage ( true );
        sGui.iGuiBackgroundRight->setColor ( SColor ( 150, 250, 250, 0 ) );

        sGui.bStart = gui->addButton ( rect<s32 > ( position2di ( sSet->resolution.Width - 210 + 50, (s32)(sSet->resolution.Height/2) -( s32 ) ( gameSettings.buttonY * 3.25 ) ), dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ), 0, GUI_ID_B_START, L"", L"Rozpocznij gre" );//co do polozenia mozna tez poda� rect<s32 > (10, 320, 110, 320 + 32)
        sGui.iBStart = smgr->getVideoDriver()->getTexture ( "textures/gui/bStart.png" );
        sGui.bStart->setImage ( sGui.iBStart );
        sGui.bStart->setPressedImage ( smgr->getVideoDriver()->getTexture ( "textures/gui/bStartP.png" ) );
        sGui.bStart->setScaleImage ( true );
        sGui.bStart->setUseAlphaChannel ( true );

        sGui.bStart=gui->addButton ( rect<s32 > ( position2di ( sSet->resolution.Width - 210 + 50, (s32)(sSet->resolution.Height/2) -( s32 ) ( gameSettings.buttonY * 2 ) ), dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ), 0, GUI_ID_B_TEST, L"", L"ZABIJ WILKOLAKA ABY UAKTYWNIC" );
        sGui.iBStart = smgr->getVideoDriver()->getTexture ( "textures/gui/bTest.png" );
        sGui.bStart->setImage ( sGui.iBStart );
        sGui.bStart->setPressedImage ( smgr->getVideoDriver()->getTexture ( "textures/gui/bTestP.png" ) );
        sGui.bStart->setScaleImage ( true );
        sGui.bStart->setUseAlphaChannel(false);

        sGui.bOption = gui->addButton ( rect<s32 > ( position2di ( sSet->resolution.Width - 210 + 50, (s32)(sSet->resolution.Height/2)  ), dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ), 0, GUI_ID_B_OPTIONS, L"", L"Ustawienia gry" );
        sGui.iBOption = smgr->getVideoDriver()->getTexture ( "textures/gui/bOptions.png" );
        sGui.bOption->setImage ( sGui.iBOption );
        sGui.bOption->setPressedImage ( smgr->getVideoDriver()->getTexture ( "textures/gui/bOptionsP.png" ) );
        sGui.bOption->setScaleImage ( true );
        sGui.bOption->setUseAlphaChannel ( true );

        sGui.bQuit = gui->addButton ( rect<s32 > ( position2di ( sSet->resolution.Width - 210 + 50, (s32)(sSet->resolution.Height/2) + ( s32 ) ( gameSettings.buttonY * 1.5 ) ), dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ), 0, GUI_ID_B_QUIT, L"", L"Wyjdz z gry" );
        sGui.iBQuit = smgr->getVideoDriver()->getTexture ( "textures/gui/bQuit.png" );
        sGui.bQuit->setImage ( sGui.iBQuit );
        sGui.bQuit->setPressedImage ( smgr->getVideoDriver()->getTexture ( "textures/gui/bQuitP.png" ) );
        sGui.bQuit->setScaleImage ( true );
        sGui.bQuit->setUseAlphaChannel ( true );

        sGui.iLogoIrrlicht = driver->getTexture ( "textures/gui/irrlichtlogo2.png" );
        sGui.iGuiLogoIrrlicht = gui->addImage ( sGui.iLogoIrrlicht, core::position2d<s32 > ( sSet->resolution.Width - 160, 60 ), true, 0, -1, L"iGuiLogoIrrlicht" );
        sGui.iGuiLogoIrrlicht->setColor(SColor(200,100,200,100));
        sGui.iLogoIrrKlang = driver->getTexture ( "textures/gui/irrKlang.png" );
        sGui.iGuiLogoIrrKlang = gui->addImage ( sGui.iLogoIrrKlang, core::position2d<s32 > ( sSet->resolution.Width - 170, sSet->resolution.Height-70 ), true, 0, -1, L"iGuiLogoIrrKlang" );
        sGui.iGuiLogoIrrKlang->setColor(SColor(100,100,200,100));
        sGui.iLogoIrrAI = driver->getTexture ( "textures/gui/irrAi.png" );
        sGui.iGuiLogoIrrAI = gui->addImage ( sGui.iLogoIrrAI, core::position2d<s32 > ( sSet->resolution.Width - 170, sSet->resolution.Height-170 ), true, 0, -1, L"iGuiLogoIrrAI" );
        sGui.iGuiLogoIrrAI->setColor(SColor(100,100,200,100));
        sGui.iLogoIrrPhysx = driver->getTexture ( "textures/gui/irrPhysxIcon.png" );
        sGui.iGuiLogoIrrPhysx = gui->addImage ( sGui.iLogoIrrPhysx, core::position2d<s32 > ( sSet->resolution.Width - 170, sSet->resolution.Height-120 ), true, 0, -1, L"iGuiLogoIrrPhysx" );
        sGui.iGuiLogoIrrPhysx->setColor(SColor(100,100,200,100));
        //gui->saveGUI("save/gui/mainMenu.xml",0);//zerowy parametr zapisal wszystko co jest aktualnie, nie tworzy folderow(musi istniec)
        IGUIImage *i;

        if(sSet->resolution.Height==600)
        {
            i = gui->addImage ( driver->getTexture ( "textures/gui/up.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/down.png" ), core::position2d<s32 > ( 0, sSet->resolution.Height-16 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/left.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/right.png" ), core::position2d<s32 > ( sSet->resolution.Width - 200 - 16, 0 ), true, 0, -1, L"iGuiFrame" );
        }
        else
        {
            i = gui->addImage ( driver->getTexture ( "textures/gui/upBig.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/downBig.png" ), core::position2d<s32 > ( 0, sSet->resolution.Height-16 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/leftBig.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/rightBig.png" ), core::position2d<s32 > ( sSet->resolution.Width - 200 - 16, 0 ), true, 0, -1, L"iGuiFrame" );

        }

        i=gui->addImage ( driver->getTexture ( "textures/gui/title.png" ), core::position2d<s32 > ( (s32)((sSet->resolution.Width-200)/2)-300, 0 ), true, 0, -1, L"iGuiTitle" );
        i->setColor(SColor(200,200,200,200));

        IGUIStaticText *t[10];
        t[0] = gui->addStaticText ( L"############################################", rect<s32 > ( position2di ( 20, sSet->resolution.Height-115 ), dimension2di ( 500, 20 ) ), false, false, 0 );
        t[1] = gui->addStaticText ( L"          --==Praca inzynierska==--", rect<s32 > ( position2di ( 20, sSet->resolution.Height-100 ), dimension2di ( 200, 20 ) ), false, false, 0 );
        t[2] = gui->addStaticText ( L"   Temat: Zastosowanie i implementacja silnika graficznego Irrlicht w grze komputerowej 3D", rect<s32 > ( position2di ( 20, sSet->resolution.Height-80 ), dimension2di ( 500, 20 ) ), false, false, 0 );
        t[3] = gui->addStaticText ( L"   Autor: Damian Pieta, GR413A", rect<s32 > ( position2di ( 20, sSet->resolution.Height-60 ), dimension2di ( 200, 20 ) ), false, false, 0 );
        t[4] = gui->addStaticText ( L"############################################", rect<s32 > ( position2di ( 20, sSet->resolution.Height-45 ), dimension2di ( 500, 20 ) ), false, false, 0 );

        for ( int i = 1; i < 4; i++ )
        {
            t[i]->setOverrideColor ( SColor ( 250, 220, 220, 220 ) );
        }

        t[0]->setOverrideColor ( SColor ( 250, 140, 140, 20 ) );

        t[4]->setOverrideColor ( SColor ( 250, 140, 140, 20 ) );
        //i=gui->addImage(rect<s32>(position2d(0,0), dimension2d(100,100)),0,-1,"textures/gui/right.png");
        //addImage (const core::rect< s32 > &rectangle, IGUIElement *parent=0, s32 id=-1, const wchar_t *text=0)=0
        i->setScaleImage ( true );
        break;
    case GUI_MENU_TYPE_OPTION:

        for ( u32 i = 0; i < EGDC_COUNT; ++i )
        {
            SColor col = gui->getSkin()->getColor ( ( EGUI_DEFAULT_COLOR ) i );
            col.setAlpha ( 250 );
            gui->getSkin()->setColor ( ( EGUI_DEFAULT_COLOR ) i, col );
        }

        //zaladowanie tla menu glownego
        if(sSet->resolution.Height==600)sGui.iBackgroundRight = driver->getTexture ( "textures/gui/iBackgroundSettingsRight.jpg" );
        else sGui.iBackgroundRight = driver->getTexture ( "textures/gui/iBackgroundSettingsRightBig.jpg" );
        sGui.iGuiBackgroundRight = gui->addImage ( sGui.iBackgroundRight, core::position2d<s32 > ( sSet->resolution.Width - 200, 0 ), true, 0, -1, L"iGuiBackgroundRight" );
        sGui.iGuiBackgroundRight->setScaleImage ( true );
        sGui.iGuiBackgroundRight->setColor ( SColor ( 200, 150, 240, 150 ) );

        //nowe okno
        sGui.window = gui->addWindow ( rect<s32 > ( position2di ( sSet->resolution.Width - 200+15, 65 ), dimension2di ( 200-30, sSet->resolution.Height-50-30 ) ), false, L"Opcje gry", 0, 101 );

        if ( sGui.window )
        {
            sGui.window->setDraggable ( false );
            sGui.window->setDrawTitlebar ( false );
            sGui.window->setDrawBackground ( false );
            sGui.window->getCloseButton ()->setVisible ( false );
        }

        sGui.s = gui->addScrollBar ( true, rect<s32 > (
                                         position2di ( sSet->resolution.Width-185, 5),
                                         dimension2di ( 170, 10 ) ), 0, GUI_ID_S_ );

        if ( sGui.s )
        {
            sGui.s->setMax ( 255 );
            sGui.s->setPos ( gui->getSkin()->getColor ( EGDC_WINDOW ).getAlpha() );
        }

        gui->addStaticText ( L"         ### ON / OFF ###", rect<s32 > ( position2di ( sSet->resolution.Width-190, 60 ), dimension2di ( 170, 20 ) ), false, false, 0 );

        gui->addStaticText ( L"-=SCENA 1=-", rect<s32 > ( position2di ( 20, 32 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );

        sGui.checkEffects=gui->addCheckBox ( false, rect<s32 > (
                position2di ( 0, 10 ),dimension2di ( 20, 20 ) ),
                                             sGui.window, GUI_ID_EFFECTS );

        gui->addStaticText ( L"EFEKTY", rect<s32 > ( position2di ( 20, 12 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkMirror =  gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 50 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_MIRROR );
        gui->addStaticText ( L"lustro", rect<s32 > ( position2di ( 20, 52 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkFire = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 70 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_FIRE );
        gui->addStaticText ( L"ogien", rect<s32 > ( position2di ( 20, 72 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkFog = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 90 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_FOG );
        gui->addStaticText ( L"mgla", rect<s32 > ( position2di ( 20, 92 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkLightAmbient = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 110 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_LIGHT_AMBIENT );
        gui->addStaticText ( L"swiatlo::rozproszone", rect<s32 > ( position2di ( 20, 112 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkLightPoint = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 130 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_LIGHT_POINT );
        gui->addStaticText ( L"swiatlo::punktowe", rect<s32 > ( position2di ( 20, 132 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkHeroLight = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 150 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_HERO_LIGHT );
        gui->addStaticText ( L"bohater::swiatlo", rect<s32 > ( position2di ( 20, 152 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkHeroTransparency = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 170 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_HERO_TRANSPARENCY );
        gui->addStaticText ( L"bohater::przezroczystosc", rect<s32 > ( position2di ( 20, 172 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkLightMap = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 190 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_LIGHT_MAP );
        gui->addStaticText ( L"swiatlo::mapa swietlna", rect<s32 > ( position2di ( 20, 192 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkHeroShadow = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 210 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_HERO_SHADOW );
        gui->addStaticText ( L"bohater::cien", rect<s32 > ( position2di ( 20, 212 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );

        gui->addStaticText ( L"-=SCENA 2=-", rect<s32 > ( position2di ( 20, 252 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkSnow = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 270 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_SNOW );
        gui->addStaticText ( L"snieg", rect<s32 > ( position2di ( 20, 272 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkShaderToon = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 290 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_SHADERTOON );
        gui->addStaticText ( L"shader toon", rect<s32 > ( position2di ( 20, 292 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkHeroSphericMap = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 310 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_HERO_SPHERIC_MAP );
        gui->addStaticText ( L"bohater::mapa sferyczna", rect<s32 > ( position2di ( 20, 312 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkForest = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 330 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_FOREST );
        gui->addStaticText ( L"drzewa", rect<s32 > ( position2di ( 20, 332 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkWerewolves = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 350 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_WEREWOLVES );
        gui->addStaticText ( L"stado wilkolakow", rect<s32 > ( position2di ( 20, 352 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkTerrainQuality = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 370 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_TERRAIN_QUALITY );
        gui->addStaticText ( L"szczegoly terenu", rect<s32 > ( position2di ( 20, 372 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkSky = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 390 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_SKY );
        gui->addStaticText ( L"niebo", rect<s32 > ( position2di ( 20, 392 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );
        sGui.checkCompass = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, 410 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_COMPASS );
        gui->addStaticText ( L"kompas", rect<s32 > ( position2di ( 20, 412 ), dimension2di ( 200, 20 ) ), false, false, sGui.window );

        sGui.checkSounds = gui->addCheckBox ( false, rect<s32 > ( position2di ( 150, 10 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_SOUNDS );
        gui->addStaticText ( L"DZWIEKI", rect<s32 > ( position2di ( 110, 12 ), dimension2di ( 40, 20 ) ), false, false, sGui.window );
        sGui.checkSTracks = gui->addCheckBox ( false, rect<s32 > ( position2di ( 150, 50 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_STRACKS );
        gui->addStaticText ( L"tracki", rect<s32 > ( position2di ( 110, 52 ), dimension2di ( 40, 20 ) ), false, false, sGui.window );
        sGui.checkSEffects = gui->addCheckBox ( false, rect<s32 > ( position2di ( 150, 70 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_SEFFECTS );
        gui->addStaticText ( L"odglosy", rect<s32 > ( position2di ( 110, 72 ), dimension2di ( 40, 20 ) ), false, false, sGui.window );
        sGui.checkS3D = gui->addCheckBox ( false, rect<s32 > ( position2di ( 150, 90 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_S3D );
        gui->addStaticText ( L"dzw 3D", rect<s32 > ( position2di ( 110, 92 ), dimension2di ( 40, 20 ) ), false, false, sGui.window );

        sGui.checkMEffects = gui->addCheckBox ( false, rect<s32 > ( position2di ( 0, sGui.window->getClientRect().getHeight()-200 ), dimension2di ( 20, 20 ) ), sGui.window, GUI_ID_M_EFFECTS );
        gui->addStaticText ( L"efekty w menu", rect<s32 > ( position2di ( 20, sGui.window->getClientRect().getHeight()-200 ), dimension2di ( 140, 20 ) ), false, false, sGui.window );

        gui->addButton ( rect<s32 > ( position2di ( 10, sGui.window->getClientRect().getHeight()-20), dimension2di ( 150, 20 ) ), sGui.window, GUI_ID_B_AUTHOR, L"Autor", L"tekst z pliku .xml" );

        sGui.bSave = gui->addButton ( rect<s32 > ( position2di ( 0, sGui.window->getClientRect().getHeight() ), dimension2di ( 85, 20 ) ), sGui.window, GUI_ID_B_SAVE, L"Zapisz", L"Zapisanie ustawien" );

        if ( sGui.bSave )
        {
            sGui.bSave->setEnabled ( false );
        }

        gui->addStaticText ( L"GLOSNOSC:", rect<s32 > ( position2di ( 10, sGui.window->getClientRect().getHeight()-180 ), dimension2di ( 50, 20 ) ), false, false, sGui.window );
        sGui.sVolume = gui->addScrollBar ( true, rect<s32 > ( position2di ( 50, sGui.window->getClientRect().getHeight()-160), dimension2di ( 110, 10 ) ), sGui.window, GUI_ID_VOLUME );
        if ( sGui.sVolume )
        {
            sGui.sVolume->setMax ( 100 );
            sGui.sVolume->setPos ( sSet->volume*100 );
        }

        gui->addStaticText ( L"Nick:", rect<s32 > ( position2di ( 10, sGui.window->getClientRect().getHeight()-140 ), dimension2di ( 80, 20 ) ), false, false, sGui.window );
        sGui.box = gui->addComboBox(core::rect<s32>(position2di ( 10, sGui.window->getClientRect().getHeight()-120 ), dimension2di ( 40, 20 )), sGui.window, 0);
        sGui.box->addItem(L"Mr");
        sGui.box->addItem(L"Ms");
        sGui.edtNick = gui->addEditBox ( L"DamianCyK", rect<s32 > ( position2di ( 50, sGui.window->getClientRect().getHeight()-120 ), dimension2di ( 100, 20 ) ), true, sGui.window, 107 );
        if ( sGui.edtNick )
        {
            sGui.edtNick->setID ( GUI_ID_EDT_NICK );
            sGui.edtNick->setMax ( 32 );
        }

        sGui.listbox = gui->addListBox(rect<s32>(10, sGui.window->getClientRect().getHeight()-100, 180, sGui.window->getClientRect().getHeight()-40), sGui.window);
        sGui.listboxCount=0;
        sGui.listbox->setAutoScrollEnabled(true);
        sGui.listbox->addItem(L"####", 1);

        sGui.bReturnToMain = gui->addButton ( rect<s32 > ( position2di ( 85, sGui.window->getClientRect().getHeight() ), dimension2di ( 85, 20 ) ), sGui.window, GUI_ID_B_RETURN_TO_MAIN, L"Powrot<=", L"Powrot do glownego menu" );

        sGui.bReturnToMain->setDrawBorder ( true );
        gui->saveGUI ( "save/gui/options.xml", 0 );

        if(sSet->resolution.Height==600)
        {
            i = gui->addImage ( driver->getTexture ( "textures/gui/up.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/down.png" ), core::position2d<s32 > ( 0, sSet->resolution.Height-16 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/left.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/right.png" ), core::position2d<s32 > ( sSet->resolution.Width - 200 - 16, 0 ), true, 0, -1, L"iGuiFrame" );
        }
        else
        {
            i = gui->addImage ( driver->getTexture ( "textures/gui/upBig.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/downBig.png" ), core::position2d<s32 > ( 0, sSet->resolution.Height-16 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/leftBig.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
            i = gui->addImage ( driver->getTexture ( "textures/gui/rightBig.png" ), core::position2d<s32 > ( sSet->resolution.Width - 200 - 16, 0 ), true, 0, -1, L"iGuiFrame" );

        }
        break;
    default:
        break;
    }

    //zaladowanie menu w pasku
    sGui.menu = gui->addMenu();

    sGui.menu->setRelativePosition ( rect<s32> ( position2di ( 0, 0 ), dimension2di ( 300, 20 ) ) ); //(rect<s32>(0,20,300,40));

    sGui.menu->addItem ( L"Menu Gry", GUI_ID_M_GAME, true, true );

    sGui.menu->setMaxSize ( dimension2du ( 75, 15 ) );

    sGui.menu->setRelativePosition ( position2di ( 600, 400 ) );

    IGUIContextMenu* subMenu;

    subMenu = sGui.menu->getSubMenu ( 0 );

    subMenu->addItem ( L"Opcje gry", GUI_ID_M_GAME_OPTIONS, true, true );

    subMenu->addItem ( L"Opcje graficzne", GUI_ID_M_GRAPHICS_OPTIONS, true, true );

    subMenu->addItem ( L"Opcje dzwiekowe", GUI_ID_M_SOUND_OPTIONS, true, true );

    subMenu->addSeparator();

    subMenu->addItem ( L"Autor", GUI_ID_M_SOUND_OPTIONS, true, true );

    subMenu = sGui.menu->getSubMenu ( 0 )->getSubMenu ( 0 );

    subMenu->addItem ( L"Zapisz gre", GUI_ID_M_SAVE_GAME );

    subMenu->addItem ( L"Wczytaj gre", GUI_ID_M_LOAD_GAME );

    subMenu->addItem ( L"Wyjdz z gry", GUI_ID_M_EXIT_GAME );

    subMenu = sGui.menu->getSubMenu ( 0 )->getSubMenu ( 1 );

    subMenu->addItem ( L"Ustaw rozdzielczosc", GUI_ID_M_SET_RESOLUTION, true, true );

    subMenu->addItem ( L"Ustaw glebie kolorow", GUI_ID_M_SET_COLOR_DEPTH, true, true );

    subMenu = sGui.menu->getSubMenu ( 0 )->getSubMenu ( 2 );

    subMenu->addItem ( L"Dzwiek 3D Wlacz/Wylacz", GUI_ID_M_SOUND_3D_ON_OFF );

    subMenu->addItem ( L"Zwieksz Glosnosc", GUI_ID_M_INCREASE_VOLUME );

    subMenu->addItem ( L"Zmniejsz Glosnosc", GUI_ID_M_DECREASE_VOLUME );

    subMenu = sGui.menu->getSubMenu ( 0 )->getSubMenu ( 1 )->getSubMenu ( 0 );

    subMenu->addItem ( L"640 x 480", GUI_ID_M_RESOLUTION_640_480 );

    subMenu->addItem ( L"800 x 600", GUI_ID_M_RESOLUTION_800_600 );

    subMenu->addItem ( L"960 x 600", GUI_ID_M_RESOLUTION_960_600 );

    subMenu->addItem ( L"1024 x 768", GUI_ID_M_RESOLUTION_1024_768 );

    subMenu->addItem ( L"1152 x 864", GUI_ID_M_RESOLUTION_1152_864 );

    subMenu->addItem ( L"1280 x 720", GUI_ID_M_RESOLUTION_1280_720 );

    subMenu = sGui.menu->getSubMenu ( 0 )->getSubMenu ( 1 )->getSubMenu ( 1 );

    subMenu->addItem ( L"16 bit", 0 );

    subMenu->addItem ( L"32 bit", 0 );

    subMenu = sGui.menu->getSubMenu ( 0 )->getSubMenu ( 4 );

    subMenu->addItem ( L"Temat Pracy Inzynierskiej", GUI_ID_M_TOPIC );

    subMenu->addItem ( L"O Autorze", GUI_ID_M_AUTHOR );

// sGui.font1->drop();
//  sGui.iGuiBackgroundRight->drop();
}

void Object::loadIntro ( Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera )
{
    smgr->clear();
    smgr->getMeshCache()->clear();

    driver->removeAllHardwareBuffers();
    //driver->removeAllTextures();
    //smgr->getVideoDriver()->setMaterial(SMaterial());
    // smgr->getVideoDriver()->removeAllTextures(); //usuwa kursor
    device->getCursorControl()->setVisible ( false );

    IProgressBar* pb = new IProgressBar(gui,rect<s32>((int)(0.1*sSet->resolution.Width),(int)(0.93*sSet->resolution.Height),(int)(0.9*sSet->resolution.Width),(int)(0.98*sSet->resolution.Height)));
    pb->addBorder(5);
    pb->setColors(SColor(250,50,50,50), SColor(250,130,130,130));
    pb->drop();

    pb->setProgress(0, "");

    pb->setProgress(10, "wczytywanie bohatera");
    mHero = smgr->getMesh ( "objects/heroSydney/tris.MD2" );
    if ( mHero )
    {
        oHero = smgr->addAnimatedMeshSceneNode ( mHero );
    }

    if ( oHero )
    {
        oHero->setID ( OBJECT_ID_HERO );
        oHero->setPosition ( vector3df ( -100, 100, 20 ) );
        /* potrzebne do animacji innych niz .md2, normalnie wystarcza EMAT_
        oHero->setFrameLoop(0, 13);
        oHero->setAnimationSpeed(15);
         */
        oHero->setMD2Animation ( scene::EMAT_STAND );
        oHero->setMaterialTexture ( 0, driver->getTexture ( "objects/heroSydney/sydney.PCX" ) );
        oHero->setMaterialFlag(EMF_LIGHTING, false);
    }

    pb->setProgress(20, "wczytywanie przeciwnika");

    mEnemy = smgr->getMesh ( "objects/alphaWerewolf/tris.MD2" );
    if ( mEnemy )
    {
        oEnemy = smgr->addAnimatedMeshSceneNode ( mEnemy );
    }

    pb->setProgress(30, "wczytywanie terenu");
    smgr->loadScene ( "objects/terrain/terrain.irr" );

    pb->setProgress(70, "wczytywanie skybox");
    //skydome = smgr->addSkyDomeSceneNode(driver->getTexture("textures/skyDomes/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    skybox = smgr->addSkyBoxSceneNode (
                 //  driver->getTexture("textures/skyBoxes/skyBoxDarkNight/floodland_up.jpg"),
                 //  driver->getTexture("textures/skyBoxes/skyBoxDarkNight/floodland_dn.jpg"),
                 //  driver->getTexture("textures/skyBoxes/skyBoxDarkNight/floodland_lf.jpg"),
                 //  driver->getTexture("textures/skyBoxes/skyBoxDarkNight/floodland_rt.jpg"),
                 //  driver->getTexture("textures/skyBoxes/skyBoxDarkNight/floodland_ft.jpg"),
                 //  driver->getTexture("textures/skyBoxes/skyBoxDarkNight/floodland_bk.jpg"));
                 driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side6.jpg" ),
                 driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side1.jpg" ),
                 driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side2.jpg" ),
                 driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side3.jpg" ),
                 driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side4.jpg" ),
                 driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side5.jpg" ) );

    pb->setProgress(80, "wczytywanie kamery");

    if ( oEnemy )
    {
        oEnemy->setID ( IDFlag_IsPickable );
        oEnemy->setPosition ( core::vector3df ( 211, 206, 646 ) );
        oEnemy->setScale ( core::vector3df ( 2.f, 2.f, 2.f ) );
        oEnemy->setMaterialFlag ( EMF_LIGHTING, false );
        oEnemy->setMD2Animation ( scene::EMAT_STAND );
        oEnemy->setMaterialTexture ( 0, driver->getTexture ( "objects/alphaWerewolf/dark.PCX" ) );
        oEnemy->setLoopMode ( true );
        ITriangleSelector *selectorShooting;
        selectorShooting = smgr->createTriangleSelector ( oEnemy );
        oEnemy->setTriangleSelector ( selectorShooting );
        selectorShooting->drop();
    }

    if ( skybox )
    {
        skybox->setVisible ( true );
    }

    driver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );

    camera = smgr->addCameraSceneNode ( 0, vector3df ( -70, 50, -220 ), vector3df ( 180, 100, 100 ), true );
    camera->setTarget ( oHero->getPosition() );
    camera->setPosition ( oEnemy->getPosition() );

    collisionsNGravity ( smgr, camera, oHero );
    pb->setProgress(90, "konczenie rysowania");
    gui->clear();
    if(sSet->resolution.Height==600)
    {
        gui->addImage ( driver->getTexture ( "textures/intro/up.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
        gui->addImage ( driver->getTexture ( "textures/intro/down.png" ), core::position2d<s32 > ( 0, sSet->resolution.Height-64 ), true, 0, -1, L"iGuiFrame" );
    }
    else
    {
        gui->addImage ( driver->getTexture ( "textures/intro/upBig.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
        gui->addImage ( driver->getTexture ( "textures/intro/downBig.png" ), core::position2d<s32 > ( 0, sSet->resolution.Height-64 ), true, 0, -1, L"iGuiFrame" );
    }

    IGUIFont* fontSimple=gui->getFont ( "textures/fonts/fontSimple.xml" );
    IGUIStaticText *tt;
    tt=gui->addStaticText ( L"ESC=>KONTYNUUJ", rect<s32 > ( position2di ( 20, sSet->resolution.Height-64-20), dimension2di ( 160, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSimple);
}

void Object::loadScene1 (Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera)
{
    smgr->clear();
    smgr->getMeshCache()->clear();

    driver->removeAllHardwareBuffers();//te dwie linijki kodu to usuwanie mapy
    //driver->removeAllTextures();//usuwa mi tekstury z efektow, to niedobrze :]
    //smgr->getVideoDriver()->setMaterial(SMaterial());
    //smgr->getVideoDriver()->removeAllTextures();
    //pDriver->removeTexture(rtt);
    // IGUIEnvironment * mygui = CDeviceFactory::s_gui;
    device->getCursorControl()->setVisible ( false );


    if(sSet->lightPoint)
    {
        ISceneNode* node;
        node = smgr->addLightSceneNode(0, vector3df ( 360, 0, 380 ),
                                       video::SColorf(1.0f, 1.f, 1.f, 1.0f), 500.0f);
        node->setID(OBJECT_ID_LIGHT_POINT);
        node->setMaterialFlag(EMF_ZBUFFER , false);
        scene::ISceneNodeAnimator* anim = 0;
        anim = smgr->createFlyCircleAnimator (core::vector3df(0,150,0),250.0f);
        node->addAnimator(anim);
        anim->drop();

        // attach billboard to light

        node = smgr->addBillboardSceneNode(node, core::dimension2d<f32>(50, 50));
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        node->setMaterialTexture(0, driver->getTexture("textures/effects/particlewhite.bmp"));
    }

    {
//     scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode ( 0, -1,
//                                     32,                              // Subdivisions on U axis
//                                     32,                              // Subdivisions on V axis
//                                     video::SColor ( 200, 255, 255, 255 ), // foot color
//                                     video::SColor ( 200, 0, 0, 0 ) );  // tail color

//  if ( n )
//    {
//      n->setScale ( core::vector3df ( 56.0f, 56.0f, 56.0f ) );
//      n->setPosition ( core::vector3df ( 200, 0, 300 ) );
//
//      array<video::ITexture*> textures;
//
//      for ( s32 g = 7; g > 0; --g )
//        {
//          core::stringc tmp;
//          tmp = "objects/terrainMenu/portal";
//          tmp += g;
//          tmp += ".bmp";
//          video::ITexture* t = driver->getTexture ( tmp.c_str() );
//          textures.push_back ( t );
//        }
//
//      scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator ( textures, 150 );
//      n->addAnimator ( glow );
//      glow->drop();
//    }

    }

    SColor s=SColor(200,100,0,0);
    IGUIImage* i;

    i=gui->addImage ( driver->getTexture ( "textures/gui/title.png" ), core::position2d<s32 > ( (s32)(sSet->resolution.Width/2-300), 0 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrlichtlogo2.png" ), core::position2d<s32 > (20, sSet->resolution.Height-128-150 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrKlang.png" ), core::position2d<s32 > (sSet->resolution.Width-138-20, sSet->resolution.Height-128-150 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrPhysxIcon.png" ), core::position2d<s32 > (20, sSet->resolution.Height-128-30 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrAi.png" ), core::position2d<s32 > (sSet->resolution.Width-40-20, sSet->resolution.Height-128-100), true, 0, -1, L"splashScreen" );
    i->setColor(s);


    IProgressBar* pb = new IProgressBar(gui,rect<s32>((int)(0.1*sSet->resolution.Width),(int)(0.93*sSet->resolution.Height),(int)(0.9*sSet->resolution.Width),(int)(0.98*sSet->resolution.Height)));
    pb->addBorder(5);
    pb->setColors(SColor(250,50,50,50), SColor(250,130,130,130));
    pb->drop();

    pb->setProgress(0, "");

    pb->setProgress(10, "WCZYTYWANIE BOHATERA");

    mHero = smgr->getMesh ( "objects/hero/tris.MD2" );
    if ( mHero )
    {
        oHero = smgr->addAnimatedMeshSceneNode ( mHero );
    }

    mGun = smgr->getMesh ( "objects/hero/weapon.MD2" );
    if ( mGun )
    {
        oGun = smgr->addAnimatedMeshSceneNode ( mGun );
    }

    if ( oGun )
    {
        oGun->setID ( OBJECT_ID_GUN );
        oGun->setPosition ( oHero->getPosition() );
        oGun->setMaterialFlag ( EMF_LIGHTING, false );
        oGun->setMD2Animation ( scene::EMAT_STAND );
        oGun->setMaterialTexture ( 0, driver->getTexture ( "objects/hero/weapon.PCX" ) );
    }

    pb->setProgress(20, "WCZYTYWANIE PRZECIWNIKA");

    mEnemy = smgr->getMesh ( "objects/alphaWerewolf/tris.MD2" );
    if ( mEnemy )
    {
        oEnemy = smgr->addAnimatedMeshSceneNode ( mEnemy );
    }

    pb->setProgress(30, "WCZYTYWANIE TERENU");
    smgr->loadScene ( "objects/terrain/terrain.irr" );

    IBillboardSceneNode* b;
    vector3df posB=vector3df(440,5,440);

    b=smgr->addBillboardTextSceneNode(0,  L"UWAGA", 0,
                                      dimension2d<f32>(120,80), posB+vector3df(0,55,0),
                                      0, SColor(250,200,0,0), SColor(250,0,0,0));

    b=smgr->addBillboardTextSceneNode(0,  L"NIE PODCHODZ", 0,dimension2d<f32>(100,20), posB+vector3df(0,20,0), 0, SColor(250,200,0,0), SColor(250,0,0,0));
    b=smgr->addBillboardTextSceneNode(0,  L"ZA BLISKO..", 0,dimension2d<f32>(40,5), posB, 0, SColor(250,200,0,0), SColor(250,0,0,0));

    pb->setProgress(70, "WCZYTYWANIE SKYBOX");

    skydome = smgr->addSkyDomeSceneNode(
                  driver->getTexture("textures/skyDomes/skydome.jpg"),
                  16, 8, 0.95f, 2.0f);

    pb->setProgress(80, "WCZYTYWANIE KAMERY");
    camera = smgr->addCameraSceneNodeFPS ( 0, 50.f, 0.1f );

    if ( camera )
    {
        camera->setID ( OBJECT_ID_CAMERA );
        camera->setFarValue ( 2000.0f );
    }

    if ( oHero )
    {
        oHero->setID ( OBJECT_ID_HERO );
        oHero->setPosition ( vector3df ( -100, 100, 20 ) );
        oHero->setMaterialTexture ( 0, driver->getTexture ( "objects/hero/fonz.PCX" ) );
        if((sSet->heroLight)&&( sSet->effects ))
        {
            oHero->setMaterialFlag ( EMF_LIGHTING, true );
            oHero->getMaterial(0).Shininess = 800.0f;
        }
        else
        {
            oHero->setMaterialFlag ( EMF_LIGHTING, false );
        }
        /* potrzebne do animacji innych niz .md2, normalnie wystarcza EMAT_
        oHero->setFrameLoop(0, 13);
        oHero->setAnimationSpeed(15);
         */
        oHero->setMD2Animation ( scene::EMAT_STAND );

        //oHero->setMaterialType(EMT_SOLID);
        if((sSet->heroTransparency)&&( sSet->effects ))oHero->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        if((sSet->heroSphericMap)&&( sSet->effects ))oHero->setMaterialType(video::EMT_SPHERE_MAP);
//      if(( sSet->effects )){
//        oHero->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(oHero->isDebugDataVisible()^EDS_BBOX));
//        oHero->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(oHero->isDebugDataVisible()^EDS_NORMALS));
//        oHero->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(oHero->isDebugDataVisible()^EDS_SKELETON));
//        oHero->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(oHero->isDebugDataVisible()^EDS_MESH_WIRE_OVERLAY));
//        oHero->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(oHero->isDebugDataVisible()^EDS_HALF_TRANSPARENCY));
//        oHero->setDebugDataVisible((E_DEBUG_SCENE_TYPE)(oHero->isDebugDataVisible()^EDS_BBOX_BUFFERS));
//      }

        if((sSet->heroShadow)&&( sSet->effects ))
        {
            oHero->addShadowVolumeSceneNode();
            smgr->setShadowColor(video::SColor(100,0,0,0));
        }
        if ( oGun ) oHero->addChild ( oGun );
    }

//    // add white light
//	smgr->addLightSceneNode(0, core::vector3df(-15,5,-105),
//			video::SColorf(1.0f, 1.0f, 1.0f));

    // set ambient light
    if(sSet->lightAmbient )smgr->setAmbientLight(video::SColor(50,50,50,250));

    if ( oEnemy )
    {
        oEnemy->setPosition ( core::vector3df ( 211, 206, 646 ) );
        oEnemy->setScale ( core::vector3df ( 2.f, 2.f, 2.f ) );
        oEnemy->setMaterialFlag ( EMF_LIGHTING, false );
        oEnemy->setMD2Animation ( scene::EMAT_STAND );
        oEnemy->setMaterialTexture ( 0, driver->getTexture ( "objects/alphaWerewolf/dark.PCX" ) );
        oEnemy->setLoopMode ( true );
        ITriangleSelector *selectorShooting;
        selectorShooting = smgr->createTriangleSelector ( oEnemy );
        oEnemy->setTriangleSelector ( selectorShooting );
        oEnemy->setMaterialFlag(video::EMF_LIGHTING, false);
        oEnemy->setID ( OBJECT_ID_ENEMY );
        oEnemy->setVisible(false);
        selectorShooting->drop();
    }

    driver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );

    CSampleSceneNode *myNode =
        new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666, vector3df(35,-30,200));
    myNode->drop();
    myNode = 0;

    collisionsNGravity ( smgr, camera, oHero );
    pb->setProgress(90, "KONCZENIE RYSOWANIA");
}

void Object::loadScene2 (Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera)
{
    smgr->clear();
    smgr->getMeshCache()->clear();

    driver->removeAllHardwareBuffers();//te dwie linijki kodu to usuwanie mapy
    //driver->removeAllTextures();//usuwa mi tekstury z efektow, to niedobrze :]
    //smgr->getVideoDriver()->setMaterial(SMaterial());
    //smgr->getVideoDriver()->removeAllTextures();
    //pDriver->removeTexture(rtt);
    // IGUIEnvironment * mygui = CDeviceFactory::s_gui;
    device->getCursorControl()->setVisible ( false );

    SColor s=SColor(200,100,0,0);
    IGUIImage* i;

    i=gui->addImage ( driver->getTexture ( "textures/gui/title.png" ), core::position2d<s32 > ( (s32)(sSet->resolution.Width/2-300), 0 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrlichtlogo2.png" ), core::position2d<s32 > (20, sSet->resolution.Height-128-150 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrKlang.png" ), core::position2d<s32 > (sSet->resolution.Width-138-20, sSet->resolution.Height-128-150 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrPhysxIcon.png" ), core::position2d<s32 > (20, sSet->resolution.Height-128-30 ), true, 0, -1, L"splashScreen" );
    i->setColor(s);
    i=gui->addImage ( driver->getTexture ( "textures/gui/irrAi.png" ), core::position2d<s32 > (sSet->resolution.Width-40-20, sSet->resolution.Height-128-100), true, 0, -1, L"splashScreen" );
    i->setColor(s);

    IProgressBar* pb = new IProgressBar(gui,rect<s32>((int)(0.1*sSet->resolution.Width),(int)(0.93*sSet->resolution.Height),(int)(0.9*sSet->resolution.Width),(int)(0.98*sSet->resolution.Height)));
    pb->addBorder(5);
    pb->setColors(SColor(250,50,50,50), SColor(250,130,130,130));
    pb->drop();

    pb->setProgress(0, "");

    pb->setProgress(10, "WCZYTYWANIE BOHATERA");

    mHero = smgr->getMesh ( "objects/heroFrosty/tris.MD2" );
    if ( mHero )
    {
        oHero = smgr->addAnimatedMeshSceneNode ( mHero );
    }

    mGun = smgr->getMesh ( "objects/heroFrosty/weapon.MD2" );
    if ( mGun )
    {
        oGun = smgr->addAnimatedMeshSceneNode ( mGun );
    }

    if ( oGun )
    {
        oGun->setID ( OBJECT_ID_GUN );
        oGun->setPosition ( oHero->getPosition() );
        oGun->setMaterialFlag ( EMF_LIGHTING, false );
        oGun->setMD2Animation ( scene::EMAT_STAND );
        oGun->setMaterialTexture ( 0, driver->getTexture ( "objects/heroFrosty/weapon.PCX" ) );
    }

    pb->setProgress(20, "WCZYTYWANIE PRZECIWNIKA");

    mEnemy = smgr->getMesh ( "objects/alphaWerewolf/tris.MD2" );
    if ( mEnemy )
    {
        oEnemy = smgr->addAnimatedMeshSceneNode ( mEnemy );
    }
    if ( oEnemy )
    {
        oEnemy->setPosition ( core::vector3df ( 211, 206, 646 ) );
        oEnemy->setScale ( core::vector3df ( 2.f, 2.f, 2.f ) );
        oEnemy->setMaterialFlag ( EMF_LIGHTING, false );
        oEnemy->setMD2Animation ( scene::EMAT_STAND );
        oEnemy->setMaterialTexture ( 0, driver->getTexture ( "objects/alphaWerewolf/dark.PCX" ) );
        oEnemy->setLoopMode ( true );
        ITriangleSelector *selectorShooting;
        selectorShooting = smgr->createTriangleSelector ( oEnemy );
        oEnemy->setTriangleSelector ( selectorShooting );
        oEnemy->setMaterialFlag(video::EMF_LIGHTING, false);
        oEnemy->setID ( OBJECT_ID_ENEMY );
        oEnemy->setVisible(false);
        selectorShooting->drop();
    }

    if(sSet->werewolfes)loadWerewolves(driver, smgr, werewolves, vector3df(-1000,-580,500));

    pb->setProgress(30, "WCZYTYWANIE TERENU");
    terrain=loadHeightMap(driver, smgr, vector3df(-2000.f, -800.f, -2000.f), L"textures/terrain/", sSet);

    pb->setProgress(70, "WCZYTYWANIE SKYBOX");

//if(sSet->sky)
//{
//  skybox = smgr->addSkyBoxSceneNode (
//             driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side6.jpg" ),
//             driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side1.jpg" ),
//             driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side2.jpg" ),
//             driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side3.jpg" ),
//             driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side4.jpg" ),
//             driver->getTexture ( "textures/skyBoxes/skyBoxDarkNight/Side5.jpg" ) );
//}
// if ( skybox )
//    {
//      skybox->setVisible ( true );
//    }

    pb->setProgress(80, "WCZYTYWANIE KAMERY");
    camera = smgr->addCameraSceneNodeFPS ( 0, 50.f, 0.1f );

    if ( camera )
    {
        camera->setID ( OBJECT_ID_CAMERA );
        camera->setFarValue ( 2000.0f );
    }

    if ( oHero )
    {
        oHero->setID ( OBJECT_ID_HERO );
        oHero->setPosition ( vector3df ( -100, 100, 20 ) );
        oHero->setMaterialTexture ( 0, driver->getTexture ( "objects/heroFrosty/Frosty.PCX" ) );
        oHero->setMaterialFlag ( EMF_LIGHTING, false );
        oHero->setMD2Animation ( scene::EMAT_STAND );

        //oHero->setMaterialType(EMT_SOLID);
        if((sSet->heroSphericMap)&&( sSet->effects ))oHero->setMaterialType(video::EMT_SPHERE_MAP);

        if ( oGun ) oHero->addChild ( oGun );
    }

    driver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );
    collisionsNGravity ( smgr, camera, oHero );
    pb->setProgress(90, "KONCZENIE RYSOWANIA");
    if(sSet->forest)loadTrees(driver, smgr);
}

void Object::loadOutro ( Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera )
{
    smgr->clear();
    smgr->getMeshCache()->clear();

    driver->removeAllHardwareBuffers();
    device->getCursorControl()->setVisible ( false );

    mHero = smgr->getMesh ( "objects/heroFrosty/tris.MD2" );
    if ( mHero )
    {
        oHero = smgr->addAnimatedMeshSceneNode ( mHero );
    }

    if ( oHero )
    {
        oHero->setID ( OBJECT_ID_HERO );
        oHero->setPosition ( vector3df(300,300,-100) );
        oHero->setScale ( core::vector3df ( 2.f, 2.f, 2.f ) );
        /* potrzebne do animacji innych niz .md2, normalnie wystarcza EMAT_
        oHero->setFrameLoop(0, 13);
        oHero->setAnimationSpeed(15);
         */
        oHero->setMD2Animation ( scene::EMAT_JUMP );
        oHero->setMaterialTexture ( 0, driver->getTexture ( "objects/heroFrosty/frosty.PCX" ) );
        oHero->setMaterialFlag(EMF_LIGHTING, false);
    }



    mEnemy = smgr->getMesh ( "objects/alphaWerewolf/tris.MD2" );
    if ( mEnemy )
    {
        oEnemy = smgr->addAnimatedMeshSceneNode ( mEnemy );
    }

    smgr->loadScene ( "objects/terrain/terrain.irr" );

    skydome = smgr->addSkyDomeSceneNode(driver->getTexture("textures/skyDomes/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    if ( oEnemy )
    {
        oEnemy->setID ( IDFlag_IsPickable );
        oEnemy->setPosition ( vector3df(400,300,-80) );
        oEnemy->setScale ( core::vector3df ( 2.f, 2.f, 2.f ) );
        oEnemy->setMaterialFlag ( EMF_LIGHTING, false );
        oEnemy->setMD2Animation ( scene::EMAT_CROUCH_DEATH );
        oEnemy->setAnimationSpeed(1);
        oEnemy->setMaterialTexture ( 0, driver->getTexture ( "objects/alphaWerewolf/dark.PCX" ) );
        oEnemy->setLoopMode ( true );
        ITriangleSelector *selectorShooting;
        selectorShooting = smgr->createTriangleSelector ( oEnemy );
        oEnemy->setTriangleSelector ( selectorShooting );
        selectorShooting->drop();
    }

    driver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );

    camera = smgr->addCameraSceneNode ( 0, vector3df ( -70, 50, -220 ), vector3df ( 180, 100, 100 ), true );
    camera->setTarget ( oEnemy->getPosition() );
    camera->setPosition ( vector3df(500,300,-400) );

    collisionsNGravity ( smgr, camera, oHero );

    IGUIFont* fontSimple=gui->getFont ( "textures/fonts/fontSimple.xml" );
    IGUIStaticText *tt;
    tt=gui->addStaticText ( L"ESC=>KONTYNUUJ", rect<s32 > ( position2di ( 20, sSet->resolution.Height-64-20), dimension2di ( 160, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSimple);

    tt=gui->addStaticText ( L"WESOLYCH SWIAT I UDANEGO ROKU 2011", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-150, (s32)(sSet->resolution.Height/2)-50), dimension2di ( 460, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,250,250,150));
    tt->setOverrideFont(fontSimple);
    tt=gui->addStaticText ( L"ZYCZY PRODUCENT GRY", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-90, (s32)(sSet->resolution.Height/2)-20), dimension2di ( 460, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,250,250,100));
    tt->setOverrideFont(fontSimple);
    tt=gui->addStaticText ( L"DAMIAN & SYN & TESCIOWA    SP Z O.O.", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-150, (s32)(sSet->resolution.Height/2)+10), dimension2di ( 460, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,250,250,50));
    tt->setOverrideFont(fontSimple);

    IGUIImage* i=sGui.iGuiLogoIrrlicht = gui->addImage ( driver->getTexture ( "textures/gui/irrlichtlogo2.png" ), core::position2d<s32 > ( (s32)(sSet->resolution.Width/2) - 64, 10 ), true, 0, -1, L"iGuiLogoIrrlicht" );
    i->setColor(SColor(100,0,0,200));
}

void Object::collisionsNGravity ( ISceneManager* smgr, ICameraSceneNode* camera, IAnimatedMeshSceneNode* oHero )
{
    scene::IMetaTriangleSelector* meta;
    meta = smgr->createMetaTriangleSelector();

    //znajdujemy wszystkie obiekty i tworzymy selektory dla nich (rozrozniamy typy obietow)
    core::array<scene::ISceneNode *> nodes;
    smgr->getSceneNodesFromType ( scene::ESNT_ANY, nodes );

    //w petli przeszukujemy wszystkie node'y i tworzymy dla nich selektory

    for ( u32 i = 0; i < nodes.size(); ++i )
    {
        scene::ISceneNode * node = nodes[i];
        scene::ITriangleSelector * selector = 0;

        switch ( node->getType() )
        {
        case scene::ESNT_CUBE:
            break;
        case scene::ESNT_ANIMATED_MESH:
            // Because the selector won't animate with the mesh,
            // and is only being used for camera collision, we'll just use an approximate
            // bounding box instead of ((scene::IAnimatedMeshSceneNode*)node)->getMesh(0)
//          if(node->getID()!=OBJECT_ID_HERO)selector =smgr->createTriangleSelectorFromBoundingBox ( node );
            break;
        case scene::ESNT_MESH:
        case scene::ESNT_SPHERE: // dostarczone z IMeshSceneNode
            selector = smgr->createTriangleSelector ( ( ( scene::IMeshSceneNode* ) node )->getMesh(), node );
            break;
        case scene::ESNT_TERRAIN:
            selector = smgr->createTerrainTriangleSelector ( ( scene::ITerrainSceneNode* ) node );
            break;
        case scene::ESNT_OCTREE:
            selector = smgr->createOctreeTriangleSelector ( ( ( scene::IMeshSceneNode* ) node )->getMesh(), node );
            break;
        default:
            break;
        }

        if ( selector )
        {
            // dodajemy kazdy tworzony selektor do globalnego meta
            meta->addTriangleSelector ( selector );
            selector->drop();
        }
    }

    //tworzymy kolizje wybranego obiektu z reszte sceny (meta)
    const core::aabbox3d<f32>& box = oHero->getBoundingBox();

    core::vector3df radius = box.MaxEdge - box.getCenter();

    if ( meta )
    {
        scene::ISceneNodeAnimator* animHero =
            smgr->createCollisionResponseAnimator (
                meta, oHero, vector3df ( 5, 12, 5 ),
                vector3df ( 0, -5, 0 ), vector3df ( 0, 10, 0 ) );

        meta->drop();
        oHero->addAnimator ( animHero );
        animHero->drop();
    }

    scene::ISceneNodeAnimator* animEnemy = smgr->createCollisionResponseAnimator(
            meta, oEnemy, core::vector3df(5, 15, 5),
            core::vector3df(0, -5, 0));
    oEnemy->addAnimator(animEnemy);
    animEnemy->drop();

    //kolizja kamery z otoczeniem
//      scene::ISceneNodeAnimator* animCam = smgr->createCollisionResponseAnimator(
//              meta, camera, core::vector3df(1, 1, 1),
//              core::vector3df(0, 0, 0));
//      camera->addAnimator(animCam);
//      animCam->drop();

}

void Object::loadMenuTerrain ( ISceneManager* smgr, IVideoDriver* driver, Settings* sSet )
{

    smgr->clear();
    smgr->getMeshCache()->clear();
//      driver->removeAllHardwareBuffers();

    IAnimatedMesh* mesh;

    if(sSet->mEffects)
    {
        mesh = smgr->getMesh ( "objects/terrainMenu/room.3ds" );
        smgr->getMeshManipulator()->makePlanarTextureMapping ( mesh->getMesh ( 0 ), 0.004f );
        ISceneNode* node = 0;
        node = smgr->addAnimatedMeshSceneNode ( mesh );
        node->setMaterialTexture ( 0, driver->getTexture ( "objects/terrainMenu/wall.jpg" ) );
        node->getMaterial ( 0 ).SpecularColor.set ( 0, 0, 0, 0 );

        mesh = smgr->addHillPlaneMesh ( "myHill",
                                        core::dimension2d<f32> ( 20, 20 ),
                                        core::dimension2d<u32> ( 40, 40 ), 0, 0,
                                        core::dimension2d<f32> ( 0, 0 ),
                                        core::dimension2d<f32> ( 10, 10 ) );

        node = smgr->addWaterSurfaceSceneNode ( mesh->getMesh ( 0 ), 3.0f, 300.0f, 30.0f );
        node->setPosition ( core::vector3df ( 0, 7, 0 ) );

        node->setMaterialTexture ( 0, driver->getTexture ( "objects/terrainMenu/stones.jpg" ) );
        node->setMaterialTexture ( 1, driver->getTexture ( "objects/terrainMenu/water.jpg" ) );

        node->setMaterialType ( video::EMT_REFLECTION_2_LAYER );

        // tworzymy swiatlo

        node = smgr->addLightSceneNode ( 0, core::vector3df ( 0, 0, 0 ),
                                         SColorf ( 1.0f, 0.6f, 0.7f, 1.0f ), 800.0f );

        ISceneNodeAnimator* anim = 0;
        anim = smgr->createFlyCircleAnimator ( core::vector3df ( 0, 150, 0 ), 250.0f );
        node->addAnimator ( anim );
        anim->drop();

        node = smgr->addBillboardSceneNode ( node, dimension2d<f32> ( 50, 50 ) );
        node->setMaterialFlag ( EMF_LIGHTING, false );
        node->setMaterialType ( EMT_TRANSPARENT_ADD_COLOR );
        node->setMaterialTexture ( 0, driver->getTexture ( "objects/terrainMenu/particlewhite.bmp" ) );


        // dodajemy efekt czasteczkowy (ogien)
        scene::IParticleSystemSceneNode* ps =
            smgr->addParticleSystemSceneNode ( false );

        scene::IParticleEmitter* em = ps->createBoxEmitter (
                                          core::aabbox3d<f32> ( -7, 0, -7, 7, 1, 7 ), // emitter size
                                          core::vector3df ( 0.0f, 0.06f, 0.0f ),   // initial direction
                                          80, 100,                            // emit rate
                                          video::SColor ( 0, 255, 255, 255 ), // darkest color
                                          video::SColor ( 0, 255, 255, 255 ), // brightest color
                                          800, 2000, 0,                       // min and max age, angle
                                          core::dimension2df ( 10.f, 10.f ),     // min size
                                          core::dimension2df ( 20.f, 20.f ) );   // max size

        ps->setEmitter ( em );
        em->drop();

        scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

        ps->addAffector ( paf );
        paf->drop();

        ps->setPosition ( core::vector3df ( -70, 60, 40 ) );
        ps->setScale ( core::vector3df ( 2, 2, 2 ) );
        ps->setMaterialFlag ( video::EMF_LIGHTING, false );
        ps->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
        ps->setMaterialTexture ( 0, driver->getTexture ( "objects/terrainMenu/fire.bmp" ) );
        ps->setMaterialType ( video::EMT_TRANSPARENT_VERTEX_ALPHA );

//dodajemy inne swiatlo
        scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode ( 0, -1,
                                           32,                              // Subdivisions on U axis
                                           32,                              // Subdivisions on V axis
                                           video::SColor ( 0, 255, 255, 255 ), // foot color
                                           video::SColor ( 0, 0, 0, 0 ) );  // tail color

        if ( n )
        {
            n->setScale ( core::vector3df ( 56.0f, 56.0f, 56.0f ) );
            n->setPosition ( core::vector3df ( -120, 50, 40 ) );

            array<video::ITexture*> textures;

            for ( s32 g = 7; g > 0; --g )
            {
                core::stringc tmp;
                tmp = "objects/terrainMenu/portal";
                tmp += g;
                tmp += ".bmp";
                video::ITexture* t = driver->getTexture ( tmp.c_str() );
                textures.push_back ( t );
            }

            scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator ( textures, 150 );
            n->addAnimator ( glow );
            glow->drop();
        }
    }

    mesh = smgr->getMesh ( "objects/terrainMenu/tris.MD2" );
    anode = 0;
    anode = smgr->addAnimatedMeshSceneNode ( mesh );
    anode->setPosition ( core::vector3df ( -150, 20, -60 ) );
    anode->setAnimationSpeed ( 15 );
    anode->setMaterialFlag ( EMF_LIGHTING, false );
    anode->setMaterialTexture ( 0, driver->getTexture ( "objects/terrainMenu/rage.PCX" ) );
    anode->addShadowVolumeSceneNode();
    smgr->setShadowColor ( video::SColor ( 150, 0, 0, 0 ) );
    anode->setScale ( core::vector3df ( 3, 3, 3 ) );
    anode->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );

    ICameraSceneNode* cam = smgr->addCameraSceneNode ( 0, vector3df ( -70, 50, -220 ), vector3df ( 180, 100, 100 ), true );
    cam->setTarget ( vector3df ( 0, 0, 0 ) );
    ISceneNodeAnimator* a = smgr->createFlyCircleAnimator ( core::vector3df ( 50, 100, 0 ), -300.0f, 0.0001f, vector3df ( 0.f, 1.f, 0.f ) );
    if ( a )
    {
        cam->addAnimator ( a );
        a->drop();
    }

    smgr->addBillboardTextSceneNode ( 0,  L"_____________________", 0, dimension2d<f32> ( 200, 50 ), vector3df ( 0, 100, 150 ), 0, SColor ( 0, 150, 0, 0 ), SColor ( 0, 150, 0, 0 ) );
    smgr->addBillboardTextSceneNode ( 0,  L"|PROJEKT INZYNIERSKI|", 0, dimension2d<f32> ( 200, 50 ), vector3df ( 0, 100, 150 ), 0, SColor ( 0, 150, 0, 0 ), SColor ( 0, 150, 0, 0 ) );
    smgr->addBillboardTextSceneNode ( 0,  L"_____________________", 0, dimension2d<f32> ( 200, 50 ), vector3df ( 0, 130, 150 ), 0, SColor ( 0, 150, 0, 0 ), SColor ( 0, 150, 0, 0 ) );

}

void Object::loadGameGUI ( IGUIEnvironment* gui, IVideoDriver* driver, IrrlichtDevice* device, ETypeOfGameMenu t, Settings* sSet )
{
    gui->clear();
    eTypeOfGameMenu = t;

    IGUIFont* fontSmall=gui->getFont ( "textures/fonts/fontSmall.xml" );
    IGUIFont* fontBig=gui->getFont ( "textures/fonts/fontBig.xml" );
    IGUIFont* fontSimple=gui->getFont ( "textures/fonts/fontSimple.xml" );

    load2D(device, driver);

    for ( u32 i = 0; i < EGDC_COUNT; ++i )
    {
        SColor col = gui->getSkin()->getColor ( ( EGUI_DEFAULT_COLOR ) i );
        col.setAlpha ( 250 );
        gui->getSkin()->setColor ( ( EGUI_DEFAULT_COLOR ) i, col );
    }

    switch ( t )
    {
    case GUI_GAME:
        device->getCursorControl()->setVisible ( false );
        break;
    case GUI_GAME_PAUSE:
        gui->addImage ( driver->getTexture ( "textures/gui/pause.png" ), core::position2d<s32 > ( (s32)(sSet->resolution.Width/2-128), (s32)(sSet->resolution.Height/2)-128 ), true, 0, -1, L"iGuiFrame" );
        IGUIButton* b;

        b = gui->addButton ( rect<s32 > ( position2di (
                                              (s32)(sSet->resolution.Width/2) - 50,
                                              (s32)(sSet->resolution.Height/2) - 50 ),
                                          dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ),
                             0, GUI_ID_BB_RETURN_TO_GAME, L"Wroc", L"Powrot do gry" );

        b = gui->addButton ( rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2) - 50, (s32)(sSet->resolution.Height/2) ), dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ), 0, GUI_ID_BB_RETURN_TO_MAIN, L"Menu glowne", L"Powrot do menu glownego" );
        b = gui->addButton ( rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2) - 50, (s32)(sSet->resolution.Height/2) + 50 ), dimension2di ( gameSettings.buttonX, gameSettings.buttonY ) ), 0, GUI_ID_BB_RETURN_TO_SYSTEM, L"Wyjdz z gry", L"Powrot do systemu operacyjnego" );
        device->getCursorControl()->setVisible ( true );
        break;
    default:
        break;
    }

    gui->addImage ( driver->getTexture ( "textures/game/theFonz.png" ), core::position2d<s32 > ( 0, 0 ), true, 0, -1, L"iGuiFrame" );
    planetIcon=gui->addImage ( driver->getTexture ( "textures/game/moon.png" ), core::position2d<s32 > ( (s32)(sSet->resolution.Width/2)-32, 0 ), true, 0, -1, L"iGuiFrame" );

    IGUIStaticText *tt;
    tt=gui->addStaticText ( L"ESC => MENU", rect<s32 >
                            ( position2di ( sSet->resolution.Width-200, 0 ),
                              dimension2di ( 160, 35 ) ), false, false, 0 );

    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSmall);


    tt=gui->addStaticText ( L"M => MISJE", rect<s32 > ( position2di ( sSet->resolution.Width-200, 30 ), dimension2di ( 160, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSmall);

    tt=gui->addStaticText ( L"N => STEROWANIE ", rect<s32 > ( position2di ( 64+5+16, 10 ), dimension2di ( 200, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSimple);

    tt=gui->addStaticText ( L"T => TEST WYDAJNOSCI ", rect<s32 > ( position2di ( 64+5+16, 25 ), dimension2di ( 200, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSimple);

    tt=gui->addStaticText ( L"F9 => ZRZUT EKRANU ", rect<s32 > ( position2di ( 64+5+16, 40 ), dimension2di ( 200, 35 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,200));
    tt->setOverrideFont(fontSimple);

    u32 alpha;
    if(sSet->effects)alpha=250;
    else alpha=100;
    SColor white=SColor(alpha,200,200,200);
    SColor yellow=SColor(alpha,200,200,0);
    SColor nowColor=SColor(250,200,200,200);

    gui->addImage ( driver->getTexture ( "textures/game/pistol.png" ), core::position2d<s32 > ( sSet->resolution.Width-16-32-5, 600-300 ), true, 0, -1, L"iGuiFrame" );

    gui->addImage ( driver->getTexture ( "textures/game/crossHealth.png" ), core::position2d<s32 > ( 16+6, sSet->resolution.Height-32-8 ), true, 0, -1, L"iGuiFrame" );
    sGui.tLife=gui->addStaticText ( L"100", rect<s32 > ( position2di ( 64, sSet->resolution.Height-40 ), dimension2di ( 120, 40 ) ), false, false, 0 );
    sGui.tLife->setOverrideColor(SColor(250,200,200,0));
    sGui.tLife->setOverrideFont(fontBig);

    gui->addImage ( driver->getTexture ( "textures/game/armor.png" ), core::position2d<s32 > ( 16+6+120, sSet->resolution.Height-32-8 ), true, 0, -1, L"iGuiFrame" );
    sGui.tArmor=gui->addStaticText ( L"100", rect<s32 > ( position2di ( 64+120, sSet->resolution.Height-40 ), dimension2di ( 120, 40 ) ), false, false, 0 );
    sGui.tArmor->setOverrideColor(SColor(250,200,200,0));
    sGui.tArmor->setOverrideFont(fontBig);

    sGui.tWatch=gui->addStaticText ( L"0:00", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-40, sSet->resolution.Height-40 ), dimension2di ( 80, 25 ) ), false, false, 0 );
    sGui.tWatch->setOverrideColor(SColor(250,100,0,0));
    sGui.tWatch->setOverrideFont(fontBig);
    sGui.tWatch->setDrawBorder(true);

    tt=gui->addStaticText ( L"CAMERA (c)", rect<s32 > ( position2di ( sSet->resolution.Width-150, sSet->resolution.Height-40 ), dimension2di ( 100, 15 ) ), false, false, 0 );
    tt->setOverrideColor(SColor(250,200,200,0));
    tt->setOverrideFont(fontSimple);
    tt->setDrawBorder(true);

    SColor active=SColor(250,200,200,0);
    SColor unActive=SColor(80,200,200,0);

    if(sSet->typeOfCamera==E_FPP)nowColor=active;
    else nowColor=unActive;
    sGui.tCameraFPP=gui->addStaticText ( L"FPP", rect<s32 > ( position2di ( sSet->resolution.Width-150, sSet->resolution.Height-25 ), dimension2di ( 50, 15 ) ), false, false, 0 );
    sGui.tCameraFPP->setOverrideColor(nowColor);
    sGui.tCameraFPP->setOverrideFont(fontSimple);
    sGui.tCameraFPP->setDrawBorder(true);

    if(sSet->typeOfCamera==E_TPP)nowColor=active;
    else nowColor=unActive;
    sGui.tCameraTPP=gui->addStaticText ( L"TPP", rect<s32 > ( position2di ( sSet->resolution.Width-100, sSet->resolution.Height-25 ), dimension2di ( 50, 15 ) ), false, false, 0 );
    sGui.tCameraTPP->setOverrideColor(nowColor);
    sGui.tCameraTPP->setOverrideFont(fontSimple);
    sGui.tCameraTPP->setDrawBorder(true);

    sGui.tFps=gui->addStaticText ( L"", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-50, 20 ), dimension2di ( 150, 40 ) ), false, false, 0 );
    sGui.tFps->setOverrideFont(fontBig);
}

void Object::changeEffectsAvailability(bool isEnabled)
{
    sGui.checkMirror->setEnabled(isEnabled);
    sGui.checkSnow->setEnabled(isEnabled);
    sGui.checkFog->setEnabled(isEnabled);
    sGui.checkFire->setEnabled(isEnabled);
    sGui.checkShaderToon->setEnabled(isEnabled);
    sGui.checkHeroLight->setEnabled(isEnabled);
    sGui.checkHeroTransparency->setEnabled(isEnabled);
    sGui.checkHeroSphericMap->setEnabled(isEnabled);
    sGui.checkHeroShadow->setEnabled(isEnabled);
    sGui.checkLightAmbient->setEnabled(isEnabled);
    sGui.checkLightPoint->setEnabled(isEnabled);
    sGui.checkLightMap->setEnabled(isEnabled);
    sGui.checkForest->setEnabled(isEnabled);
    sGui.checkWerewolves->setEnabled(isEnabled);
    sGui.checkTerrainQuality->setEnabled(isEnabled);
    sGui.checkSky->setEnabled(isEnabled);
    sGui.checkCompass->setEnabled(isEnabled);
}

void Object::changeSoundsAvailability(bool isEnabled)
{
    sGui.checkSTracks->setEnabled(isEnabled);
    sGui.checkSEffects->setEnabled(isEnabled);
    sGui.checkS3D->setEnabled(isEnabled);
}

void Object::setChecked(Settings* sSet)
{
    sGui.checkMEffects->setChecked(sSet->mEffects);

    sGui.checkEffects->setChecked(sSet->effects);
    sGui.checkSounds->setChecked(sSet->sounds);

    sGui.checkMirror->setChecked(sSet->mirror);
    sGui.checkSnow->setChecked(sSet->snow);
    sGui.checkFog->setChecked(sSet->fog);
    sGui.checkFire->setChecked(sSet->fire);
    sGui.checkShaderToon->setChecked(sSet->shaderToon);
    sGui.checkHeroLight->setChecked(sSet->heroLight);
    sGui.checkHeroTransparency->setChecked(sSet->heroTransparency);
    sGui.checkHeroSphericMap->setChecked(sSet->heroSphericMap);
    sGui.checkHeroShadow->setChecked(sSet->heroShadow);
    sGui.checkLightAmbient->setChecked(sSet->lightAmbient);
    sGui.checkLightPoint->setChecked(sSet->lightPoint);
    sGui.checkLightMap->setChecked(sSet->lightMap);
    sGui.checkForest->setChecked(sSet->forest);
    sGui.checkWerewolves->setChecked(sSet->werewolfes);
    sGui.checkTerrainQuality->setChecked(sSet->terrainQuality);
    sGui.checkSky->setChecked(sSet->sky);
    sGui.checkCompass->setChecked(sSet->compass);

    sGui.checkSTracks->setChecked(sSet->soundTracks);
    sGui.checkSEffects->setChecked(sSet->soundEffects);
    sGui.checkS3D->setChecked(sSet->sound3D);

    sGui.checkMirror->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkSnow->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkFog->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkFire->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkShaderToon->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkHeroLight->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkHeroTransparency->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkHeroSphericMap->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkHeroShadow->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkLightAmbient->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkLightPoint->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkLightMap->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkForest->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkWerewolves->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkTerrainQuality->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkSky->setEnabled(sGui.checkEffects->isChecked());
    sGui.checkCompass->setEnabled(sGui.checkEffects->isChecked());


    sGui.checkSTracks->setEnabled(sGui.checkSounds->isChecked());
    sGui.checkSEffects->setEnabled(sGui.checkSounds->isChecked());
    sGui.checkS3D->setEnabled(sGui.checkSounds->isChecked());
}



void Object::addMessage(const wchar_t *text, s32 icon)
{
    sGui.listboxCount++;
    if(sGui.listboxCount>2)sGui.listbox->removeItem(1);
    sGui.listbox->addItem(text, icon);
}

void Object::load2D(IrrlichtDevice* device, IVideoDriver* driver)
{
    dimension2D.images = driver->getTexture("textures/2ddemo.png");
    driver->makeColorKeyTexture(dimension2D.images, position2d<s32>(0,0));
    dimension2D.font = device->getGUIEnvironment()->getBuiltInFont();
    dimension2D.font2 = device->getGUIEnvironment()->getFont("textures/fonts/fonthaettenschweiler.bmp");
    dimension2D.imp1=rect<s32>(349,15,385,78);
    dimension2D.imp2=rect<s32>(387,15,423,78);
    driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
    driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;
}

void Object::draw2D(IrrlichtDevice* device, IVideoDriver* driver, u32 time, Settings* sSet)
{
// rysuj ogien i smoki w tle

    driver->draw2DImage(dimension2D.images,
                        position2d<s32>(sSet->resolution.Width-342,50),
                        rect<s32>(0,0,342,224), 0,
                        SColor(255,255,255,255), true);
//x:450,564,590
//y:50,145,55
    // draw flying imp
    driver->draw2DImage(dimension2D.images, position2d<s32>(sSet->resolution.Width-228,145),
                        (time/500 % 2) ? dimension2D.imp1 : dimension2D.imp2, 0,
                        video::SColor(255,255,255,255), true);

    // draw second flying imp with colorcylce
    driver->draw2DImage(dimension2D.images, position2d<s32>(sSet->resolution.Width-202,55),
                        (time/500 % 2) ? dimension2D.imp1 : dimension2D.imp2, 0,
                        video::SColor(255,(time) % 255,255,255), true);

    if (dimension2D.font)
        dimension2D.font->draw(L"2D graphics",
                               core::rect<s32>(sSet->resolution.Width-70,50,sSet->resolution.Width,90),
                               video::SColor(255,255,255,255));

    // draw some other text
    if (dimension2D.font2)
        dimension2D.font2->draw(L"WILCOUAQ",
                                core::rect<s32>(sSet->resolution.Width-160,70,sSet->resolution.Width-90,90),
                                video::SColor(255,time % 255,time % 255,255));

    /*
    Next, we draw the Irrlicht Engine logo (without
    using a color or an alpha channel). Since we slightly scale
    the image we use the prepared filter mode.
    */
    driver->enableMaterial2D();
    driver->draw2DImage(dimension2D.images, core::rect<s32>(10,10,108,48),
                        core::rect<s32>(354,87,442,118));
    driver->enableMaterial2D(false);

    /*
    Finally draw a half-transparent rect under the mouse cursor.
    */
    core::position2d<s32> m = device->getCursorControl()->getPosition();
    driver->draw2DRectangle(video::SColor(100,255,255,255),
                            core::rect<s32>(m.X-4, m.Y-4, m.X+4, m.Y+4));
}

ETypeOfObject Object::shoot ( ISceneNode* selectedSceneNode, IAnimatedMeshSceneNode *oHero, IAnimatedMeshSceneNode *oEnemy, ISoundEngine *engineSnd )
{
    ETypeOfObject type=OBJECT_ID_NONE;
    if(selectedSceneNode)
    {
        if(selectedSceneNode->getID()==OBJECT_ID_ENEMY)type=OBJECT_ID_ENEMY;
    }
    return type;
}
//

void Object::updateFpsGui ( IrrlichtDevice* device, IVideoDriver* driver, IGUIEnvironment* gui)
{
    s32 lastFPS = 60;
    s32 fps = driver->getFPS();
    if ( lastFPS != fps )
    {
        lastFPS = fps;
        stringw str;
        str += "FPS:";
        str += fps;
        device->setWindowCaption ( str.c_str() );
        sGui.tFps->setText(str.c_str());
    }
}

void Object::updateGuiHeroStats(f32 life, f32 armor)
{
    stringw sLife;
    sLife+=(u32)(life*100);
    sGui.tLife->setText(sLife.c_str());

    stringw sArmor;
    sArmor+=(u32)(armor*100);
    sGui.tArmor->setText(sArmor.c_str());
}

ITerrainSceneNode* Object::loadHeightMap(IVideoDriver* driver, ISceneManager* smgr, vector3df pos, stringw path, Settings* sSet)
{
    ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
                                     path+"terrain-heightmap.bmp",
                                     0,					// parent node
                                     -1,					// node id
                                     pos,		// position
                                     vector3df(0.f, 0.f, 0.f),		// rotation
                                     vector3df(500.f, 1.f, 500.f),	// scale
                                     SColor ( 255, 255, 255, 255 ),	// vertexColor
                                     5,					// maxLOD
                                     ETPS_17,				// patchSize
                                     4					// smoothFactor
                                 );

    terrain->setMaterialFlag(video::EMF_LIGHTING, false);

    terrain->setMaterialTexture(0,
                                driver->getTexture(path+"terrain-texture.jpg"));
    terrain->setMaterialTexture(1,
                                driver->getTexture(path+"detailmap3.jpg"));
    if(sSet->terrainQuality)terrain->setMaterialType(video::EMT_DETAIL_MAP);
    else terrain->setMaterialType(video::EMT_SOLID);
    terrain->scaleTexture(100.0f, 100.0f);
    return terrain;
}

void Object::loadTrees(IVideoDriver* driver, ISceneManager* smgr)
{
    IBillboardSceneNode* n;
    srand(time(NULL));

    for(int i=0; i<1000; i++)
    {
        f32 randX=rand()%6000-3000;
        f32 randY=rand()%200+200;
        f32 randXW=rand()%200+150;
        f32 randZ=rand()%6000-3000;

        n = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(randXW, randY), vector3df(randX,-600+randY/2,randZ));
        n->setMaterialFlag(video::EMF_LIGHTING, false);
        n->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        n->setMaterialTexture(0, driver->getTexture("textures/terrain/tree2.png"));
        n->setColor(SColor(250,250,0,0));
        n->setID(OBJECT_ID_TREE);
    }

    for(int i=0; i<1000; i++)
    {
        f32 randX=rand()%6000-3000;
        f32 randY=rand()%200+200;
        f32 randXW=rand()%200+150;
        f32 randZ=rand()%6000-3000;

        n = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(randXW, randY), vector3df(randX,-600+randY/2,randZ));
        n->setMaterialFlag(video::EMF_LIGHTING, false);
        n->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        n->setMaterialTexture(0, driver->getTexture("textures/terrain/tree3.png"));
        n->setID(OBJECT_ID_TREE);
    }
}

void Object::loadWerewolves(IVideoDriver* driver, ISceneManager* smgr, IAnimatedMeshSceneNode* werewolves[10][10], vector3df pos)
{
    mEnemy = smgr->getMesh ( "objects/alphaWerewolf/tris.MD2" );

    if ( mEnemy )
    {
        for(int x=0; x<10; x++)
        {
            for(int y=0; y<10; y++)
            {
                werewolves[x][y] = smgr->addAnimatedMeshSceneNode ( mEnemy );
                werewolves[x][y]->setPosition ( pos+vector3df ( x*200, 0, y*200 ) );
                werewolves[x][y]->setScale ( vector3df ( 2.f, 2.f, 2.f ) );;
                werewolves[x][y]->setMD2Animation ( EMAT_RUN );
                werewolves[x][y]->setMaterialTexture ( 0, driver->getTexture ( "objects/alphaWerewolf/dark.PCX" ) );
                werewolves[x][y]->setLoopMode ( true );
                werewolves[x][y]->setMaterialFlag(video::EMF_LIGHTING, false);
                werewolves[x][y]->setVisible(true);
            }
        }
    }

}

void Object::moveWerewolves(IAnimatedMeshSceneNode* werewolves[10][10], vector3df pos)
{
    for(int x=0; x<10; x++)
    {
        for(int y=0; y<10; y++)
        {
            werewolves[x][y]->setPosition (werewolves[x][y]->getPosition()+pos);
        }
    }
}
