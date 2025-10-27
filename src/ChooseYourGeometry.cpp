#include "ChooseYourGeometry.hpp"
#include "Utils.hpp"
#include <random>

bool ChooseYourGeometry::setup() {
    this->setTitle("Choose Your Geometry");

    auto menu = CCMenu::create();
    menu->setContentSize({300.f, 180.f});
    menu->setPosition({0.f, 0.f});

    m_daLogo = CCSprite::createWithSpriteFrameName("GJ_logo_001.png");

    CCSize logoSize = m_daLogo->getContentSize();
    float popupWidth = 360.f;
    float targetWidth = popupWidth * 0.8f;
    float scale = (logoSize.width > 0.f) ? targetWidth / logoSize.width : 1.f;

    m_daLogo->setID("logo");
    m_daLogo->setScale(scale);

    auto buttonSpr = ButtonSprite::create("Choose!");
    m_chooseButton = CCMenuItemSpriteExtra::create(
        buttonSpr,
        this,
        menu_selector(ChooseYourGeometry::choose)
    );
    m_chooseButton->setID("choose-button");
    menu->addChildAtPosition(m_chooseButton, Anchor::Bottom);

    auto disclaimer = InfoAlertButton::create(
        "Disclaimer",
        "You can also choose a Geometry in the Geometry Xash settings. This popup will reappear in the main menu only if you switch your Geometry to \"Geometry Xash\".",
        1.f
    );
    disclaimer->setColor({ 255, 50, 50 });
    disclaimer->setPosition({ 20.f, 20.f });
    m_buttonMenu->addChild(disclaimer);

    m_mainLayer->addChildAtPosition(m_daLogo, Anchor::Center);
    m_mainLayer->addChildAtPosition(menu, Anchor::Center);

    return true;
}

void ChooseYourGeometry::choose(CCObject*) {
    m_chooseButton->setEnabled(false);

    auto moveDown = CCMoveBy::create(0.6f, {0, -150.f});
    auto ease = CCEaseBounceOut::create(moveDown);
    m_chooseButton->runAction(ease);

    m_logos = {
        "Ash.png"_spr,
        "Bash.png"_spr,
        "Cash.png"_spr,
        "Hash.png"_spr,
        "Lash.png"_spr,
        "Mash.png"_spr,
        "Rash.png"_spr,
        "Trash.png"_spr,
        "Wash.png"_spr,
        "Smash.png"_spr,
        "Crash.png"_spr,
        "Slash.png"_spr
    };

    m_logosPerSecondTop = 30;
    m_lostTime = 4;
    m_logosLostPerTime = 2;
    m_totalLogos = 150;
    m_counter = 0;

    m_targetHeight = m_daLogo->getContentSize().height * m_daLogo->getScaleY();

    this->unschedule(schedule_selector(ChooseYourGeometry::updateLogo));

    float delay = 1.0f / static_cast<float>(m_logosPerSecondTop);
    this->schedule(schedule_selector(ChooseYourGeometry::updateLogo), delay);
}

void ChooseYourGeometry::updateLogo(float dt) {
    auto fmod = FMODAudioEngine::sharedEngine();

    if (m_counter >= m_totalLogos) {
        this->unschedule(schedule_selector(ChooseYourGeometry::updateLogo));
        return;
    }

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(m_logos.size() - 1));

    std::string logoFile = m_logos[dist(rng)];

    if (m_daLogo && m_daLogo->getParent()) {
        m_daLogo->removeFromParentAndCleanup(true);
        m_daLogo = nullptr;
    }

    m_daLogo = CCSprite::createWithSpriteFrameName(logoFile.c_str());
    if (!m_daLogo) return;

    CCSize texSize = m_daLogo->getContentSize();
    if (texSize.height > 0) {
        float scale = m_targetHeight / texSize.height;
        m_daLogo->setScale(scale);
    }

    m_daLogo->setID("logo");
    m_mainLayer->addChildAtPosition(m_daLogo, Anchor::Center);

    if (m_logosPerSecondTop > 1) {
        fmod->playEffect("click.ogg"_spr);
    }

    m_counter++;

    if (m_counter % m_lostTime == 0) {
        m_logosPerSecondTop = std::max(1, m_logosPerSecondTop - m_logosLostPerTime);
        float delay = 1.0f / static_cast<float>(m_logosPerSecondTop);

        if (m_logosPerSecondTop <= 2) {
            this->unschedule(schedule_selector(ChooseYourGeometry::updateLogo));
            fmod->playEffect("success.ogg"_spr);

            std::string filename = logoFile.substr(0, logoFile.find(".png"));
            size_t slashPos = filename.find_last_of("/\\");
            if (slashPos != std::string::npos) filename = filename.substr(slashPos + 1);
            std::string chosenGeometry = "Geometry " + filename;

            Mod::get()->setSavedValue("slot-machine", true);
            Mod::get()->setSettingValue("chosen-geometry", chosenGeometry);

            float currentScale = m_daLogo->getScale();
            auto enlarge = CCScaleTo::create(0.15f, currentScale * 1.2f);
            auto shrink = CCScaleTo::create(0.2f, currentScale);
            auto easeIn = CCEaseInOut::create(enlarge, 2.0f);
            auto easeOut = CCEaseInOut::create(shrink, 2.0f);
            auto seq = CCSequence::create(
                easeIn,
                easeOut,
                CCCallFunc::create(this, callfunc_selector(ChooseYourGeometry::updateMenuLogo)),
                CCDelayTime::create(0.5f),
                CCCallFunc::create(this, callfunc_selector(ChooseYourGeometry::disappear)),
                nullptr
            );
            m_daLogo->runAction(seq);
            return;
        }

        this->unschedule(schedule_selector(ChooseYourGeometry::updateLogo));
        this->schedule(schedule_selector(ChooseYourGeometry::updateLogo), delay);
    }
}

ChooseYourGeometry* ChooseYourGeometry::create() {
    auto ret = new ChooseYourGeometry();
    if (ret->initAnchored(360.f, 260.f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ChooseYourGeometry::disappear() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto sequence = CCSequence::create(
        CCEaseBounceInOut::create(CCMoveTo::create(0.5f, { 0, (this->getPositionY() - winSize.height) })),
        CCDelayTime::create(0.5f),
        CCCallFunc::create(this, callfunc_selector(ChooseYourGeometry::close)),
        nullptr
    );
    this->runAction(sequence);
}

void ChooseYourGeometry::close() {
    this->onClose(nullptr);
}

void ChooseYourGeometry::updateMenuLogo() {
    MenuLayer* menuLayer = GameManager::sharedState()->m_menuLayer;
    Utils::updateMenuLayer(menuLayer);
}
