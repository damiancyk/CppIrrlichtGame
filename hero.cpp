
#include "hero.h"

Hero::Hero()
{
    MOVEMENT_SPEED = 1.5f;
    xDirection = 0;
    yDirection = 0;
    anglePlus = 0;
    life = 1;
    armor = 1;
    animation = EMAT_STAND;
    yJump=0;
    yJumpSpeed=3;
    isJump=false;
    heroName=L"Damiancyk";
    isAttacking=false;
    crosshairSize = dimension2d<f32 > ( 5, 5 );
    crosshairSizeBig = dimension2d<f32 > ( 8, 8 );
}

void Hero::updateHero(IAnimatedMeshSceneNode *oHero, IAnimatedMeshSceneNode *oGun)
{
    jump(oHero, oGun);
    if(nodeName)nodeName->setPosition(oHero->getPosition()+vector3df(0,35,0) );
    oHero->setRotation(core::vector3df(0, xDirection - anglePlus, 0)); //obrot bohatera
    //oGun->setRotation(oHero->getRotation());-(
    //oGun->setPosition(oHero->getPosition());-]
    facing.set(sin((oHero->getRotation().Y + 90) * PI / 180.0f), 0, cos((oHero->getRotation().Y + 90) * PI / 180.0f)); //kierunek ruchu bohatera
    facing.normalize();
}

void Hero::updateCamera ( Settings* sSet, IAnimatedMeshSceneNode* oHero, ICameraSceneNode* camera, position2d<f32> cursorPos, f32 rateMouse, f32 rateSniper, f32 rateSniperY)
{
    if ( ( oHero ) && ( camera ) )
    {
        /**zmienne obrotu bohatera i uaktualnianie ich poprzez mysz*/
        f32 change_x = ( cursorPos.X - 0.5 ) * sSet->mouseSensivityX*rateMouse*rateSniper; //czulosc
        f32 change_y = ( cursorPos.Y - 0.5 ) * sSet->mouseSensivityY*rateMouse*rateSniperY; //myszki
        xDirection += change_x;
        yDirection -= change_y;

        //zabezpieczenie osi X przed przerostem wartosci
        if ( xDirection < -360 )
        {
            xDirection = 0;
        }

        else if ( xDirection > 360 )
        {
            xDirection = 0;
        }

        //zmienne pomocnicze dla bohatera i kamery bazujace na trygonometrycznych funkcjach
        vector3df playerPos = oHero->getPosition();
        vector3df facingCameraFPP;
        switch(sSet->typeOfCamera)
        {
        case E_FPP:
            sSet->cursorYRestriction = -30;
            sSet->cursorXRestriction = -2;
            facingCameraFPP=vector3df(facing*70);
            break;
        case E_TPP:
            sSet->cursorYRestriction = -60;
            sSet->cursorXRestriction = 25;
            facingCameraFPP=vector3df(0,0,0);
            break;
        default:
            break;
        }

        if ( yDirection < sSet->cursorYRestriction )
            yDirection = sSet->cursorYRestriction;
        else if ( yDirection > sSet->cursorXRestriction )
            yDirection = sSet->cursorXRestriction;


        f32 cameraPosX = playerPos.X - cos ( ( xDirection + sSet->posCamAhead ) * PI / 180.0f ) * 64.0f;
        f32 cameraPosY = playerPos.Y - sin ( yDirection * PI / 180.0f ) * 128.f;
        f32 cameraPosZ = playerPos.Z + sin ( ( xDirection + sSet->posCamAhead ) * PI / 180.0f ) * 64.0f;
        //camera->setPosition ( vector3df ( playerPos.X, playerPos.Y +20, playerPos.Z ) +h->facing*10);
        camera->setPosition ( core::vector3df ( cameraPosX, cameraPosY , cameraPosZ ) + facingCameraFPP);
        camera->setTarget ( vector3df ( playerPos.X, playerPos.Y+20, playerPos.Z ) + facingCameraFPP*2);

    }
}

void Hero::jump(IAnimatedMeshSceneNode* oHero, IAnimatedMeshSceneNode* oGun)
{
    if(isJump)
    {
        oHero->setPosition(oHero->getPosition()+vector3df(0,yJump,0));
        if(yJumpSpeed==3)oHero->setMD2Animation(EMAT_JUMP);
        if(yJumpSpeed==3)oGun->setMD2Animation(EMAT_JUMP);
        yJump+=yJumpSpeed;
        if(yJumpSpeed>1)yJumpSpeed-=0.2;
        if(yJump>=15)
        {
            yJump=0;
            yJumpSpeed=3;
            oHero->setMD2Animation(EMAT_RUN);
            oGun->setMD2Animation(EMAT_RUN);
            isJump=false;
        }

    }

}

void Hero::setName(ISceneManager* smgr, IAnimatedMeshSceneNode* oHero)
{
    SColor cWhite=SColor(250,200,200,200);
    SColor cYellow=SColor(250,200,200,0);
    nodeName = smgr->addBillboardTextSceneNode(0,  heroName.c_str(), 0,dimension2d<f32>(15,3), oHero->getPosition(), 0, cWhite, cYellow);
}

void Hero::loadHeroBillboards(ISceneManager* smgr, IVideoDriver* driver)
{
    // pasek zycia (aktualny stan)
    bHealthStatus = smgr->addBillboardSceneNode();
    bHealthStatus->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    bHealthStatus->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bHealthStatusT.bmp" ) );
    bHealthStatus->setMaterialFlag ( video::EMF_LIGHTING, false );
    bHealthStatus->setMaterialFlag ( video::EMF_ZBUFFER, true );
    bHealthStatus->setSize ( core::dimension2d<f32 > ( 0.0f, 1.9f ) );

    // pasek zycia (tlo paska)
    bHealthStatusT = smgr->addBillboardSceneNode();
    bHealthStatusT->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    bHealthStatusT->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bHealthStatus.bmp" ) );
    bHealthStatusT->setMaterialFlag ( video::EMF_LIGHTING, false );
    bHealthStatusT->setMaterialFlag ( video::EMF_ZBUFFER, true );
    bHealthStatusT->setSize ( core::dimension2d<f32 > ( 20.0f, 2.0f ) );

    bCrosshair = smgr->addBillboardSceneNode();
    bCrosshair->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    bCrosshair->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bCrosshair.bmp" ) );
    bCrosshair->setMaterialFlag ( video::EMF_LIGHTING, false );
    //bCrosshair->setMaterialFlag(video::EMF_ZBUFFER, false);
    bCrosshair->setSize ( crosshairSize );
    bCrosshair->setVisible ( true );
    bCrosshair->setID(OBJECT_ID_CROSSHAIR);

    bAim = smgr->addBillboardSceneNode();
    bAim->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    bAim->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bAim.bmp" ) );
    bAim->setMaterialFlag ( video::EMF_LIGHTING, false );
    bAim->setMaterialFlag ( video::EMF_ZBUFFER, false );
    bAim->setPosition ( vector3df ( 0, -200, 0 ) );
    bAim->setSize ( core::dimension2d<f32 > ( 10.0f, 10.0f ) );
    bAim->setID(OBJECT_ID_AIM);

}

void Hero::updateHeroBillboards(IAnimatedMeshSceneNode* oHero, vector3df intersection)
{
//pozycja paska zycia
    bHealthStatusT->setPosition ( oHero->getPosition() + core::vector3df ( 0, 30, 0 ) );
    bHealthStatus->setPosition ( oHero->getPosition() + core::vector3df ( 0, 30, 0 ) );
    bHealthStatus->setSize ( core::dimension2d<f32 > ( life * 19.0f, 1.9f ) );


    //poprawka na bron i celownik(aby byl troche z boku)
    facingGun.set ( sin ( ( oHero->getRotation().Y + 101 ) * PI / 180.0f ), 0, cos ( ( oHero->getRotation().Y + 101 ) * PI / 180.0f ) );
    facingGun.normalize();
    posRayPlus.set ( vector3df ( 0, 30, 0 ) );
    posRayPlus += facingGun * 35;

    //pozycja celownika
    vector3df posShowViewer ( oHero->getPosition().X, oHero->getPosition().Y, oHero->getPosition().Z );
    bCrosshair->setPosition ( core::vector3df ( posShowViewer ) + posRayPlus );
    bAim->setPosition ( intersection );
}

void Hero::setStartValues()
{
    this->life=1;
    this->armor=1;
}
