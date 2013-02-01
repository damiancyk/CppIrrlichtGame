
#ifndef __SOUND_H_INCLUDED__
#define __SOUND_H_INCLUDED__

#include "device.h"

/**
 * Sound
 * Klasa przechowujaca uchwyty dzwiekow
 * @note Wczytuje dzwieki oraz daje nam mozliwosc ich uruchamiania
 */
enum TypeOfSound
{
    E_S_INTRO,
    E_S_WOLF_DUSK
};

class Sound
{
private:

public:

    /**wskaznik na silnik muzyczny przechowywany jako lokalna zmienna w obiekcie sound*/
    ISoundEngine* engineSnd;

    /**dzwiek muzyki z intro*/
    ISound* sSplashScreen;
    ISoundSource* sSourceSplashScreen;

    /**dzwiek muzyki z intro*/
    ISound* sOutro;
    ISoundSource* sSourceOutro;

    /**dzwiek muzyki z intro*/
    ISound* sIntro;
    ISoundSource* sSourceIntro;

    /**dzwiek menu*/
    ISound* sMenu;
    ISoundSource* sSourceMenu;

    /**podklad w pierwszej scenie*/
    ISound* sScene1;
    ISoundSource* sSourceScene1;

    /**podklad w drugiej scenie*/
    ISound* sScene2;
    ISoundSource* sSourceScene2;

    /**dzwiek krokow bohatera w scenie 1*/
    ISound* sStep;
    ISoundSource* sSourceStep;

    /**dzwiek krokow bohatera w scenie 2*/
    ISound* sStep2;
    ISoundSource* sSourceStep2;

    /**dzwiek ognia*/
    ISound* sFire;
    ISoundSource* sSourceFire;

    /**dzwiek wilka(oddech)*/
    ISound* sWolf1;
    ISoundSource* sSourceWolf1;

    /**dzwiek zranionego bohatera*/
    ISound* sHeroDie;
    ISoundSource* sSourceHeroDie;

    /**dzwiek zranionego wilka*/
    ISound* sEnemyAttacking;
    ISoundSource* sSourceEnemyAttacking;

    /**dzwiek zranionego wilka*/
    ISound* sWolfDeath;
    ISoundSource* sSourceWolfDeath;

    /**skowyt wilka*/
    ISound* sWolfDusk;
    ISoundSource* sSourceWolfDusk;

    /**dzwiek strzalu w scenie 1*/
    ISound* sShoot;
    ISoundSource* sSourceShoot;

    /**dzwiek przeladowania pistoletu w scenie 1*/
    ISound* sReload;
    ISoundSource* sSourceReload;

    /**dzwiek strzalu w scenie 2*/
    ISound* sShoot2;
    ISoundSource* sSourceShoot2;

    /**dzwiek przeladowania guna w scenie 2*/
    ISound* sReload2;
    ISoundSource* sSourceReload2;

    /**dzwiek rozpoczecia misji*/
    ISound* sMissionStart;
    ISoundSource* sSourceMissionStart;

    /**dzwiek zakonczenia misji*/
    ISound* sMissionEnd;
    ISoundSource* sSourceMissionEnd;

    /**misja nieudana*/
    ISound* sMissionFailed;
    ISoundSource* sSourceMissionFailed;

    /**alarm w samochodzie*/
    ISoundSource* sSourceAlarm;

    /**dzwiek po zabiciu wilka*/
    ISoundSource* sSourceZapchlonyKundel;

    /**dzwiek kliku przycisku*/
    ISoundSource* sSourceClick;

    /**dzwiek kruka w scenie 2*/
    ISoundSource* sSourceCrow;

    /**konstruktor klasy sound*/
    Sound ( ISoundEngine *engine );

    /**dzwiek zakonczenia misji*/
    ISound* sWind;
    ISoundSource* sSourceWind;

    /**zaladowanie dzwiekow*/
    void loadSounds (  );

    /**graj stale dzwieki z okreslonych scen*/
    void playSounds(Settings* sSet, ELevels level);

    /**graj dzwiek strzalu*/
    void playShoot();

    /**graj okreslony typ dzwieku*/
    void playSound(TypeOfSound t);

    /**ustaw ogolna glosnosc dzwieku*/
    void setVolume(f32 volume);
};

#endif
