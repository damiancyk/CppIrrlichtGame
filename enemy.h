
#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "device.h"

/**
 * Enemy
 * Klasa wroga
 * @note .
 */
class Enemy
{
private:

    /**czy zaczal byc atakowany, potrzebne np do obslugi animacji*/
    bool startAttacked;

    /**czy przestal byc atakowany*/
    bool stopAttacked;

    /**status zycia wroga*/
    IBillboardSceneNode* bHealthStatusEnemy;

    /**tlo statusu zycia wroga*/
    IBillboardSceneNode* bHealthStatusEnemyT;

public:

    /**konstruktor klasy wroga*/
    Enemy();

    /**destruktor klasy wroga*/
    ~Enemy();

    /**ilosc zycia*/
    f32 life;

    /**zaladowanie billboardow wroga*/
    void loadEnemyBillboards(ISceneManager* smgr, IVideoDriver* driver);

    /**aktualizacja billboardow bohatera*/
    void updateEnemyBillboards(IAnimatedMeshSceneNode* oEnemy, IAnimatedMeshSceneNode* oHero, ISceneNode* selectedSceneNode);

    /**funkcja ustawiajaca wartosci startowe dla wroga*/
    void setStartValues();
};

#endif
