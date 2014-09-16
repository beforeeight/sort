/*
 * FinishScene.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "FinishScene.h"
#include "BgLayer.h"

FinishLayer::FinishLayer() {
	// TODO Auto-generated constructor stub

}

FinishLayer::~FinishLayer() {
	// TODO Auto-generated destructor stub
}

bool FinishLayer::init() {
	if (CCMenu::init()) {

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
