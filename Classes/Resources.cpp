/*
 * Counter.cpp
 *
 *  Created on: Aug 4, 2014
 *      Author: zhangchen
 */

#include "Resources.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace CocosDenshion;

const Resource smallResource = { cocos2d::CCSizeMake(640, 1136), "iphone" };
const Resource largeResource = { cocos2d::CCSizeMake(1536, 2048), "ipad" };

LocalResources::LocalResources() {
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();
	if (frameSize.height >= largeResource.size.height) {
		resource = largeResource;
	} else {
		resource = smallResource;
	}
	prepareResPath();
	loadConf();
}

LocalResources::~LocalResources() {
	CC_SAFE_RELEASE(conf);
	CC_SAFE_RELEASE(conf_lang);
	CC_SAFE_RELEASE(conf_resolution);
}

LocalResources* LocalResources::sharedResources() {
	static LocalResources* res = NULL;
	if (!res) {
		res = new LocalResources();
	}
	return res;
}

CCSize LocalResources::designResolutionSize() {
	return resource.size;
}

const CCString* LocalResources::valueByKey(const char * key) {
	const CCString * value = NULL;
	if ((value = this->langByKey(key))) {
		return value;
	} else if ((value = this->resoByKey(key))) {
		return value;
	} else {
		return this->confByKey(key);
	}
}

const CCString* LocalResources::confByKey(const char * key) {
	if (conf) {
		return this->conf->valueForKey(key);
	} else {
		return NULL;
	}
}

const CCString* LocalResources::langByKey(const char * key) {
	if (conf_lang) {
		return this->conf_lang->valueForKey(key);
	} else {
		return NULL;
	}
}

const CCString* LocalResources::resoByKey(const char * key) {
	if (conf_resolution) {
		return this->conf_resolution->valueForKey(key);
	} else {
		return NULL;
	}
}

void LocalResources::prepareResPath() {
	/*-- Add the Directory Base on Resolution --*/
	string dir = resource.directory;
	CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder(
			(dir + "/").c_str());
	CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder("global/");

	/*-- Add the Directory Base on Language --*/
	ccLanguageType languageType =
			CCApplication::sharedApplication()->getCurrentLanguage();
	std::string lang_str;
	if (kLanguageChinese == languageType)
		lang_str = "zh_cn";
	else
		lang_str = "en_us";

	vector<std::string> pathVector =
			CCFileUtils::sharedFileUtils()->getSearchResolutionsOrder();
	for (vector<string>::const_iterator it = pathVector.begin();
			it < pathVector.end(); ++it) {
		CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder(
				((*it) + lang_str + "/").c_str());
	}
}

void LocalResources::loadConf() {
	/*-- 加载总配置 --*/
	if (CCFileUtils::sharedFileUtils()->isFileExist("configuration.plist")) {
		conf = CCDictionary::createWithContentsOfFile("configuration.plist");
		if (conf) {
			CC_SAFE_RETAIN(conf);
		}
	} else {
		conf = 0;
	}

	/*-- 语言配置 --*/
	if (CCFileUtils::sharedFileUtils()->isFileExist("language.plist")) {
		conf_lang = CCDictionary::createWithContentsOfFile("language.plist");
		if (conf_lang) {
			CC_SAFE_RETAIN(conf_lang);
		}
	} else {
		conf_lang = 0;
	}

	/*-- 加载分辨率配置 --*/
	if (CCFileUtils::sharedFileUtils()->isFileExist("resolution.plist")) {
		conf_resolution = CCDictionary::createWithContentsOfFile(
				"resolution.plist");
		if (conf_resolution) {
			CC_SAFE_RETAIN(conf_resolution);
		}
	} else {
		conf_resolution = 0;
	}
}

Context::Context() {
}

Context::~Context() {
}

void Context::save() {
}

void Context::load() {
}
