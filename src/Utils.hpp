#include <Geode/Geode.hpp>
#include "ChooseYourGeometry.hpp"
#include "GeometryAshLayer.hpp"

using namespace geode::prelude;

class Utils : public CCObject {
public:
    static void updateMenuLayer(MenuLayer* layer) {
        Mod* mod = Mod::get();
        std::string xash = mod->getSettingValue<std::string>("chosen-geometry");
        bool clogo = mod->getSettingValue<bool>("geometry-logo");
        bool notFirstTime = mod->getSavedValue<bool>("first-done");
        auto bottomMenu = layer->getChildByID("bottom-menu");

        if (xash != "Geometry Dash") {
            if (clogo) {
                CCSprite* dalogo = typeinfo_cast<CCSprite*>(layer->getChildByID("main-title"));
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
                    layer->addChild(newLogo, dalogo->getZOrder());
                }
                newLogo->setPosition(dalogo->getPosition());
                dalogo->setVisible(false);

                layer->addChild(newLogo);
            }

            if (xash == "Geometry Ash") {
                auto ashSprite = CircleButtonSprite::create(
                    CCSprite::createWithSpriteFrameName("island_lava_001.png"),
                    CircleBaseColor::Gray,
                    CircleBaseSize::MediumAlt
                );
                auto ashButton = CCMenuItemSpriteExtra::create(
                    ashSprite,
                    layer,
                    menu_selector(Utils::onGeometryAsh)
                );

                bottomMenu->addChild(ashButton);
                bottomMenu->updateLayout();
            }
        }
        if (!notFirstTime) {
            auto sequence = CCSequence::create(
                CCCallFuncN::create(layer, callfuncN_selector(Utils::geometryXash)),
                nullptr
            );
            layer->runAction(sequence);
        }
    }

    void geometryXash(MenuLayer* layer) {
        auto popup = ChooseYourGeometry::create();
        popup->m_scene = layer;
        popup->show();
    }

    void onGeometryAsh(CCObject* IDFCABOUTYOULMAO) {
        auto transition = CCTransitionFade::create(0.5f, GeometryAshLayer::scene());
        CCDirector::sharedDirector()->pushScene(transition);
    }
};
