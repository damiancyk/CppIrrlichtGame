
#ifndef __HERO_H_INCLUDED__
#define __HERO_H_INCLUDED__

#include "device.h"

/**
 * Hero
 * Klasa reprezentujaca bohatera
 * @note Posiada wiele pol umozliwiajacych obsluge jego ruchu oraz update'owanie tych zmiennych
 */
class Hero
{
private:

    /**skladowa X kierunku bohatera*/
    f32 xDirection;

    /**skladowa Y kierunku bohatera*/
    f32 yDirection;

    /**aktualna animacja typu enum EMAT_****/
    s32 animation;

    /**wysokosc skoku bohatera*/
    f32 yJump;

    /**poczatkowa predkosc skoku bohatera*/
    f32 yJumpSpeed;

    /**nick bohatera wyswietlany nad nim*/
    IBillboardSceneNode* nodeName;

    /**status zycia wyswietlany nad bohaterem*/
    IBillboardSceneNode* bHealthStatus;

    /**tlo statusu zycia wyswietlanego nad bohaterem*/
    IBillboardSceneNode* bHealthStatusT;

    /**billboard informujacy o potencjalnym celu*/
    IBillboardSceneNode* bAim;

public:
    /**ilosc zycia bohatera*/
    f32 life;

    /**ilosc pancerza bohatera*/
    f32 armor;

    /**czy bohater jest atakowany w danej chwili*/
    bool isAttacking;

    /**nick bohatera*/
    stringw heroName;

    /**celownik bohatera*/
    IBillboardSceneNode* bCrosshair;

    /**kat dodatkowego aktualnego obrotu w osi Y*/
    f32 anglePlus;

    /**wektor kierunkowy broni bohatera*/
    vector3df facingGun;

    /**zmienna pomocnicza do obslugi ray tracingu dla obslugi kamer 3 osoby (TPP/FPP)*/
    vector3df posRayPlus;

    /**predkosc poruszania sie bohatera*/
    f32 MOVEMENT_SPEED;

    /**wektor kierunkowy bohatera, uzywany np do poruszania sie bohatera*/
    vector3df facing;

    /**rozmiar celownika*/
    dimension2d<f32> crosshairSize;

    /**rozmiar niekatywnego celownika*/
    dimension2d<f32> crosshairSizeBig;

    bool isJump;

    /**konstruktor klasy Hero*/
    Hero();

    /**destruktor klasy Hero*/
    ~Hero();

    /**update obiektu bohatera na podstawie pol z klasy Hero*/
    void updateHero(IAnimatedMeshSceneNode *oHero, IAnimatedMeshSceneNode *oGun);

    /**update kamery (jest to kamera z trzeciej osoby)*/
    void updateCamera(Settings* sSet, IAnimatedMeshSceneNode* oHero,
                      ICameraSceneNode* camera, position2d<f32> cursorPos,
                      f32 rateMouse, f32 rateSniper, f32 rateSniperY);

    /**skok bohatera*/
    void jump(IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oGun);

    /**ustawienie nicku bohatera*/
    void setName(ISceneManager* smgr, IAnimatedMeshSceneNode* oHero);

    /**zaladowanie billboardow bohatera*/
    void loadHeroBillboards(ISceneManager* smgr, IVideoDriver* driver);

    /**aktualizacja billboardow bohatera*/
    void updateHeroBillboards(IAnimatedMeshSceneNode* oHero, vector3df intersection);

    /**funkcja ustawiajaca wartosci startowe dla bohatera*/
    void setStartValues();

};

#endif
