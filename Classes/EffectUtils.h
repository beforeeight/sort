/*
 * EffectUtils.h
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#ifndef EFFECTUTILS_H_
#define EFFECTUTILS_H_

#include "cocos2d.h"

USING_NS_CC;

namespace effect {

void clickButtonSound(CCMenuItemImage *bg, CCSprite *txt);

void clickButton(CCMenuItem *p_sender);

void clickButton(CCMenuItem *p_sender, CCObject *target, SEL_CallFunc func);

class EffectUtil: public CCObject {

public:
	void clickButtonFinish(CCNode *p_sender);
};

}

#endif /* EFFECTUTILS_H_ */
