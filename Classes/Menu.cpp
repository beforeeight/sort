/*
 * Menu.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: zhangchen
 */

#include "Menu.h"

MenuLayer::MenuLayer() {
	// TODO Auto-generated constructor stub

}

MenuLayer::~MenuLayer() {
	// TODO Auto-generated destructor stub
}

bool MenuLayer::init() {
	if (CCMenu::init()) {
		CCMenuItem *item = CCMenuItemFont::create("abcd");
		item->setColor(ccc3(255, 0, 0));
		item->setAnchorPoint(ccp(1, 1));
		item->setPosition(ccpp(0.5, 0.5));
		this->addChild(item);
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
}

void MenuLayer::enableButton(CCNode *p_sender) {
}

void MenuLayer::onFeedbackItem(CCObject *object) {
}

void MenuLayer::onStartItem(CCObject *object) {
}

void MenuLayer::onRankingItem(CCObject *object) {
}

void MenuLayer::onMoreItem(CCObject *object) {
}
