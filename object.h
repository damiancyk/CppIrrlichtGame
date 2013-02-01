
#ifndef __OBJECT_H_INCLUDED__
#define __OBJECT_H_INCLUDED__

#include "device.h"
#include "CProgressBar.h"
#include "Mirror.h"
#include "effect.h"
#include "CSampleSceneNode.h"


/**typ wyliczeniowy nadajacy ID kazdemu elementowi GUI*/
enum
{
    GUI_ID_B_TEST,
    GUI_ID_B_START,
    GUI_ID_B_OPTIONS,
    GUI_ID_B_SAVE,
    GUI_ID_B_RETURN_TO_MAIN,
    GUI_ID_B_AUTHOR,
    GUI_ID_B_QUIT,
    GUI_ID_BB_RETURN_TO_GAME,
    GUI_ID_BB_RETURN_TO_MAIN,
    GUI_ID_BB_RETURN_TO_SYSTEM,
    GUI_ID_S_,
    GUI_ID_EDT_NICK,
    GUI_ID_M_GAME,
    GUI_ID_M_GAME_OPTIONS,
    GUI_ID_M_GRAPHICS_OPTIONS,
    GUI_ID_M_SOUND_OPTIONS,
    GUI_ID_M_OTHER,
    GUI_ID_M_SAVE_GAME,
    GUI_ID_M_LOAD_GAME,
    GUI_ID_M_EXIT_GAME,
    GUI_ID_M_SET_RESOLUTION,
    GUI_ID_M_SET_COLOR_DEPTH,
    GUI_ID_M_RESOLUTION_640_480,
    GUI_ID_M_RESOLUTION_800_600,
    GUI_ID_M_RESOLUTION_960_600,
    GUI_ID_M_RESOLUTION_1024_768,
    GUI_ID_M_RESOLUTION_1152_864,
    GUI_ID_M_RESOLUTION_1280_720,
    GUI_ID_M_SOUND_3D_ON_OFF,
    GUI_ID_M_INCREASE_VOLUME,
    GUI_ID_M_DECREASE_VOLUME,
    GUI_ID_M_TOPIC,
    GUI_ID_M_AUTHOR,
    GUI_ID_M_EFFECTS,
    GUI_ID_EFFECTS,
    GUI_ID_MIRROR,
    GUI_ID_SNOW,
    GUI_ID_FOG,
    GUI_ID_FIRE,
    GUI_ID_SHADERTOON,
    GUI_ID_HERO_LIGHT,
    GUI_ID_HERO_TRANSPARENCY,
    GUI_ID_HERO_SPHERIC_MAP,
    GUI_ID_HERO_SHADOW,
    GUI_ID_LIGHT_AMBIENT,
    GUI_ID_LIGHT_POINT,
    GUI_ID_LIGHT_MAP,
    GUI_ID_FOREST,
    GUI_ID_WEREWOLVES,
    GUI_ID_TERRAIN_QUALITY,
    GUI_ID_SKY,
    GUI_ID_COMPASS,

    GUI_ID_VOLUME,
    GUI_ID_SOUNDS,
    GUI_ID_STRACKS,
    GUI_ID_SEFFECTS,
    GUI_ID_S3D
};

/**typ wyliczeniowy nadajacy ID kazdemu typowi menu glownego*/
enum eTypeOfMainMenu
{
    GUI_MENU_TYPE_MAIN,
    GUI_MENU_TYPE_OPTION,
    GUI_MENU_TYPE_OPTION_GRAPHICS,
    GUI_MENU_TYPE_OPTION_SOUND,
    GUI_MENU_TYPE_OPTION_GAME,
    GUI_MENU_TYPE_AUTHOR
};

/**podzial na gre/pauze*/
enum ETypeOfGameMenu
{
    GUI_GAME_PAUSE,
    GUI_GAME
};

/**nadawanie id obiektom, potrzebne do rayTracingu*/
enum
{
//    ID_IsNotPickable = 0,
    IDFlag_IsPickable = 1 << 0
//    IDFlag_IsHighlightable = 1 << 1
};

/**
 *klasa Object to jedna z kilku klas projektowanego przeze mnie silnika
 *posiada ona pola, które reprezentuja adresy do wszystkich obiektow w grze
 *metody tej klasy pozwalaja na obsluge obiektow
 */
class Object
{
private:

    /**siatka modelu bohatera*/
    IAnimatedMesh* mHero;

    /**siatka modelu broni*/
    IAnimatedMesh* mGun;

    /**siatka modelu wroga*/
    IAnimatedMesh* mEnemy;

    /**wskaznik na skybox (niebo)*/
    ISceneNode* skybox;

    /**wskaznik na skydome (niebo)*/
    ISceneNode* skydome;

    /**wskaznik na siatke mapy typu .bsp*/
    IAnimatedMesh* mBasePortal;

    /**wskaznik na obiekt mapy typu .bsp*/
    ISceneNode* oBasePortal;

    /**domyslna, wbudowana czcionka w GUI*/
    IGUIFont* fontDefault;

    /**struktura skupiajaca wskazniki na wszystkie obiekty GUI w menu glownym*/
    struct structGUI
    {
        structGUI ()
        {
            memset ( this, 0, sizeof ( *this ) );
        }

        IGUIFont* font1;
        IGUIWindow* window;
        IGUISkin* skin;
        IGUIButton* bStart;
        IGUIButton* bOption;
        IGUIButton* bQuit;
        IGUIButton* bSave;
        IGUIButton* bReturnToMain;
        IGUIEditBox* edtNick;
        IGUIScrollBar* s;
        IGUIScrollBar* sVolume;
        IGUICheckBox* checkS3D;
        IGUIContextMenu* menu;
        ITexture* iBackground;
        ITexture* iBackgroundRight;
        IGUIImage* iGuiBackground;
        IGUIImage* iGuiBackgroundRight;
        IGUIImage* iGuiTitle;
        ITexture* iBStart;
        ITexture* iBOption;
        ITexture* iBQuit;
        ITexture* iLogoIrrlicht;
        ITexture* iLogoIrrKlang;
        ITexture* iLogoIrrAI;
        ITexture* iLogoIrrPhysx;
        IGUIImage* iGuiLogoIrrlicht;
        IGUIImage* iGuiLogoIrrKlang;
        IGUIImage* iGuiLogoIrrAI;
        IGUIImage* iGuiLogoIrrPhysx;

        IGUICheckBox* checkMEffects;

        IGUICheckBox* checkEffects;
        IGUICheckBox* checkMirror;
        IGUICheckBox* checkRain;
        IGUICheckBox* checkSnow;
        IGUICheckBox* checkFog;
        IGUICheckBox* checkFire;
        IGUICheckBox* checkShaderToon;

        IGUICheckBox* checkHeroLight;
        IGUICheckBox* checkHeroTransparency;
        IGUICheckBox* checkHeroSphericMap;
        IGUICheckBox* checkHeroShadow;
        IGUICheckBox* checkLightAmbient;
        IGUICheckBox* checkLightPoint;
        IGUICheckBox* checkLightMap;
        IGUICheckBox* checkForest;
        IGUICheckBox* checkWerewolves;
        IGUICheckBox* checkTerrainQuality;
        IGUICheckBox* checkSky;
        IGUICheckBox* checkCompass;

        IGUICheckBox* checkSounds;
        IGUICheckBox* checkSTracks;
        IGUICheckBox* checkSEffects;

        IGUIStaticText* tFps;
        IGUIStaticText* tCameraFPP;
        IGUIStaticText* tCameraTPP;
        IGUIStaticText* tWatch;
        IGUIStaticText* tLife;
        IGUIStaticText* tArmor;
        gui::IGUIComboBox* box;
        IGUIListBox * listbox;
        u32 listboxCount;
    };

    /**struktura skupiajaca wskazniki na obiekty prezentacji 2D w menu gry*/
    struct struct2D
    {
        struct2D ()
        {
            memset ( this, 0, sizeof ( *this ) );
        }

        ITexture* images;
        IGUIFont* font;
        IGUIFont* font2;
        rect<s32> imp1;
        rect<s32> imp2;
    };

    /**ustawienia GUI gry*/
    struct structGameSettings
    {
        structGameSettings ()
        {
            memset ( this, 0, sizeof ( *this ) );
        }

        u32 resolutionX;
        u32 resolutionY;
        s32 buttonX;
        s32 buttonY;
        bool isSound;
    };

    /**skupienie wskaznikow typowego srodowiska 2D*/
    struct2D dimension2D;

    /**ustawienia gry*/
    structGameSettings gameSettings;

    /**metoda która tworzy animator odpowiedzialny za kolizje*/
    void collisionsNGravity(ISceneManager* smgr, ICameraSceneNode* camera, IAnimatedMeshSceneNode* oHero);

    ITerrainSceneNode* loadHeightMap(IVideoDriver* driver, ISceneManager* smgr, vector3df pos, stringw path, Settings* sSet);

    void loadTrees(IVideoDriver* driver, ISceneManager* smgr);

public:

    /**wskaznik na animowany obiekt w menu glownym*/
    IAnimatedMeshSceneNode* anode;

    /**siatka modelu bohatera*/
    IAnimatedMeshSceneNode* oHero;

    /**siatka modelu broni*/
    IAnimatedMeshSceneNode* oGun;

    /**siatka modelu wroga*/
    IAnimatedMeshSceneNode* oEnemy;

    /**ikona ksiezyca w GUI gry*/
    IGUIImage* planetIcon;

    /**wskaznik na teren*/
    ISceneNode* terrain;

    /**skupienie wskaznikow obiektow GUI w menu glownym*/
    structGUI sGui;

    /**rozroznienie na gre/pauze*/
    ETypeOfGameMenu eTypeOfGameMenu;

    IAnimatedMeshSceneNode* werewolves[10][10];

    /**konstruktor klasy Object*/
    Object();

    /**destruktor klasy Object*/
    ~Object();

    /**zmiana na biezaco statystyk bohatera w GUI gry*/
    void updateGuiHeroStats(f32 life, f32 armor);

    /**rysowanie prezentacji 2D w menu gry*/
    void draw2D(IrrlichtDevice* device, IVideoDriver* driver, u32 time, Settings* sSet);

    /**zaladowanie menu*/
    void loadMenuGUI(Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, eTypeOfMainMenu);

    /**zaladowanie terenu do menu glownego*/
    void loadMenuTerrain(ISceneManager* smgr, IVideoDriver* driver, Settings* sSet);

    /**zaladowanie intro*/
    void loadIntro(Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera);

    /**zaladowanie sceny 1*/
    void loadScene1(Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera);

    /**zaladowanie GUI gry*/
    void loadGameGUI(IGUIEnvironment* gui, IVideoDriver* driver, IrrlichtDevice* device, ETypeOfGameMenu, Settings* sSet);

    /**zaladowanie sceny 2*/
    void loadScene2(Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera);

    /**zaladowanie outro*/
    void loadOutro(Settings* sSet, IrrlichtDevice* device, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ICameraSceneNode* camera);

    /**ustaw editBoxy w menu w zaleznosci od stanu zmiennych*/
    void setChecked(Settings* sSet);

    /**jesli ustawisz na nieaktywna opcje efekty, nie mozesz tez ich zmieniac*/
    void changeEffectsAvailability(bool isEnabled);

    /**dodaj wiadomosc na mini konsolki w menu glownym*/
    void addMessage(const wchar_t *text, s32 icon);

    /**jesli ustawisz na nieaktywna opcje dzwieki, nie mozesz tez ich zmieniac*/
    void changeSoundsAvailability(bool isEnabled);

    /**
    *funkcja strzalu
    *zwraca ID obiektu ktory trafilismy
    */
    ETypeOfObject shoot(ISceneNode* selectedSceneNode,
                        IAnimatedMeshSceneNode *oHero,
                        IAnimatedMeshSceneNode *oEnemy,
                        ISoundEngine *engineSnd);

    /**zaladuj prezentacje 2D w menu gry*/
    void load2D(IrrlichtDevice* device, IVideoDriver* driver);

    /**zmieniaj na biezaco wartosc FPS w GUI gry*/
    void updateFpsGui ( IrrlichtDevice* device, IVideoDriver* driver,
                        IGUIEnvironment* gui);

    /**funkcja ladujaca model wilkolaka wielokorotnie w jednej scenie*/
    void loadWerewolves(IVideoDriver* driver, ISceneManager* smgr,IAnimatedMeshSceneNode* werewolves[10][10], vector3df pos);

    /**funkcja poruszajaca wieloma wilkolalkami znajdujacymi sie na jednej scenie*/
    void moveWerewolves(IAnimatedMeshSceneNode* werewolves[10][10], vector3df pos);

};

#endif
