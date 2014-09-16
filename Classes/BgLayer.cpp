/*
 * BgLayer.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "BgLayer.h"

USING_NS_CC;

BgLayer::BgLayer() {

}

BgLayer::~BgLayer() {
}

bool BgLayer::init() {
	if (CCLayer::init()) {
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		this->setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));
//		float width = s.width / 2;
//		float height = s.height / 2;
		/* initialize the counter */
		CCTextureCache *cache = CCTextureCache::sharedTextureCache();
		/*-- 背景 --*/
		CCSprite *bg = CCSprite::createWithTexture(
				cache->textureForKey(("bg_back.png")));
		this->addChild(bg);
		return true;
	} else {
		return false;
	}
}
