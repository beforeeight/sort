/*
 * GameScene.h
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "Resources.h"

#define ITEM_NUM 5

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

	CCSprite * item[ITEM_NUM + 1];

	void moveForward();

	void initPosition();

	void onPauseItem(CCObject *object);

	void onResumeItem(CCObject *object);

	void onBackItem(CCObject *object);

	CCLayer * createPauseButtonLayer();

	void createPauseLayer();

	CCSprite* createNewItem();

	void enable();
};

#endif /* GAMESCENE_H_ */
