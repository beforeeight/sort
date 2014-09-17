/*
 * FinishScene.h
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#ifndef FINISHSCENE_H_
#define FINISHSCENE_H_

#include "Resources.h"

class FinishLayer: public cocos2d::CCMenu {
public:
	FinishLayer();

	virtual ~FinishLayer();

	virtual bool init();

	CREATE_FUNC (FinishLayer)

	static CCScene * scene();

private:

	void onAgainItem(CCObject *object);

	void onShareItem(CCObject *object);

	void onBackItem(CCObject *object);
};

#endif /* FINISHSCENE_H_ */
