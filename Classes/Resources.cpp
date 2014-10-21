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

const Resource iphone5Resource = { cocos2d::CCSizeMake(640, 1136), "iphone" };
const Resource iphone4Resource = { cocos2d::CCSizeMake(640, 960), "iphone" };
const Resource nexusResource = { cocos2d::CCSizeMake(768, 1184), "iphone" };

#define ASPECT_RATIO_N_5 (768.0f/1184.f+640.0f/1136.0f)/2
#define ASPECT_RATIO_4_N (640.0f/960.f+768.0f/1184.0f)/2

LocalResources::LocalResources() {
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();
	if (frameSize.width / frameSize.height > ASPECT_RATIO_4_N) {
		resource = iphone4Resource;
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(
				resource.size.width, resource.size.height,
				kResolutionFixedHeight);
	} else if (frameSize.width / frameSize.height > ASPECT_RATIO_N_5) {
		resource = nexusResource;
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(
				resource.size.width, resource.size.height, kResolutionNoBorder);
	} else {
		resource = iphone5Resource;
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(
				resource.size.width, resource.size.height, kResolutionNoBorder);
	}
	prepareResPath();
	loadConf();
	loadResources();
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
	} else if ((value = this->confByKey(key))) {
		return value;
	} else {
		return CCString::create("");
	}
}

const CCString* LocalResources::confByKey(const char * key) {
	if (conf) {
		return dynamic_cast<CCString*>(conf->objectForKey(key));
	} else {
		return NULL;
	}
}

const CCString* LocalResources::langByKey(const char * key) {
	if (conf_lang) {
		return dynamic_cast<CCString*>(conf_lang->objectForKey(key));
	} else {
		return NULL;
	}
}

const CCString* LocalResources::resoByKey(const char * key) {
	if (conf_resolution) {
		return dynamic_cast<CCString*>(conf_resolution->objectForKey(key));
	} else {
		return NULL;
	}
}

void LocalResources::prepareResPath() {
	CCFileUtils::sharedFileUtils()->purgeCachedEntries();
	/*-- Add the Search Directory --*/
	ccLanguageType languageType =
			CCApplication::sharedApplication()->getCurrentLanguage();
	string lang;
	if (kLanguageChinese == languageType) {
		lang = "zh_cn";
	} else {
		lang = "en_us";
	}
	CCFileUtils::sharedFileUtils()->addSearchPath(lang.c_str());
	CCFileUtils::sharedFileUtils()->addSearchPath("global");

	/*-- Add the Directory Base on Resolution --*/
	string dir = resource.directory;
	CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder(
			(dir + "/").c_str());
}

void LocalResources::loadConf() {
	/*-- 加载总配置 --*/
	if (CCFileUtils::sharedFileUtils()->isFileExist(
			CCFileUtils::sharedFileUtils()->fullPathForFilename(
					"configuration.plist"))) {
		conf = CCDictionary::createWithContentsOfFile("configuration.plist");
		if (conf) {
			CC_SAFE_RETAIN(conf);
		}
	} else {
		conf = 0;
	}

	/*-- 语言配置 --*/
	if (CCFileUtils::sharedFileUtils()->isFileExist(
			CCFileUtils::sharedFileUtils()->fullPathForFilename(
					"language.plist"))) {
		conf_lang = CCDictionary::createWithContentsOfFile("language.plist");
		if (conf_lang) {
			CC_SAFE_RETAIN(conf_lang);
		}
	} else {
		conf_lang = 0;
	}

	/*-- 加载分辨率配置 --*/
	if (CCFileUtils::sharedFileUtils()->isFileExist(
			CCFileUtils::sharedFileUtils()->fullPathForFilename(
					"resolution.plist"))) {
		conf_resolution = CCDictionary::createWithContentsOfFile(
				"resolution.plist");
		if (conf_resolution) {
			CC_SAFE_RETAIN(conf_resolution);
		}
	} else {
		conf_resolution = 0;
	}
}

void LocalResources::loadResources() {
	/*-- 文字 --*/
	CCMenuItemFont::setFontName(this->valueByKey("font")->getCString());
	CCMenuItemFont::setFontSize(this->valueByKey("font_size")->intValue());

	/*-- 图片 --*/
	CCTextureCache::sharedTextureCache()->addImage(("bg_back.png"));
	CCTextureCache::sharedTextureCache()->addImage(("btn_sound_on.png"));
	CCTextureCache::sharedTextureCache()->addImage(("btn_sound_off.png"));

	/*-- 声音 --*/
//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("gameover.mp3");
//SimpleAudioEngine::sharedEngine()->preloadEffect("jump.mp3");
	/*-- 动画 --*/

}

Context::Context() :
		score(0), newrecord(false) {
	load();
}

Context::~Context() {
}

Context * Context::sharedContext() {
	static Context* res = NULL;
	if (!res) {
		res = new Context();
	}
	return res;
}

void Context::save() {
	CCUserDefault::sharedUserDefault()->setBoolForKey("sound", this->sound);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("highScore",
			this->highScore);
	CCUserDefault::sharedUserDefault()->setBoolForKey("first", this->first);
	CCUserDefault::sharedUserDefault()->flush();
}

void Context::load() {
	this->first = CCUserDefault::sharedUserDefault()->getBoolForKey("first",
			true);
	this->sound = CCUserDefault::sharedUserDefault()->getBoolForKey("sound",
			true);
	this->highScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(
			"highScore", 0);
}

void Context::playBgMusic() {
	if (this->isSound()
			&& !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3", true);
	}
}

void Context::playEffect(const char * mp3) {
	if (this->isSound()) {
		SimpleAudioEngine::sharedEngine()->playEffect(mp3);
	}
}

bool Context::isSound() const {
	return this->sound;
}

void Context::onSound() {
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3", true);
	this->sound = true;
	save();
}

void Context::offSound() {
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	this->sound = false;
	save();
}

bool Context::isFirstRun() const {
	return first;
}

bool Context::firstRun() {
	if (first) {
		this->first = false;
		save();
		return true;
	} else {
		return false;
	}
}

ccColor3B Context::getFontColor() const {
	return ccc3(LOCAL_RESOURCES->valueByKey("font_color_r")->intValue(),LOCAL_RESOURCES->valueByKey("font_color_g")->intValue(),LOCAL_RESOURCES->valueByKey("font_color_b")->intValue());
}

unsigned int Context::increaseScore() {
	score++;
	if (score > highScore) {
		highScore = score;
		newrecord = true;
	}
	return this->score;
}

void Context::clearScore() {
	score = 0;
	newrecord = false;
}

unsigned int Context::getScore() const {
	return this->score;
}

unsigned int Context::getHighScore() const {
	return this->highScore;
}

bool Context::isNewRecord() const {
	return this->newrecord;
}
