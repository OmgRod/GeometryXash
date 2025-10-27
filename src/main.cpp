#include <Geode/Geode.hpp>
#include <Geode/binding/ShaderLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "ChooseYourGeometry.hpp"
#include "GeometryAshLayer.hpp"
#include "MyShaderLayer.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
public:
    bool init() {
        if (!MenuLayer::init()) return false;

        Utils::updateMenuLayer(this);

        return true;
    }
};

class $modify(MyLoadingLayer, LoadingLayer) {
public:
    bool init(bool p0) {
        if (!LoadingLayer::init(p0)) return false;

        auto SFC = CCSpriteFrameCache::get();
        auto searchPathRoot = dirs::getModRuntimeDir() / Mod::get()->getID() / "resources";
        std::string xash = Mod::get()->getSettingValue<std::string>("chosen-geometry");
        
        CCFileUtils::sharedFileUtils()->addSearchPath(searchPathRoot.string().c_str());
        SFC->addSpriteFramesWithFile("GeometryXash.plist"_spr);

        if (xash != "Geometry Xash") {
            CCSprite* dalogo = this->getChildByType<CCSprite>(1);
            dalogo->setVisible(false);

            auto end = xash.find_last_not_of(" \t\n\r");
            if (end != std::string::npos) xash.erase(end + 1);
            else xash.clear();

            auto pos = xash.find_last_of(" \t");
            std::string finalWord = (pos == std::string::npos) ? xash : xash.substr(pos + 1);

            CCSprite* newLogo = CCSprite::createWithSpriteFrameName(fmt::format("{}.png"_spr, finalWord).c_str());
            if (newLogo) {
                float targetHeight = dalogo->getContentSize().height * dalogo->getScaleY();
                float srcHeight = newLogo->getContentSize().height * newLogo->getScaleY();
                if (srcHeight > 0.0f) {
                    float scale = targetHeight / srcHeight;
                    newLogo->setScale(scale);
                }

                newLogo->setAnchorPoint(dalogo->getAnchorPoint());
                newLogo->setPosition(dalogo->getPosition());
                this->addChild(newLogo, dalogo->getZOrder());
            }
            newLogo->setPosition(dalogo->getPosition());
            dalogo->setVisible(false);

            this->addChild(newLogo);
        }

        return true;
    }

    void loadAssets() {
        LoadingLayer::loadAssets();

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GauntletSheet.plist");
    }
};

class $modify(MyPlayLayer, PlayLayer) {
public:
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (Mod::get()->getSettingValue<bool>("lq-gameplay")) {
            auto bandicamLogo = CCSprite::create("bandicam.png"_spr);
            bandicamLogo->setScale(2.f);
            bandicamLogo->setPosition({ winSize.width * 0.5f, winSize.height * 0.9f });
            this->addChild(bandicamLogo, 10000);

            auto pixelateLayer = MyShaderLayer::create("simple.vsh"_spr, "pixelate.fsh"_spr);
            pixelateLayer->setPixelSize(4.0f);
            this->addChild(pixelateLayer, 9999);
        }

        return true;
    }
};
