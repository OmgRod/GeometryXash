#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MyShaderLayer : public CCLayer {
protected:
    CCRenderTexture* m_renderTex;
    CCSprite* m_screenSprite;
    CCGLProgram* m_program;
    GLint m_resolutionUniform;
    GLint m_pixelSizeUniform;
    float m_pixelSize = 6.f;

public:
    static MyShaderLayer* create(const char* vShaderFilename, const char* fShaderFilename);
    bool init(const char* vShaderFilename, const char* fShaderFilename);

    void update(float dt);
    void setPixelSize(float size) { m_pixelSize = size; }
};
