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
class TimerSprite: public CCProgressTimer {
public:
	TimerSprite();

	virtual ~TimerSprite();

	virtual bool init();

	CREATE_FUNC (TimerSprite)

	void reset();

	void setTarget(CCObject * m_listener, SEL_CallFunc m_callfunc);

	void start();

	void resume();

	void pause();
private:
	unsigned int time;

	CCObject * m_listener;

	SEL_CallFunc m_callfunc;

	void timeout();

	void initTimer();
};

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

	CCSprite * items[ITEM_NUM + 1];

	CCSprite * blocks[ITEM_NUM + 1];

	TimerSprite *timer;

	void moveForward();

	void initPosition();

	void onPauseItem(CCObject *object);

	void onResumeItem(CCObject *object);

	void onBackItem(CCObject *object);

	CCLayer * createPauseButtonLayer();

	void createHelpLayer();

	void removeHelpLayer();

	void createPauseLayer();

	CCSprite* createNewItem();

	CCSprite* createNewBlock();

	void disable();

	void enable();

	void gameover();

	void correct(float offset);

	void mistake(float offset);
	void resume();
	void backMenu();
};

#endif /* GAMESCENE_H_ */
