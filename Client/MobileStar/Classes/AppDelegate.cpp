#include "AppDelegate.h"
#include "GameWorld.h"
#include "DefineHeader.h"

#include "GameClient.h"
#include "NetworkLayer.h"
#include "LoginScene.h"
#include "BasicDefines.h"
#include "HelloWorldScene.h"

USING_NS_CC;



AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Starcraft");
        director->setOpenGLView(glview);
    }
    
    glview->setDesignResolutionSize(DISPLAY_WIDTH, DISPLAY_HEIGHT, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    register_all_packages();
    
    ///////////////////////////로그인 서버로 접속

    if(PLAY_ALONE) {
        glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);
        auto scene = GameWorld::createScene();
        
        // run
        director->runWithScene(scene);
    }else{
        GameClient::GetInstance().Initialize();
        
        auto networkBGLayer = NetworkLayer::create();
        networkBGLayer->retain();
        
        GameClient::GetInstance().currentScene = NO_SCENE_NOW;
        
        Scene* scene = HelloWorld::createScene();
        
        scene->addChild(networkBGLayer, 0, TAG_NETWORK_LAYER);
        Director::getInstance()->runWithScene(scene);
        
        networkBGLayer->handler->frontSendFirstConnectReq();
    }

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
