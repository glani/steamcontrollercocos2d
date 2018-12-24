#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#ifdef _USE_STEAM

#include <EventListenerSteamController.h>

#endif

USING_NS_CC;



HelloWorld::HelloWorld() : currentSteamController(nullptr) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    _mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
    _mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _mouseListener->onMouseScroll = CC_CALLBACK_1(HelloWorld::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

#ifdef _USE_STEAM
    _steamControllerListener = EventListenerSteamController::create();
    _steamControllerListener->onConnected = CC_CALLBACK_2(HelloWorld::onConnectSteamController, this);
    _steamControllerListener->onDisconnected = CC_CALLBACK_2(HelloWorld::onDisconnectedSteamController, this);
    _steamControllerListener->onUpdate = CC_CALLBACK_2(HelloWorld::onUpdateSteamController, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_steamControllerListener, this);
#endif
#endif
    scheduleUpdate();
}

HelloWorld::~HelloWorld() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    _eventDispatcher->removeEventListener(_keyboardListener);
    _eventDispatcher->removeEventListener(_mouseListener);

#ifdef _USE_STEAM
    _eventDispatcher->removeEventListener(_steamControllerListener);
    SteamController::stopDiscoverySteamController();
#endif
#endif
}

Scene *HelloWorld::createScene() {
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    auto spriteSteam0 = Sprite::create("ss_0.jpg");
    spriteSteam0->setScale(2 / 3.0f);
    spriteSteam0->setPosition(Vec2(origin.x + 320, visibleSize.height / 2));
    this->addChild(spriteSteam0, 0);

    auto spriteSteam1 = Sprite::create("ss_1.jpg");
    spriteSteam1->setScale(2 / 3.0f);
    spriteSteam1->setPosition(Vec2(origin.x + 960, visibleSize.height / 2));
    this->addChild(spriteSteam1, 1);



    auto label = Label::createWithTTF("Steam Controller Cocos2D DEMO", "fonts/Marker Felt.ttf", 36);
    if (label == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 2);
    }


    auto sprite = Sprite::create("Cocos2DLogo.png");
    sprite->setScale(0.5f);
    sprite->setPosition(Vec2(sprite->getScale() * sprite->getContentSize().width / 2 + origin.x,
            visibleSize.height + origin.y - sprite->getScale() * sprite->getContentSize().height / 2));
    this->addChild(sprite, 3);

    auto ttfConfigNormal = TTFConfig("fonts/arial.ttf", 24, GlyphCollection::DYNAMIC, nullptr, true);
    ttfConfigNormal.outlineSize = 2;

    this->labelController = Label::createWithTTF(ttfConfigNormal, DISCONNECTED_TEXT, TextHAlignment::LEFT,
                                                 static_cast<int>(getContentSize().width));


    this->labelController->setPosition(Vec2(origin.x + this->labelController->getContentSize().width,
                                            origin.y + OFFSET_LABEL_Y + this->labelController->getContentSize().height));


    this->addChild(this->labelController, 4);


    auto ttfConfigCoordinates = TTFConfig("fonts/arial.ttf", 18, GlyphCollection::DYNAMIC, nullptr, true);
    ttfConfigCoordinates.outlineSize = 1;

    this->labelLeftPad = Label::createWithTTF(ttfConfigCoordinates, LEFT_PAD_FORMAT, TextHAlignment::LEFT,
                                              static_cast<int>(250));

    this->labelLeftPad->setPosition(Vec2(origin.x +
                                                 this->getContentSize().width - 250,
                                            origin.y + OFFSET_LABEL_RIGHT_Y + this->labelLeftPad->getContentSize().height));

    this->labelLeftPad->setTag(TAG_LEFT_PAD);
    this->addChild(this->labelLeftPad, 5);

    LabelHolder lhLabelLeftPad;
    lhLabelLeftPad.dt = 0;
    lhLabelLeftPad.format = LEFT_PAD_FORMAT;
    lhLabelLeftPad.label = this->labelLeftPad;
    xyLabels.emplace_back(lhLabelLeftPad);


    this->labelRightPad = Label::createWithTTF(ttfConfigCoordinates, RIGHT_PAD_FORMAT, TextHAlignment::LEFT,
                                               static_cast<int>(250));

    this->labelRightPad->setPosition(Vec2(origin.x +
                                         this->getContentSize().width - 250,
                                         origin.y + OFFSET_LABEL_RIGHT_Y + this->labelRightPad->getContentSize().height
                                                                     + this->labelLeftPad->getContentSize().height));
    this->labelRightPad->setTag(TAG_RIGHT_PAD);
    this->addChild(this->labelRightPad, 6);

    LabelHolder lhLabelRightPad;
    lhLabelRightPad.dt = 0;
    lhLabelRightPad.format = RIGHT_PAD_FORMAT;
    lhLabelRightPad.label = this->labelRightPad;
    xyLabels.emplace_back(lhLabelRightPad);

    this->labelStick = Label::createWithTTF(ttfConfigCoordinates, STICK_FORMAT, TextHAlignment::LEFT,
                                            static_cast<int>(250));

    this->labelStick->setPosition(Vec2(origin.x +
                                          this->getContentSize().width - 250,
                                          origin.y + OFFSET_LABEL_RIGHT_Y + this->labelRightPad->getContentSize().height
                                          + this->labelLeftPad->getContentSize().height
                                          + this->labelStick->getContentSize().height));

    this->labelStick->setTag(TAG_STICK);
    this->addChild(this->labelStick, 7);

    LabelHolder lhLabelStick;
    lhLabelStick.dt = 0;
    lhLabelStick.format = STICK_FORMAT;
    lhLabelStick.label = this->labelStick;
    xyLabels.emplace_back(lhLabelStick);

    this->labelTriggers = Label::createWithTTF(ttfConfigCoordinates, TRIGGERS_FORMAT, TextHAlignment::LEFT,
                                               static_cast<int>(250));

    this->labelTriggers->setPosition(Vec2(origin.x +
                                       this->getContentSize().width - 250,
                                       origin.y + OFFSET_LABEL_RIGHT_Y + this->labelRightPad->getContentSize().height
                                       + this->labelLeftPad->getContentSize().height
                                       + this->labelStick->getContentSize().height
                                       + this->labelTriggers->getContentSize().height));

    this->labelTriggers->setTag(TAG_TRIGGERS);
    this->addChild(this->labelTriggers, 8);

    LabelHolder lhLabelTriggers;
    lhLabelTriggers.dt = 0;
    lhLabelTriggers.format = TRIGGERS_FORMAT;
    lhLabelTriggers.label = this->labelTriggers;
    xyLabels.emplace_back(lhLabelTriggers);

    for (LabelHolder& holder : xyLabels) {
        holder.label->setVisible(false);
    }

    int offsetZOrder = 100;
    int num;
    // 0 -> ButtonA
    num = 0;
    SpriteHolder spriteHolder0;
    spriteHolder0.sprite = Sprite::create("circle-selected.png");
    spriteHolder0.num = num;
    spriteHolder0.position.set(392.0f, visibleSize.height - 384.f);
    spriteHolder0.scale = 0.5f;
    actionSprites.emplace_back(spriteHolder0);

    // 1 -> ButtonX
    num = 1;
    SpriteHolder spriteHolder1;
    spriteHolder1.sprite = Sprite::create("circle-selected.png");
    spriteHolder1.num = num;
    spriteHolder1.position.set(358.0f, visibleSize.height - 350.f);
    spriteHolder1.scale = 0.5f;
    actionSprites.emplace_back(spriteHolder1);

    // 2 -> ButtonB
    num = 2;
    SpriteHolder spriteHolder2;
    spriteHolder2.sprite = Sprite::create("circle-selected.png");
    spriteHolder2.num = num;
    spriteHolder2.position.set(428.0f, visibleSize.height - 350.f);
    spriteHolder2.scale = 0.5f;
    actionSprites.emplace_back(spriteHolder2);

    // 3 -> ButtonY
    num = 3;
    SpriteHolder spriteHolder3;
    spriteHolder3.sprite = Sprite::create("circle-selected.png");
    spriteHolder3.num = num;
    spriteHolder3.position.set(392.0f, visibleSize.height - 314.f);
    spriteHolder3.scale = 0.5f;
    actionSprites.emplace_back(spriteHolder3);


    // 12 -> ButtonPrev
    num = 12;
    SpriteHolder spriteHolder12;
    spriteHolder12.sprite = Sprite::create("circle-selected.png");
    spriteHolder12.num = num;
    spriteHolder12.position.set(272.0f, visibleSize.height - 255.f);
    spriteHolder12.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder12);

    // 13 -> ButtonHome
    num = 13;
    SpriteHolder spriteHolder13;
    spriteHolder13.sprite = Sprite::create("circle-selected.png");
    spriteHolder13.num = num;
    spriteHolder13.position.set(321.0f, visibleSize.height - 248.f);
    spriteHolder13.scale = 0.5f;
    actionSprites.emplace_back(spriteHolder13);

    // 14 -> ButtonNext
    num = 14;
    SpriteHolder spriteHolder14;
    spriteHolder14.sprite = Sprite::create("circle-selected.png");
    spriteHolder14.num = num;
    spriteHolder14.position.set(370.0f, visibleSize.height - 255.f);
    spriteHolder14.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder14);

    //17 -> ButtonStick
    num = 17;
    SpriteHolder spriteHolder17;
    spriteHolder17.sprite = Sprite::create("circle-selected.png");
    spriteHolder17.num = num;
    spriteHolder17.position.set(250.0f, visibleSize.height - 345.f);
    spriteHolder17.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder17);

    //8 -> ButtonDpadUp
    num = 8;
    SpriteHolder spriteHolder8;
    spriteHolder8.sprite = Sprite::create("circle-selected.png");
    spriteHolder8.num = num;
    spriteHolder8.position.set(152.f, visibleSize.height - 206.f);
    spriteHolder8.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder8);

    //11 -> ButtonDpadDown
    num = 11;
    SpriteHolder spriteHolder11;
    spriteHolder11.sprite = Sprite::create("circle-selected.png");
    spriteHolder11.num = num;
    spriteHolder11.position.set(157.f, visibleSize.height - 319.f);
    spriteHolder11.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder11);


    //9 -> ButtonDpadRight
    num = 9;
    SpriteHolder spriteHolder9;
    spriteHolder9.sprite = Sprite::create("circle-selected.png");
    spriteHolder9.num = num;
    spriteHolder9.position.set(209.0f, visibleSize.height - 260.f);
    spriteHolder9.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder9);

    //10 -> ButtonDpadLeft
    num = 10;
    SpriteHolder spriteHolder10;
    spriteHolder10.sprite = Sprite::create("circle-selected.png");
    spriteHolder10.num = num;
    spriteHolder10.position.set(101.f, visibleSize.height - 263.f);
    spriteHolder10.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder10);

    //18 -> ButtonRPad
    num = 18;
    SpriteHolder spriteHolder18;
    spriteHolder18.sprite = Sprite::create("circle-selected.png");
    spriteHolder18.num = num;
    spriteHolder18.position.set(487.f , visibleSize.height - 259.f);
    spriteHolder18.scale = 0.3f;
    actionSprites.emplace_back(spriteHolder18);

    //19 -> TouchLFinger
    num = 19;
    SpriteHolder spriteHolder19;
    spriteHolder19.sprite = Sprite::create("circle-axis.png");
    spriteHolder19.num = num;
    spriteHolder19.position.set(155.f , visibleSize.height - 260.f);
    spriteHolder19.scale = 0.3f;
    spriteHolder19.radius = 68.f;
    spriteHolder19.group = TAG_LEFT_PAD;
    actionSprites.emplace_back(spriteHolder19);

    //20 -> TouchRFinger
    num = 20;
    SpriteHolder spriteHolder20;
    spriteHolder20.sprite = Sprite::create("circle-axis.png");
    spriteHolder20.num = num;
    spriteHolder20.position.set(488.0f , visibleSize.height - 259.f);
    spriteHolder20.scale = 0.3f;
    spriteHolder20.radius = 68.f;
    spriteHolder20.group = TAG_RIGHT_PAD;
    actionSprites.emplace_back(spriteHolder20);

    //21 -> Stick
    num = 21;
    SpriteHolder spriteHolder21;
    spriteHolder21.sprite = Sprite::create("circle-axis.png");
    spriteHolder21.num = num;
    spriteHolder21.position.set(249.f , visibleSize.height - 345.f);
    spriteHolder21.scale = 0.3f;
    spriteHolder21.radius = 32.f;
    spriteHolder20.group = TAG_STICK;
    actionSprites.emplace_back(spriteHolder21);


    //16 -> ButtonLGrip
    num = 16;
    SpriteHolder spriteHolder16;
    spriteHolder16.sprite = Sprite::create("grip0-selected.png");
    spriteHolder16.num = num;
    spriteHolder16.position.set(798.f , visibleSize.height - 380.f);
    spriteHolder16.scale = 0.6f;
    actionSprites.emplace_back(spriteHolder16);

    //15 -> ButtonRGrip
    num = 15;
    SpriteHolder spriteHolder15;
    spriteHolder15.sprite = Sprite::create("grip0-selected.png");
    spriteHolder15.sprite->setScaleX(-1);
    spriteHolder15.num = num;
    spriteHolder15.position.set(1118.f , visibleSize.height - 380.f);
    spriteHolder15.scale = 0.6f;
    actionSprites.emplace_back(spriteHolder15);


    //5 -> ButtonLT
    num = 5;
    SpriteHolder spriteHolder5;
    spriteHolder5.sprite = Sprite::create("button0-selected.png");
    spriteHolder5.sprite->setScaleX(-1);
    spriteHolder5.num = num;
    spriteHolder5.position.set(1150.f , visibleSize.height - 235.f);
    spriteHolder5.scale = 0.7f;
    spriteHolder5.group = TAG_TRIGGERS;
    actionSprites.emplace_back(spriteHolder5);

    //4 -> ButtonRT
    num = 4;
    SpriteHolder spriteHolder4;
    spriteHolder4.sprite = Sprite::create("button0-selected.png");
    spriteHolder4.sprite->setScaleX(1);
    spriteHolder4.num = num;
    spriteHolder4.position.set(768.f , visibleSize.height - 235.f);
    spriteHolder4.scale = 0.7f;
    spriteHolder4.group = TAG_TRIGGERS;
    actionSprites.emplace_back(spriteHolder4);


    //7 -> ButtonLS
    num = 7;
    SpriteHolder spriteHolder7;
    spriteHolder7.sprite = Sprite::create("button1-selected.png");
    spriteHolder7.sprite->setScaleX(-1);
    spriteHolder7.num = num;
    spriteHolder7.position.set(1136.f , visibleSize.height - 171.0f);
    spriteHolder7.scale = 0.7f;
    actionSprites.emplace_back(spriteHolder7);

    //6 -> ButtonRS
    num = 6;
    SpriteHolder spriteHolder6;
    spriteHolder6.sprite = Sprite::create("button1-selected.png");
    spriteHolder6.sprite->setScaleX(1);
    spriteHolder6.num = num;
    spriteHolder6.position.set(789.f, visibleSize.height - 171.0f);
    spriteHolder6.scale = 0.7f;
    actionSprites.emplace_back(spriteHolder6);

    for (SpriteHolder& spriteHolder : actionSprites) {
        spriteHolder.sprite->setScaleX(spriteHolder.sprite->getScaleX() * spriteHolder.scale);
        spriteHolder.sprite->setScaleY(spriteHolder.sprite->getScaleY() * spriteHolder.scale);
        spriteHolder.sprite->setPosition(spriteHolder.position);
        spriteHolder.sprite->setVisible(false);
        this->addChild(spriteHolder.sprite, offsetZOrder + spriteHolder.num);
    }

    return true;
}


#ifdef _USE_STEAM

void HelloWorld::restartSteamControllerDiscovery() {
    SteamController::stopDiscoverySteamController();
    SteamController::startDiscoverySteamController();
}

void HelloWorld::onConnectSteamController(cocos2d::SteamController *steamController, cocos2d::Event *event) {
    auto it = this->steamControllers.find(steamController->getDeviceId());
    if (it == this->steamControllers.end()) {
        this->steamControllers.emplace(steamController->getDeviceId(), steamController);
        if (nullptr == this->currentSteamController) {
            this->currentSteamController = steamController;
        }
        this->updateSteamControllers();
    }
}

void HelloWorld::onDisconnectedSteamController(cocos2d::SteamController *steamController, cocos2d::Event *event) {
    auto it = this->steamControllers.find(steamController->getDeviceId());
    if (it != this->steamControllers.end()) {
        this->steamControllers.erase(it);
        if (steamController == this->currentSteamController) {
            this->currentSteamController = nullptr;
        }
    }
    this->updateSteamControllers();
}

void HelloWorld::onUpdateSteamController(cocos2d::SteamController *steamController, cocos2d::Event *event) {
    if (steamController == this->currentSteamController) {
        cocos2d::EventSteamController *evt = dynamic_cast<cocos2d::EventSteamController *>(event);
        if (nullptr == evt) {
            return;
        }
        
        for (SpriteHolder& spriteHolder : actionSprites) {
            const uint8_t *v = determineSteamControllerValueByNumber(evt->getValues(), spriteHolder.num);
            if (v != nullptr) {
                if (*v == SteamControllerValues::YES) {
                    if (std::fabs(spriteHolder.radius) > 0) {
                        const float *x = nullptr, *y = nullptr;
                        if (spriteHolder.num == 19) {
                            x = &evt->getValues().LeftPadX;
                            y = &evt->getValues().LeftPadY;
                        } else if (spriteHolder.num == 20) {
                            x = &evt->getValues().RightPadX;
                            y = &evt->getValues().RightPadY;
                        } else if (spriteHolder.num == 21) {
                            x = &evt->getValues().StickX;
                            y = &evt->getValues().StickY;
                        }

                        if (x != nullptr && y != nullptr) {
                            float x1 = *x * spriteHolder.radius + spriteHolder.position.x;
                            float y1 = *y * spriteHolder.radius + spriteHolder.position.y;
                            spriteHolder.sprite->setPosition(x1, y1);
                        }
                    }

                    for (LabelHolder& holder : xyLabels) {
                        if (holder.label->getTag() == spriteHolder.group) {
                            holder.dt = 0.5f;
                        }
                    }

                    spriteHolder.sprite->setVisible(true);
                    spriteHolder.sprite->setOpacity((GLubyte)(255));
                    spriteHolder.sprite->stopAllActions();
                    auto actionFadeOut = FadeOut::create(0.5f);
                    actionFadeOut->setTag(100);
                    spriteHolder.sprite->runAction(Sequence::create(actionFadeOut, nullptr));
                } else {
                    spriteHolder.sprite->stopAllActions();
                    spriteHolder.sprite->setVisible(false);
                }
            }
        }

        labelStick->setString(getString(STICK_FORMAT, evt->getValues().StickX, evt->getValues().StickY));
        labelTriggers->setString(getString(TRIGGERS_FORMAT, evt->getValues().LeftTrigger, evt->getValues().RightTrigger));
        labelLeftPad->setString(getString(LEFT_PAD_FORMAT, evt->getValues().LeftPadX, evt->getValues().LeftPadY));
        labelRightPad->setString(getString(RIGHT_PAD_FORMAT, evt->getValues().RightPadX, evt->getValues().RightPadY));
    }
}


void HelloWorld::updateSteamControllers() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (nullptr == this->currentSteamController) {
        this->labelController->setString(DISCONNECTED_TEXT);
        for (LabelHolder& holder : xyLabels) {
            holder.label->setVisible(false);
        }

    } else {
        std::string s("Steam Controller: ");
        s.append(this->currentSteamController->getDeviceId());
        this->labelController->setString(s);
        for (LabelHolder& holder : xyLabels) {
            holder.label->setVisible(true);
        }
    }

    this->labelController->setPosition(Vec2(origin.x + this->labelController->getContentSize().width,
                                            origin.y + OFFSET_LABEL_Y + this->labelController->getContentSize().height));
}

const uint8_t *HelloWorld::determineSteamControllerValueByNumber(const SteamControllerValues &value, int num) const {
    switch(num) {
        case 0:
            return &value.ButtonA;
        case 1:
            return &value.ButtonX;
        case 2:
            return &value.ButtonB;
        case 3:
            return &value.ButtonY;
        case 4:
            return &value.ButtonRT;
        case 5:
            return &value.ButtonLT;
        case 6:
            return &value.ButtonRS;
        case 7:
            return &value.ButtonLS;
        case 8:
            return &value.ButtonDpadUp;
        case 9:
            return &value.ButtonDpadRight;
        case 10:
            return &value.ButtonDpadLeft;
        case 11:
            return &value.ButtonDpadDown;
        case 12:
            return &value.ButtonPrev;
        case 13:
            return &value.ButtonHome;
        case 14:
            return &value.ButtonNext;
        case 15:
            return &value.ButtonRGrip;
        case 16:
            return &value.ButtonLGrip;
        case 17:
            return &value.ButtonStick;
        case 18:
            return &value.ButtonRPad;
        case 19:
            return &value.TouchLFinger;
        case 20:
            return &value.TouchRFinger;
        case 21:
            return &value.Stick;

        default:
            break;

    }
    return nullptr;
}

#endif


void HelloWorld::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::initController() {
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::initControllerSchedule), 0.5f);
}

void HelloWorld::onEnter() {
    Node::onEnter();
    this->initController();
}

void HelloWorld::initControllerSchedule(float dt) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#ifdef _USE_STEAM
    restartSteamControllerDiscovery();
#endif
#endif
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
        menuCloseCallback(nullptr);
    }
}

void HelloWorld::update(float delta) {
    Node::update(delta);

    for (LabelHolder& holder : xyLabels) {
        holder.dt -= delta;
        if (holder.dt <= 0) {
            holder.dt = 0;
            holder.label->setString(getString(holder.format, 0.0f, 0.0f));
        }
    }
}

void HelloWorld::onMouseDown(cocos2d::Event *event) {

}

void HelloWorld::onMouseUp(cocos2d::Event *event) {
    auto mouseEvent = dynamic_cast<EventMouse *>(event);
    const Vec2 &v = convertToNodeSpace(mouseEvent->getLocation());
    CCLOG("v: %f %f", v.x, v.y);
}

void HelloWorld::onMouseMove(cocos2d::Event *event) {

}

void HelloWorld::onMouseScroll(cocos2d::Event *event) {

}

const std::string HelloWorld::getString(const char *const format, float x, float y) const {
    char buff[255];
    std::sprintf(buff, format, x, y);
    return std::string(buff);
}


