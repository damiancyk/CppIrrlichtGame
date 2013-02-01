
#include "game.h"

Game::Game()
{
    sSet=new Settings();

    if(set(false))
    {
        printf("set resolution, device etc.\n");

    }
    else
    {
        exit(0);
    }
    /**inicjalizacja parametrow tworzonego urzadzenia Irrlicht*/

    sSet->stencilBuffer = true;
    sSet->vsync = true;
    level = E_INTRO;
    sSet->typeOfCamera=E_TPP;
    sSet->mouseSensivityX = 64.f;
    sSet->mouseSensivityY = 64.f;
    sSet->rateSniper=1.f;
    sSet->rateSniperY=1.f;
    sSet->posCamAhead = 0;
    sSet->RUN=1;
    sSet->werewolfDead=false;

    sSet->mEffects = true;
    sSet->effects = true;

    sSet->mirror = false;
    sSet->fog = true;
    sSet->fire = true;
    sSet->heroLight= false;
    sSet->heroTransparency= false;
    sSet->heroShadow= false;
    sSet->lightAmbient=false;
    sSet->lightPoint=false;
    sSet->lightMap=false;

    sSet->snow = true;
    sSet->shaderToon = false;
    sSet->heroSphericMap= false;
    sSet->forest=false;
    sSet->terrainQuality=true;
    sSet->werewolfes=false;
    sSet->sky=true;
    sSet->compass=true;

    sSet->sounds = true;
    sSet->soundTracks = true;
    sSet->soundEffects = true;
    sSet->sound3D = true;

    sSet->volume = .8f;

    /**stworzenie glownego urzadzenia Irrlicht-a*/
    device = createDevice ( sSet->driverType, sSet->resolution, sSet->bits, sSet->fullscreen, sSet->stencilBuffer, sSet->vsync, 0 );

    if ( device )
    {
        printf ( "..device started successfully..\n" );
        device->setResizable ( true );
        device->setEventReceiver ( &receiver );
        device->setWindowCaption ( L"--==Irrlicht==--" );
    }

    else
    {
        printf ( "!!could not run main device of Irrlicht!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie sterownikow*/
    driver = device->getVideoDriver();

    if ( driver )
    {
        printf ( "..driver started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run driver!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie GUI*/
    gui = device->getGUIEnvironment();

    if ( gui )
    {
        printf ( "..gui started successfully..\n" );
        gui->getSkin()->setColor ( EGDC_BUTTON_TEXT, video::SColor(250,250,250,250) );
        gui->getSkin()->setColor ( EGDC_3D_HIGH_LIGHT, video::SColor(240,0x22,0x22,0x22) );
        gui->getSkin()->setColor ( EGDC_3D_FACE, video::SColor(240,0x44,0x44,0x44) );
        gui->getSkin()->setColor ( EGDC_WINDOW, video::SColor(240,0x66,0x66,0x66) );
    }

    else
    {
        printf ( "!!could not run gui!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie menedzera scen Irrlichta*/
    smgr = device->getSceneManager();

    if ( smgr )
    {
        printf ( "..scene manager started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run scene manager!!\n" );
        exit ( 1 );
    }

    camera = smgr->addCameraSceneNodeFPS ( 0, 50.f, 0.1f );

    camera->setFarValue ( 10000.0f );

    /**zainicjalizowanie menedzera kolizji Irrlichta*/
    collMan = smgr->getSceneCollisionManager();

    if ( collMan )
    {
        printf ( "..collision manager started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run collision manager!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie urzadzenia dzwiekowego*/
    engineSnd = createIrrKlangDevice();

    if ( engineSnd )
    {
        printf ( "..engine Irrklang started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run engine Irrklang!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie timera Irrlichta*/
    timer = device->getTimer();

    if ( timer )
    {
        printf ( "..timer started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run timer!!\n" );
        exit ( 1 );
    }

    this->lastFPS = 60;
    isJoystick=false;

    o = new Object ();
    e = new Effect (device);
    s = new Sound ( engineSnd );
    a = new Action(s);
    e->shaderToon(driver);
    //e->loadTextures(smgr);
    changeSettings = false;
    stop = false;
}

void Game::run()
{
    while ( !stop )
    {
        setStage ( E_MENU);
        u32 then = device->getTimer()->getTime();
        while ( device->run() && !changeSettings )
        {
            const u32 now = device->getTimer()->getTime();
            const f32 frameDeltaTime = (f32)(now - then) / 10.f;
            then = now;
            /**jesli okno aktywne*/
            if ( device->isWindowActive() )
            {
                receiver.endEventProcess();
                driver->setViewPort(rect<s32>(0,0,sSet->resolution.Width,sSet->resolution.Height));
                driver->beginScene ( true, true, SColor ( 0, 0, 0, 0 ) );
                switch ( level )
                {
                case E_SPLASH_SCREEN:
                    if ( gui )
                        gui->drawAll();
                    {
                        ELevels lvl=a->actSplashScreen(sSet);
                        if(lvl!=level)
                        {
                            setStage(lvl);
                        }
                    }
                    break;
                case E_INTRO:
                    if(smgr)
                        smgr->drawAll();
                    if ( gui )
                        gui->drawAll();
                    {
                        ELevels lvl=a->actIntro(o->oHero, o->oEnemy, sSet, camera, frameDeltaTime);
                        if(lvl!=level)
                        {
                            setStage(lvl);
                        }
                    }
                    keyFuncIntro ( &receiver);
                    break;
                case E_MENU:
                    if ( smgr )
                        smgr->drawAll();
                    if ( gui )
                        gui->drawAll();
                    if(device&&driver)
                        showFps(device, driver);
                    if(myCursor)myCursor->drawCursor();
                    keyFuncMenu ( &receiver, frameDeltaTime, device, gui );
                    break;
                case E_SCENE1:
                    if ( o->eTypeOfGameMenu == GUI_GAME )
                    {
                        updateCursor ( device );
                        a->h->updateCamera ( sSet, o->oHero, camera, cursorPos, frameDeltaTime, sSet->rateSniper, sSet->rateSniper );
                        if(o->oHero&&o->oGun)
                            a->h->updateHero ( o->oHero, o->oGun );
                        specialPreference();

                        e->updateEffects(sSet, level, driver, smgr, camera);
                        e->performanceTest(driver);

                        smgr->setActiveCamera(camera);
                        driver->setViewPort(rect<s32>(0,0,sSet->resolution.Width,sSet->resolution.Height));
                        if(smgr)
                            smgr->drawAll();
                        if(device&&driver)
                            showFps ( device, driver );
                        a->h->updateHeroBillboards(o->oHero, intersection);
                        a->y->updateEnemyBillboards(o->oEnemy, o->oHero, selectedSceneNode);
                        rayTracing ( o->oHero, camera, collMan );
                        if(sSet->sound3D)engineSnd->setListenerPosition ( o->oHero->getAbsolutePosition(), a->h->facing, vector3df ( 0, 0, 0 ), vector3df ( 0, 1, 0 ) );
                        o->updateFpsGui(device, driver, gui);
                        o->updateGuiHeroStats(a->h->life, a->h->armor);
                        a->showTime(device, o->sGui.tWatch);
                    }
                    if ( o->eTypeOfGameMenu == GUI_GAME_PAUSE )
                    {
                        o->draw2D(device, driver, then, sSet);
                    }
                    if ( gui )gui->drawAll();
                    {
                        ELevels lvl=a->actScene1(device, o->oHero, o->oEnemy, sSet, gui, camera, frameDeltaTime ,o->planetIcon);
                        if(lvl!=level)
                        {
                            setStage(lvl);
                            break;
                        }
                    }
                    keyFuncScene ( &receiver, frameDeltaTime);
                    break;
                case E_SCENE2:
                    if ( o->eTypeOfGameMenu == GUI_GAME )
                    {
                        updateCursor ( device );
                        a->h->updateCamera ( sSet, o->oHero, camera, cursorPos, frameDeltaTime, sSet->rateSniper, sSet->rateSniper );
                        if(o->oHero&&o->oGun)
                            a->h->updateHero ( o->oHero, o->oGun );
                        specialPreference();
//
                        e->updateEffects(sSet, level, driver, smgr, camera);
                        e->performanceTest(driver);
                        smgr->setActiveCamera(camera);

////if(splitScreen)
//                    driver->setViewPort(rect<s32>(0,0,sSet->resolution.Width,sSet->resolution.Height));
//                    driver->setViewPort(rect<s32>(0,0,200,200));



                        if(smgr)
                            smgr->drawAll();
                        if(device&&driver)
                            showFps ( device, driver );
                        a->h->updateHeroBillboards(o->oHero, intersection);
                        a->y->updateEnemyBillboards(o->oEnemy, o->oHero, selectedSceneNode);
                        if(sSet->werewolfes)o->moveWerewolves(o->werewolves, vector3df(1*frameDeltaTime,0,0));
                        rayTracing ( o->oHero, camera, collMan );
                        if(sSet->sound3D)engineSnd->setListenerPosition ( o->oHero->getAbsolutePosition(), a->h->facing, vector3df ( 0, 0, 0 ), vector3df ( 0, 1, 0 ) );
                        o->updateFpsGui(device, driver, gui);
                        o->updateGuiHeroStats(a->h->life, a->h->armor);
                        a->showTime(device, o->sGui.tWatch);
                    }
                    if ( o->eTypeOfGameMenu == GUI_GAME_PAUSE )
                    {
                        o->draw2D(device, driver, then, sSet);
                    }
                    //if(splitScreen)
                    //driver->setViewPort(rect<s32>(0,0,sSet->resolution.Width,sSet->resolution.Height));
                    if(o->terrain&&sSet->compass)e->updateCompass(device, smgr, o->terrain);
                    if ( gui )gui->drawAll();
                    {
                        ELevels lvl=a->actScene2(device, o->oHero, o->oEnemy, sSet, gui, camera, frameDeltaTime ,o->planetIcon);
                        if(lvl!=level)
                        {
                            setStage(lvl);
                            break;
                        }
                    }
                    keyFuncScene ( &receiver, frameDeltaTime);
                    break;
                case E_OUTRO:
                    if(smgr)
                        smgr->drawAll();
                    if ( gui )
                        gui->drawAll();
                    {
                        ELevels lvl=a->actOutro(sSet);
                        if(lvl!=level)
                        {
                            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
                            stop = true;
                            changeSettings = true;
                        }
                    }
                    keyFuncOutro ( &receiver);
                    break;
                default:
                    break;
                }

                driver->endScene();
                receiver.startEventProcess();
            }

            else
            {
                device->yield();
            }
        }

        if ( !stop )
        {
            reload();
        }
    }

    if(device)
        device->closeDevice();

    if ( engineSnd )
        engineSnd->drop();

    if ( myCursor )
        delete myCursor;
}

void Game::setStage ( ELevels level )
{
    this->level = level;
    s->setVolume(sSet->volume);
    switch ( level )
    {
    case E_SPLASH_SCREEN:
        a->initializeData(level);
        gui->addImage ( driver->getTexture ( "textures/splashScreen.jpg" ),
                        core::position2d<s32 > ( (s32)(sSet->resolution.Width/2)-320,
                                (s32)(sSet->resolution.Height/2)-240 ),
                        true, 0, -1, L"iGuiBackgroundRight" );
        break;
    case E_INTRO:
        if ( smgr )
            smgr->clear();
        if ( gui )
            gui->clear();
        a->initializeData(level);
        o->loadIntro ( sSet, device, smgr, driver, gui, camera);
        driver->setFog ( SColor ( 0, 150, 150, 150 ), EFT_FOG_EXP /*EFT_FOG_EXP EFT_FOG_LINEAR EFT_FOG_EXP2*/ , 0, 3000, 0.003f, false, false );
        e->changeFog ( smgr, sSet->fog );
        a->setStartTime(device);
        s->playSounds(sSet, level);
        break;

    case E_MENU:
        if ( smgr )
            smgr->clear();
        if ( gui )
            gui->clear();
        a->initializeData(level);
        myCursor = new CCursor ( device, "textures/gui/cursor.png" );
        o->loadMenuTerrain ( smgr, driver, sSet );
        o->loadMenuGUI ( sSet, device, smgr, driver, gui, GUI_MENU_TYPE_MAIN);
        showStartMessage(level, sSet);
        joysticks(sSet, gui);
        s->playSounds(sSet, level);
        break;

    case E_SCENE1:
        if ( smgr )
            smgr->clear();
        if ( gui )
            gui->clear();
        a->initializeData(level);
        o->loadScene1 ( sSet, device, smgr, driver, gui, camera);
        a->h->setName(smgr, o->oHero);
        o->loadGameGUI ( gui, driver, device, GUI_GAME, sSet );

        a->h->loadHeroBillboards(smgr, driver);
        a->y->loadEnemyBillboards(smgr, driver);
        a->h->setStartValues();
        a->y->setStartValues();
        e->loadEffects(sSet, level, device, driver, smgr, camera, o->oHero);
        s->playSounds(sSet, level);
        a->setStartTime(device);
        break;

    case E_SCENE2:
        if ( smgr )
            smgr->clear();
        if ( gui )
            gui->clear();
        a->initializeData(level);
        o->loadScene2 ( sSet, device, smgr, driver, gui, camera );
        a->h->setName(smgr, o->oHero);
        o->loadGameGUI ( gui, driver, device, GUI_GAME, sSet );

        a->h->loadHeroBillboards(smgr, driver);
        a->y->loadEnemyBillboards(smgr, driver);
        a->h->setStartValues();
        a->y->setStartValues();
        e->loadEffects(sSet, level, device, driver, smgr, camera, o->oHero);
        s->playSounds(sSet, level);
        a->setStartTime(device);
        if(sSet->compass)e->createCompass(driver, gui, sSet);
        break;
    case E_OUTRO:
        if ( smgr )
            smgr->clear();
        if ( gui )
            gui->clear();
        a->initializeData(level);
        o->loadOutro ( sSet, device, smgr, driver, gui, camera);
        e->createSnow ( smgr, driver );
        s->playSounds(sSet, level);
        break;
    default:
        break;
    }
}

void Game::updateCursor ( IrrlichtDevice* device )
{
    //pobranie pozycji kursora*/
    cursorPos = device->getCursorControl()->getRelativePosition();
    //daje dostêp do kontroli kursora*/
    device->getCursorControl()->setPosition ( 0.5f, 0.5f );
}

void Game::rayTracing ( IAnimatedMeshSceneNode* oHero, ICameraSceneNode* camera, ISceneCollisionManager* collMan )
{
    ray.start = camera->getPosition();
    ray.end = ray.start + ( oHero->getPosition() - ray.start + a->h->posRayPlus ).normalize() * 2000.0f;
    selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay ( ray, intersection, hitTriangle );

    driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

}

void Game::keyFuncMenu ( CIrrEventReceiver* receiver, f32 rate, IrrlichtDevice* device, IGUIEnvironment* gui )
{
    joysticksControlling(receiver, rate, o->anode);

    if ( receiver->getEventCallerOfMenuByID() == GUI_ID_M_EXIT_GAME )
    {
        stop = true;
        changeSettings = true;

    }

    if ( receiver->isKeyPressed ( KEY_ESCAPE ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        stop = true;
        changeSettings = true;
        printf ( "QNIEC\n" );
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_TEST && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        if(sSet->werewolfDead)
        {
            setStage ( E_SCENE2 );
        }
        else
        {
            device->getGUIEnvironment()->addMessageBox(
                L"..ZAKAZ WSTEPU..", L"NIE ROZPOCZNIESZ POZOSTALYCH TESTOW!\n\
		ZABIJ WPIERW STRASZNEGO WILKOLAKA.. \n\
		");
        }
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_START && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        setStage ( E_SCENE1 );
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_OPTIONS && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );

        o->loadMenuGUI ( sSet, device, smgr, driver, gui, GUI_MENU_TYPE_OPTION);
        o->setChecked(sSet);
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_AUTHOR && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        showAboutText();
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_QUIT && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        stop = true;
        changeSettings = true;
//setStage(E_OUTRO);
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_SAVE && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        a->h->heroName=o->sGui.edtNick->getText ();
        o->sGui.bSave->setEnabled ( false );
    }

    if ( receiver->getEventCallerByID() == GUI_ID_B_RETURN_TO_MAIN && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        o->loadMenuGUI ( sSet, device, smgr, driver, gui, GUI_MENU_TYPE_MAIN);
        s->playSounds(sSet, E_MENU);
    }

    if ( receiver->getEventCallerByID() == GUI_ID_S_ /*&& receiver->getEventCallerByElement(EGET_SCROLL_BAR_CHANGED)*/ )
    {
        s32 pos = ( ( IGUIScrollBar* ) o->sGui.s )->getPos();

        for ( u32 i = 0; i < EGDC_COUNT; ++i )
        {
            SColor col = gui->getSkin()->getColor ( ( EGUI_DEFAULT_COLOR ) i );
            col.setAlpha ( pos );
            gui->getSkin()->setColor ( ( EGUI_DEFAULT_COLOR ) i, col );
        }
    }

    if ( receiver->getEventCallerByID() == GUI_ID_VOLUME /*&& receiver->getEventCallerByElement(EGET_SCROLL_BAR_CHANGED)*/ )
    {
        if ( receiver->isMouseButtonPressed ( MBLEFT ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            s32 pos = ( ( IGUIScrollBar* ) o->sGui.sVolume )->getPos();
            f32 newVolume=pos;
            newVolume/=100;
            sSet->volume=newVolume;
            s->setVolume(sSet->volume);
            stringw strVolume="ustawiono glosnosc: ";
            strVolume+=newVolume;
            o->addMessage(strVolume.c_str(),1);
        }
    }

    if ( receiver->getEventCallerByID() == GUI_ID_EDT_NICK && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        o->sGui.bSave->setEnabled ( true );
    }

    if ( receiver->getEventCallerByID() == GUI_ID_M_EFFECTS && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        if(sSet->mEffects==true)
        {
            o->addMessage(L"efekty w menu::off",1);
            sSet->mEffects=false;
            o->loadMenuTerrain(smgr, driver, sSet);
        }
        else
        {
            o->addMessage(L"efekty w menu::on",1);
            sSet->mEffects=true;
            o->loadMenuTerrain(smgr, driver, sSet);
        }
    }

    if ( receiver->getEventCallerByID() == GUI_ID_EFFECTS && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        if(sSet->effects)
        {
            o->addMessage(L"--==wylaczono efekty==--",0);
            sSet->effects=false;
            o->changeEffectsAvailability(false);
        }
        else
        {
            o->addMessage(L"--==wlaczono efekty==--",0);
            sSet->effects=true;
            o->changeEffectsAvailability(true);
        }
    }

    if(sSet->effects)
    {

        if ( receiver->getEventCallerByID() == GUI_ID_MIRROR && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->mirror==true)
            {
                o->addMessage(L"lustro::off",1);
                sSet->mirror=false;
            }
            else
            {
                o->addMessage(L"lustro::on",1);
                sSet->mirror=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_SNOW && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->snow==true)
            {
                o->addMessage(L"snieg::off", 1);
                sSet->snow=false;
            }
            else
            {
                o->addMessage(L"snieg::on", 1);
                sSet->snow=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_FOG && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );

            if(sSet->fog==true)
            {
                o->addMessage(L"mgla::off", 1);
                sSet->fog=false;
            }
            else
            {
                o->addMessage(L"mgla::on", 1);
                sSet->fog=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_FIRE && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->fire==true)
            {
                o->addMessage(L"ogien::off", 1);
                sSet->fire=false;
            }
            else
            {
                o->addMessage(L"mgla::on", 1);
                sSet->fire=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_SHADERTOON && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->shaderToon==true)
            {
                o->addMessage(L"shaderToon::off", 1);
                sSet->shaderToon=false;
            }
            else
            {
                o->addMessage(L"shaderToon::on", 1);
                sSet->shaderToon=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_HERO_LIGHT && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->heroLight==true)
            {
                o->addMessage(L"oswietlony bohater::off", 1);
                sSet->heroLight=false;
            }
            else
            {
                o->addMessage(L"oswietlony bohater::on", 1);
                sSet->heroLight=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_HERO_TRANSPARENCY && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->heroTransparency==true)
            {
                o->addMessage(L"przezrocz. bohatera::off", 1);
                sSet->heroTransparency=false;
            }
            else
            {
                o->addMessage(L"przezrocz. bohatera::on", 1);
                sSet->heroTransparency=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_HERO_SPHERIC_MAP && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->heroSphericMap==true)
            {
                o->addMessage(L"mapowanie sferyczne::off", 1);
                sSet->heroSphericMap=false;
            }
            else
            {
                o->addMessage(L"mapowanie sferyczne::on", 1);
                sSet->heroSphericMap=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_HERO_SHADOW && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->heroShadow==true)
            {
                o->addMessage(L"cien bohatera::off", 1);
                sSet->heroShadow=false;
            }
            else
            {
                o->addMessage(L"cien bohatera::on", 1);
                sSet->heroShadow=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_LIGHT_AMBIENT && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->lightAmbient==true)
            {
                o->addMessage(L"swiatlo rozproszone::off", 1);
                sSet->lightAmbient=false;
            }
            else
            {
                o->addMessage(L"swiatlo rozproszone::on", 1);
                sSet->lightAmbient=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_LIGHT_POINT && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->lightPoint==true)
            {
                o->addMessage(L"swiatlo punktowe::off", 1);
                sSet->lightPoint=false;
            }
            else
            {
                o->addMessage(L"swiatlo punktowe::on", 1);
                sSet->lightPoint=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_LIGHT_MAP && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->lightMap==true)
            {
                o->addMessage(L"swiatlo mapa sw::off", 1);
                sSet->lightMap=false;
            }
            else
            {
                o->addMessage(L"swiatlo mapa sw::on", 1);
                sSet->lightMap=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_FOREST && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->forest==true)
            {
                o->addMessage(L"las::off", 1);
                sSet->forest=false;
            }
            else
            {
                o->addMessage(L"las::on", 1);
                sSet->forest=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_WEREWOLVES && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->werewolfes==true)
            {
                o->addMessage(L"stado wilkolakow::off", 1);
                sSet->werewolfes=false;
            }
            else
            {
                o->addMessage(L"stado wilkolakow::on", 1);
                sSet->werewolfes=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_TERRAIN_QUALITY && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->terrainQuality==true)
            {
                o->addMessage(L"szczegoly terenu::off", 1);
                sSet->terrainQuality=false;
            }
            else
            {
                o->addMessage(L"szczegoly terenu::on", 1);
                sSet->terrainQuality=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_SKY && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->sky==true)
            {
                o->addMessage(L"niebo::off", 1);
                sSet->sky=false;
            }
            else
            {
                o->addMessage(L"niebo::on", 1);
                sSet->sky=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_COMPASS && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->compass==true)
            {
                o->addMessage(L"kompas::off", 1);
                sSet->compass=false;
            }
            else
            {
                o->addMessage(L"kompas::on", 1);
                sSet->compass=true;
            }
        }


    }

    if ( receiver->getEventCallerByID() == GUI_ID_SOUNDS && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        if(sSet->sounds)
        {
            o->addMessage(L"--==wylaczono dzwieki==--", 0);
            sSet->sounds=false;
            o->changeSoundsAvailability(false);
        }
        else
        {
            o->addMessage(L"--==wlaczono dzwieki==--", 0);
            sSet->sounds=true;
            o->changeSoundsAvailability(true);
        }

    }

    if(sSet->sounds)
    {

        if ( receiver->getEventCallerByID() == GUI_ID_STRACKS && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );

            if(sSet->soundTracks==true)
            {
                o->addMessage(L"sound tracks::off", 1);
                sSet->soundTracks=false;
            }
            else
            {
                o->addMessage(L"sound tracks::on", 1);
                sSet->soundTracks=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_SEFFECTS && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->soundEffects==true)
            {
                o->addMessage(L"sound effects::off", 1);
                sSet->soundEffects=false;
            }
            else
            {
                o->addMessage(L"sound effects::on", 1);
                sSet->soundEffects=true;
            }
        }

        if ( receiver->getEventCallerByID() == GUI_ID_S3D && receiver->getEventCallerByElement ( EGET_ELEMENT_FOCUSED ) )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            if(sSet->sound3D==true)
            {
                o->addMessage(L"sound 3D::off", 1);
                sSet->sound3D=false;
            }
            else
            {
                o->addMessage(L"sound 3D::on", 1);
                sSet->sound3D=true;
            }
        }
    }

    //obsluga menu w pasku
    if ( receiver->isKeyPressed ( KEY_KEY_1 ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        setStage ( E_MENU );
    }

    if ( receiver->isKeyPressed ( KEY_KEY_2 ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        setStage ( E_SCENE1 );
    }

    if ( receiver->isKeyPressed ( KEY_KEY_3 ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        setStage ( E_SCENE2 );
    }
}

void Game::keyFuncScene ( CIrrEventReceiver* receiver, f32 rate)
{
    if(a->h->life>0)
    {

        if(o->oHero&&o->oGun)keyFuncHero ( receiver , rate);

    }

    if ( receiver->isKeyPressed ( KEY_KEY_T ) && !receiver->isKeyPressed ( KEY_RETURN ))
    {
        if(!e->isTesting)
        {
            e->performanceReport(level, sSet, device, driver, gui, rate);
            e->isTesting=true;
        }
    }

    if ( receiver->isKeyPressed ( KEY_RETURN ))
    {
        if(e->isTesting)
        {
            e->isTesting=false;
        }
    }

    if ( receiver->isKeyPressed ( KEY_F9 ) )
    {
        stringw str = L"screenshots/screenshot";
        str += device->getTimer()->getRealTime () ;
        str += ".jpg";
        IImage* i= driver->createScreenShot ();
        driver->writeImageToFile (i, str.c_str(), 0);
    }


    if ( receiver->isKeyPressed ( KEY_KEY_C ) )
    {
        if(sSet->typeOfCamera==E_TPP)
        {
            sSet->typeOfCamera=E_FPP;
            sSet->rateSniper=0.2;
            sSet->rateSniperY=0.1;
            o->sGui.tCameraFPP->setOverrideColor(SColor(250,200,200,0));
            o->sGui.tCameraTPP->setOverrideColor(SColor(80,200,200,0));
        }
        else
        {
            sSet->typeOfCamera=E_TPP;
            sSet->rateSniper=1;
            o->sGui.tCameraTPP->setOverrideColor(SColor(250,200,200,0));
            o->sGui.tCameraFPP->setOverrideColor(SColor(80,200,200,0));
        }
    }

    if ( receiver->isKeyPressed ( KEY_KEY_M ) )
    {
        showMissionMessage(level);
    }

    if ( receiver->isKeyPressed ( KEY_KEY_N ) )
    {
        device->getGUIEnvironment()->addMessageBox(
            L"STEROWANIE", L"WSAD - poruszanie bohatera. \n\
		SPACJA - podskok: \n\
		MYSZKA - dodatkowe poruszanie kamera i bohaterem\n\
        LEWY KLAWISZ MYSZKI - strzal \n\
		");
    }

    if ( receiver->isKeyPressed ( KEY_KEY_P ) )
    {
        if ( o->eTypeOfGameMenu == GUI_GAME )
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            o->loadGameGUI ( gui, driver, device, GUI_GAME_PAUSE, sSet );
        }
        else
        {
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
            o->loadGameGUI ( gui, driver, device, GUI_GAME, sSet );
        }

    }

    if ( receiver->getEventCallerByID() == GUI_ID_BB_RETURN_TO_GAME && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        o->loadGameGUI ( gui, driver, device, GUI_GAME, sSet );
        if(level==E_SCENE2&&sSet->compass)e->createCompass(driver, gui, sSet);
    }

    if ( receiver->getEventCallerByID() == GUI_ID_BB_RETURN_TO_MAIN && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        setStage ( E_MENU );
    }

    if ( receiver->getEventCallerByID() == GUI_ID_BB_RETURN_TO_SYSTEM && receiver->getEventCallerByElement ( EGET_BUTTON_CLICKED ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        stop = true;
        changeSettings = true;
    }

    if ( receiver->isKeyPressed ( KEY_ESCAPE ) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        stop = true;
        changeSettings = true;
    }

}

void Game::showFps ( IrrlichtDevice* device, IVideoDriver* driver)
{
    s32 fps = driver->getFPS();
    if ( lastFPS != fps )
    {
        lastFPS = fps;
        stringw str = L"Irrlicht Engine [";
        str += driver->getName();
        str += "] FPS:";
        str += fps;
        device->setWindowCaption ( str.c_str() );
    }
}

void Game::reload()
{
    changeSettings = false;
//if (engineSnd)engineSnd->drop();

    if ( device )
    {
        device->closeDevice();
        device->run(); // Very important to do this here!
        device->drop();
    }

//if(gui)
//{
//    gui->clear();
//    gui->drop();
//}
//if(driver)
//{
//    driver->drop();
//}

///**stworzenie glownego urzadzenia Irrlicht-a*/
    device = createDevice ( sSet->driverType, sSet->resolution, sSet->bits, sSet->fullscreen, sSet->stencilBuffer, sSet->vsync, 0 );

    if ( device )
    {
        printf ( "..device started successfully..\n" );
        device->setResizable ( true );
        device->setEventReceiver ( &receiver );
        device->setWindowCaption ( L"--==Irrlicht==--" );
    }

    else
    {
        printf ( "!!could not run main device of Irrlicht!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie sterownikow*/
    driver = device->getVideoDriver();

    if ( driver )
    {
        printf ( "..driver started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run driver!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie GUI*/
    gui = device->getGUIEnvironment();

    if ( gui )
    {
        printf ( "..gui started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run gui!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie menedzera scen Irrlichta*/
    smgr = device->getSceneManager();

    if ( smgr )
    {
        printf ( "..scene manager started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run scene manager!!\n" );
        exit ( 1 );
    }

    camera = smgr->addCameraSceneNodeFPS ( 0, 50.f, 0.1f );

    camera->setFarValue ( 10000.0f );

    /**zainicjalizowanie menedzera kolizji Irrlichta*/
    collMan = smgr->getSceneCollisionManager();

    if ( collMan )
    {
        printf ( "..collision manager started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run collision manager!!\n" );
        exit ( 1 );
    }

    /**zainicjalizowanie timera Irrlichta*/
    timer = device->getTimer();

    if ( timer )
    {
        printf ( "..timer started successfully..\n" );
    }

    else
    {
        printf ( "!!could not run timer!!\n" );
        exit ( 1 );
    }

    run();
}

void Game::keyFuncIntro ( CIrrEventReceiver* receiver)
{
    if (  receiver->isKeyPressed ( KEY_ESCAPE) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        setStage(E_MENU);
    }
}

void Game::keyFuncOutro ( CIrrEventReceiver* receiver)
{
    if (  receiver->isKeyPressed ( KEY_ESCAPE) )
    {
        if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceClick );
        stop = true;
        changeSettings = true;
    }
}

void Game::keyFuncHero ( CIrrEventReceiver* receiver , f32 rate)
{
    if(level==E_SCENE1)
    {
        if(o->oHero->getPosition().X>1060)o->oHero->setPosition(vector3df(1060, o->oHero->getPosition().Y, o->oHero->getPosition().Z));
        if(o->oHero->getPosition().X<-887)o->oHero->setPosition(vector3df(-887, o->oHero->getPosition().Y, o->oHero->getPosition().Z));
        if(o->oHero->getPosition().Z>1050)o->oHero->setPosition(vector3df(o->oHero->getPosition().X, o->oHero->getPosition().Y, 1050));
        if(o->oHero->getPosition().Z<-1070)o->oHero->setPosition(vector3df(o->oHero->getPosition().X, o->oHero->getPosition().Y, -1070));
    }
    else if (level==E_SCENE2)
    {

    }
    driver->getFPS();
///klawiatura
    if (  receiver->isKeyPressed ( KEY_SPACE) )
    {
        if(!a->h->isJump)a->h->isJump=true;
    }
//if wykonywany do strzalu gdy zaden klawisz wsadu nie jest wcisniety, wykonywane ciagle
    if ( ( !receiver->isKeyDown ( KEY_KEY_W ) ) && ( !receiver->isKeyDown ( KEY_KEY_S ) ) && ( !receiver->isKeyDown ( KEY_KEY_A ) ) && ( !receiver->isKeyDown ( KEY_KEY_D ) ) )
    {
        if(!a->h->isJump&&!a->h->isAttacking&&(!receiver->isMouseButtonPressed ( MBLEFT )))
        {
            o->oHero->setMD2Animation(EMAT_STAND);//oby nic sie
            o->oGun->setMD2Animation(EMAT_STAND);//przez to nie dzialo..
        }
///myszka(gdy nie poruszamy postacia to mozemy strzelac)
        if ( ( receiver->isMouseButtonPressed ( MBLEFT ) ) && ( a->h->anglePlus == 0 ) && ( o->eTypeOfGameMenu == GUI_GAME ) )
        {
            ETypeOfObject t;
            t=o->shoot ( selectedSceneNode, o->oHero, o->oEnemy, engineSnd );

            if(selectedSceneNode)
            {
                if(selectedSceneNode->getID()==OBJECT_ID_ENEMY)
                {
                    engineSnd->play2D ( s->sSourceWolfDeath);
                    a->y->life-=0.1;
//          engineSnd->play3D(s->sSourceWolfDeath, o->oEnemy->getAbsolutePosition());
                }
            }
            if(sSet->sounds&&sSet->soundEffects)s->playShoot();
            printf ( "x: %f y: %f z: %f\n", o->oHero->getPosition().X, o->oHero->getPosition().Y, o->oHero->getPosition().Z );
            o->oHero->setMD2Animation ( EMAT_ATTACK );
            o->oGun->setMD2Animation ( EMAT_ATTACK );
            e->createExplosion ( smgr, o->oHero->getPosition() + vector3df ( 0, 12, 0 ) + a->h->facingGun*35, 10, 10, 100, 100 );
            e->createBlood ( smgr, intersection, 15, 15, 300, 100 );
        }
    }

//if wykonywany gdy zaden klawisz wsadu nie jest wcisniety, wykonywane ciagle
    if ( ( !receiver->isKeyDown ( KEY_KEY_W ) ) && ( !receiver->isKeyDown ( KEY_KEY_S ) ) && ( !receiver->isKeyDown ( KEY_KEY_A ) ) && ( !receiver->isKeyDown ( KEY_KEY_D ) ) )
    {
//if wykonywany jednorazowo gdy puszczamy klawisze wsadu
        if ( ( receiver->isKeyReleased ( KEY_KEY_W ) ) || ( receiver->isKeyReleased ( KEY_KEY_S ) ) || ( receiver->isKeyReleased ( KEY_KEY_A ) ) || ( receiver->isKeyReleased ( KEY_KEY_D ) ) )
        {
//printf("nic\n");
            if(sSet->sounds&&sSet->soundEffects)engineSnd->play2D ( s->sSourceReload );
            a->h->anglePlus = 0;
            a->h->bCrosshair->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bCrosshair.bmp" ) );
            a->h->bCrosshair->setSize ( a->h->crosshairSize );
//standy musza byc w Up-ach bo sa problemy z przelaczaniem animacji
            o->oHero->setMD2Animation ( EMAT_STAND );
            o->oGun->setMD2Animation ( EMAT_STAND );
        }
    }

//pojedyncze uruchomienie ruchu

    if ( receiver->isKeyPressed ( KEY_KEY_W ) )
    {
//printf("jedno 'w'\n");
        a->h->bCrosshair->setSize ( a->h->crosshairSizeBig );
        a->h->bCrosshair->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bCrosshairD.bmp" ) );
        o->oHero->setMD2Animation ( EMAT_RUN );
        o->oGun->setMD2Animation ( EMAT_RUN );
    }

    else if ( receiver->isKeyPressed ( KEY_KEY_S ) )
    {
//printf("jedno 's'\n");
        a->h->bCrosshair->setSize ( a->h->crosshairSizeBig );
        a->h->bCrosshair->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bCrosshairD.bmp" ) );
        o->oHero->setMD2Animation ( EMAT_RUN );
        o->oGun->setMD2Animation ( EMAT_RUN );
    }

    else if ( receiver->isKeyPressed ( KEY_KEY_A ) )
    {
//printf("jedno 'a'\n");
        a->h->bCrosshair->setSize ( a->h->crosshairSizeBig );
        a->h->bCrosshair->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bCrosshairD.bmp" ) );
        o->oHero->setMD2Animation ( EMAT_RUN );
        o->oGun->setMD2Animation ( EMAT_RUN );
    }

    else if ( receiver->isKeyPressed ( KEY_KEY_D ) )
    {
//printf("jedno 'd'\n");
        a->h->bCrosshair->setSize ( a->h->crosshairSizeBig );
        a->h->bCrosshair->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bCrosshairD.bmp" ) );
        o->oHero->setMD2Animation ( EMAT_RUN );
        o->oGun->setMD2Animation ( EMAT_RUN );
    }

//skok?

    if ( receiver->isKeyPressed ( KEY_SPACE ) )
    {

    }

//poruszanie bohatera 'wsad'-em
    if ( receiver->isKeyDown ( KEY_KEY_W ) )
    {
        if(sSet->sounds&&sSet->soundEffects)
        {
            if(level==E_SCENE1)
            {
                if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep ) )
                {
                    engineSnd->play2D ( s->sSourceStep );
                }
            }
            else if(level==E_SCENE2)
            {
                if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep2 ) )
                {
                    engineSnd->play2D ( s->sSourceStep2 );
                }
            }
        }

//printf("'w'\n");
        a->h->anglePlus = 0;

        if ( receiver->isKeyDown ( KEY_KEY_W ) && ( receiver->isKeyDown ( KEY_KEY_A ) ) )
        {
//printf("'wa'\n");
            a->h->anglePlus = 45;
        }

        if ( receiver->isKeyDown ( KEY_KEY_W ) && ( receiver->isKeyDown ( KEY_KEY_D ) ) )
        {
//printf("'wd'\n");
            a->h->anglePlus = -45;
        }

        vector3df newPos = ( a->h->facing * a->h->MOVEMENT_SPEED * rate * sSet->RUN) + o->oHero->getPosition();

        o->oHero->setPosition ( newPos );
    }

    else if ( receiver->isKeyDown ( KEY_KEY_S ) )
    {
        if(sSet->sounds&&sSet->soundEffects)
        {
            if(sSet->sounds&&sSet->soundEffects)
            {
                if(level==E_SCENE1)
                {
                    if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep ) )
                    {
                        engineSnd->play2D ( s->sSourceStep );
                    }
                }
                else if(level==E_SCENE2)
                {
                    if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep2 ) )
                    {
                        engineSnd->play2D ( s->sSourceStep2 );
                    }
                }
            }
        }
//printf("'s'\n");
        a->h->anglePlus = 180;

        if ( receiver->isKeyDown ( KEY_KEY_S ) && ( receiver->isKeyDown ( KEY_KEY_A ) ) )
        {
//printf("'sa'\n");
            a->h->anglePlus = 135;
        }

        if ( receiver->isKeyDown ( KEY_KEY_S ) && ( receiver->isKeyDown ( KEY_KEY_D ) ) )
        {
//printf("'sd'\n");
            a->h->anglePlus = -135;
        }

        core::vector3df newPos = ( a->h->facing * a->h->MOVEMENT_SPEED * rate * sSet->RUN  ) + o->oHero->getPosition();

        o->oHero->setPosition ( newPos );
    }

    else if ( receiver->isKeyDown ( KEY_KEY_A ) )
    {
        if(sSet->sounds&&sSet->soundEffects)
        {
            if(sSet->sounds&&sSet->soundEffects)
            {
                if(level==E_SCENE1)
                {
                    if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep ) )
                    {
                        engineSnd->play2D ( s->sSourceStep );
                    }
                }
                else if(level==E_SCENE2)
                {
                    if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep2 ) )
                    {
                        engineSnd->play2D ( s->sSourceStep2 );
                    }
                }
            }
        }
//printf("'a'\n");
        a->h->anglePlus = 90;

        vector3df newPos = ( a->h->facing * a->h->MOVEMENT_SPEED * rate * sSet->RUN ) + o->oHero->getPosition();

        o->oHero->setPosition ( newPos );
    }

    else if ( receiver->isKeyDown ( KEY_KEY_D ) )
    {
        if(sSet->sounds&&sSet->soundEffects)
        {
            if(sSet->sounds&&sSet->soundEffects)
            {
                if(level==E_SCENE1)
                {
                    if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep ) )
                    {
                        engineSnd->play2D ( s->sSourceStep );
                    }
                }
                else if(level==E_SCENE2)
                {
                    if ( !engineSnd->isCurrentlyPlaying ( s->sSourceStep2 ) )
                    {
                        engineSnd->play2D ( s->sSourceStep2 );
                    }
                }
            }
        }
        //printf("'d'\n");
        a->h->anglePlus = -90;

        vector3df newPos = ( a->h->facing * a->h->MOVEMENT_SPEED * rate *sSet->RUN ) + o->oHero->getPosition();

        o->oHero->setPosition ( newPos );
    }

    //przyspieszenie
    if ( receiver->isKeyDown ( KEY_LSHIFT ) )
    {
        sSet->RUN = 1.7f;
    }

    else
    {
        sSet->RUN = 1.f;
    }

}

void Game::specialPreference()
{
    if(selectedSceneNode)
    {

        if((selectedSceneNode->getID() & OBJECT_ID_ENEMY)==OBJECT_ID_ENEMY) //jest tylko wrogiem
        {
            o->oEnemy->setMaterialFlag(video::EMF_LIGHTING, false);
        }
    }
    else
    {
        o->oEnemy->setMaterialFlag(EMF_LIGHTING, true);
    }
}

void Game::showAboutText()
{
    IXMLReader* xml = device->getFileSystem()->createXMLReader( L"save/config.xml");
    stringw MessageText;
    stringw Caption;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
        case io::EXN_TEXT:
            MessageText = xml->getNodeData();
            break;
        case io::EXN_ELEMENT:
        {
            if (core::stringw("messageText") == xml->getNodeName())
                Caption = xml->getAttributeValue(L"caption");
        }
        break;
        default:
            break;
        }
    }

    if (xml)
        xml->drop();

    device->getGUIEnvironment()->addMessageBox(
        Caption.c_str(), MessageText.c_str());
}

void Game::joysticks(Settings* sSet, IGUIEnvironment* gui)
{
    if(device->activateJoysticks(sSet->joystickInfo))
    {
        std::cout << "Joystick support is enabled and " << sSet->joystickInfo.size() << " joystick(s) are present." << std::endl;

        for(u32 joystick = 0; joystick < sSet->joystickInfo.size(); ++joystick)
        {
            std::cout << "Joystick " << joystick << ":" << std::endl;
            std::cout << "\tName: '" << sSet->joystickInfo[joystick].Name.c_str() << "'" << std::endl;
            std::cout << "\tAxes: " << sSet->joystickInfo[joystick].Axes << std::endl;
            std::cout << "\tButtons: " << sSet->joystickInfo[joystick].Buttons << std::endl;

            std::cout << "\tHat is: ";

            switch(sSet->joystickInfo[joystick].PovHat)
            {
            case SJoystickInfo::POV_HAT_PRESENT:
                std::cout << "present" << std::endl;
                break;

            case SJoystickInfo::POV_HAT_ABSENT:
                std::cout << "absent" << std::endl;
                break;

            case SJoystickInfo::POV_HAT_UNKNOWN:
            default:
                std::cout << "unknown" << std::endl;
                break;
            }
        }
    }
    else
    {
        std::cout << "Joystick support is not enabled." << std::endl;
    }
    if(sSet->joystickInfo.size()>0)
    {
        isJoystick=true;
        IGUIStaticText *t=gui->addStaticText ( L"--==wykryto joystick -->> masz mozliwosc poruszania wilkiem : )==--", rect<s32 > ( position2di ( 20, 465 ), dimension2di ( 400, 20 ) ), false, false, 0 );
        t->setOverrideColor( SColor ( 250, 220, 20, 20 ) );

        gui->addImage ( driver->getTexture ( "textures/gui/joystick-icon.png" ), core::position2d<s32 > ( 20, 400 ), true, 0, -1, L"iGuiBackgroundRight" );


    }
    core::stringw tmp = L"Irrlicht Joystick Example (";
    tmp += sSet->joystickInfo.size();
    tmp += " joysticks)";
    device->setWindowCaption(tmp.c_str());
}

void Game::joysticksControlling(CIrrEventReceiver* receiver, f32 rate, IAnimatedMeshSceneNode* node)
{
    bool movedWithJoystick = false;
    //if(sSet->joystickInfo.size() > 0)
    if(isJoystick)
    {
        f32 moveHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
        f32 moveVertical = 0.f; // -1.f for full down to +1.f for full up.

        const SEvent::SJoystickEvent & joystickData = receiver->GetJoystickState();

        // We receive the full analog range of the axes, and so have to implement our
        // own dead zone.  This is an empirical value, since some joysticks have more
        // jitter or creep around the center point than others.  We'll use 5% of the
        // range as the dead zone, but generally you would want to give the user the
        // option to change this.
        const f32 DEAD_ZONE = 0.05f;

        moveHorizontal =
            (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
        if(fabs(moveHorizontal) < DEAD_ZONE)
            moveHorizontal = 0.f;

        moveVertical =
            (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
        if(fabs(moveVertical) < DEAD_ZONE)
            moveVertical = 0.f;

        // POV hat info is only currently supported on Windows, but the value is
        // guaranteed to be 65535 if it's not supported, so we can check its range.
        const u16 povDegrees = joystickData.POV / 100;
        if(povDegrees < 360)
        {
            if(povDegrees > 0 && povDegrees < 180)
                moveHorizontal = 1.f;
            else if(povDegrees > 180)
                moveHorizontal = -1.f;

            if(povDegrees > 90 && povDegrees < 270)
                moveVertical = -1.f;
            else if(povDegrees > 270 || povDegrees < 90)
                moveVertical = +1.f;
        }

        if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f))
        {
            node->setPosition(node->getPosition()+vector3df(rate*moveHorizontal,0,rate * moveVertical));
            movedWithJoystick = true;
        }
    }
}

bool Game::set(bool ask)
{
    //sSet->driverType=EDT_OPENGL;
//sSet->resolution=dimension2d<u32>(800 ,600);
    sSet->resolution=dimension2d<u32>(1280 ,768);
    sSet->bits=16;
//sSet->fullscreen=false;
    sSet->fullscreen=true;
    std::cout<<"uruchomiono z domyslnymi parametrami"<<endl;
    std::cout<<"sterownik:OPENGL,  rozdzielczosc:800x600, 16bit,  tryb okienkowy"<<endl<<endl;

    if(!ask)
    {
        sSet->driverType=EDT_OPENGL;
//sSet->resolution=dimension2d<u32>(800 ,600);
        sSet->resolution=dimension2d<u32>(1280 ,768);
        sSet->bits=16;
//sSet->fullscreen=false;
        sSet->fullscreen=true;
        std::cout<<"uruchomiono z domyslnymi parametrami"<<endl;
        std::cout<<"sterownik:OPENGL,  rozdzielczosc:800x600, 16bit,  tryb okienkowy"<<endl<<endl;
    }
    else
    {

        printf("Wybierz sterownik dla tej gry:\n"\
               " (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"\
               " (d) Burning's Software Renderer\n (e) Software Renderer\n"\
               " (f) NullDevice\n (inny klawisz) wyjscie\n\n");

        char a1;
        std::cout<<">>";
        std::cin >> a1;
        std::cout<<endl<<endl;

        switch(a1)
        {
        case 'a':
            sSet->driverType = EDT_OPENGL;
            break;
        case 'b':
            sSet->driverType = EDT_DIRECT3D9;
            break;
        case 'c':
            sSet->driverType = EDT_DIRECT3D8;
            break;
        case 'd':
            sSet->driverType = EDT_BURNINGSVIDEO;
            break;
        case 'e':
            sSet->driverType = EDT_SOFTWARE;
            break;
        case 'f':
            sSet->driverType = EDT_NULL;
            break;
        default:
            return false;
        }

printf("Wybierz preferowana rozdzielczosc:\n"\
		" (a) 800 x 600\n"\
		" (b) 1280 x 768\n"\
        "(inny klawisz) wyjscie\n\n");

	char a2;
	std::cout<<">>";
	std::cin >> a2;
	std::cout<<endl<<endl;

	switch(a2)
	{
		case 'a': sSet->resolution = dimension2d<u32 >(800,600); break;
		case 'b': sSet->resolution = dimension2d<u32 >(1280,768);break;
		default: return false;
	}

printf("Ilosc bitow:\n"\
		" (a) 16\n"\
		" (b) 32\n"\
        "(inny klawisz) wyjscie\n\n");

        	char a3;
	std::cout<<">>";
	std::cin >> a3;
	std::cout<<endl<<endl;

	switch(a3)
	{
		case 'a': sSet->bits = 16; break;
		case 'b': sSet->bits = 32; break;
		default: return false;
	}

	printf("Oraz ostatnia rzecz:\n"\
		" (a) tryb okienkowy\n"\
		" (b) pelny ekran\n"\
        "(inny klawisz) wyjscie\n\n");

        	char a4;
	std::cout<<">>";
	std::cin >> a4;
	std::cout<<endl<<endl;

	switch(a4)
	{
		case 'a': sSet->fullscreen = false; break;
		case 'b': sSet->fullscreen = true; break;
		default: return false;
	}
    }

    return true;
}

void Game::showStartMessage(ELevels level, Settings* sSet)
{
    switch(level)
    {
    case E_MENU:
        switch(sSet->werewolfDead)
        {
        case true:
            break;
        case false:
device->getGUIEnvironment()->addMessageBox(
		L"BLER UICZ. v 1.0 TRIAL 30 DNI, SZCZEGOLY NA www.empick.com", L"WYKONAJ PIERWSZA MISJE I ZWIAZANE Z NIA EFEKTY!\n\
		NASTEPNIE WEJDZ DO ZAGINIONEJ DOLINY, GDZIE ZNAJDZIESZ POZOSTALE WSPANIALOSCI, \n\
		JAKIE KRYJE JASNIE KROL IRRLICHT III WSPANIALY\n\
		");
            break;
        default:
            break;
        }
        break;
    case E_SCENE1:
        break;
    case E_SCENE2:
        break;
    default:
        break;
    }

}

void Game::showMissionMessage(ELevels level)
{
    switch(level)
    {
    case E_SCENE1:
        device->getGUIEnvironment()->addMessageBox(
            L"CEL GRY", L"Jedyna misja jest zabic wilkolaka albo zginac ;) \n\
		Poza tym przetestuj dostepne efekty w tej scenie\n\n\
		DOSTEPNE EFEKTY:\n\
		SWIATLO (ROZPROSZONE, PUNKTOWE, GRADIENT), MGLA, ROZMYTY EKRAN, LUSTRO CIEN, OGIEN, PRZEZROCZYSTOSC\n\n\
		Gra NIE POSIADA: \n\
		>>rozbudowanej akcji, sztucznej inteligencji \n\
		 >>fizyki \n\
        Jest to tylko demonstracja mozliwosci silnika graficznego \n\
        Oraz pokazanie, jak zaimplementowac taki silnik we wlasnym projekcie\
        W wizualizacji pomaga silnik muzyczny IrrKlang \n\
		");
        break;
    case E_SCENE2:
        device->getGUIEnvironment()->addMessageBox(
            L"CEL GRY", L"Jedyna misja jest przetestowac pozostale efekty \n\
		DOSTEPNE EFEKTY:\n\
		SHADER TOON, SNIEG, STADO WILKOW, MAPOWANIE SFERYCZNE, WIELKI LAS, SZCZEGOLOWY TEREN, SKYBOX, KOMPAS \n\
		");
        break;
    default:
        break;
    }
}
