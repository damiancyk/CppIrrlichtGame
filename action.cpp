
#include "action.h"

Action::Action(Sound* s)
{
    this->s=s;
    h = new Hero();
    y = new Enemy();
//!timeStart=device->getTimer()->getTime();
    initializeData(E_SPLASH_SCREEN);
    initializeData(E_INTRO);
    initializeData(E_SCENE1);
    initializeData(E_SCENE2);
    initializeData(E_OUTRO);
}

void Action::showTime(IrrlichtDevice* device, IGUIStaticText* tWatch)
{
    timeNow=device->getTimer()->getTime()/1000;
    stringw str;
    str+=timeNow;
    tWatch->setText(str.c_str());
}

vector3df Action::rotateToFace ( const core::vector3df& objectPosition, const core::vector3df& targetPosition )
{
    core::vector3df r = targetPosition - objectPosition;
    core::vector3df angle;
    angle.Y = atan2 ( r.X, r.Z );
    angle.Y *= ( 180 / core::PI );

    if ( angle.Y < 0 ) angle.Y += 360;
    if ( angle.Y >= 360 ) angle.Y -= 360;

    angle -= core::vector3df ( 0, 90, 0 );

    return angle;
}

bool Action::moveObject(IAnimatedMeshSceneNode* oObject, vector3df target, f32 rate, f32 isNear)
{
    bool move=true;
    if(oObject->getPosition().getDistanceFrom(target)<50.f)
    {
        move=false;
    }
    else
    {
        oObject->setRotation(rotateToFace(oObject->getPosition(), target));
        vector3df facing;
        facing.set ( sin ( ( oObject->getRotation().Y + 101 ) * PI / 180.0f ), 0, cos ( ( oObject->getRotation().Y + 101 ) * PI / 180.0f ) );
        facing.normalize();
        vector3df newPos = ( facing * rate) + oObject->getPosition();
        oObject->setPosition ( newPos );
    }
    return move;
}

ELevels Action::actSplashScreen(Settings* sSet)
{
    ELevels level=E_SPLASH_SCREEN;
    switch(this->eventSplashScreen)
    {
    case E_SPLASH_START:
        s->engineSnd->setSoundVolume(1);
        s->engineSnd->play2D(s->sSourceSplashScreen);
        eventSplashScreen=E_SPLASH_TO_INTRO;
        break;
    case E_SPLASH_TO_INTRO:
        if(!s->engineSnd->isCurrentlyPlaying(s->sSourceSplashScreen))level=E_INTRO;
        break;
    default:
        break;
    }
    return level;
}

ELevels Action::actIntro(IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oEnemy, Settings* sSet, ICameraSceneNode* camera, f32 rate)
{
    ELevels level=E_INTRO;
    switch(this->eventIntro)
    {
    case E_INTRO_START:
        oHero->setPosition(vector3df(-750,100,-250));
        oHero->setMD2Animation(EMAT_RUN);
        oHero->setAnimationSpeed(15);
        if(sSet->sounds&&sSet->soundTracks)s->engineSnd->play2D(s->sSourceIntro);
        eventIntro=E_INTRO_GO_HERO;
        break;
    case E_INTRO_GO_HERO:
        camera->setPosition(oHero->getPosition()+vector3df(-40,40,-40));
        camera->setTarget(oHero->getPosition());
        if(!moveObject(oHero, vector3df(300,0,355), rate, 50.f))
        {
            oHero->setMD2Animation(EMAT_STAND);
            oEnemy->setMD2Animation(EMAT_RUN);
            s->engineSnd->play2D(s->sSourceAlarm);
            s->engineSnd->play2D(s->sSourceEnemyAttacking);
            eventIntro=E_INTRO_ATTACK;
        }
        break;
    case E_INTRO_ATTACK:
        camera->setPosition(oHero->getPosition()+vector3df(40,20,-40));
        camera->setTarget(oHero->getPosition());
        if(!moveObject(oEnemy, oHero->getPosition(), rate, 50.f))
        {
            oHero->setMD2Animation(EMAT_CROUCH_DEATH);
            oEnemy->setMD2Animation(EMAT_CROUCH_ATTACK);
            eventIntro=E_INTRO_ATTACKING;
        }
        break;
    case E_INTRO_ATTACKING:
        if(!s->engineSnd->isCurrentlyPlaying(s->sSourceEnemyAttacking))eventIntro=E_INTRO_TO_MENU;
        break;
    case E_INTRO_TO_MENU:
        level=E_MENU;
        break;
    default:
        break;
    }
    return level;
}

ELevels Action::actScene1(IrrlichtDevice* device, IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oEnemy, Settings* sSet, IGUIEnvironment* gui, ICameraSceneNode* camera, f32 rate, IGUIImage* planetIcon)
{
    ELevels level=E_SCENE1;
    switch(this->eventScene1)
    {
    case E_SCENE1_START:
        oHero->setPosition(vector3df(-300,200,0));
device->getGUIEnvironment()->addMessageBox(
		L"..WIADOMOSC OD KROLA..", L"WITAJ WEDROWCZE!\n\
		ZABIJ WILKOLAKA DRECZACEGO POBLISKA WIOSKE!\n\
        I NIE DAJ SIE ZWIESC JEGO CZARNEJ MOCY..\n\
        W NAGRODE PRZEPUSCIMY CIE DO ZAGINIONEJ DOLINY.\n\
        TAM ZNAJDZIESZ EFEKTY, KTORE WPRAWIA CIE W OSLUPIENIE!\n\
         TESTOWANIE ROZPOCZYNASZ KLAWISZEM 'T', KONCZYSZ ENTEREM..\n\
		");
        eventScene1=E_SCENE1_GO_HERO;
        break;
    case E_SCENE1_GO_HERO:
        if(oHero->getPosition().getDistanceFrom(vector3df(300,0,355))<100.f)
        {
            s->engineSnd->play2D(s->sSourceWolfDusk);
            oEnemy->setVisible(true);
            oEnemy->setPosition(vector3df(461,200,667));
            planetIcon->setColor(SColor(250,200,150,150));
            oEnemy->setMD2Animation(EMAT_RUN);
            eventScene1=E_SCENE1_PURSUIT;
        }
        break;
    case E_SCENE1_PURSUIT:
        if(y->life<=0)
        {
            oEnemy->setVisible(false);
            oEnemy->setPosition(vector3df(0,-1000,0));
            s->engineSnd->stopAllSounds();
            s->engineSnd->play2D(s->sSourceMissionEnd);
            IGUIFont* fontBig=gui->getFont ( "textures/fonts/fontBig.xml" );
            IGUIStaticText *tt;
            tt=gui->addStaticText ( L"WYGRALES! MOZESZ WEJSC TERAZ DO MROCZNEGO LASU", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-100, (s32)(sSet->resolution.Height/2)-20 ), dimension2di ( 200, 40 ) ), false, false, 0 );
            tt->setOverrideColor(SColor(250,200,200,200));
            tt->setOverrideFont(fontBig);
            oEnemy->setMD2Animation(EMAT_BOOM);
            eventScene1=E_SCENE1_VICTORY;
            break;
        }
        if(!moveObject(oEnemy, oHero->getPosition(), rate, 50.f))
        {
            oEnemy->setMD2Animation(EMAT_ATTACK);
            oHero->setMD2Animation(EMAT_PAIN_B);
            eventScene1=E_SCENE1_ATTACK;
        }
        break;
    case E_SCENE1_ATTACK:
        e->createAnaglyph(camera, device->getVideoDriver(), device->getSceneManager());
        if(!s->engineSnd->isCurrentlyPlaying(s->sSourceHeroDie))s->engineSnd->play2D(s->sSourceHeroDie);
        if(h->armor>0)
        {
            h->armor-=0.01;
        }
        else if(h->life>0)
        {
            h->life-=0.01;
        }
        else
        {
            oHero->setVisible(false);
            oEnemy->setMD2Animation(EMAT_STAND);
            IGUIFont* fontBig=gui->getFont ( "textures/fonts/fontBig.xml" );
            IGUIStaticText *tt;
            tt=gui->addStaticText ( L"PRZEGRALES. ROZPOCZNIJ OD NOWA TE MISJE..", rect<s32 > ( position2di ( (s32)(sSet->resolution.Width/2)-120, (s32)(sSet->resolution.Height/2)-20 ), dimension2di ( 240, 40 ) ), false, false, 0 );
            tt->setOverrideColor(SColor(250,200,200,200));
            tt->setOverrideFont(fontBig);
            s->engineSnd->stopAllSounds();
            s->engineSnd->play2D(s->sSourceMissionFailed);
            eventScene1=E_SCENE1_LOOSE;
            break;
        }
        if(moveObject(oEnemy, oHero->getPosition(), rate, 50.f))
        {
            oEnemy->setMD2Animation(EMAT_RUN);
//oHero->setMD2Animation(EMAT_RUN);
            eventScene1=E_SCENE1_PURSUIT;
        }
        break;

    case E_SCENE1_VICTORY:
        if(!s->engineSnd->isCurrentlyPlaying(s->sSourceMissionEnd))
        {
            sSet->werewolfDead=true;
            printf("dead");
            eventScene1=E_SCENE1_TO_MENU;
        }
        break;

    case E_SCENE1_LOOSE:
        if(!s->engineSnd->isCurrentlyPlaying(s->sSourceMissionFailed))
        {
            sSet->werewolfDead=false;
            eventScene1=E_SCENE1_TO_MENU;
        }
        break;

    case E_SCENE1_TO_SCENE2:
        level=E_SCENE2;
        break;

    case E_SCENE1_TO_MENU:
        eventScene1=E_SCENE1_START;
        level=E_MENU;
        break;

    default:
        break;
    }
    return level;
}

ELevels Action::actScene2(IrrlichtDevice* device, IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oEnemy, Settings* sSet, IGUIEnvironment* gui, ICameraSceneNode* camera, f32 rate, IGUIImage* planetIcon)
{
    ELevels level=E_SCENE2;
    switch(this->eventScene2)
    {
    case E_SCENE2_START:
        oHero->setPosition(vector3df(1800,-550,1000));
        if(sSet->werewolfes)
        {
            device->getGUIEnvironment()->addMessageBox(
                L">>ENTER ABY ZAMKNAC", L"ODWROC SIE ZA SIEBIE.. I PRZETESTUJ EFEKT\n\
		");
        }

device->getGUIEnvironment()->addMessageBox(
		L"..WIADOMOSC OD KROLA..", L"WITAJ WEDROWCZE!\n\
		DOSTAPILES ZASZCZYU TESTOWANIA KOLEJNEJ PORCJI EFEKTOW W MOIM SWIECIE \n\
		TESTOWANIE ROZPOCZYNASZ KLAWISZEM 'T', KONCZYSZ ENTEREM..\n\
        OWOCNYCH PLONOW I SPRAWNEGO RENDERERA ZYCZY KROL IRRLICHT III WSPANIALY\n\
		");
        eventScene2=E_SCENE2_TESTING;
        break;
    case E_SCENE2_TESTING:
        oHero->setRotation(rotateToFace(oHero->getPosition(), vector3df(0,-500,0)));
        break;
    default:
        break;
    }
    return level;
}

ELevels Action::actOutro(Settings* sSet)
{
    ELevels level=E_OUTRO;
    switch(this->eventOutro)
    {
    case E_OUTRO_START:
        s->engineSnd->setSoundVolume(1);
        s->engineSnd->play2D(s->sSourceOutro);
        eventOutro=E_OUTRO_QUIT;
        break;
    case E_OUTRO_QUIT:
        if(!s->engineSnd->isCurrentlyPlaying(s->sSourceOutro))level=E_NONE;
        break;
    default:
        break;
    }
    return level;
}

void Action::initializeData(ELevels level)
{
    switch(level)
    {
    case E_SPLASH_SCREEN:
        eventSplashScreen=E_SPLASH_START;
        break;
    case E_INTRO:
        eventIntro=E_INTRO_START;
        break;
    case E_SCENE1:
        eventScene1=E_SCENE1_START;
        break;
    case E_SCENE2:
        eventScene2=E_SCENE2_START;
        break;
    case E_OUTRO:
        eventOutro=E_OUTRO_START;
        break;
    default:
        break;
    }
}

void Action::setStartTime(IrrlichtDevice* device)
{
    timeStart=device->getTimer()->getTime()/1000;
}

