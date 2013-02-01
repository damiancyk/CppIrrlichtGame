
#include "effect.h"

Effect::Effect (IrrlichtDevice* device)
{
    test.test=false;
    test.mirror=false;
    test.snow=false;
    test.fog=false;
    test.fire=false;
    test.shaderToon=false;
    isTesting=false;
    loadTextures(device->getSceneManager());
}

void Effect::loadTextures ( ISceneManager* smgr )
{
    c8 tmp[64];
    for ( s32 i = 1; i <= 10; ++i )
    {
        sprintf ( tmp, "textures/effects/explosion/%02d.jpg", i );
        explosionTextures.push_back ( smgr->getVideoDriver()->getTexture ( tmp ) );
    }

    for ( s32 i = 1; i <= 9; ++i )
    {
        sprintf ( tmp, "textures/effects/blood/%1d.jpg", i );
        bloodTextures.push_back ( smgr->getVideoDriver()->getTexture ( tmp ) );
    }
}

void Effect::loadEffects(Settings* sSet, ELevels level, IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, ICameraSceneNode* camera, IAnimatedMeshSceneNode* node)
{

    if ( sSet->effects )
    {

        switch(level)
        {
        case E_MENU:
            break;
        case E_SCENE1:
            if(sSet->mirror)
                createMirror ( device, driver, smgr, camera );
            if ( sSet->fog )
            {
                driver->setFog ( SColor ( 50, 250, 250, 250 ), EFT_FOG_EXP /*EFT_FOG_EXP EFT_FOG_LINEAR EFT_FOG_EXP2*/ , 0, 3000, 0.003f, false, false );
            }
            changeFog ( smgr, sSet->fog );
            if ( sSet->fire )createFire ( smgr, driver, vector3df ( 360, 0, 380 ) );
            if(sSet->lightAmbient||sSet->lightMap||sSet->lightPoint)changeLight(smgr, sSet->lightAmbient);
            break;
        case E_SCENE2:
            if (sSet->snow)createSnow ( smgr, driver );
            if (sSet->shaderToon)node->setMaterialType ( ( E_MATERIAL_TYPE ) mtlToonShader );
            break;
        default:
            break;
        }
    }
}

void Effect::updateEffects(Settings* sSet, ELevels level, IVideoDriver* driver, ISceneManager* smgr, ICameraSceneNode* camera)
{
    switch(level)
    {
    case E_INTRO:
        break;
    case E_MENU:
        break;
    case E_SCENE1:
        if ( sSet->effects )
        {

            if(sSet->mirror && mirror )
            {
                mirror->reflect();
            }
        }
        break;
    case E_SCENE2:

        break;
    default:
        break;
    }
}

void Effect::createAnaglyph ( ICameraSceneNode* camera, IVideoDriver* driver, ISceneManager* smgr )
{
    vector3df oldPosition = camera->getPosition();
    vector3df oldTarget = camera->getTarget();
    matrix4 startMatrix = camera->getAbsoluteTransformation();
    vector3df focusPoint = ( camera->getTarget() -
                             camera->getAbsolutePosition() ).setLength ( 1 ) +
                           camera->getAbsolutePosition() ;

    //lewe oko
    vector3df leftEye;
    matrix4 leftMove;
    leftMove.setTranslation ( vector3df ( -0.04f, 0.0f, 0.0f ) );
    leftEye = ( startMatrix * leftMove ).getTranslation();

    driver->beginScene ( true, true, SColor ( 0, 200, 200, 255 ) );

    driver->getOverrideMaterial().Material.ColorMask = ECP_RED;
    driver->getOverrideMaterial().EnableFlags = EMF_COLOR_MASK;
    driver->getOverrideMaterial().EnablePasses = ESNRP_SKY_BOX + ESNRP_SOLID + ESNRP_TRANSPARENT + ESNRP_TRANSPARENT_EFFECT + ESNRP_SHADOW;

    camera->setPosition ( leftEye );
    camera->setTarget ( focusPoint );

    smgr->drawAll();

    //prawe oko
    vector3df rightEye;
    matrix4 rightMove;
    rightMove.setTranslation ( vector3df ( 0.01f, 0.0f, 0.0f ) );
    rightEye = ( startMatrix * rightMove ).getTranslation();

    driver->clearZBuffer();

    driver->getOverrideMaterial().Material.ColorMask = ECP_GREEN + ECP_BLUE;
    driver->getOverrideMaterial().EnableFlags = EMF_COLOR_MASK;
    driver->getOverrideMaterial().EnablePasses = ESNRP_SKY_BOX + ESNRP_SOLID + ESNRP_TRANSPARENT + ESNRP_TRANSPARENT_EFFECT + ESNRP_SHADOW;

    camera->setPosition ( rightEye );
    camera->setTarget ( focusPoint );

    smgr->drawAll();

    driver->endScene();
    driver->getOverrideMaterial().Material.ColorMask = ECP_ALL;
    driver->getOverrideMaterial().EnableFlags = 0;
    driver->getOverrideMaterial().EnablePasses = 0;

    camera->setPosition ( oldPosition );
    camera->setTarget ( oldTarget );

}

void Effect::createSnow ( ISceneManager* smgr, IVideoDriver* driver )
{
    scene::IParticleSystemSceneNode* ps = 0;
    ps = smgr->addParticleSystemSceneNode ( false );
    ps->setPosition ( core::vector3df ( -160, 170, 80 ) );
    ps->setScale ( core::vector3df ( 1, 1, 1 ) );

    scene::IParticleEmitter* em = ps->createBoxEmitter ( core::aabbox3d<f32> ( -2000, -270, -2000, 3000, 800, 3000 ),
                                  core::vector3df ( 0.00f, 0.00f, 0.0f ), 800, 800,
                                  video::SColor ( 0, 155, 155, 255 ), video::SColor ( 0, 255, 255, 255 ),
                                  8000, 10000 );

    ps->setEmitter ( em );
    em->drop();

    scene::IParticleAffector* paf = ps->createGravityAffector (
                                        vector3df ( 0.00f, -0.15f, 0.00f ), 1000 );

    ps->addAffector ( paf );
    paf->drop();

    ps->setMaterialFlag ( video::EMF_LIGHTING, false );
    ps->setMaterialTexture ( 0, driver->getTexture ( "textures/effects/snow.bmp" ) );
    ps->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    ps->setParticleSize ( core::dimension2d<f32> ( 5.f, 5.f ) );
}

void Effect::changeFog ( ISceneManager* smgr, bool change )
{
    //przeglad wszystkich obiektow i utworzenie dla nich mgly
    list<ISceneNode*>::ConstIterator it =
        smgr->getRootSceneNode()->getChildren().begin();

    for ( ; it != smgr->getRootSceneNode()->getChildren().end(); ++it )
    {
        if(((*it)->getID()!=OBJECT_ID_TREE)
                &&((*it)->getID()!=OBJECT_ID_CROSSHAIR)
                &&((*it)->getID()!=OBJECT_ID_LIGHT_POINT))
        {
            ( *it )->setMaterialFlag ( EMF_FOG_ENABLE, change ); //obiekty
        }
        list<ISceneNode*>::ConstIterator it2 = ( *it )->getChildren().begin();

        for ( ; it2 != ( *it )->getChildren().end(); ++it2 )
        {
            if(((*it2)->getID()!=OBJECT_ID_TREE)
                    &&((*it2)->getID()!=OBJECT_ID_CROSSHAIR)
                    &&((*it)->getID()!=OBJECT_ID_LIGHT_POINT))
            {
                ( *it2 )->setMaterialFlag ( EMF_FOG_ENABLE, change ); //obiekty
            }
            list<ISceneNode*>::ConstIterator it3 = ( *it2 )->getChildren().begin();
            for ( ; it3 != ( *it2 )->getChildren().end(); ++it3 )
            {
                if(((*it3)->getID()!=OBJECT_ID_TREE)
                        &&((*it3)->getID()!=OBJECT_ID_CROSSHAIR)
                        &&((*it)->getID()!=OBJECT_ID_LIGHT_POINT))
                {
                    ( *it3 )->setMaterialFlag ( EMF_FOG_ENABLE, change ); //obiekty
                }
            }
        }
    }
}

void Effect::changeLight( ISceneManager* smgr, bool change )
{
    //przeglad wszystkich obiektow i utworzenie dla nich mgly
    list<ISceneNode*>::ConstIterator it =
        smgr->getRootSceneNode()->getChildren().begin();

    for ( ; it != smgr->getRootSceneNode()->getChildren().end(); ++it )
    {
        (*it)->setMaterialFlag(video::EMF_LIGHTING, true);
        list<ISceneNode*>::ConstIterator it2 = ( *it )->getChildren().begin();

        for ( ; it2 != ( *it )->getChildren().end(); ++it2 )
        {
            (*it2)->setMaterialFlag(video::EMF_LIGHTING, true);

            list<ISceneNode*>::ConstIterator it3 = ( *it2 )->getChildren().begin();
            for ( ; it3 != ( *it2 )->getChildren().end(); ++it3 )
            {
                (*it3)->setMaterialFlag(video::EMF_LIGHTING, true);
            }
        }
    }
}

void Effect::createFire ( ISceneManager *smgr, IVideoDriver *driver, vector3df p )
{
    // stworzenie systemu cz¹steczek
    IParticleSystemSceneNode* ps =
        smgr->addParticleSystemSceneNode ( false );

    IParticleEmitter* em = ps->createBoxEmitter (
                               aabbox3d<f32 > ( -7, 0, -7, 7, 1, 7 ), // rozmiar emitera
                               vector3df ( 0.0f, 0.03f, 0.0f ), // kierunek
                               80, 100, // min i max ilosc czasteczek w tym samym czasie
                               SColor ( 0, 255, 255, 255 ), // minimalny kolor
                               SColor ( 0, 255, 255, 255 ), // maksymalny kolor
                               800, 2000, 0, // czas zycia min i max, maksymalny kat
                               dimension2df ( 3.f, 3.f ), // minimalny rozmiar
                               dimension2df ( 16.f, 16.f ) ); // maksymalny rozmiar

    ps->setEmitter ( em ); // system czasteczek ustawia emiter
    em->drop(); // usuniecie emitera

    IParticleAffector* paf = ps->createFadeOutParticleAffector();
    ps->addAffector ( paf ); //dodajemy zanikanie do systemu czasteczek
    paf->drop();

    ps->setPosition ( vector3df ( p ) );
    ps->setScale ( vector3df ( 2, 2, 2 ) );

    ps->setMaterialFlag ( EMF_LIGHTING, false ); //wylaczenie swiatla dla czasteczek
    ps->setMaterialFlag ( EMF_ZWRITE_ENABLE, false ); //wylaczenie Z - bufora
    ps->setMaterialTexture ( 0, driver->getTexture
                             ( "textures/effects/fire.bmp" ) ); //nalozenie tekstury
    ps->setMaterialType ( EMT_TRANSPARENT_VERTEX_ALPHA ); //ustawienie typu materialu
    ps->setID(OBJECT_ID_FIRE);
}

void Effect::createExplosion ( ISceneManager* smgr, vector3df position, s32 sizeX, s32 sizeY, s32 timeDel, s32 timeCh )
{
    scene::ISceneNodeAnimator* anim = NULL;
    // animator dla eksplozji
    anim = smgr->createTextureAnimator ( explosionTextures, timeCh, false );
    // billboard dla eksplozji
    scene::IBillboardSceneNode* bill = smgr->addBillboardSceneNode ( smgr->getRootSceneNode(), core::dimension2d<f32 > ( sizeX, sizeY ), position, -5 );
    bill->setMaterialFlag ( EMF_LIGHTING, false );
    bill->setMaterialType ( EMT_TRANSPARENT_ADD_COLOR );
    bill->addAnimator ( anim );
    anim->drop();
    // animator ktory usuwa billboarda
    anim = smgr->createDeleteAnimator ( timeDel );
    bill->addAnimator ( anim );
    anim->drop();
}

void Effect::createBlood ( ISceneManager* smgr, vector3df position, s32 sizeX, s32 sizeY, s32 timeDel, s32 timeCh )
{
    //animator dla krwi i wybuchu
    ISceneNodeAnimator* anim = NULL;
    anim = smgr->createTextureAnimator ( bloodTextures, timeCh, false );

    // billboard dla eksplozji
    IBillboardSceneNode* bill = smgr->addBillboardSceneNode ( smgr->getRootSceneNode(), core::dimension2d<f32 > ( sizeX, sizeY ), position, -5 );
    bill->setMaterialFlag ( EMF_LIGHTING, false );
    bill->setMaterialType ( EMT_TRANSPARENT_ADD_COLOR );
    bill->addAnimator ( anim );
    anim->drop();

    // animator ktory usuwa billboarda
    anim = smgr->createDeleteAnimator ( timeDel );
    bill->addAnimator ( anim );
    anim->drop();
}

void Effect::createLight ( ISceneManager* smgr, IVideoDriver* driver )
{
    // tworzymy swiatlo
    ISceneNode* node = 0;
    node = smgr->addLightSceneNode ( 0, vector3df ( 0, 0, 0 ),
                                     SColorf ( 1.0f, 0.6f, 0.7f, 1.0f ), 10.0f );
    ISceneNodeAnimator* anim = 0;
    anim = smgr->createFlyCircleAnimator ( vector3df ( 200, 150, 500 ), 250.0f );
    node->addAnimator ( anim );
    anim->drop();

    // dolaczamy billboard do swiatla
    node = smgr->addBillboardSceneNode ( node, dimension2d<f32 > ( 50, 50 ) );
    node->setMaterialFlag ( EMF_LIGHTING, false );
    node->setMaterialType ( EMT_TRANSPARENT_ADD_COLOR );
    node->setMaterialTexture ( 0, driver->getTexture ( "textures/particlewhite.bmp" ) );
}

void Effect::shaderToon ( IVideoDriver* driver )
{
    // --------------------------------------------------------------------------

// OpenGL Vertex Program 1.1
    static const stringc vertToonShader =
        "varying vec3 normal;"
        "void main()"
        "{"
        "   normal = gl_NormalMatrix * gl_Normal;"
        "   gl_Position = ftransform();"
        "}";

// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
    static const stringc fragToonShader =
        "varying vec3 normal;"
        "void main()"
        "{"
        "   float intensity;"
        "   vec4 color;"
        "   vec3 n = normalize(normal);"
        ""
        "   intensity = dot(vec3(gl_LightSource[0].position),n);"
        ""
        "   if (intensity > 0.95)"
        "      color = vec4(1.0,0.5,0.5,1.0);"
        "   else if (intensity > 0.5)"
        "      color = vec4(0.6,0.3,0.3,1.0);"
        "   else if (intensity > 0.25)"
        "      color = vec4(0.4,0.2,0.2,1.0);"
        "   else"
        "      color = vec4(0.2,0.1,0.1,1.0);"
        ""
        "   gl_FragColor = color;"
        "}";
// -------------------------------------------------------------------------

    // Create GLSL shaders
    video::IGPUProgrammingServices* gpu =
        driver->getGPUProgrammingServices();
    mtlToonShader = video::EMT_SOLID; // awaryjny typ materialu
    bool bCanDoGLSL_1_1 = false;

    if ( gpu )
    {
        bCanDoGLSL_1_1 = true; // provisionally accept

        if ( !driver->queryFeature ( EVDF_ARB_FRAGMENT_PROGRAM_1 ) )
        {
            printf ( "queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) failed\n" );
            bCanDoGLSL_1_1 = false;
        }

        if ( !driver->queryFeature ( EVDF_ARB_VERTEX_PROGRAM_1 ) )
        {
            printf ( "queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) failed\n" );
            bCanDoGLSL_1_1 = false;
        }
    }

    if ( bCanDoGLSL_1_1 )
    {
        // Shader uses built-in OpenGL GLSL uniforms, hence needs no explicit constants.
        // Therefore a callback function is not needed and is NULL.
        mtlToonShader = gpu->addHighLevelShaderMaterial (
                            vertToonShader.c_str(), "main", EVST_VS_1_1,
                            fragToonShader.c_str(), "main", EPST_PS_1_1,
                            NULL, video::EMT_SOLID );
    }

    else
    {
        // This demo is for OpenGL!
        printf ( "This demo requires OpenGL with GLSL High-Level shaders\n" );
        mtlToonShader = video::EMT_SOLID;
    }

}

void Effect::createMirror ( IrrlichtDevice* device, IVideoDriver* driver,
                            ISceneManager* smgr, ICameraSceneNode* camera )
{
    mirror = new cMirror ( smgr->getRootSceneNode(), smgr, 999,
                           device, camera,
                           driver->getTexture ( "textures/terrain/mirror.png" ) );
    mirror->setPosition ( vector3df ( 0, 30, 30.f ) );
    mirror->setRotation ( vector3df ( 0, 0, 0 ) );
    mirror->setScale ( vector3df ( 1, 2, 0 ) );
}

void Effect::createCompass(IVideoDriver* driver, IGUIEnvironment* gui, Settings* sSet)
{
    compass = new CGUICompass(rect<s32>(sSet->resolution.Width-128, 0, sSet->resolution.Width, 128), gui, gui->getRootGUIElement(), 30);
    compass->setTexture(driver->getTexture("textures/terrain/compass.jpg"));
    compass->setScale(1.f, 1.f);
    gui->addImage ( driver->getTexture ( "textures/game/compassOver.png" ), position2d<s32 > ( sSet->resolution.Width-128, 0 ), true, 0, -1, L"iGuiFrame" );

}

void Effect::updateCompass(IrrlichtDevice* device, ISceneManager* smgr, ISceneNode* terrain)
{
    ICameraSceneNode* camera = smgr->getActiveCamera();
    if (camera)
    {
        //ustawiamy kierunek kamery
        const vector3df camera_position = camera->getAbsolutePosition();
        const vector3df camera_target   = camera->getTarget();
        vector3df camera_direction = (camera_target - camera_position);
        camera_direction.Y = 0.f;

        static const f32 epsilon = .0001f;

        // sprawdzamy czy kamera nie jest ustawiona calkowicie w dol lub w gore
        if (epsilon < fabsf (camera_direction.X) && epsilon < fabsf (camera_direction.Z))
        {
            camera_direction.normalize();

            f32 angle = compass->rad_to_deg (acosf (camera_direction.Z));
            if (camera_direction.X < 0.f)
                angle *= -1.f;

            compass->setHeading (angle);
        }

        //wczytujemy dane z drugiego obiektu jakim jest teren
        aabbox3df bbox = terrain->getBoundingBox();
        terrain->getAbsoluteTransformation().transformBoxEx(bbox);

        const vector3df extents = bbox.getExtent();
        const vector2df camera_offset(camera_position.X / extents.X, camera_position.Z / extents.Z);

        //ustawiamy odpowiednio kompas
        compass->setTranslation(camera_offset.X, camera_offset.Y);
    }
}

void Effect::performanceReport(ELevels level, Settings* sSet, IrrlichtDevice* device, IVideoDriver* driver, IGUIEnvironment* gui, f32 frameDeltaTime)
{
    SColor cYellow=SColor(200,200,200,0);
    SColor cGreen=SColor(200,80,200,80);
    SColor cOrange=SColor(200,200,200,80);
    SColor cBlack=SColor(250,0,0,0);

    gui->getSkin()->setColor ( EGDC_BUTTON_TEXT, cOrange );
    gui->getSkin()->setColor ( EGDC_3D_HIGH_LIGHT, video::SColor(240,0x22,0x22,0x22) );
    gui->getSkin()->setColor ( EGDC_3D_FACE, cBlack );
    gui->getSkin()->setColor ( EGDC_WINDOW, cYellow );

    stringw sNameOfFile;
    sNameOfFile+="save/performanceTest_";
    if(driver->getDriverType ()==EDT_OPENGL)sNameOfFile+="_OpenGL_";
    else if(driver->getDriverType ()==EDT_DIRECT3D9)sNameOfFile+="_Direct3D9_";
    else if(driver->getDriverType ()==EDT_DIRECT3D8)sNameOfFile+="_Direct3D8_";
    else if(driver->getDriverType ()==EDT_BURNINGSVIDEO)sNameOfFile+="_DriverBurningsVideo_";
    else if(driver->getDriverType ()==EDT_SOFTWARE)sNameOfFile+="_DriverSoftware_";
    else if(driver->getDriverType ()==EDT_NULL)sNameOfFile+="_DriverNull_";
    sNameOfFile+="_";
    sNameOfFile+=driver->getVendorInfo();
    sNameOfFile+="_";
    sNameOfFile+=sSet->resolution.Width;
    sNameOfFile+="x";
    sNameOfFile+=sSet->resolution.Height;
    sNameOfFile+="_";
    sNameOfFile+=sSet->bits;
    sNameOfFile+="bpp";
    sNameOfFile+=".xml";

    stringw sFrameDeltaTime;
    sFrameDeltaTime="";
    sFrameDeltaTime+=(u32)(frameDeltaTime*10);
    stringw sFps;
    sFps="";
    sFps+=driver->getFPS();

    stringw c;
    c+=driver->getVendorInfo();
    stringw sWidth;
    sWidth+=driver->getScreenSize().Width;
    stringw sHeight;
    sHeight+=driver->getScreenSize().Height;

    IXMLWriter* xml = device->getFileSystem()->createXMLWriter( sNameOfFile.c_str());
    xml->writeXMLHeader ();
    xml->writeElement(L"KartaGraficzna", true, L"typ", c.c_str());
    xml->writeLineBreak();
    xml->writeElement(L"BibliotekaGraficzna", true, L"typ", driver->getName());
    xml->writeLineBreak();
    xml->writeElement(L"RozdzielczoscX", true, L"X", sWidth.c_str());
    xml->writeLineBreak();
    xml->writeElement(L"RozdzielczoscY", true, L"Y", sHeight.c_str());
    xml->writeLineBreak();
    xml->writeElement(L"CzasRysowaniaKlatki(ms)", true, L"t", sFrameDeltaTime.c_str());
    xml->writeLineBreak();
    xml->writeElement(L"IloscFps", true, L"fps", sFps.c_str());
    xml->writeLineBreak();
    xml->writeComment(L"W listach ponizej znajduja sie TYLKO rodzaje dzwiekow/efektow, ktore sa wlaczone");
    xml->writeLineBreak();
    xml->writeElement(L"Dzwieki", false);
    xml->writeLineBreak();
    if(sSet->soundTracks&&sSet->sounds)
    {
        xml->writeText(L"PODKLAD MUZYCZNY");
        xml->writeLineBreak();
    }
    if(sSet->soundEffects&&sSet->sounds)
    {
        xml->writeText(L"EFEKTY DZWIEKOWE");
        xml->writeLineBreak();
    }
    if(sSet->sound3D&&sSet->sounds)
    {
        xml->writeText(L"DZWIEK 3D");
        xml->writeLineBreak();
    }
    xml->writeClosingTag(L"Dzwieki");
    xml->writeLineBreak();
    xml->writeElement(L"Efekty", false);
    xml->writeLineBreak();

    if(sSet->effects)
    {
        if(level==E_SCENE1)
        {
//swiatlo punktowe i rozproszone
            if(sSet->heroLight)
            {
                xml->writeText( L"HERO::LIGHT");
                xml->writeLineBreak();
            }
            if(sSet->fog)
            {
                xml->writeText(L"MGLA" );
                xml->writeLineBreak();
            }
//rozmazywanie ekranu
            if(sSet->mirror)
            {
                xml->writeText(L"LUSTRO" );
                xml->writeLineBreak();
            }
            if(sSet->heroShadow)
            {
                xml->writeText( L"HERO::SHADOW");
                xml->writeLineBreak();
            }
            if(sSet->fire)
            {
                xml->writeText(L"OGIEN" );
                xml->writeLineBreak();
            }
            if(sSet->heroTransparency)
            {
                xml->writeText(L"HERO::TRANSPARENCY" );
                xml->writeLineBreak();
            }
        }//koniec dla sceny 1

        if(level==E_SCENE2)
        {
            if(sSet->snow)
            {
                xml->writeText(L"SNIEG" );
                xml->writeLineBreak();
            }
            if(sSet->shaderToon)
            {
                xml->writeText( L"SHADER::TOON");
                xml->writeLineBreak();
            }
            if(sSet->heroSphericMap)
            {
                xml->writeText(L"HERO::SPHERICMAP" );
                xml->writeLineBreak();
            }
            if(sSet->werewolfes)
            {
                xml->writeText(L"WEREWOLVES" );
                xml->writeLineBreak();
            }
        }//koniec dla sceny 2
    }

    xml->writeClosingTag(L"Efekty");
    xml->writeLineBreak();
    xml->writeElement(L"WielokrotniePowtorzonyTest::Lustro", true, L"t(ms)", sFps.c_str());
    xml->writeLineBreak();
    xml->writeComment(L"to jest plik .xml ktory wyswietla wlaczone efekty i ich wydajnosc");
    xml->writeLineBreak();
    xml->writeComment(L"zapis i odczyt jaki zastosowalem moze rownie dobrze sluzyc do zapisu/odczytu wszelakich danych");
    if(xml)
        xml->drop();

    stringw MessageText;
    MessageText+="UWAGA! TEST WYKONAJ WIECEJ NIZ JEDEN RAZ,\n";
    MessageText+="PONIEWAZ NIEKTORE EFEKTY (NP LUSTRO) SA JESZCZE TESTOWANE\n";
    MessageText+="TEST WYKONAJ W ROZNYCH MIEJSCACH PLANSZY I NAJLEPIEJ OBLICZ SREDNIA\n";
    MessageText+="WYNIKI ZAPISANE ZOSTALY DO PLIKU W    ../SAVE/***.XML\n\n";
    MessageText+="        #####PARAMETRY RENDEROWANIA:#####\n";
    stringw Caption;
    stringc StartUpModelFile;
    stringc cFrameDeltaTime="CZAS RYSOWANIA SCENY (ms):";
    stringc cFps="ILOSC FPS:";

    IXMLReader* xmlR = device->getFileSystem()->createXMLReader( sNameOfFile.c_str());
    while(xmlR && xmlR->read())
    {
        switch(xmlR->getNodeType())
        {
        case io::EXN_TEXT:
            MessageText += xmlR->getNodeData();
            break;
        case io::EXN_ELEMENT:
        {
            if (core::stringw("CzasRysowaniaKlatki(ms)") == xmlR->getNodeName())
            {
                cFrameDeltaTime += xmlR->getAttributeValue(L"t");
                cFrameDeltaTime+="\n";
                MessageText+=cFrameDeltaTime;
            }
            else if (core::stringw("IloscFps") == xmlR->getNodeName())
            {
                cFps += xmlR->getAttributeValue(L"fps");
                cFps+="\n";
                MessageText+=cFps;
                MessageText+="\n\n\n        #####WLACZONE DZWIEKI  /  EFEKTY:#####";
            }
            else if (core::stringw("BibliotekaGraficzna") == xmlR->getNodeName())
            {
                MessageText+="BIBLIOTEKA GRAFICZNA:";
                MessageText += xmlR->getAttributeValue(L"typ");
                MessageText+="\n";
            }
            else if (core::stringw("KartaGraficzna") == xmlR->getNodeName())
            {
                MessageText+="KARTA GRAFICZNA:";
                MessageText += xmlR->getAttributeValue(L"typ");
                MessageText+="\n";
            }
            else if (core::stringw("RozdzielczoscX") == xmlR->getNodeName())
            {
                MessageText+="ROZDZIELCZOSC: ";
                MessageText += xmlR->getAttributeValue(L"X");
                MessageText+=" x ";
            }

            else if (core::stringw("RozdzielczoscY") == xmlR->getNodeName())
            {
                MessageText += xmlR->getAttributeValue(L"Y");
                MessageText+="\n";
            }
            else if (core::stringw("WielokrotniePowtorzonyTest::Lustro") == xmlR->getNodeName())
            {
                MessageText += xmlR->getAttributeValue(L"t(ms)");
                MessageText+="\n";
            }
        }
        break;
        default:
            break;
        }
    }

    if(xmlR)
        xmlR->drop();
    MessageText+="\n";
    MessageText+="\n";
    MessageText+=test.testMany;
    device->getGUIEnvironment()->addMessageBox(
        Caption.c_str(), MessageText.c_str());

    if(sSet->effects&&sSet->mirror&&level==E_SCENE1)startNewPerformanceTest(gui);
}

void Effect::performanceTest(IVideoDriver* driver)
{
    if(test.test)
    {
        if(test.mirror)
        {
            clock_t start, finish;
            double duration;
            start = clock();

            gui->drawAll();
            driver->endScene();
            driver->beginScene ( true, true, SColor ( 0, 0, 0, 0 ) );

            for(int i=0; i<100; i++)mirror->reflect();
            finish = clock();
            duration = (double)(finish - start) / CLOCKS_PER_SEC;
            duration/=100;
            test.testMany="\n";
            test.testMany+="EFEKTY, KTORYCH DZIALANIE ZBADANE ZOSTALO POPRZEZ ICH WIELOKROTNE POWTORZENIE:\n";
            test.testMany+="LUSTRO: ";
            test.testMany += duration;
            test.testMany += "\n";

            test.mirror=false;
        }

        test.test=false;
    }
}

void Effect::startNewPerformanceTest(IGUIEnvironment* gui)
{
    this->gui=gui;
    test.test=true;
    test.mirror=true;
}

void Effect::addRenderTargetToTexture(IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* gui)
{
    // create test cube
    scene::ISceneNode* test = smgr->addCubeSceneNode(60);

    // let the cube rotate and set some light settings
    ISceneNodeAnimator* anim = smgr->createRotationAnimator(
                                   core::vector3df(0.3f, 0.3f,0));

    test->setPosition(core::vector3df(-100,200,-100));
    test->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
    test->addAnimator(anim);
    anim->drop();

    video::ITexture* rt = 0;

    if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
    {
        rt = driver->addRenderTargetTexture(core::dimension2d<u32>(256,256), "RTT1");
        test->setMaterialTexture(0, rt); // set material of cube to render target
    }
    else
    {
        // create problem text
        gui::IGUISkin* skin = gui->getSkin();
        gui::IGUIFont* font = gui->getFont("textures/fonts/fonthaettenschweiler.bmp");
        if (font)
            skin->setFont(font);

        gui::IGUIStaticText* text = gui->addStaticText(
                                        L"Your hardware or this renderer is not able to use the "\
                                        L"render to texture feature. RTT Disabled.",
                                        core::rect<s32>(150,20,470,60));

        text->setOverrideColor(video::SColor(100,255,255,255));
    }

}
