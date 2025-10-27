#include "GeometryAshLayer.hpp"

bool GeometryAshLayer::init() {
    if (!CCLayer::init()) return false;

    return true;
}
CCScene* GeometryAshLayer::scene() {
    auto scene = CCScene::create();
    auto layer = GeometryAshLayer::create();

    scene->addChild(layer);

    return scene;
}
GeometryAshLayer* GeometryAshLayer::create() {
    auto ret = new GeometryAshLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
