/*
 * BgLayer.h
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#ifndef BGLAYER_H_
#define BGLAYER_H_

#include "cocos2d.h"

class BgLayer: public cocos2d::CCLayer {
public:
	BgLayer();

	virtual ~BgLayer();

	virtual bool init();

	CREATE_FUNC(BgLayer)
};

#endif /* BGLAYER_H_ */
