//
//  CoverView.h
//  Animal_Squad
//
//  Created by wd on 9/29/14.
//
//

#ifndef __Animal_Squad__CoverView__
#define __Animal_Squad__CoverView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class CoverView : public Node , public ScrollViewDelegate
{
public:
    CoverView();
    ~CoverView();
    /**
     *
     * @param scrollView的rect
     * @param scrollView滚动size
     * @param card之间的距离
     * @param card之间的缩放
     */
    static CoverView* create(Rect swBox, Size slSize ,float disDistance , float disScale );
    virtual bool init(Rect swBox , Size slSize , float disDistance , float disScale);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    void initData();
    void scrollViewDidScroll(ScrollView* view);
    void scrollViewDidZoom(ScrollView* view);
    void adjustCardScale(Point adjustPoint);
    void adjustScrollView(Point adjustPoint);
    void adjusetEndScrollView();
    void cardViewEnd_callBack(Node* pSender);
    int getCurCardIndex();//当前中间card索引 从0开始
    void addCard(Node * card);
    void addCard(Node * card, int zOrder);
    void addCard(Node* card, int zOrder, int tag);
    CC_SYNTHESIZE(Point , swPosition , SwPosition);//scrollView 位置
    CC_SYNTHESIZE(Size , swSize , SwSize);//scrollView大小
    CC_SYNTHESIZE(Size , slSize , SlSize);//scrollLayer 大小
    CC_SYNTHESIZE(float , disDistance , DisDistance);//card距离间隔
    CC_SYNTHESIZE(float , disScale , DisScale);//crad缩放间隔
    CC_SYNTHESIZE(Rect , swBox , SwBox);//scrollview 边框
    CC_SYNTHESIZE(Layer* , scrollLayer , scrollLayer);//scrollView的containLayer
    CC_SYNTHESIZE(int , cardNum , CardNum);//card索引
    CC_PROPERTY(Point , offsetPosition , OffsetPosition);//card起始位置
private:
    Size wSize;
    Array* cardArray;
    ScrollView* scrollView;
};

#endif /* defined(__Animal_Squad__CoverView__) */
