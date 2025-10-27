#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ChooseYourGeometry : public geode::Popup<> {
protected:
    bool setup() override;

public:
    static ChooseYourGeometry* create();
    void choose(CCObject* noOneCaresAboutMe);
    void close();

private:
    CCSprite* m_daLogo = nullptr;
    CCMenuItemSpriteExtra* m_chooseButton;
    std::vector<std::string> m_logos;
    int m_logosPerSecondTop = 30;
    int m_lostTime = 3;
    int m_logosLostPerTime = 2;
    int m_totalLogos = 50;
    int m_counter = 0;
    float m_targetHeight = 0.f;

    void updateLogo(float dt);
    void disappear();
    void updateMenuLogo();
};
