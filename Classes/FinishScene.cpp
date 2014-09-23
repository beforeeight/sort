/*
 * FinishScene.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "FinishScene.h"
#include "BgLayer.h"
#include "Menu.h"
#include "GameScene.h"
#include "EffectUtils.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

FinishLayer::FinishLayer() {
	// TODO Auto-generated constructor stub

}

FinishLayer::~FinishLayer() {
	// TODO Auto-generated destructor stub
}

bool FinishLayer::init() {
	if (CCMenu::init()) {
		/*-- 声音 --*/
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		LOCAL_CONTEXT->playEffect("gameover.mp3");
		/*-- 文字 --*/
		CCMenuItemImage *gameover = CCMenuItemImage::create("gameover.png",
				"gameover.png");
		gameover->setAnchorPoint(ccp(0.5, 0.5));
		gameover->setPosition(ccpp(0, 0.32));
		this->addChild(gameover);

		CCMenuItemImage *resultbg = CCMenuItemImage::create("result_bg.png",
				"result_bg.png");
		resultbg->setAnchorPoint(ccp(0.5, 0.5));
		resultbg->setPosition(ccpp(0, 0.146));
		this->addChild(resultbg);

		CCMenuItemFont *scoreLabel = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_score")->getCString());
		scoreLabel->setColor(LOCAL_CONTEXT->getFontColor());
		scoreLabel->setAnchorPoint(ccp(0, 0.5));
		scoreLabel->setPosition(
				ccp(resultbg->getContentSize().width * 0.1,
						resultbg->getContentSize().height * 0.75));
		CCMenuItemFont *score = CCMenuItemFont::create(
				CCString::createWithFormat("%d", LOCAL_CONTEXT->getScore())->getCString());
		score->setColor(LOCAL_CONTEXT->getFontColor());
		score->setAnchorPoint(ccp(0, 0.5));
		score->setPosition(
				ccp(resultbg->getContentSize().width * 0.55,
						resultbg->getContentSize().height * 0.75));

		CCMenuItemFont *highScoreLabel = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_highscore")->getCString());
		highScoreLabel->setColor(LOCAL_CONTEXT->getFontColor());
		highScoreLabel->setAnchorPoint(ccp(0, 0.5));
		highScoreLabel->setPosition(
				ccp(resultbg->getContentSize().width * 0.1,
						resultbg->getContentSize().height * 0.25));
		CCMenuItemFont *highScore = CCMenuItemFont::create(
				CCString::createWithFormat("%d", LOCAL_CONTEXT->getHighScore())->getCString());
		highScore->setColor(LOCAL_CONTEXT->getFontColor());
		highScore->setAnchorPoint(ccp(0, 0.5));
		highScore->setPosition(
				ccp(resultbg->getContentSize().width * 0.55,
						resultbg->getContentSize().height * 0.25));
		resultbg->addChild(scoreLabel);
		resultbg->addChild(highScoreLabel);
		resultbg->addChild(score);
		resultbg->addChild(highScore);

		if (LOCAL_CONTEXT->isNewRecord()) {
			CCMenuItemImage *newrecord = CCMenuItemImage::create("newrecord.png","newrecord.png");
			newrecord->setAnchorPoint(ccp(0, 0.5));
			newrecord->setPosition(score->getPosition()+ccp(score->getContentSize().width,0)+ ccpp(0.1,0));
			resultbg->addChild(newrecord);
		}

		//再来一次
		CCMenuItemImage *againbg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		againbg->setTarget(againbg, menu_selector(FinishLayer::onAgainItem));
		CCMenuItemFont *againtxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_again")->getCString());
		againbg->setAnchorPoint(ccp(0.5, 0.5));
		againbg->setPosition(ccpp(0, -0.07));
		againtxt->setPosition(
				ccp(againbg->getContentSize().width / 2,
						againbg->getContentSize().height / 2));
		againtxt->setAnchorPoint(ccp(0.5, 0.5));
		againtxt->setColor(LOCAL_CONTEXT->getFontColor());
		againbg->addChild(againtxt);
		this->addChild(againbg);

		//分享
		CCMenuItemImage *sharebg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		sharebg->setTarget(sharebg, menu_selector(FinishLayer::onShareItem));
		CCMenuItemFont *sharetxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_share")->getCString());
		sharebg->setAnchorPoint(ccp(0.5, 0.5));
		sharebg->setPosition(ccpp(0, -0.19));
		sharetxt->setPosition(
				ccp(sharebg->getContentSize().width / 2,
						sharebg->getContentSize().height / 2));
		sharetxt->setAnchorPoint(ccp(0.5, 0.5));
		sharetxt->setColor(LOCAL_CONTEXT->getFontColor());
		sharebg->addChild(sharetxt);
		this->addChild(sharebg);

		//返回菜单
		CCMenuItemImage *backbg = CCMenuItemImage::create(("btn_big.png"),
				("btn_big.png"));
		backbg->setTarget(this, menu_selector(FinishLayer::onBackItem));
		CCMenuItemFont *backtxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_back")->getCString());
		backbg->setAnchorPoint(ccp(0.5, 0.5));
		backbg->setPosition(ccpp(0, -0.31));
		backtxt->setPosition(
				ccp(backbg->getContentSize().width / 2,
						backbg->getContentSize().height / 2));
		backtxt->setAnchorPoint(ccp(0.5, 0.5));
		backtxt->setColor(LOCAL_CONTEXT->getFontColor());
		backbg->addChild(backtxt);
		this->addChild(backbg);

		return true;
	} else {
		return false;
	}
}

CCScene * FinishLayer::scene() {
	CCScene *scene = CCScene::create();
	if (scene && scene->init()) {
		if (scene) {
			/*-- 背景 --*/
			scene->addChild(BgLayer::create());
			FinishLayer *layer = FinishLayer::create();
			scene->addChild(layer);
		}
		return scene;
	} else {
		CC_SAFE_DELETE(scene);
		return NULL;
	}
}

void FinishLayer::onAgainItem(CCObject *object) {
	effect::clickButton((CCMenuItem*) object);
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFadeDown::create(0.5f, GameLayer::scene()));
}

void FinishLayer::onShareItem(CCObject *object) {
	effect::clickButton((CCMenuItem*) object);
	CCLog(LOCAL_RESOURCES->valueByKey("i18n_share")->getCString());
}

void FinishLayer::onBackItem(CCObject *object) {
	effect::clickButton((CCMenuItem*) object);
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFadeDown::create(0.5f, MenuLayer::scene()));
}
