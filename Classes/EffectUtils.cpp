/*
 * EffectUtils.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "EffectUtils.h"
#include "Resources.h"

using namespace effect;

void effect::clickButtonSound(CCMenuItemImage *bg, CCSprite *txt) {
	Context *context = Context::sharedContext();
	if (context->isSound()) {
		context->offSound();
		txt->setTexture(
				CCTextureCache::sharedTextureCache()->textureForKey(
						"btn_sound_off.png"));

	} else {
		context->onSound();
		txt->setTexture(
				CCTextureCache::sharedTextureCache()->textureForKey(
						"btn_sound_on.png"));
	}
	effect::clickButton(bg);
}

void effect::clickButton(CCMenuItem *p_sender) {
	p_sender->setEnabled(false);
	p_sender->runAction(
			CCSequence::create(CCScaleBy::create(0.1f, 0.95f),
					CCScaleBy::create(0.1f, 10.0 / 9.5f),
					CCCallFuncN::create(p_sender,
							callfuncN_selector(EffectUtil::clickButtonFinish)),
					NULL));
}

void effect::clickButton(CCMenuItem *p_sender, CCObject *target,
		SEL_CallFunc func) {
	p_sender->setEnabled(false);
	p_sender->runAction(
			CCSequence::create(CCScaleBy::create(0.1f, 0.95f),
					CCScaleBy::create(0.1f, 10.0 / 9.5f),
					CCCallFunc::create(target, func),
					CCCallFuncN::create(p_sender,
							callfuncN_selector(EffectUtil::clickButtonFinish)),
					NULL));
}
void EffectUtil::clickButtonFinish(CCNode *p_sender) {
	((CCMenuItemImage *) p_sender)->setEnabled(true);
}
