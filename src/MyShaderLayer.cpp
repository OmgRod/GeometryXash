#include "MyShaderLayer.hpp"

bool MyShaderLayer::init(const char* vShaderFilename, const char* fShaderFilename) {
    if (!CCLayer::init()) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_renderTex = CCRenderTexture::create(winSize.width, winSize.height);
    m_renderTex->retain();

    m_screenSprite = CCSprite::createWithTexture(m_renderTex->getSprite()->getTexture());
    m_screenSprite->setFlipY(true);
    m_screenSprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(m_screenSprite);

    m_program = new CCGLProgram();
    m_program->initWithVertexShaderFilename(vShaderFilename, fShaderFilename);
    m_program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    m_program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    m_program->link();
    m_program->updateUniforms();

    m_resolutionUniform = glGetUniformLocation(m_program->getProgram(), "resolution");
    m_pixelSizeUniform = glGetUniformLocation(m_program->getProgram(), "pixelSize");

    m_screenSprite->setShaderProgram(m_program);

    this->scheduleUpdate();

    return true;
}

void MyShaderLayer::update(float dt) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_renderTex->begin();
    CCDirector::sharedDirector()->getRunningScene()->visit();
    m_renderTex->end();

    m_program->use();
    m_program->setUniformLocationWith2f(m_resolutionUniform, winSize.width, winSize.height);
    m_program->setUniformLocationWith1f(m_pixelSizeUniform, m_pixelSize);
}

MyShaderLayer* MyShaderLayer::create(const char* vShaderFilename, const char* fShaderFilename) {
    auto ret = new MyShaderLayer();
    if (ret && ret->init(vShaderFilename, fShaderFilename)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
