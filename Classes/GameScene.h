/*
 * GameScene.h
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "Resources.h"

class GameLayer: public CCLayer {
public:
	GameLayer();

	virtual ~GameLayer();

	virtual bool init();

	CREATE_FUNC (GameLayer)

	static CCScene * scene();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

private:
	bool running;

	void onPauseItem(CCObject *object);

	void onResumeItem(CCObject *object);

	void onBackItem(CCObject *object);

	CCLayer * createPauseButtonLayer();

	void createPauseLayer();
};

#endif /* GAMESCENE_H_ */
