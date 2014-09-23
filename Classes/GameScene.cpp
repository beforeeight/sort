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
#define TAG_LAYER_HELP 4

GameLayer::GameLayer() :
		running(true) {
	timer = TimerSprite::create();
	CC_SAFE_RETAIN(timer);
}

GameLayer::~GameLayer() {
	CC_SAFE_RELEASE(timer);
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
		timeBack->setAnchorPoint(ccp(0.5,0.5));
		timeBack->setPosition(ccpp(0,0.32));
		this->addChild(timeBack);
		timer->setAnchorPoint(ccp(0.5,0.5));
		timer->setPosition(ccp(timeBack->getContentSize().width/2,timeBack->getContentSize().height/2));
		timer->setTarget(this,callfunc_selector(GameLayer::gameover));
		timeBack->addChild(timer);

		/*-- 放瓶子 --*/
		initPosition();

		if (LOCAL_CONTEXT->isFirstRun()) {
			this->createHelpLayer();
		} else {
			timer->start();
		}
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

void GameLayer::correct(float offset) {
	CCSprite *lastItem = items[0];
	CCSprite *lastBlock = blocks[0];
	LOCAL_CONTEXT->increaseScore();
	((CCLabelTTF*) this->getChildByTag(TAG_SCORE))->setString(
			CCString::createWithFormat("%d", LOCAL_CONTEXT->getScore())->getCString());
	moveForward();
	lastItem->runAction(
			CCSequence::create(CCMoveBy::create(0.3, ccpp(offset, 0)),
					CCFadeOut::create(0.3),
					CCCallFunc::create(lastItem,
							callfunc_selector(CCSprite::removeFromParent)),
					NULL));
	lastBlock->runAction(
			CCSequence::createWithTwoActions(
					CCMoveTo::create(0.3, ccpp(0, -0.5)),
					CCCallFunc::create(lastBlock,
							callfunc_selector(CCSprite::removeFromParent))));
	timer->reset();
}

void GameLayer::mistake(float offset) {
	CCSprite *lastItem = items[0];
	CCSprite *wrong = CCSprite::create("bg_wrong.png");
	wrong->setAnchorPoint(ccp(0.5, 0));
	wrong->setPosition(lastItem->getPosition() + ccpp(offset, 0));
	this->addChild(wrong);
	lastItem->runAction(
			CCSequence::createWithTwoActions(
					CCMoveBy::create(0.6, ccpp(offset, 0)),
					CCCallFunc::create(this,
							callfunc_selector(GameLayer::gameover))));
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	if (LOCAL_CONTEXT->firstRun()) { //第一次启动，点击取消帮助画面层
		this->removeHelpLayer();
		return false;
	}

	if (running) {
		disable();
		CCSprite *lastItem = items[0];
		long itemnum = (long) lastItem->getUserData();
		//CCDirector::sharedDirector()->replaceScene(FinishLayer::scene());
		CCSize winsize = CCDirector::sharedDirector()->getWinSize();
		CCPoint location = pTouch->getLocation();
		if (location.x < winsize.width / 2) {
			//瓶子 6-10
			if (itemnum >= 6) {
				correct(-0.32);
			} else {
				mistake(-0.32);
			}
		} else {
			//罐子 1-5
			if (itemnum <= 5) {
				correct(0.32);
			} else {
				mistake(0.32);
			}
		}
		return true;
	} else {
		return false;
	}
}

void GameLayer::onPauseItem(CCObject *object) {
	if (running) {
		disable();
		timer->pause();
		effect::clickButton((CCMenuItem*) object);
		createPauseLayer();
	}
}

void GameLayer::resume() {
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(
	TAG_LAYER_PAUSE);
	timer->resume();
	enable();
}

void GameLayer::onResumeItem(CCObject *object) {
	if (!running) {
		effect::clickButton((CCMenuItem*) object, this,
				callfunc_selector(GameLayer::resume));
	}
}

void GameLayer::backMenu() {
	CCDirector::sharedDirector()->replaceScene(MenuLayer::scene());
}

void GameLayer::onBackItem(CCObject *object) {
	effect::clickButton((CCMenuItem*) object, this,
			callfunc_selector(GameLayer::backMenu));
}

CCLayer * GameLayer::createPauseButtonLayer() {
	CCMenu *layer = CCMenu::create();
	//暂停
	CCMenuItemImage *pausebg = CCMenuItemImage::create(("btn_small.png"),
			("btn_small.png"));
	CCSprite *pausetxt = CCSprite::create("btn_pause.png");
	pausebg->setTarget(this, menu_selector(GameLayer::onPauseItem));
	pausebg->setAnchorPoint(ccp(0, 1));
	pausebg->setPosition(ccpp(-0.49, 0.49));
	pausetxt->setPosition(
			ccp(pausebg->getContentSize().width / 2,
					pausebg->getContentSize().height / 2));
	pausetxt->setAnchorPoint(ccp(0.5, 0.5));
	pausebg->addChild(pausetxt);
	layer->addChild(pausebg);
	return layer;
}

void GameLayer::createHelpLayer() {
	CCLayer *layer = CCLayer::create();
	CCSprite *left = CCSprite::create("help_left.png");
	left->setAnchorPoint(ccp(0.5, 0));
	left->setPosition(ccpp(-0.33, -0.345));
	CCSprite *right = CCSprite::create("help_right.png");
	right->setAnchorPoint(ccp(0.5, 0));
	right->setPosition(ccpp(0.33, -0.345));
	layer->addChild(left);
	layer->addChild(right);
	this->addChild(layer, 0, TAG_LAYER_HELP);
}

void GameLayer::removeHelpLayer() {
	this->removeChildByTag(TAG_LAYER_HELP);
	timer->start();
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
	disable();
	items[ITEM_NUM] = createNewItem();
	blocks[ITEM_NUM] = createNewBlock();
	items[ITEM_NUM]->setPosition(items[ITEM_NUM - 1]->getPosition());
	blocks[ITEM_NUM]->setPosition(blocks[ITEM_NUM - 1]->getPosition());
	/* 向下移动的动画 */
	for (unsigned int i = ITEM_NUM - 1; i > 0; i--) {
		items[i]->runAction(
				CCSequence::createWithTwoActions(
						CCMoveTo::create(0.1f, items[i - 1]->getPosition()),
						CCCallFunc::create(this,
								callfunc_selector(GameLayer::enable))));
		blocks[i]->runAction(
				CCMoveTo::create(0.1f, blocks[i - 1]->getPosition()));
	}
	/* 重新设置数组存储顺序和前后遮挡排列顺序 */
	for (unsigned int i = 0; i < ITEM_NUM; i++) {
		items[i] = items[i + 1];
		items[i]->setZOrder(ITEM_NUM - i);

		blocks[i] = blocks[i + 1];
		blocks[i]->setZOrder(ITEM_NUM - i);
	}
}

void GameLayer::initPosition() {
	for (unsigned int i = 0; i < ITEM_NUM; i++) {
		items[i] = createNewItem();
		items[i]->setZOrder(ITEM_NUM - i);
		items[i]->setPosition(ccpp(0, (-0.38 + 0.11 * i)));

		blocks[i] = createNewBlock();
		blocks[i]->setZOrder(ITEM_NUM - i);
		blocks[i]->setPosition(items[i]->getPosition() + ccpp(0, -0.02));
	}
}

CCSprite* GameLayer::createNewItem() {
	long i = rand() % 10 + 1;
	CCString *s = CCString::createWithFormat("item_%ld.png", i);
	CCSprite* itemnew = CCSprite::create(s->getCString());
	itemnew->setUserData((void *) i);
	itemnew->setAnchorPoint(ccp(0.5, 0));
	itemnew->runAction(CCFadeIn::create(1.0f));
	this->addChild(itemnew);
	return itemnew;
}

CCSprite* GameLayer::createNewBlock() {
	CCSprite *block = CCSprite::create("bg_block.png");
	block->setAnchorPoint(ccp(0.5, 0));
	this->addChild(block);
	return block;
}

void GameLayer::disable() {
	this->running = false;
	timer->pause();
}

void GameLayer::enable() {
	this->running = true;
	timer->resume();
}

void GameLayer::gameover() {
	LOCAL_CONTEXT->save();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f, FinishLayer::scene()));
}

/*-- 计时器 --*/
TimerSprite::TimerSprite() {
	initTimer();
}

TimerSprite::~TimerSprite() {
}

bool TimerSprite::init() {
	if (TimerSprite::initWithSprite(CCSprite::create("time_front.png"))) {
		this->setType(kCCProgressTimerTypeBar);
		this->setMidpoint(ccp(0, 0));
		this->setBarChangeRate(ccp(1, 0));
		return true;
	} else {
		return false;
	}
}

void TimerSprite::initTimer() {
	int score = LOCAL_CONTEXT->getScore();
	if (score <= 30) {
		this->time = 5;
	} else if (score <= 60) {
		this->time = 4;
	} else if (score <= 100) {
		this->time = 3;
	} else {
		this->time = 2;
	}
}

void TimerSprite::reset() {
	this->stopAllActions();
	initTimer();
	this->runAction(
			CCSequence::createWithTwoActions(
					CCProgressFromTo::create(time, 100.0f, 0.0f),
					CCCallFunc::create(this,
							callfunc_selector(TimerSprite::timeout))));
}

void TimerSprite::start() {
	reset();
}

void TimerSprite::resume() {
	this->resumeSchedulerAndActions();
}

void TimerSprite::pause() {
	this->pauseSchedulerAndActions();
}

void TimerSprite::timeout() {
	if (m_listener && m_callfunc) {
		(m_listener->*m_callfunc)();
	}
}

void TimerSprite::setTarget(CCObject * target, SEL_CallFunc function) {
	this->m_listener = target;
	this->m_callfunc = function;
}
