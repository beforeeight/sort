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
#define TAG_SCORE 3

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

		/*-- 计分 --*/
		CCLabelTTF *score = CCLabelTTF::create(CCString::createWithFormat("%d", LOCAL_CONTEXT->getScore())->getCString(),LOCAL_RESOURCES->valueByKey("font")->getCString(),LOCAL_RESOURCES->valueByKey("font_size")->floatValue());
		score->setColor(LOCAL_CONTEXT->getFontColor());
		score->setAnchorPoint(ccp(1, 1));
		score->setPosition(ccpp(0.5,0.5));
		this->addChild(score,0,TAG_SCORE);

		/*-- 倒计时 --*/
		CCSprite *timeBack = CCSprite::create("time_back.png");
		CCSprite *timeFront = CCSprite::create("time_front.png");
		timeBack->setAnchorPoint(ccp(0.5,0.5));
		timeFront->setAnchorPoint(ccp(0.5,0.5));
		timeBack->setPosition(ccpp(0,0.32));
		timeFront->setPosition(ccpp(0,0.32));
		this->addChild(timeBack);
		this->addChild(timeFront);

		/*-- 放瓶子 --*/
		CCSprite *block1 = CCSprite::create("bg_block.png");
		block1->setAnchorPoint(ccp(0.5,0));
		block1->setPosition(ccpp(0,-0.4));
		this->addChild(block1);

		initPosition();
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
		CCSprite *lastItem = item[0];
		//CCDirector::sharedDirector()->replaceScene(FinishLayer::scene());
		CCSize winsize = CCDirector::sharedDirector()->getWinSize();
		CCPoint location = pTouch->getLocation();
		if (location.x < winsize.width / 2) {
			//瓶子 6-10
			if ((long) lastItem->getUserData() >= 6) {
				LOCAL_CONTEXT->increaseScore();((CCLabelTTF*)this->getChildByTag(TAG_SCORE))->setString(CCString::createWithFormat("%d", LOCAL_CONTEXT->getScore())->getCString());
				moveForward();
				lastItem->runAction(
						CCSequence::createWithTwoActions(
								CCMoveBy::create(0.3, ccpp(-0.32, 0)),
								CCFadeOut::create(0.3)));

			}
		} else {
			//罐子 1-5
			if ((long) lastItem->getUserData() <= 5) {
				LOCAL_CONTEXT->increaseScore();
				((CCLabelTTF*)this->getChildByTag(TAG_SCORE))->setString(CCString::createWithFormat("%d", LOCAL_CONTEXT->getScore())->getCString());
				moveForward();
				lastItem->runAction(
						CCSequence::createWithTwoActions(
								CCMoveBy::create(0.3, ccpp(0.32, 0)),
								CCFadeOut::create(0.3)));
			}
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
	effect::clickButton((CCMenuItem*) object);
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(
	TAG_LAYER_PAUSE);
	this->running = true;
}

void GameLayer::onBackItem(CCObject *object) {
	effect::clickButton((CCMenuItem*) object);
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

void GameLayer::moveForward() {
	running = false;
	item[ITEM_NUM] = createNewItem();
	item[ITEM_NUM]->setPosition(item[ITEM_NUM - 1]->getPosition());
	for (unsigned int i = ITEM_NUM - 1; i > 0; i--) {
		item[i]->runAction(
				CCSequence::createWithTwoActions(
						CCMoveTo::create(0.5f, item[i - 1]->getPosition()),
						CCCallFunc::create(this,
								callfunc_selector(GameLayer::enable))));
	}
	for (unsigned int i = 0; i < ITEM_NUM; i++) {
		item[i] = item[i + 1];
		item[i]->setZOrder(ITEM_NUM - i);
	}
}

void GameLayer::initPosition() {
	for (unsigned int i = 0; i < ITEM_NUM; i++) {
		item[i] = createNewItem();
		item[i]->setZOrder(ITEM_NUM - i);
		item[i]->setPosition(ccpp(0, (-0.38 + 0.11 * i)));
	}
}

CCSprite* GameLayer::createNewItem() {
	int i = rand() % 10 + 1;
	CCString *s = CCString::createWithFormat("item_%d.png", i);
	CCSprite* itemnew = CCSprite::create(s->getCString());
	itemnew->setUserData((void *) i);
	itemnew->setAnchorPoint(ccp(0.5, 0));
	this->addChild(itemnew);
	return itemnew;
}

void GameLayer::enable() {
	this->running = true;
}
