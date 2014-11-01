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

void IrregularButton::loadNormalTransparentInfo()
{
    Image* normalImage = new Image();
    normalImage->initWithImageFile(_normalFileName);
    normalImageWidth_ = normalImage->getWidth();
    normalImageHeight_ = normalImage->getHeight();

    auto dataLen = normalImage->getDataLen();
    if (normalTransparent_ != nullptr) {
        delete[] normalTransparent_;
    }
    auto normalPixels = normalImage->getData();
    normalTransparent_ = new bool[dataLen / (sizeof(unsigned char) * 4)];
    for (auto i = 0; i < normalImageHeight_; i++) {
        for (auto j = 0; j < normalImageWidth_; j++) {
            normalTransparent_[i * normalImageWidth_ + j] = (normalPixels[(i * normalImageWidth_ + j) * 4] == 0);
        }
    }

    delete normalImage;
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
