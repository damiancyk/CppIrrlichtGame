
#ifndef __ACTION_H_INCLUDED__
#define __ACTION_H_INCLUDED__

#include "device.h"
#include "sound.h"
#include "hero.h"
#include "enemy.h"
#include "effect.h"

class Action
{
private:
    Effect* e;
    /**typ wyliczeniowy watkow splash screen*/
    enum EventSplashScreen
    {
        E_SPLASH_START,
        E_SPLASH_TO_INTRO
    };
    EventSplashScreen eventSplashScreen;

    /**typ wyliczeniowy watkow intro*/
    enum EventIntro
    {
        E_INTRO_START,
        E_INTRO_GO_HERO,
        E_INTRO_ATTACK,
        E_INTRO_ATTACKING,
        E_INTRO_TO_MENU
    };
    EventIntro eventIntro;

    /**typ wyliczeniowy watkow sceny1*/
    enum EventScene1
    {
        E_SCENE1_START,
        E_SCENE1_GO_HERO,
        E_SCENE1_PURSUIT,
        E_SCENE1_ATTACK,
        E_SCENE1_VICTORY,
        E_SCENE1_LOOSE,
        E_SCENE1_TO_SCENE2,
        E_SCENE1_TO_MENU
    };
    EventScene1 eventScene1;

    /**typ wyliczeniowy watkow sceny2*/
    enum EventScene2
    {
        E_SCENE2_START,
        E_SCENE2_TESTING

    };
    EventScene2 eventScene2;

    /**typ wyliczeniowy watkow splash screen*/
    enum EventOutro
    {
        E_OUTRO_START,
        E_OUTRO_QUIT
    };
    EventOutro eventOutro;

    /**czas ropoczecia akcji*/
    u32 timeStart;

    /**aktualny czas*/
    u32 timeNow;

    /**wskaznik na dzwiek*/
    Sound* s;

    /**poruszaj obiekt*/
    bool moveObject(IAnimatedMeshSceneNode* oHero, vector3df target, f32 rate, f32 isNear);

    /**metoda zapozyczona z silnika open source (irrAI) obracajaca obiekt w dana strone*/
    vector3df rotateToFace(const core::vector3df& objectPosition, const core::vector3df& targetPosition);

public:

    /**wskaznik na obiekt klasy reprezentujacej bohatera*/
    Hero* h;

    /**wskaznik na obiekt klasy reprezentujacej przeciwnika*/
    Enemy* y;

    /**konstruktor klasy akcja*/
    Action(Sound* s);

    /**destruktor klasy akcja*/
    ~Action();

    /**wykonywanie splash screen*/
    ELevels actSplashScreen(Settings* sSet);

    /**wykoanywanie akcji intra*/
    ELevels actIntro(IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oEnemy, Settings* sSet, ICameraSceneNode* camera, f32 rate);

    /**wykonywanie akcji sceny 1*/
    ELevels actScene1(IrrlichtDevice* device, IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oEnemy, Settings* sSet, IGUIEnvironment* gui, ICameraSceneNode* camera, f32 rate, IGUIImage* planetIcon);

    /**wykonywanie akcji sceny 2*/
    ELevels actScene2(IrrlichtDevice* device, IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oEnemy, Settings* sSet, IGUIEnvironment* gui, ICameraSceneNode* camera, f32 rate, IGUIImage* planetIcon);

    /**wykoanywanie akcji outra*/
    ELevels actOutro(Settings* sSet);

    /**ustaw parametry akcji*/
    void initializeData(ELevels level);

    /**pokaz aktualny czas akcji*/
    void showTime(IrrlichtDevice* device, IGUIStaticText* tWatch);

    /**ustaw czas startu akcji*/
    void setStartTime(IrrlichtDevice* device);

};

#endif
