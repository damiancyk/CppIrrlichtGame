
#ifndef __DEVICE_H_INCLUDED__
#define __DEVICE_H_INCLUDED__

/**wlaczenie do programu bibliotek Irrlicht*/
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif
#include <irrlicht.h>

/**przestrzen nazw Irrlicht*/
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**wlaczenie do programu bibliotek Irrklang*/
#include <irrKlang.h>
#if defined(WIN32)
#include <conio.h>
#else
#include "../common/conio.h"
#endif

/**przestrzen nazw irrKlang*/
using namespace irrklang;

/**
 * eLevels
 * typ wyliczeniowy poziomow dostepnych w grze
 */
enum ELevels
{
    E_SPLASH_SCREEN,
    E_INTRO, /**intro*/
    E_MENU, /**menu*/
    E_SCENE1, /**scena pierwsza*/
    E_SCENE2,/**scena druga*/
    E_OUTRO,
    E_NONE
};

enum eTypeOfCamera
{
    E_FPP,
    E_TPP
};

/**typ wyliczeniowy nadajacy unikalne ID kazdemu typowi obiektow w scenie 3D*/
enum ETypeOfObject
{
    OBJECT_ID_HERO,
    OBJECT_ID_GUN,
    OBJECT_ID_ENEMY,
    OBJECT_ID_TERRAIN,
    OBJECT_ID_CAMERA,
    OBJECT_ID_OTHER,
    OBJECT_ID_TREE,
    OBJECT_ID_CROSSHAIR,
    OBJECT_ID_AIM,
    OBJECT_ID_FIRE,
    OBJECT_ID_LIGHT_POINT,
    OBJECT_ID_NONE
};

struct Settings
{
    /**typ sterownika, którego Irrlicht ma obslugiwac*/
    E_DRIVER_TYPE driverType;
    /**rozdzielczosc ekranu stworzonego przez Irrlicht*/
    dimension2d<u32 > resolution;
    /**ilosc bitów rysowanych przez urzadzenie wideo Irrlichta*/
    u32 bits;

    /**czy wlaczony tryb pelnego ekranu*/
    bool fullscreen;

    /**czy wlaczony bufor szablonowy*/
    bool stencilBuffer;

    /**ty kamery (TPP/FPP)*/
    eTypeOfCamera typeOfCamera;

    /**
     *zmienna do okreslenia pionowej synchronizacji
     *jesli ustawiona na true, sterownik bedzie czekal na przerysowanie w pionie, w przeciwnym razie nie
     */
    bool vsync;

    /**czulosc myszki w osi X*/
    f32 mouseSensivityX;

    /**czulosc myszki w osi Y*/
    f32 mouseSensivityY;

    /**wspolczynnik ruchu myszki w osi X podczas trybu kamery FPP*/
    f32 rateSniper;

    /**wspolczynnik ruchu myszki w osi Y podczas trybu kamery FPP*/
    f32 rateSniperY;

    /**ograniczenie poruszania myszki w osi Y*/
    s32 cursorYRestriction;

    /**ogranieczenie poruszania myszki w osi X*/
    s32 cursorXRestriction;

    /**zmienna pomocnicza do obslugi kamery, w tej chwili nieuzywana*/
    s32 posCamAhead;

    /**wspolczynnik biegania*/
    f32 RUN;

    /**informacje o potencjalnych joystickach*/
    array<SJoystickInfo> joystickInfo;

    bool werewolfDead;

    /**wlaczono?*/
    bool effects;
    /**wlaczono?*/
    bool mirror;
    /**wlaczono?*/
    bool snow;
    /**wlaczono?*/
    bool fog;
    /**wlaczono?*/
    bool fire;
    /**wlaczono?*/
    bool shaderToon;
    /**wlaczono?*/
    bool heroLight;
    /**wlaczono?*/
    bool heroTransparency;
    /**wlaczono?*/
    bool heroSphericMap;
    /**wlaczono?*/
    bool heroShadow;
    /**wlaczono?*/
    bool lightAmbient;
    /**wlaczono?*/
    bool lightPoint;
    /**wlaczono?*/
    bool lightMap;
    /**wlaczono?*/
    bool werewolfes;
    /**wlaczono?*/
    bool forest;
    /**wlaczono?*/
    bool terrainQuality;
    /**wlaczono?*/
    bool sky;
    /**wlaczono?*/
    bool compass;


    /**wlaczono?*/
    bool mEffects;

    /**wlaczono?*/
    bool sounds;
    /**wlaczono?*/
    bool soundTracks;
    /**wlaczono?*/
    bool soundEffects;
    /**wlaczono?*/
    bool sound3D;

    /**glosnosc dzwieku*/
    f32 volume;

};

#endif
