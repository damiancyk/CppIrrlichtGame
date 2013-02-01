
#ifndef __EFFECT_H_INCLUDED__
#define __EFFECT_H_INCLUDED__

#include "device.h"
#include "mirror.h"
#include "compass.h"
#include <time.h>

class Effect
{
private:

    /**testy poszczegolnych efektow*/
    struct Test
    {
        bool test;
        bool mirror;
        bool snow;
        bool fog;
        bool fire;
        bool shaderToon;

        stringw testMany;
    };
    Test test;

    /**wskaznik na GUI*/
    IGUIEnvironment* gui;

    /**tablica tekstur uzywanych do efektu eksplozji*/
    array<ITexture*> explosionTextures;

    /**tablica tekstur uzywanych do efektu krwi*/
    array<ITexture*> bloodTextures;

    /**obiekt lustra*/
    cMirror* mirror;

    /**obiekt kompasu*/
    CGUICompass* compass;

    /**zmienna pomcnicza do shadera(toon)*/
    s32 mtlToonShader;

    /**stworz efekt ognia*/
    void createFire(ISceneManager *smgr, IVideoDriver *driver,
                    vector3df p);

    /**stworz efekt swiatla*/
    void createLight(ISceneManager* smgr, IVideoDriver* driver);

    /**stworz efekt lustra*/
    void createMirror(IrrlichtDevice* device, IVideoDriver* driver,
                      ISceneManager* smgr, ICameraSceneNode* camera);

    /**funkcja wlaczajaca / wylaczajaca swiatla w scenie*/
    void changeLight( ISceneManager* smgr, bool turn );

public:

    /**czy w danej chwili nastepuje testowanie*/
    bool isTesting;

    /**kontruktor klasy effect*/
    Effect(IrrlichtDevice* device);

    /**destruktor klasy effect*/
    ~Effect();

    /**zaladowanie tekstur*/
    void loadTextures(ISceneManager* smgr);

    /**zaladowanie efektow do poszczegolnych scen*/
    void loadEffects(Settings* sSet, ELevels level, IrrlichtDevice* device,
                     IVideoDriver* driver, ISceneManager* smgr,
                     ICameraSceneNode* camera, IAnimatedMeshSceneNode* node);

    /**zaladuj efekt krwi do menedzera scen*/
    void createBlood(ISceneManager* smgr, vector3df position,
                     s32 sizeX, s32 sizeY, s32 timeDel, s32 timeCh);

    /**zaladuj efekt eksplozji do menedzera scen*/
    void createExplosion(ISceneManager* smgr, vector3df position,
                         s32 sizeX, s32 sizeY, s32 timeDel, s32 timeCh);

    /**efekt shadera toon*/
    void shaderToon(IVideoDriver* driver);

    /**altualizuj efekty w scenach*/
    void updateEffects(Settings* sSet, ELevels level, IVideoDriver* driver, ISceneManager* smgr, ICameraSceneNode* camera);

    /**stworz raport wydajnosci*/
    void performanceReport(ELevels level, Settings* sSet,IrrlichtDevice* device, IVideoDriver* driver, IGUIEnvironment* gui, f32 frameDeltaTime);

    /**test wydajnosci oparty na wykonywaniu tylko jednego efektu*/
    void performanceTest(IVideoDriver* driver);

    /**rozpocznij test wydajnosci pojedynczego efektu*/
    void startNewPerformanceTest(IGUIEnvironment* gui);

    /**stworz kompas (niedostepny w pierwszej scenie*/
    void createCompass(IVideoDriver* driver, IGUIEnvironment* gui, Settings* sSet);

    /**aktualizuj kompas (niedostepny w pierwszej scenie)*/
    void updateCompass(IrrlichtDevice* device, ISceneManager* smgr,
                       ISceneNode* terrain);

    /**prezentacja siatki obiektu na szescianie*/
    void addRenderTargetToTexture(IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* gui);

    /**stworz efekt 3D*/
    void createAnaglyph(ICameraSceneNode* camera, IVideoDriver* driver,
                        ISceneManager* smgr);

    /**stworz efekt sniegu*/
    void createSnow(ISceneManager* smgr, IVideoDriver* driver);

    /**stworz efekt mgly*/
    void changeFog(ISceneManager* smgr, bool change);

};

#endif
