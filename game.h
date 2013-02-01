
/**
 * @mainpage
 * @author Damian Pieta
 * @date 2010.07.09
 * @version 1.0
 * @par Kontakt:
 * e-mail: @a damiancyk@gmail.com
 * @par TODO:
 * See @ref todo
 */

/**
 * @page todo List of TODO
 *
 * -# Projekt Inzynierski.
 * -# Zastosowanie silnika graficznego Irrlicht w grze komputerowej 3D.
 *
 * @n Projekt poszerzony o IrrKlang(dzwiek) oraz kilka funkcji IrrAi. @n
 * @n Nie jest to typowa gra. To tylko prezentacja implementacji silnika we wlasnym projekcie @n
 */

#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__

#include "device.h"
#include "cIrrEventReceiver.h"
#include "object.h"
#include "hero.h"
#include "effect.h"
#include "sound.h"
#include "enemy.h"
#include "cursor.h"
#include "action.h"

class Game
{
private:

    /**zmienne uzywane do zmiany ustawien silnika podczas jego dzialania*/
    bool changeSettings, stop;

    /**klasa obslugujaca I/O*/
    CIrrEventReceiver receiver;

    /**wskaznik na ustawienia gry, najwazniejsza struktura w tym silniku*/
    Settings* sSet;

    /**wskaznik na glowne urzadzenie Irrlichta*/
    IrrlichtDevice* device;

    /**wskaznik na glowne urzadzenie wideo*/
    IVideoDriver* driver;

    /**menedzer scen Irrlichta*/
    ISceneManager* smgr;

    /**kamera w Irrlichcie*/
    ICameraSceneNode* camera;

    /**srodowisko GUI Irrlichta*/
    IGUIEnvironment* gui;

    /**menedzer kolizji w Irrlichcie*/
    ISceneCollisionManager* collMan;

    /**wskaznik na silnik dzwieku*/
    ISoundEngine* engineSnd;

    /**zdarzenie konca odtwarzanego tracka*/
    ISoundStopEventReceiver* sndReceiver;

    /**selektor*/
    ITriangleSelector* selector;

    /**wskaznik na timer wykorzystywany w grze*/
    ITimer* timer;

    /**aktualny level reprezentowany przez typ wyliczeniowy*/
    ELevels level;

    /**wskaznik na obiekt klasy reprezentujacej wszystkie obiekty w grze*/
    Object* o;

    /**wskaznik na obiekt klasy reprezentujacej efekty specjalne*/
    Effect* e;

    /**wskaznik na obiekt klasy reprezentujacej dzwieki*/
    Sound* s;

    /**wskaznik na klase obslugujaca akcje gry*/
    Action* a;

    /**wskaznik na klase obslugujaca kursor*/
    CCursor *myCursor;

    /**czas rozpoczecia gry*/
    s32 timeStarted;

    /**aktualny czas gry*/
    s32 timeNow;

    /**liczba FPS*/
    s32 lastFPS;

    /**pozycja kursora*/
    position2d<f32> cursorPos;

    /**czy namierzono obiekt (zwraca go przy namierzeniu)*/
    ISceneNode* selectedSceneNode;

    /**promien uzywany do ray tracingu*/
    line3d<f32> ray;

    /**pozycja przeciecia z obiektem namierzonym*/
    vector3df intersection;

    /**zwraca trojkat namierzonego obiektu*/
    triangle3df hitTriangle;

    /**zmienna dostarczajaca informacje, czy aktualnie podlaczono joystick*/
    bool isJoystick;

    /**ustawianie planszy (switch z wykorzystaniem enum'a eLevels*/
    void setStage(ELevels level);

    /**update kursora (utrzymanie go w srodku ekranu podczas gry)*/
    void updateCursor(IrrlichtDevice* device);

    /**metoda obslugujaca technike ray tracingu*/
    void rayTracing(IAnimatedMeshSceneNode* oHero, ICameraSceneNode* camera, ISceneCollisionManager* collMan);

    /**metoda pokazujaca aktualna ilosc klatek na sekunde*/
    void showFps(IrrlichtDevice* device, IVideoDriver* driver);

    /**metoda obslugujaca I/O menu oparta o klase IEventReceiver*/
    void keyFuncMenu(CIrrEventReceiver* receiver, f32 rate,
                     IrrlichtDevice* device, IGUIEnvironment* gui);

    /**metoda obslugujaca I/O intro oparta o klase IEventReceiver*/
    void keyFuncIntro ( CIrrEventReceiver* receiver);

    /**metoda obslugujaca I/O outro oparta o klase IEventReceiver*/
    void keyFuncOutro ( CIrrEventReceiver* receiver);

    /**metoda obslugujaca I/O sceny oparta o klase IEventReceiver*/
    void keyFuncScene(CIrrEventReceiver* receiver, f32 rate);

    /**metoda obslugujaca I/O bohatera oparta o klase IEventReceiver*/
    void keyFuncHero(CIrrEventReceiver* receiver, f32 rate);

    /**metoda przeladowujaca caly silnik*/
    void reload(void);

    /**pokazywanie onfo o grze*/
    void showAboutText(void);

    /**podswietlenie namierzonego obiektu*/
    void specialPreference(void);

    /**sprawdzenie czy podlaczono jakies joysticki*/
    void joysticks(Settings* sSet, IGUIEnvironment* gui);

    /**obsluga potencjalnych joystickow*/
    void joysticksControlling(CIrrEventReceiver* receiver,
                              f32 rate, IAnimatedMeshSceneNode* node);

    /**zaladowanie podstawowych klas silnika Irrlicht i Irrklang*/
    void loadDevices(void );

    /**podstawowe ustawienia gry*/
    bool set(bool ask);

    /**info o zadaniach w grze*/
    void showStartMessage(ELevels level, Settings* sSet);

    /**misje*/
    void showMissionMessage(ELevels level);

public:

    /**
     *konstruktor klasy Game
     *tutaj nastepuje zainicjowanie wszystkich niezbednych urzadzen
     */
    Game(void);

    /**destruktor klasy Game*/
    ~Game(void);

    /**
     *funkcja odpowiadajaca za glowna petle w grze
     *oparta o silnik Irrlicht i jego warunki zatrzymania petli
     *@return void
     */
    void run(void);

};

#endif
