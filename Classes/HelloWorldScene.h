#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

static const char *const DISCONNECTED_TEXT = "Steam Controller: disconnected";

static const int OFFSET_LABEL_Y = 75;

static const int OFFSET_LABEL_RIGHT_Y = 15;

static const char *const LEFT_PAD_FORMAT = "Left pad: x=%.2f y=%.2f";

static const char *const RIGHT_PAD_FORMAT = "Right pad: x=%.2f y=%.2f";

static const char *const STICK_FORMAT = "Stick: x=%.2f y=%.2f";

static const char *const TRIGGERS_FORMAT = "Triggers: left=%.2f right=%.2f";

static const int TAG_STICK = 104;

static const int TAG_TRIGGERS = 103;

static const int TAG_RIGHT_PAD = 102;

static const int TAG_LEFT_PAD = 101;

#include "cocos2d.h"

#ifdef _USE_STEAM
#include "SteamController.h"
#endif


typedef struct _SpriteHolder {
public:
    int num;
    cocos2d::Sprite *sprite;
    std::string image;
    cocos2d::Vec2 position;
    float scale;

    _SpriteHolder() {
        this->scale = 1.0f;
        this->radius = 0;
        this->group = INT_MAX;
    }

    float radius;
    int group;
} SpriteHolder;

typedef struct _LabelHolder {
public:
    float dt;
    const char* format;
    cocos2d::Label *label;
    _LabelHolder() {
        format = nullptr;
    }
} LabelHolder;

class HelloWorld : public cocos2d::Scene
{
public:
    HelloWorld();

    virtual ~HelloWorld();

    static cocos2d::Scene* createScene();

    void initController();


#ifdef _USE_STEAM
    void onConnectSteamController(cocos2d::SteamController* steamController, cocos2d::Event* event);
    void onDisconnectedSteamController(cocos2d::SteamController* steamController, cocos2d::Event* event);
    void onUpdateSteamController(cocos2d::SteamController* steamController, cocos2d::Event* event);
#endif

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);


    void onEnter() override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);


    void onMouseDown(cocos2d::Event *event);
    void onMouseUp(cocos2d::Event *event);
    void onMouseMove(cocos2d::Event *event);
    void onMouseScroll(cocos2d::Event *event);

    void update(float delta) override;

private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    cocos2d::EventListenerKeyboard *_keyboardListener;
    cocos2d::EventListenerMouse *_mouseListener;
#endif
    void initControllerSchedule(float dt);
#ifdef _USE_STEAM
    void restartSteamControllerDiscovery();
    void updateSteamControllers();
    const uint8_t *determineSteamControllerValueByNumber(const cocos2d::SteamControllerValues &value, int num) const;
    cocos2d::EventListenerSteamController* _steamControllerListener;
    std::map<std::string, cocos2d::SteamController*> steamControllers;
    cocos2d::SteamController *currentSteamController;
#endif
    cocos2d::Label *labelController;

    std::vector<SpriteHolder> actionSprites;
    cocos2d::Label *labelLeftPad;
    cocos2d::Label *labelRightPad;
    cocos2d::Label *labelStick;
    cocos2d::Label *labelTriggers;

    std::vector<LabelHolder> xyLabels;

    const std::string getString(const char * format, float x, float y) const;
};

#endif // __HELLOWORLD_SCENE_H__
