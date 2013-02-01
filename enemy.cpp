
#include "enemy.h"

Enemy::Enemy()
{
    life = 1;
}

void Enemy::loadEnemyBillboards(ISceneManager* smgr, IVideoDriver* driver)
{
    bHealthStatusEnemy = smgr->addBillboardSceneNode();
    bHealthStatusEnemy->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    bHealthStatusEnemy->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bHealthStatusTEnemy.bmp" ) );
    bHealthStatusEnemy->setMaterialFlag ( video::EMF_LIGHTING, false );
    bHealthStatusEnemy->setMaterialFlag ( video::EMF_ZBUFFER, true );
    bHealthStatusEnemy->setSize ( core::dimension2d<f32 > ( 19.0f, 1.9f ) );
    bHealthStatusEnemy->setVisible ( false );

    bHealthStatusEnemyT = smgr->addBillboardSceneNode();
    bHealthStatusEnemyT->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    bHealthStatusEnemyT->setMaterialTexture ( 0, driver->getTexture ( "textures/basicBillboards/bHealthStatusEnemy.bmp" ) );
    bHealthStatusEnemyT->setMaterialFlag ( video::EMF_LIGHTING, false );
    bHealthStatusEnemyT->setMaterialFlag ( video::EMF_ZBUFFER, true );
    bHealthStatusEnemyT->setSize ( core::dimension2d<f32 > ( 20.0f, 2.0f ) );
    bHealthStatusEnemyT->setVisible ( false );
}

void Enemy::updateEnemyBillboards(IAnimatedMeshSceneNode* oEnemy, IAnimatedMeshSceneNode* oHero, ISceneNode* selectedSceneNode)
{
    if ( selectedSceneNode )
    {
        bHealthStatusEnemyT->setVisible ( true );
        bHealthStatusEnemy->setVisible ( true );
        bHealthStatusEnemyT->setPosition ( oHero->getPosition() + core::vector3df ( 0, 32, 0 ) );
        bHealthStatusEnemy->setPosition ( oHero->getPosition() + core::vector3df ( 0, 32, 0 ) );
        bHealthStatusEnemy->setSize(core::dimension2d<f32 > (life*19.0f, 1.9f));
    }
    else
    {
        bHealthStatusEnemyT->setVisible ( false );
        bHealthStatusEnemy->setVisible ( false );
    }
}

void Enemy::setStartValues()
{
    this->life=1;
}
