/*
 * Menu.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: zhangchen
 */

#include "Menu.h"
#include "SimpleAudioEngine.h"
#include "BgLayer.h"
#include "EffectUtils.h"
#include "GameScene.h"
using namespace CocosDenshion;

MenuLayer::MenuLayer() {
}

MenuLayer::~MenuLayer() {
}

bool MenuLayer::init() {
	if (CCMenu::init()) {
		Context *context = Context::sharedContext();
		//声音
		CCMenuItemImage *soundbg = CCMenuItemImage::create(("btn_small.png"),
				("btn_small.png"));
		CCSprite *soundtxt;
		if (context->isSound()) {
			soundtxt = CCSprite::create("btn_sound_on.png");
		} else {
			soundtxt = CCSprite::create("btn_sound_off.png");
		}
		soundbg->setTarget(soundtxt, menu_selector(MenuLayer::onSoundItem));
		soundbg->setAnchorPoint(ccp(0, 1));
		soundbg->setPosition(ccpp(-0.49, 0.49));
		soundtxt->setPosition(
				ccp(soundbg->getContentSize().width / 2,
						soundbg->getContentSize().height / 2));
		soundtxt->setAnchorPoint(ccp(0.5, 0.5));
		soundbg->addChild(soundtxt);
		this->addChild(soundbg);
		//标题
		CCMenuItemImage *gamename = CCMenuItemImage::create(("name.png"),
				("name.png"));
		gamename->setAnchorPoint(ccp(0.5, 0.5));
		gamename->setPosition(ccpp(0, 0.302));
		this->addChild(gamename);

		//开始游戏
		CCMenuItemImage *startbg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		startbg->setTarget(startbg, menu_selector(MenuLayer::onStartItem));
		CCMenuItemFont *starttxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_start")->getCString());
		startbg->setAnchorPoint(ccp(0.5, 0.5));
		startbg->setPosition(ccpp(0, 0));
		starttxt->setPosition(
				ccp(startbg->getContentSize().width / 2,
						startbg->getContentSize().height / 2));
		starttxt->setAnchorPoint(ccp(0.5, 0.5));
		starttxt->setColor(context->getFontColor());
		startbg->addChild(starttxt);
		this->addChild(startbg);

		//排行榜
		CCMenuItemImage *rankingbg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		rankingbg->setTarget(rankingbg,
				menu_selector(MenuLayer::onRankingItem));
		CCMenuItemFont *rankingtxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_ranking")->getCString());
		rankingbg->setAnchorPoint(ccp(0.5, 0.5));
		rankingbg->setPosition(ccpp(0, -0.12));
		rankingtxt->setPosition(
				ccp(rankingbg->getContentSize().width / 2,
						rankingbg->getContentSize().height / 2));
		rankingtxt->setAnchorPoint(ccp(0.5, 0.5));
		rankingtxt->setColor(context->getFontColor());
		rankingbg->addChild(rankingtxt);
		this->addChild(rankingbg);

		//更多游戏
		CCMenuItemImage *morebg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		morebg->setTarget(morebg, menu_selector(MenuLayer::onMoreItem));
		CCMenuItemFont *moretxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_more")->getCString());
		morebg->setAnchorPoint(ccp(0.5, 0.5));
		morebg->setPosition(ccpp(0, -0.24));
		moretxt->setPosition(
				ccp(morebg->getContentSize().width / 2,
						morebg->getContentSize().height / 2));
		moretxt->setAnchorPoint(ccp(0.5, 0.5));
		moretxt->setColor(context->getFontColor());
		morebg->addChild(moretxt);
		this->addChild(morebg);

		//反馈
		CCMenuItemImage *feedbacktbg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		feedbacktbg->setTarget(feedbacktbg,
				menu_selector(MenuLayer::onFeedbackItem));
		CCMenuItemFont *feedbacktxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_feedback")->getCString());
		feedbacktbg->setAnchorPoint(ccp(0.5, 0.5));
		feedbacktbg->setPosition(ccpp(0, -0.36));
		feedbacktxt->setPosition(
				ccp(feedbacktbg->getContentSize().width / 2,
						feedbacktbg->getContentSize().height / 2));
		feedbacktxt->setAnchorPoint(ccp(0.5, 0.5));
		feedbacktxt->setColor(context->getFontColor());
		feedbacktbg->addChild(feedbacktxt);
		this->addChild(feedbacktbg);
		return true;
	} else {
		return false;
	}
}

CCScene * MenuLayer::scene() {
	CCScene *scene = CCScene::create();
	if (scene && scene->init()) {
		if (scene) {
			/*-- 背景 --*/
			scene->addChild(BgLayer::create());

			MenuLayer *layer = MenuLayer::create();
			scene->addChild(layer);
		}
		return scene;
	} else {
		CC_SAFE_DELETE(scene);
		return NULL;
	}
}

void MenuLayer::onSoundItem(CCObject *object) {
	CCMenuItemImage *bg = (CCMenuItemImage *) object;
	CCSprite *txt = (CCSprite *) this;
	effect::clickButtonSound(bg, txt);
}

void MenuLayer::onFeedbackItem(CCObject *object) {
	CCMenuItemImage *bg = (CCMenuItemImage *) object;
	effect::clickButton(bg);
}

void MenuLayer::onStartItem(CCObject *object) {
	CCMenuItemImage *bg = (CCMenuItemImage *) object;
	effect::clickButton(bg);
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFadeDown::create(0.5f, GameLayer::scene()));
}

void MenuLayer::onRankingItem(CCObject *object) {
	//TODO::排行榜
	CCMenuItemImage *bg = (CCMenuItemImage *) object;
	effect::clickButton(bg);
}

void MenuLayer::onMoreItem(CCObject *object) {
	//TODO::更多游戏
	CCMenuItemImage *bg = (CCMenuItemImage *) object;
	effect::clickButton(bg);
}
