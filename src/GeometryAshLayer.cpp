#include "GeometryAshLayer.hpp"

void GeometryAshLayer::keyBackClicked() {
    GameManager::sharedState()->fadeInMenuMusic();
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

struct ScrollEntry {
    enum class Type { Text, Sprite, Spacer };

    Type type;
    std::string text;
    std::string fontFile;
    bool isTitle;
    CCSprite* sprite;
    float spacerHeight;

    ScrollEntry(const std::string& t, bool title = false, const std::string& font = "bigFont.fnt")
        : type(Type::Text), text(t), fontFile(font), isTitle(title), sprite(nullptr), spacerHeight(0.f) {}

    ScrollEntry(CCSprite* s)
        : type(Type::Sprite), text(""), fontFile(""), isTitle(false), sprite(s), spacerHeight(0.f) {}

    ScrollEntry(float height)
        : type(Type::Spacer), text(""), fontFile(""), isTitle(false), sprite(nullptr), spacerHeight(height) {}
};

bool GeometryAshLayer::init() {
    if (!CCLayer::init()) return false;

    setKeypadEnabled(true);
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    FMODAudioEngine::sharedEngine()->playMusic("BackOnTrack.mp3", false, 0.1f, 0);
    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->setAnchorPoint({0, 0});
    addChild(menu);

    auto background = createLayerBG();
    background->setPosition({-5.f, -5.f});
    background->setColor({100, 100, 100});
    addChild(background, -1);
    addSideArt(this, SideArt::Bottom, SideArtStyle::LayerGray, false);

    auto backButton = CCMenuItemSpriteExtra::create(
        CCSpriteGrayscale::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(GeometryAshLayer::backWrapper)
    );
    backButton->setPosition({25, winSize.height - 25});
    menu->addChild(backButton);

    auto containerNode = CCNode::create();
    containerNode->setPosition({0.f, -winSize.height});
    addChild(containerNode);

    std::vector<ScrollEntry> credits = {
        ScrollEntry(CCSprite::createWithSpriteFrameName("Ash.png"_spr)),
        ScrollEntry(40.f),
        ScrollEntry("Geometry Dash began as a simple game."),
        ScrollEntry(30.f),
        ScrollEntry(50.f),
        ScrollEntry("But a dark force loomed within the Chamber of Time."),
        ScrollEntry("The Volcano chamber awoke..."),
        // ScrollEntry(CCSprite::create("volcano.png"_spr)),
        ScrollEntry("The eruption shook the foundations of\nGeometry Dash."),
        ScrollEntry("And from its fiery ashes, the world was\nchanged forever."),
    };

    float labelY = winSize.height * 0.5f;
    float totalHeight = 0.f;

    for (auto& entry : credits) {
        switch (entry.type) {
            case ScrollEntry::Type::Text: {
                auto label = CCLabelBMFont::create(entry.text.c_str(), entry.fontFile.c_str());
                label->setAnchorPoint({0.5f, 1.f});
                label->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
                label->setPosition({winSize.width / 2.f, labelY});
                float scale = entry.isTitle ? 1.f : 0.5f;
                label->setScale(scale);
                containerNode->addChild(label);

                float spacing = entry.isTitle
                    ? (label->getContentSize().height + 16.f)
                    : (label->getContentSize().height * 0.5f + 4.f);

                labelY -= spacing;
                totalHeight += spacing;
                break;
            }
            case ScrollEntry::Type::Sprite: {
                if (!entry.sprite) break;
                entry.sprite->setPosition({winSize.width / 2.f, labelY});
                containerNode->addChild(entry.sprite);
                float spacing = entry.sprite->getContentSize().height;
                labelY -= spacing + 4.f;
                totalHeight += spacing + 4.f;
                break;
            }
            case ScrollEntry::Type::Spacer: {
                labelY -= entry.spacerHeight;
                totalHeight += entry.spacerHeight;
                break;
            }
        }
    }

    auto moveAction = CCMoveTo::create(30.f, {0.f, winSize.height + totalHeight});
    auto callBack = CCCallFunc::create(this, callfunc_selector(GeometryAshLayer::keyBackClicked));
    auto sequence = CCSequence::create(moveAction, callBack, nullptr);
    containerNode->runAction(sequence);

    return true;
}

GeometryAshLayer* GeometryAshLayer::create() {
    auto layer = new GeometryAshLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

CCScene* GeometryAshLayer::scene() {
    auto scene = CCScene::create();
    auto layer = GeometryAshLayer::create();
    scene->addChild(layer);
    return scene;
}

void GeometryAshLayer::backWrapper(CCObject* sender) {
    keyBackClicked();
}

void GeometryAshLayer::onExit() {
    CCLayer::onExit();
}