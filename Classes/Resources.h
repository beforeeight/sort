/*
 * Resources.h
 *
 *  Created on: Aug 4, 2014
 *      Author: zhangchen
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "cocos2d.h"

USING_NS_CC;

typedef struct tagResource {
	cocos2d::CCSize size;
	char directory[100];
} Resource;

extern const Resource smallResource;
extern const Resource largeResource;

class LocalResources {

public:
	LocalResources();

	~LocalResources();

	static LocalResources *sharedResources();

	CCSize designResolutionSize();

	const CCString* valueByKey(const char * key);

private:
	Resource resource;

	CCDictionary *conf_lang;

	CCDictionary *conf_resolution;

	CCDictionary *conf;

	void prepareResPath();

	void loadConf();

	void loadResources();

	const CCString* langByKey(const char * key);

	const CCString* resoByKey(const char * key);

	const CCString* confByKey(const char * key);
};

class Context {
public:
	Context();

	virtual ~Context();

	static Context *sharedContext();

	virtual void save();

	virtual void load();

};

#define LOCAL_RESOURCES LocalResources::sharedResources()

#define LOCAL_RESOLUTION LocalResources::sharedResources()->designResolutionSize()

#define ccpp(__X__,__Y__) cocos2d::CCPointMake(((float)(__X__))*LOCAL_RESOLUTION.width, ((float)(__Y__))*LOCAL_RESOLUTION.height)

#endif
