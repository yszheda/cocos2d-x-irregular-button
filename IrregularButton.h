/*
 * =====================================================================================
 *
 *       Filename:  IrregularButton.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/10/29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuai Yuan (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __IRREGULAR_BUTTON_H__
#define __IRREGULAR_BUTTON_H__ 

#include "cocos2d.h"
#include "ui/CocosGUI.h"

/*
 * =====================================================================================
 *        Class:  IrregularButton
 *  Description:  
 * =====================================================================================
 */
class IrregularButton : public cocos2d::ui::Button
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        IrregularButton ();                                     /* constructor      */
        virtual ~IrregularButton ();                            /* destructor       */

        static IrregularButton* create();
        static IrregularButton* create(const std::string& normalImage,
                const std::string& selectedImage = "",
                const std::string& disableImage = "",
                cocos2d::ui::Widget::TextureResType texType = cocos2d::ui::Widget::TextureResType::LOCAL);

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */

        virtual bool hitTest(const cocos2d::Vec2 &pt) override;
    protected:
        virtual bool init() override;
        virtual bool init(const std::string& normalImage,
                const std::string& selectedImage = "",
                const std::string& disableImage = "",
                cocos2d::ui::Widget::TextureResType texType = cocos2d::ui::Widget::TextureResType::LOCAL) override;
        void loadNormalTransparentInfo();
        bool getIsTransparentAtPoint(cocos2d::Vec2 point);

        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        int normalImageWidth_;
        int normalImageHeight_;
        bool* normalTransparent_;

}; /* -----  end of class IrregularButton  ----- */


#endif /* __IRREGULAR_BUTTON_H__ */
