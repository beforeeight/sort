#include "AppDelegate.h"
#include "Menu.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	pDirector->setOpenGLView(pEGLView);
	LocalResources::sharedResources();
	// turn on display FPS
	pDirector->setDisplayStats(false);
	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	pDirector->runWithScene(MenuLayer::scene());
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	CCDirector::sharedDirector()->startAnimation();
}
