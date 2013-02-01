
#include "sound.h"

Sound::Sound ( ISoundEngine *engine )
{
    this->engineSnd = engine;
    loadSounds();
}

void Sound::loadSounds ( )
{
    sSourceOutro = engineSnd->addSoundSourceFromFile ( "sounds/tracks/sOutro.mp3" );
    sSourceSplashScreen = engineSnd->addSoundSourceFromFile ( "sounds/tracks/sSplashScreen.mp3" );
    sSourceIntro = engineSnd->addSoundSourceFromFile ( "sounds/tracks/sIntro.ogg" );
    sSourceMenu = engineSnd->addSoundSourceFromFile ( "sounds/tracks/sMenu.mp3" );
    sSourceScene1 = engineSnd->addSoundSourceFromFile ( "sounds/tracks/sScene1.ogg" );
    sSourceScene2 = engineSnd->addSoundSourceFromFile ( "sounds/tracks/sScene2.mp3" );
    sSourceHeroDie = engineSnd->addSoundSourceFromFile ( "sounds/hero/die1.wav" );
    sSourceEnemyAttacking = engineSnd->addSoundSourceFromFile ( "sounds/wolf/Monster_Demon_Growl_SE.mp3" );
    sSourceWolfDeath = engineSnd->addSoundSourceFromFile ( "sounds/wolf/WolfDeath1.wav" );
    sSourceWolfDusk = engineSnd->addSoundSourceFromFile ( "sounds/wolf/DuskWolf.wav" );
    sSourceShoot = engineSnd->addSoundSourceFromFile ( "sounds/weapons/Gunshot_Bang.mp3" );
    sSourceReload = engineSnd->addSoundSourceFromFile ( "sounds/weapons/aug_boltpull.wav" );
    sSourceShoot2 = engineSnd->addSoundSourceFromFile ( "sounds/weapons/sg552-1.wav" );
    sSourceReload2 = engineSnd->addSoundSourceFromFile ( "sounds/weapons/sg552_clipin.wav" );
    sSourceMissionStart = engineSnd->addSoundSourceFromFile ( "sounds/missions/QuestNew.wav" );
    sSourceMissionEnd = engineSnd->addSoundSourceFromFile ( "sounds/missions/GameFound.mp3" );
    sSourceMissionFailed = engineSnd->addSoundSourceFromFile ( "sounds/missions/LoseCombat.mp3" );
    sSourceStep = engineSnd->addSoundSourceFromFile ( "sounds/hero/pl_dirt1.wav" );
    sSourceStep2 = engineSnd->addSoundSourceFromFile ( "sounds/hero/fall2.wav" );
    sSourceFire = engineSnd->addSoundSourceFromFile ( "sounds/effects/fire.mp3" );
    sSourceWolf1 = engineSnd->addSoundSourceFromFile ( "sounds/wolf/Wolf1.wav" );
    sSourceAlarm = engineSnd->addSoundSourceFromFile ( "sounds/effects/alarm.mp3" );
    sSourceClick = engineSnd->addSoundSourceFromFile ( "sounds/menu/beep-25.wav" );
    sSourceZapchlonyKundel = engineSnd->addSoundSourceFromFile ( "sounds/hero/OgreBeastMasterYesAttack3.wav" );
    sSourceCrow = engineSnd->addSoundSourceFromFile ( "sounds/missions/crow.wav" );
    sSourceWind = engineSnd->addSoundSourceFromFile ( "sounds/effects/Windhowl_Storm.mp3" );
}

void Sound::playSounds ( Settings* sSet, ELevels level )
{
    engineSnd->stopAllSounds();

    switch ( level )
    {
    case E_MENU:

        if ( sSet->sounds )
        {
            if ( sSet->soundTracks )
            {
                sMenu = engineSnd->play2D ( sSourceMenu, true, false );
            }

            if ( sSet->soundEffects )
            {
                engineSnd->play2D ( sSourceFire, true, false );
            }
        }

        break;
    case E_SCENE1:

        if ( sSet->sounds )
        {
            if ( sSet->soundTracks )
                sScene1 = engineSnd->play2D ( sSourceScene1, true, false );

            if ( sSet->soundEffects )
            {
                engineSnd->play2D ( sSourceWind, true, false );
                sFire = engineSnd->play3D ( sSourceFire, vec3df ( 320, -32, 380 ), true, true );
                if ( sFire )
                {
                    sFire->setMinDistance ( 100.0f );
                    sFire->setIsLooped ( true );
                    sFire->setIsPaused ( false );
                }


                sWolf1 = engineSnd->play3D ( sSourceWolf1, vec3df ( 320, -32, 380 ), true, true );
                if ( sWolf1 )
                {
                    sWolf1->setMinDistance ( 100.0f );
                    sWolf1->setIsLooped ( true );
                    sWolf1->setIsPaused ( false );
                }

                //engineSnd->play2D ( sSourceStartScene1 );

                //sMissionStart = engineSnd->play2D ( sSourceMissionStart );
            }
        }
        break;
    case E_SCENE2:
        if ( sSet->soundTracks )
            sScene2 = engineSnd->play2D ( sSourceScene2, true, false );
        break;
    default:
        break;
    }
}

void Sound::playShoot()
{
    engineSnd->play2D ( sSourceShoot );
}

void Sound::playSound(TypeOfSound t)
{
    switch(t)
    {
    case E_S_INTRO:
        sIntro = engineSnd->play2D ( sSourceIntro, true, false );
        break;
    default:
        break;

    }
}

void Sound::setVolume(f32 volume)
{
    engineSnd->setSoundVolume(volume);
}
