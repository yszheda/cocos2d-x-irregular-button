/*
 * =====================================================================================
 *
 *       Filename:  IrregularButton.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/10/30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuai Yuan (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "IrregularButton.h"

USING_NS_CC;
using namespace ui;

IrregularButton::IrregularButton():
    Button(),
    normalTransparent_(nullptr)
{}

IrregularButton::~IrregularButton()
{
    delete[] normalTransparent_;
}

IrregularButton* IrregularButton::create()
{
    IrregularButton* widget = new IrregularButton();
    if (widget && widget->init()) {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

IrregularButton* IrregularButton::create(const std::string& normalImage,
        const std::string& selectedImage,
        const std::string& disableImage,
        TextureResType texType)
{
    IrregularButton* btn = new IrregularButton;
    if (btn && btn->init(normalImage, selectedImage, disableImage, texType)) {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

bool IrregularButton::init(const std::string &normalImage,
                  const std::string& selectedImage ,
                  const std::string& disableImage,
                  TextureResType texType)
{
    bool ret = true;
    do {
        if (!Button::init(normalImage, selectedImage, disableImage, texType)) {
            ret = false;
            break;
        }
    } while (0);
    loadNormalTransparentInfo();
//    loadNormalTransparentInfoFromFile();
    return ret;
}

bool IrregularButton::init()
{
    if (Button::init())
    {
        return true;
    }
    return false;
}

void IrregularButton::loadNormalTransparentInfoFromFile()
{
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto start = std::chrono::steady_clock::now();
#endif
    
    Image* normalImage = new Image();
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto stepStart = std::chrono::steady_clock::now();
#endif
    normalImage->initWithImageFile(_normalFileName);
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto stepEnd = std::chrono::steady_clock::now();
    auto stepTime = std::chrono::duration_cast<std::chrono::milliseconds>(stepEnd - stepStart);
    CCLOG("Image::initWithImageFile: %lld ms\n", stepTime.count());
#endif
    
    normalImageWidth_ = normalImage->getWidth();
    normalImageHeight_ = normalImage->getHeight();

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepStart = std::chrono::steady_clock::now();
#endif
    auto dataLen = normalImage->getDataLen();
    if (normalTransparent_ != nullptr) {
        delete[] normalTransparent_;
    }
    auto normalPixels = normalImage->getData();
    normalTransparent_ = new bool[dataLen / (sizeof(unsigned char) * 4)];
    for (auto i = 0; i < normalImageHeight_; i++) {
        for (auto j = 0; j < normalImageWidth_; j++) {
            normalTransparent_[i * normalImageWidth_ + j] = (normalPixels[(i * normalImageWidth_ + j) * 4 + 3] == 0);
        }
    }
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepEnd = std::chrono::steady_clock::now();
    stepTime = std::chrono::duration_cast<std::chrono::milliseconds>(stepEnd - stepStart);
    CCLOG("init normalTransparent_: %lld ms\n", stepTime.count());
#endif

    delete normalImage;
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto end = std::chrono::steady_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    CCLOG("load from file: %lld ms\n", totalTime.count());
#endif
}

void IrregularButton::loadNormalTransparentInfo()
{
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto start = std::chrono::steady_clock::now();
#endif
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto stepStart = std::chrono::steady_clock::now();
#endif
    Sprite* normalRenderer = static_cast<Sprite*>(_buttonNormalRenderer);
    auto normalTexture = normalRenderer->getTexture();
    const Size& s = normalTexture->getContentSizeInPixels();
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto stepEnd = std::chrono::steady_clock::now();
    auto stepTime = std::chrono::duration_cast<std::chrono::milliseconds>(stepEnd - stepStart);
    CCLOG("Sprite::getTexture: %lld ms\n", stepTime.count());
#endif
    
    int savedBufferWidth = (int)s.width;
    int savedBufferHeight = (int)s.height;
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    CCLOG("image: %s, savedBuffWidth: %d, savedBuffHeight: %d\n", _normalFileName.c_str(), savedBufferWidth, savedBufferHeight);
#endif
    
    GLubyte *buffer = nullptr;
    
    // the FBO which cocos2dx used is not window-system-provided (non-zero id)
    GLint oldFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    CCLOG("oldFBO: %d\n", oldFBO);
#endif
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepStart = std::chrono::steady_clock::now();
#endif
    glBindTexture(GL_TEXTURE_2D, normalTexture->getName());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, savedBufferWidth, savedBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalTexture->getName(), 0);
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepEnd = std::chrono::steady_clock::now();
    stepTime = std::chrono::duration_cast<std::chrono::milliseconds>(stepEnd - stepStart);
    CCLOG("bind texture: %lld ms\n", stepTime.count());
#endif
    
    CCASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not attach texture to framebuffer");
    
    buffer = new (std::nothrow) GLubyte[savedBufferWidth * savedBufferHeight * 4];
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepStart = std::chrono::steady_clock::now();
#endif
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, savedBufferWidth, savedBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepEnd = std::chrono::steady_clock::now();
    stepTime = std::chrono::duration_cast<std::chrono::milliseconds>(stepEnd - stepStart);
    CCLOG("glReadPixels: %lld ms\n", stepTime.count());
#endif
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepStart = std::chrono::steady_clock::now();
#endif
    auto dataLen = savedBufferWidth * savedBufferHeight * 4;
    if (normalTransparent_ != nullptr) {
        delete[] normalTransparent_;
    }
    normalImageWidth_ = savedBufferWidth;
    normalImageHeight_ = savedBufferHeight;
    normalTransparent_ = new bool[dataLen / (sizeof(unsigned char) * 4)];
    for (auto i = 0; i < normalImageHeight_; i++) {
        for (auto j = 0; j < normalImageWidth_; j++) {
            normalTransparent_[i * normalImageWidth_ + j] = (buffer[(i * normalImageWidth_ + j) * 4 + 3] == 0);
        }
    }
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    stepEnd = std::chrono::steady_clock::now();
    stepTime = std::chrono::duration_cast<std::chrono::milliseconds>(stepEnd - stepStart);
    CCLOG("init normalTransparent_: %lld ms\n", stepTime.count());
#endif
    
    CC_SAFE_DELETE_ARRAY(buffer);
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto end = std::chrono::steady_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    CCLOG("load from memory: %lld ms\n", totalTime.count());
#endif
}

bool IrregularButton::getIsTransparentAtPoint(cocos2d::Vec2 point)
{
    point.y = _buttonNormalRenderer->getContentSize().height - point.y;
    int x = (int) point.x - 1;
    if (x < 0) {
        x = 0;
    } else if (x >= normalImageWidth_) {
        x = normalImageWidth_ - 1;
    }
    int y = (int) point.y - 1;
    if (y < 0) {
        y = 0;
    } else if (y >= normalImageHeight_) {
        y = normalImageHeight_ - 1;
    }
    return normalTransparent_[normalImageWidth_ * y + x];
}

bool IrregularButton::hitTest(const Vec2 &pt)
{
    Vec2 localLocation = _buttonNormalRenderer->convertToNodeSpace(pt);
    Rect validTouchedRect;
    validTouchedRect.size = _buttonNormalRenderer->getContentSize();
    if (validTouchedRect.containsPoint(localLocation) && getIsTransparentAtPoint(localLocation) == false)
    {
        return true;
    }
    return false;
}
