#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GeometryAshLayer : public CCLayer {
public:
    bool init();
    static GeometryAshLayer* create();
    static CCScene* scene();
    void keyBackClicked();
    void backWrapper(CCObject* sender);
    void onExit();
};
