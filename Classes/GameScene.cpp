/*
 * GameScene.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "GameScene.h"
#include "BgLayer.h"
#include "EffectUtils.h"
#include "Menu.h"
#include "FinishScene.h"

#define TAG_LAYER_GAME 0
#define TAG_LAYER_PAUSE 1

GameLayer::GameLayer() {
	if (Context::sharedContext()->firstRun()) {
		running = false;
	} else {
		running = true;
	}
}

GameLayer::~GameLayer() {
	// TODO Auto-generated destructor stub
}

bool GameLayer::init() {
	if (CCLayer::init()) {
		LOCAL_CONTEXT->clearScore();
		this->setTouchMode(kCCTouchesOneByOne);
		this->setTouchEnabled(true);
		this->scheduleUpdate();
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		this->setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));

		return true;

	} else {
		return false;
	}
}

CCScene * GameLayer::scene() {
	CCScene *scene = CCScene::create();
	if (scene && scene->init()) {
		if (scene) {
			/*-- 背景 --*/
			scene->addChild(BgLayer::create());
			GameLayer *layer = GameLayer::create();
			scene->addChild(layer, TAG_LAYER_GAME);
			scene->addChild(layer->createPauseButtonLayer());
		}
		return scene;
	} else {
		CC_SAFE_DELETE(scene);
		return NULL;
	}

}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	if (running) {
		CCDirector::sharedDirector()->replaceScene(FinishLayer::scene());
		CCSize winsize = CCDirector::sharedDirector()->getWinSize();
		CCPoint location = pTouch->getLocation();
		CCNode *node = 0;
		if (location.x < winsize.width / 2) {

		} else {

		}
		return true;
	} else {
		return false;
	}
}

void GameLayer::onPauseItem(CCObject *object) {
	if (running) {
		this->running = false;
		effect::clickButton((CCMenuItem*) object);
		createPauseLayer();
	}
}

void GameLayer::onResumeItem(CCObject *object) {
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(
	TAG_LAYER_PAUSE);
	this->running = true;
	//effect::clickButton((CCMenuItem*) object);

}

void GameLayer::onBackItem(CCObject *object) {
	CCDirector::sharedDirector()->replaceScene(MenuLayer::scene());
}

CCLayer * GameLayer::createPauseButtonLayer() {
	CCMenu *layer = CCMenu::create();
	//暂停
	CCMenuItemImage *pausebg = CCMenuItemImage::create(("btn_small.png"),
			("btn_small.png"));
	CCSprite *pausetxt = CCSprite::create("btn_pause.png");
	pausebg->setTarget(this, menu_selector(GameLayer::onPauseItem));
	pausebg->setAnchorPoint(ccp(0, 1));
	pausebg->setPosition(ccpp(-0.5, 0.5));
	pausetxt->setPosition(
			ccp(pausebg->getContentSize().width / 2,
					pausebg->getContentSize().height / 2));
	pausetxt->setAnchorPoint(ccp(0.5, 0.5));
	pausebg->addChild(pausetxt);
	layer->addChild(pausebg);
	return layer;
}

void GameLayer::createPauseLayer() {
	Context *context = Context::sharedContext();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	/* 半透明层 */
	CCLayerColor* pauseLayer = CCLayerColor::create();
	pauseLayer->setContentSize(CCSizeMake(s.width, s.height));
	pauseLayer->setColor(ccc3(0, 0, 0));
	pauseLayer->setOpacity(150);
	CCMenu *menu = CCMenu::create();
	//回到游戏
	CCMenuItemImage *resumebg = CCMenuItemImage::create(("btn_big.png"),
			("btn_big.png"));
	resumebg->setTarget(this, menu_selector(GameLayer::onResumeItem));
	CCMenuItemFont *resumetxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_resume")->getCString());
	resumebg->setAnchorPoint(ccp(0.5, 0.5));
	resumebg->setPosition(ccpp(0, 0));
	resumetxt->setPosition(
			ccp(resumebg->getContentSize().width / 2,
					resumebg->getContentSize().height / 2));
	resumetxt->setAnchorPoint(ccp(0.5, 0.5));
	resumetxt->setColor(context->getFontColor());
	resumebg->addChild(resumetxt);
	menu->addChild(resumebg);

	//回到菜单
	CCMenuItemImage *backbg = CCMenuItemImage::create(("btn_big.png"),
			("btn_big.png"));
	backbg->setTarget(this, menu_selector(GameLayer::onBackItem));
	CCMenuItemFont *backtxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_back")->getCString());
	backbg->setAnchorPoint(ccp(0.5, 0.5));
	backbg->setPosition(ccpp(0, -0.12));
	backtxt->setPosition(
			ccp(backbg->getContentSize().width / 2,
					backbg->getContentSize().height / 2));
	backtxt->setAnchorPoint(ccp(0.5, 0.5));
	backtxt->setColor(context->getFontColor());
	backbg->addChild(backtxt);
	menu->addChild(backbg);

	pauseLayer->addChild(menu);
	CCDirector::sharedDirector()->getRunningScene()->addChild(pauseLayer, 0,
	TAG_LAYER_PAUSE);
}
