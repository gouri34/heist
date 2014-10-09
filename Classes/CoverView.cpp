//
//  CoverView.cpp
//  Animal_Squad
//
//  Created by wd on 9/29/14.
//
//

#include "CoverView.h"
CoverView::CoverView()
{
    
}

CoverView::~CoverView()
{
    CC_SAFE_RELEASE_NULL(cardArray);
}

CoverView* CoverView::create(Rect swBox, Size slSize , float disDistance , float disScale)
{
    CoverView* cover = new CoverView();
    if(cover && cover->init(swBox,slSize,disDistance,disScale))
    {
        cover->autorelease();
        return cover;
    }
    CC_SAFE_DELETE(cover);
    return NULL;
}

bool CoverView::init(Rect swBox , Size slSize , float disDistance , float disScale)
{
    if(!Node::init()) return false;
    this->swBox = swBox;
    this->swPosition = swBox.origin;
    this->swSize = swBox.size;
    this->slSize = slSize;
    this->disDistance = disDistance;
    this->disScale = disScale;
    initData();
    
    //enable touching
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(CoverView::ccTouchBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(CoverView::ccTouchMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(CoverView::ccTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    
    return true;
}


void CoverView::initData()
{
    wSize = Director::getInstance()->getVisibleSize();
    cardArray = CCArray::create();
    cardArray->retain();
    cardNum = 0;
    offsetPosition = Point(swSize.width/2,swSize.height/2);
    
    scrollLayer = CCLayer::create();
    scrollLayer->setAnchorPoint(Point(0,0));
    scrollLayer->setPosition(Point(0,0));
    scrollLayer->setContentSize(slSize);
    scrollView = ScrollView::create(swSize,scrollLayer);
    scrollView->setAnchorPoint(Point(0,0));
    scrollView->setContentOffset(Point(0,0));
    scrollView->setTouchEnabled(false);
    scrollView->setDelegate(this);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    addChild(scrollView,1);
}

void CoverView::onEnter()
{
    Node::onEnter();
    //Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0 , false);
}

void CoverView::onExit()
{
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeAllChildren();
    Node::onExit();
}

bool CoverView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void CoverView::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    Point scroll_prepoint = pTouch->getPreviousLocation();
    Point scroll_movepoint = pTouch->getLocation();

    if(swBox.containsPoint(scroll_movepoint))
    {
        Point adjustPoint = scroll_movepoint-scroll_prepoint;
        adjustScrollView(adjustPoint);
        adjustCardScale(adjustPoint);
    }
}

void CoverView::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    Point scroll_prepoint = pTouch->getPreviousLocation();
    Point scroll_endpoint = pTouch->getLocation();
    float disX = scroll_endpoint.x - scroll_endpoint.x;
    adjusetEndScrollView();
}

void CoverView::adjustCardScale(Point adjustPoint)
{
    float disX = adjustPoint.x;
    Object* obj = NULL;
    CCARRAY_FOREACH(cardArray,obj)
    {
        Node* card = (Node*) obj;
        float offset = scrollView->getContentOffset().x;
        float posX = card->getPositionX() + offset;
        float disMid = abs(swSize.width/2-posX);
        float scale = 1- disMid/disDistance*disScale;
        card->setScale(scale);
        int zOr = (int) (1000-disMid*0.1);
        card->setZOrder(zOr);
    }
}

void CoverView::adjustScrollView(Point adjustPoint)
{
    Point endPoint = ccpAdd(scrollView->getContentOffset(),ccp(adjustPoint.x,0));
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffset(endPoint,false);
}

void CoverView::adjusetEndScrollView()
{
    Object* obj = NULL;
    float minX = wSize.height;
    float midX = swSize.width/2;
    //获取距离中间最小值的card
    CCARRAY_FOREACH(cardArray,obj)
    {
        Node* card = (Node*) obj;
        float offset = scrollView->getContentOffset().x;
        //转化父类坐标
        float posX = card->getPositionX() + offset;
        float disMid = midX-posX;
        if(abs(disMid) < abs(minX)) minX = disMid;
    }
    
    CCARRAY_FOREACH(cardArray,obj)
    {
        Node* item = (Node*) obj;
        //转化父类坐标
        float offset = scrollView->getContentOffset().x;
        float posX = item->getPositionX() + offset ;
        //距离中间长度
        float disMid = abs(midX - posX - minX);
        //目标scale
        float scale = 1- disMid/disDistance*disScale;
        ScaleTo* scaleBy = ScaleTo::create(0.2f,scale);
        item->runAction(scaleBy);
        int zOr = (int) (1000-disMid*0.1);
        item->setZOrder(zOr);
    }
    Layer* scrollLayer = (Layer*)scrollView->getContainer();
    MoveBy* moveBy = MoveBy::create(0.2f,Point(minX,0));
    //CCCallFuncN* callFuncN = CCCallFuncN::create(this,callfuncN_selector(CoverView::cardViewEnd_callBack));
    //CCSequence* seq = CCSequence::create(moveBy,callFuncN,NULL);
    //scrollLayer->runAction(seq);
    scrollLayer->runAction(moveBy);
}

void CoverView::cardViewEnd_callBack(Node* pSender)
{
    getCurCardIndex();
}

void CoverView::scrollViewDidScroll(ScrollView* view)
{
    
}

void CoverView::scrollViewDidZoom(ScrollView* view)
{
    
}



void CoverView::addCard(Node * card)
{
    int zOrder = 1000 - cardNum;
    this->addCard(card, zOrder, 0);
}

void CoverView::addCard(Node * card, int zOrder)
{
    this->addCard(card, zOrder,0);
}

void CoverView::addCard(Node* card, int zOrder, int tag)
{
    float positionX = offsetPosition.x + disDistance*cardNum;
    float scale = 1 - disScale*cardNum;
    card->setPosition(Point(positionX,offsetPosition.y));
    card->setScale(scale);
    cardArray->addObject(card);
    scrollLayer->addChild(card , zOrder,tag);
    cardNum++;
    //CCLog("crad%d:[%f , %f]",cardNum,card->getPositionX(),card->getPositionY());
}

int CoverView::getCurCardIndex()
{
    float distance1 = - scrollLayer->getPositionX();
    float distance2 = swSize.width/2 - offsetPosition.x;
    //+5 浮点数误差
    int index = (distance1 + distance2 + 5) / (disDistance);
    //CCLog("card index:%d  distance1:%f",index,distance1);
    return index;
}

void CoverView::setOffsetPosition(Point var)
{
    offsetPosition = var;
    Object* obj = NULL;
    cardNum = 0;
    CCARRAY_FOREACH(cardArray,obj)
    {
        Node* card = (Node*) obj;
        float positionX = offsetPosition.x + disDistance*cardNum;
        card->setPosition(Point(positionX,offsetPosition.y));
        cardNum++;
    }
    adjustCardScale(Vec2::ZERO);
}

Point CoverView::getOffsetPosition()
{
    return offsetPosition;
}