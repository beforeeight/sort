/*
 * Menu.h
 *
 *  Created on: Sep 12, 2014
 *      Author: zhangchen
 */

#ifndef MENU_H_
#define MENU_H_

#include "Resources.h"

class MenuLayer: public CCMenu {
public:
	MenuLayer();

	virtual ~MenuLayer();

	virtual bool init();

	CREATE_FUNC(MenuLayer)

	static CCScene * scene();

private:

	void onSoundItem(CCObject *object);

	void onStartItem(CCObject *object);

	void onRankingItem(CCObject *object);

	void onMoreItem(CCObject *object);

	void onFeedbackItem(CCObject *object);

	void enableButton(CCNode *p_sender);
};

#endif /* MENU_H_ */
