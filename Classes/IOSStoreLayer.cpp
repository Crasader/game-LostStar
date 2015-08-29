#include "IOSStoreLayer.h"

#include "UserDataManager.h"
#include "font.h"
#include "IOSStoreLayerScrollView.h"
#include "GameInforEditer.h"
#include "LFStrings.h"
#include "IOSMarketUseSuperToolLayer.h"
//#include "History.h"
#include "LevelManager.h"
#include "SeasonSelectorLayerGameGoods.h"
#include "CommonFunc.h"
#include "VersionControl.h"
#include "BaseScense.h"
USING_NS_CC;


IOSStoreLayer::IOSStoreLayer(void)
{
	m_pBuyStarNode = NULL;
	m_pKaibaoxiangNode = NULL;
}

IOSStoreLayer::~IOSStoreLayer(void)
{
}

bool IOSStoreLayer::init()
{
	if (!BaseShakeLayer::init())
	{
		return false;
	}

	this->initMemeber();
	this->initUI();
	this->initMenuToggle();
	this->initBtn();

	this->initKaibaoxiangNode();
	this->initBuystarNode();

	this->setToggleStateByNodeIndex(m_iCurShowNodeIndex);

	m_pBuyStarNode->setVisible(false);
	this->setTableViewVisble(m_pBuyStarNode, false);

	return true;
}

void IOSStoreLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void IOSStoreLayer::onEnter()
{
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IOSStoreLayer::payEnd), MSG_PayEndRsp, NULL);
//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IOSStoreLayer::openBoxRsp), MSG_openBoxRsp, NULL);

	BaseShakeLayer::onEnter();
}

void IOSStoreLayer::onExit()
{
	BaseShakeLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCTableView* pView = (CCTableView*)m_pKaibaoxiangNode->getChildByTag(kaibaoxiang_node_tableview_tag);
	if (pView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	}

	pView = (CCTableView*)m_pBuyStarNode->getChildByTag(buystar_node_tableview_tag);
	if (pView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	}
}

void IOSStoreLayer::initMemeber()
{
	m_iCurShowNodeIndex = kaibaoxiang_node_index;
	m_iBaoxiangCellCount = 0;
	m_iBuyStarCellCount = 0;
	m_bBaoxiangBuyTenTimes = false;
	m_pGetItemNode = NULL;
	m_enumComeFrom = from_MarketStore;

	//初始化宝箱信息
	m_baoxiangInfoVector.clear();
	CellInfo baoxiangInfo;
	baoxiangInfo.bBaoxiang = true;
	baoxiangInfo.nameStr = LFStrings::getValue("OneBaoxiangStr");
	baoxiangInfo.describeStr = LFStrings::getValue("OneBaoxiangDescribe");
	baoxiangInfo.iBaoxiangCount = 1;
	baoxiangInfo.iconNameStr = ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang.png");
	baoxiangInfo.iconBgNameStr = "";
	baoxiangInfo.btnIconNorNameStr = ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_yici.png");
	baoxiangInfo.btnIconSelNameStr = ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_yici_select.png");
	m_baoxiangInfoVector.push_back(baoxiangInfo);

	//十个宝箱信息
	baoxiangInfo.iBaoxiangCount = 10;
	baoxiangInfo.nameStr = LFStrings::getValue("TenBaoxiangStr");
	baoxiangInfo.describeStr = LFStrings::getValue("TenBaoxiangDescribe");
	baoxiangInfo.iconNameStr = ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang.png");
	baoxiangInfo.iconBgNameStr = ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang_xiaoguo_1.png");
	baoxiangInfo.btnIconNorNameStr = ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_shici.png");
	baoxiangInfo.btnIconSelNameStr = ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_shici_select.png");
	m_baoxiangInfoVector.push_back(baoxiangInfo);

	m_iBaoxiangCellCount = m_baoxiangInfoVector.size();

	//初始化购买星星的列表
	CellInfo starInfo;
	int iStartPrice = 6;
	char tempchar[64] = {0}; 
	for (int i = 0; i < 5; ++i)
	{
		starInfo.bBaoxiang = false;

		sprintf(tempchar, "buy%d_title", 6*(i+1)*10);
		if (i == 3)
		{
			sprintf(tempchar, "buy%d_title", 6*(i+1)*10+10);
		}
		starInfo.nameStr = LFStrings::getValue(tempchar);
		sprintf(tempchar, "buy%d_describe", 6*(i+1)*10);
		if (i == 3)
		{
			sprintf(tempchar, "buy%d_describe", 6*(i+1)*10+10);
		}
		starInfo.describeStr = LFStrings::getValue(tempchar);
		starInfo.iBaoxiangCount = 0;
		starInfo.iconBgNameStr = "";
		sprintf(tempchar, "daoju_xingxing_%drmb.png", iStartPrice);
		starInfo.iconNameStr = ResManager::getManager()->getSharedFilePath(g_storelayerPath+tempchar);;
		starInfo.iPrice = iStartPrice;
		if (i == 3)
		{
			starInfo.iPrice = iStartPrice+1;
		}
		sprintf(tempchar, "main_shangdian_anniu_%drmb.png", iStartPrice);
		if (i == 3)
		{
			sprintf(tempchar, "main_shangdian_anniu_%drmb.png", iStartPrice+1);
		}
		starInfo.btnIconNorNameStr = ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+tempchar);

		sprintf(tempchar, "main_shangdian_anniu_%drmb_select.png", iStartPrice);
		if (i == 3)
		{
			sprintf(tempchar, "main_shangdian_anniu_%drmb_select.png", iStartPrice+1);
		}
		starInfo.btnIconSelNameStr = ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+tempchar);
		iStartPrice = 6*(i+2);
		m_buystarInfoVector.push_back(starInfo);
	}
	m_iBuyStarCellCount = m_buystarInfoVector.size();
}

void IOSStoreLayer::initUI()
{
	//初始化背景
	m_pBg = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_bg.png").c_str());
	this->addChild(m_pBg, 1, bg_tag);
	m_pBg->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));

	m_pKaibaoxiangNode = CCScale9Sprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"nodebg.png").c_str());
	m_pKaibaoxiangNode->ignoreAnchorPointForPosition(false);
	m_pKaibaoxiangNode->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pKaibaoxiangNode->setContentSize(m_pBg->getContentSize());
	m_pKaibaoxiangNode->setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2));
	m_pBuyStarNode = CCScale9Sprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"nodebg.png").c_str());
	m_pBuyStarNode->ignoreAnchorPointForPosition(false);
	m_pBuyStarNode->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pBuyStarNode->setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2));
	m_pBuyStarNode->setContentSize(m_pBg->getContentSize());

	m_pBg->addChild(m_pKaibaoxiangNode, 3, kaibaoxiang_node_tag);
	m_pBg->addChild(m_pBuyStarNode, 3, buystar_node_tag);

	m_pBgMenu = CCMenu::create();
	m_pBgMenu->setPosition(CCPointZero);
	m_pBg->addChild(m_pBgMenu, 2);

	//左上角提示背景
	CCSprite* pTip = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_tishi.png").c_str());
	m_pBg->addChild(pTip, 0, tip_bg_tag);
	pTip->setPosition(ccp(pTip->getContentSize().width/2 - 25,
		m_pBg->getContentSize().height-pTip->getContentSize().height/2 - 30));

	//左上角提示字符串
	CCLabelTTF* pTipStr = CCLabelTTF::create(LFStrings::getValue("ShangdiangTipStr_lefttop_juqing").c_str(), fontStr_katong, 24, CCSizeMake(210, 80), kCCTextAlignmentLeft);
	m_pBg->addChild(pTipStr, 1, lefttop_tip_str_tag);
	pTipStr->setPosition(ccp(pTip->getPositionX() + 20, pTip->getPositionY() - 25));
	pTipStr->setString("");

	//中间提示字符
	CCLabelTTF* pTipCenterStr = CCLabelTTF::create(LFStrings::getValue("ShangdiangTipStr_center_01").c_str(), fontStr_katong, 24);
	m_pBg->addChild(pTipCenterStr, 1, center_tip_str_tag);
	pTipCenterStr->setColor(ccc3(150, 60, 20));
	pTipCenterStr->setPosition(ccp(m_pBg->getContentSize().width/2,  m_pBg->getContentSize().height - 160));
	pTipCenterStr->setString("");
	
	//右边星星数
	CCLabelAtlas* lable = CCLabelAtlas::create(
		"0123456789",
		ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_star_amount.png").c_str(),
		30, 47,//24
		'0');
	m_pBg->addChild(lable, 1, star_count_str_tag);
	lable->setScale(0.6f);
	char charStarCount[10] = {}; 
	int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
	sprintf(charStarCount, "%d", iStarCount);
	lable->setString(charStarCount);
	lable->setPosition(ccp(m_pBg->getContentSize().width - 120, m_pBg->getContentSize().height - 175));

	//右边字符
	CCSprite* pShuzi = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_seasonlayerPath+"season_amount.png").c_str());
	m_pBg->addChild(pShuzi, 1);
	pShuzi->setScale(0.6f);
	pShuzi->setPosition(ccp(lable->getPositionX() - 50, pTipCenterStr->getPositionY()));

	//右边星星图标
	CCSprite* pXingxing = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_xiaogk+"YX_start.png").c_str());
	m_pBg->addChild(pXingxing, 1);
	pXingxing->setPosition(ccp(pShuzi->getPositionX() - 50, pShuzi->getPositionY()));
}

void IOSStoreLayer::setComefrom(OpenBoxComeFrom openboxComefrom)
{
	m_enumComeFrom = openboxComefrom;
	bool bGet = false;
	if (m_enumComeFrom == from_MarketStore)
	{	
// 		if (History::getHistory()->getFirstOpenBoxThing(Openbox_firstFromMarketItem, bGet) && bGet)
// 		{
// 			this->setLeftTopTipStr(LFStrings::getValue("ShangdiangTipStr_lefttop_juqing").c_str());
// 			this->setTipBgVisible(true);
// 		}
// 		else
		{
			this->setTipBgVisible(false);
		}
		this->setLeftBgString(LFStrings::getValue("comefrom_MarketStore_left").c_str());
	}
	else if (m_enumComeFrom == from_GameInSuperTool)
	{
// 		if (History::getHistory()->getFirstOpenBoxThing(Openbox_firstFromSuperTool, bGet) && bGet)
// 		{
// 			this->setLeftTopTipStr(LFStrings::getValue("ShangdiangTipStr_lefttop_chaoneng").c_str());
// 			this->setTipBgVisible(true);
// 		}
// 		else
		{
			this->setTipBgVisible(false);
			this->setLeftTopTipStr("");
		}
		this->setCenterTipStr(LFStrings::getValue("ShangdiangTipStr_center_chaoneng").c_str());
		this->setLeftBgString(LFStrings::getValue("comefrom_GameInSuperTool_left").c_str());
	}
	else if (m_enumComeFrom == from_GameInHelpMap)
	{
		this->setTipBgVisible(true);
		this->setLeftTopTipStr(LFStrings::getValue("ShangdiangTipStr_lefttop_gongluetu").c_str());
		this->setCenterTipStr(LFStrings::getValue("ShangdiangTipStr_center_gongluetu").c_str());
		this->setLeftBgString(LFStrings::getValue("comefrom_GameInHelpMap_left").c_str());
	}
	else if (m_enumComeFrom == from_SeasonSelector)
	{
// 		if (History::getHistory()->getFirstOpenBoxThing(Openbox_firstFromSeasonSel, bGet)  && bGet)
// 		{
// 			this->setLeftTopTipStr(LFStrings::getValue("ShangdiangTipStr_lefttop_juqing").c_str());
// 			this->setTipBgVisible(true);
// 		}
// 		else
		{
			this->setTipBgVisible(false);
			this->setLeftTopTipStr("");
		}
		this->setLeftBgString(LFStrings::getValue("comefrom_SeasonSelector_left").c_str());
		this->setDownBgVisible(true);
	}
}

void IOSStoreLayer::setLeftBgString(const char* charString)
{
	if (CCNode* pnode = m_pKaibaoxiangNode->getChildByTag(leftbg_tag))
	{
		CCLabelTTF* pLable = (CCLabelTTF*)pnode->getChildByTag(leftbg_string_tag);
		if (pLable)
		{
			pLable->setString(charString);
			if (m_enumComeFrom == from_SeasonSelector)
			{
				pLable->setPosition(ccp(pLable->getPositionX(), 198));
			}
			else if (m_enumComeFrom == from_MarketStore)
			{
				pLable->setPosition(ccp(pLable->getPositionX(), 188));
			}
		}
	}
}

void IOSStoreLayer::setDownBgVisible(bool e)
{
	if (CCNode* pnode = m_pKaibaoxiangNode->getChildByTag(leftbg_tag))
	{
		pnode = pnode->getChildByTag(downbg_tag);
		if (pnode)
		{
			pnode->setVisible(e);
		}
		pnode = pnode->getChildByTag(downbg_string_tag);
		if (pnode)
		{
			pnode->setVisible(e);

			 int num = 10-GameInforEditer::getGameInfor()->getOpenboxNum()%10;
			 if (num == 1)
			 {
				 ((CCLabelTTF*)pnode)->setString(
					LFStrings::getValue("comefrom_SeasonSelector_getdaoju_xiaci_left").c_str());
			 }
			 else
			 {
				 ((CCLabelTTF*)pnode)->setString(
					 CCString::createWithFormat(LFStrings::getValue("comefrom_SeasonSelector_getdaoju_left").c_str(), num)->getCString());
			 }
		}
	}
}

void IOSStoreLayer::setADVisible(bool e, bool bRemove /* = false*/)
{
	if (!m_pBg)
	{
		return;
	}
	CCNode* pnode = m_pBg->getChildByTag(ad_tip_string_tag);
	if (pnode && pnode->isVisible() != e)
	{
		pnode->setVisible(e);
		if (bRemove)
		{
			pnode->removeFromParent();
		}
	}
	pnode = m_pBg->getChildByTag(ad_tip_bg_tag);
	if (pnode && pnode->isVisible() != e)
	{
		pnode->setVisible(e);
		if (bRemove)
		{
			pnode->removeFromParent();
		}
	}
}

void IOSStoreLayer::setTipBgVisible(bool e)
{
	if (CCNode* pnode = m_pBg->getChildByTag(tip_bg_tag))
	{
		pnode->setVisible(e);
	}
}

void IOSStoreLayer::setLeftTopTipStr(const char* str)
{
	CCLabelTTF* pTipStr = (CCLabelTTF*)m_pBg->getChildByTag(lefttop_tip_str_tag);
	if (pTipStr)
	{
		pTipStr->setString(str);
	}
}

void IOSStoreLayer::setLeftTopTipStrVisible(bool e)
{
	CCLabelTTF* pTipStr = (CCLabelTTF*)m_pBg->getChildByTag(lefttop_tip_str_tag);
	if (pTipStr)
	{
		pTipStr->setVisible(e);
	}
}

void IOSStoreLayer::setCenterTipStr(const char* str)
{
	CCLabelTTF* pTipStr = (CCLabelTTF*)m_pBg->getChildByTag(center_tip_str_tag);
	if (pTipStr)
	{
		pTipStr->setString(str);
	}
}

void IOSStoreLayer::setStarCount(int num)
{
	CCLabelAtlas* pTipStr = (CCLabelAtlas*)m_pBg->getChildByTag(star_count_str_tag);
	if (pTipStr)
	{
		pTipStr->setString(CCString::createWithFormat("%d", num)->getCString());
	}
	this->setKaiBaoxiangBtnVisiable();
}

void IOSStoreLayer::setKaiBaoxiangBtnVisiable()
{
	if (!m_pKaibaoxiangNode)
	{
		return;
	}
	CCTableView* pTableViwe = (CCTableView*)m_pKaibaoxiangNode->getChildByTag(kaibaoxiang_node_tableview_tag);
	if (pTableViwe)
	{
		pTableViwe->reloadData();
	}
}

void IOSStoreLayer::initMenuToggle()
{
	//开宝箱按钮开关
	CCMenuItemSprite* pNorItem = CCMenuItemSprite::create(
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_kaibaoxiang.png").c_str()),
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_kaibaoxiang.png").c_str()));
	CCMenuItemSprite* pPreItem = CCMenuItemSprite::create(
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_kaibaoxiang_select.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_kaibaoxiang_select.png").c_str()));

	CCMenuItemToggle* pToggle = CCMenuItemToggle::createWithTarget(this, 
		menu_selector(IOSStoreLayer::clickToggleCallBack), 
		pNorItem,
		pPreItem, NULL);
	pToggle->setTag(kaibaoxiang_toggle_tag);
	pToggle->setPosition(ccp(m_pBg->getContentSize().width/2 - pToggle->getContentSize().width/2, 
		m_pBg->getContentSize().height/2 + pToggle->getContentSize().width/2 +25));
	pToggle->setSelectedIndex(1);
	m_pBgMenu->addChild(pToggle);

	//购买星星按钮开关
	pNorItem = CCMenuItemSprite::create(
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_goumaixingxing.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_goumaixingxing.png").c_str()));
	pPreItem = CCMenuItemSprite::create(
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_goumaixingxing_select.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_goumaixingxing_select.png").c_str()));

	pToggle = CCMenuItemToggle::createWithTarget(this, 
		menu_selector(IOSStoreLayer::clickToggleCallBack), 
		pNorItem,
		pPreItem, NULL);
	pToggle->setTag(buystar_toggle_tag);
	pToggle->setPosition(ccp(m_pBg->getContentSize().width/2 + pToggle->getContentSize().width/2,
		m_pBg->getContentSize().height/2 + pToggle->getContentSize().width/2 + 25));
	m_pBgMenu->addChild(pToggle);
}

void IOSStoreLayer::initBtn()
{
	//关闭按钮
	CCMenuItemImage* pClose = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi_select.png").c_str(), 
		this,
		menu_selector(IOSStoreLayer::menuBtnCallBack));
	pClose->setTag(close_btn_tag);
	m_pBgMenu->addChild(pClose);

	CCNode* pBg = this->getChildByTag(bg_tag);
	if (pBg)
	{
		pClose->setPosition(ccp(pBg->getContentSize().width - pClose->getContentSize().width/2 + 20, 
			pBg->getContentSize().height - pClose->getContentSize().height/2 - 40));
	}
}

void IOSStoreLayer::initKaibaoxiangNode()
{
	//获取要显示的所有道具的个数
	if (m_iBaoxiangCellCount <= 0 )
	{
		return;
	}
	//添加道具的tableview
	CCTableView* tableView = (CCTableView*)m_pKaibaoxiangNode->getChildByTag(kaibaoxiang_node_tableview_tag);
	if (tableView)
	{
		tableView->removeFromParent();
	}
	tableView = CCTableView::create(this, CCSizeMake(230*m_iBaoxiangCellCount, 380));
// 	tableView->ignoreAnchorPointForPosition(false);
// 	tableView->setAnchorPoint(ccp(0.5f, 0.5f));
	tableView->setViewSize(CCSizeMake(920, 380));
	tableView->setDirection(kCCScrollViewDirectionHorizontal);
	tableView->setPosition(ccp(35, 40));
// 	tableView->setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2));
	tableView->setDelegate(this);
	tableView->setTag(kaibaoxiang_node_tableview_tag);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(tableView);

	if (m_iBaoxiangCellCount > 4)
	{
		tableView->runAction(CCSequence::create(CCDelayTime::create(0.01f), CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::UpTableViewPrioority)), NULL));
	}
	
	m_pKaibaoxiangNode->addChild(tableView, 2);

	tableView->reloadData();
	if (m_iBaoxiangCellCount > 0 && m_iBaoxiangCellCount <= 2)
	{
		tableView->setViewSize(CCSizeMake(230*m_iBaoxiangCellCount, 380));
	}
	if(m_iBaoxiangCellCount == 1)
	{
		tableView->setPosition(ccp(m_pBg->getContentSize().width/2 - 115, 40));
	}
	else if (m_iBaoxiangCellCount == 2)
	{
		tableView->setPosition(ccp(m_pBg->getContentSize().width/2 - 220, 40));
	}
	else if (m_iBaoxiangCellCount == 3)
	{
		tableView->setPosition(ccp(m_pBg->getContentSize().width/2 - 345, 40));
	}

	//右边说明文字背景
	CCSprite* pLeftBg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg_2.png").c_str());
	pLeftBg->setPosition(ccp(tableView->getPositionX() - pLeftBg->getContentSize().width/2, tableView->getPositionY()+pLeftBg->getContentSize().height/2));
	m_pKaibaoxiangNode->addChild(pLeftBg);
	pLeftBg->setTag(leftbg_tag);

	//右边说明文字
	CCLabelTTF* pLabel = CCLabelTTF::create("", fontStr_katong, 24, CCSize(180, 0), kCCTextAlignmentLeft);
	pLabel->setColor(fontColor_Store);
	pLabel->setPosition(ccp(pLeftBg->getContentSize().width/2, pLeftBg->getContentSize().height/2+10));
	pLeftBg->addChild(pLabel, 1);
	pLabel->setTag(leftbg_string_tag);

	//下边说明文字背景
	CCSprite* pDownBg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg_3.png").c_str());
	pDownBg->setPosition(ccp(pLeftBg->getContentSize().width/2, pLeftBg->getContentSize().height/2));
	pLeftBg->addChild(pDownBg, 1, downbg_tag);

	CCLabelTTF* pDownLabel = CCLabelTTF::create("", fontStr_katong, 24, CCSize(200, 0), kCCTextAlignmentCenter);
	pDownLabel->setPosition(ccp(pDownBg->getPositionX()-3, 60));
	pDownBg->addChild(pDownLabel, 1);
	pDownLabel->setTag(downbg_string_tag);

	if (m_enumComeFrom != from_SeasonSelector)
	{
		pDownBg->setVisible(false);
		pDownLabel->setVisible(false);
	}

	this->setComefrom(m_enumComeFrom);
}

void IOSStoreLayer::initBuystarNode()
{
	//获取要显示的所有道具的个数
	if (m_iBuyStarCellCount <= 0 )
	{
		return;
	}
	//添加道具的tableview
	CCTableView* tableView = (CCTableView*)m_pKaibaoxiangNode->getChildByTag(buystar_node_tableview_tag);
	if (tableView)
	{
		tableView->removeFromParent();
	}
	tableView = CCTableView::create(this, CCSizeMake(230*m_iBuyStarCellCount, 380));
	tableView->setViewSize(CCSizeMake(920, 380));
	tableView->setDirection(kCCScrollViewDirectionHorizontal);
	tableView->setPosition(ccp(35, 40));
	tableView->setDelegate(this);
	tableView->setTag(buystar_node_tableview_tag);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(tableView);
	if (m_iBuyStarCellCount > 4)
	{
		tableView->runAction(CCSequence::create(CCDelayTime::create(0.01f), CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::UpTableViewPrioority)), NULL));
	}
	m_pBuyStarNode->addChild(tableView, 2);
	tableView->reloadData();

	//左边箭头
	CCSprite* pLeftArrows = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"sectionItem_buttom_left.png").c_str());
	m_pBuyStarNode->addChild(pLeftArrows, 3, left_arrow_tag);
	pLeftArrows->setPosition(ccp(pLeftArrows->getContentSize().width/2, tableView->getViewSize().height/2 + tableView->getPositionY()));
	float dt = 1.2f;
	pLeftArrows->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(dt), CCFadeOut::create(dt), NULL)));
	pLeftArrows->setVisible(false);

	//右边箭头
	CCSprite* pRightArrows = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"sectionItem_buttom_right.png").c_str());
	m_pBuyStarNode->addChild(pRightArrows, 3, right_arrow_tag);
	pRightArrows->setPosition(ccp(tableView->getViewSize().width + tableView->getPositionX(), pLeftArrows->getPositionY()));
	pRightArrows->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(dt), CCFadeOut::create(dt), NULL)));

}

void IOSStoreLayer::UpTableViewPrioority(CCNode* pNode)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((CCTableView*)pNode, kCCMenuHandlerPriority-1, true);
}

void IOSStoreLayer::scrollViewDidScroll(CCScrollView* view)
{
	CCLog("scroll....");
	CCLog("scroll %f, %f", view->getContentOffset().x, view->getContentOffset().y);
	CCNode* pnodeLeft = m_pBuyStarNode->getChildByTag(left_arrow_tag);
	CCNode* pnodeRight = m_pBuyStarNode->getChildByTag(right_arrow_tag);
	CCPoint point = view->getContentOffset();
	if (!pnodeLeft || !pnodeRight)
	{
		return;
	}
	//显示左边隐藏右边
	if (point.x <= -210)
	{
		if (!pnodeLeft->isVisible())
		{
			pnodeLeft->setVisible(true);
		}
		if (pnodeRight->isVisible())
		{
			pnodeRight->setVisible(false);
		}
	}
	//隐藏左边显示右边
	else if (point.x >= 0)
	{
		if (pnodeLeft->isVisible())
		{
			pnodeLeft->setVisible(false);
		}
		if (!pnodeRight->isVisible())
		{
			pnodeRight->setVisible(true);
		}
	}
	//左右都显示
	else
	{
		if (!pnodeLeft->isVisible())
		{
			pnodeLeft->setVisible(true);
		}
		if (!pnodeRight->isVisible())
		{
			pnodeRight->setVisible(true);
		}
	}
}

CCTableViewCell* IOSStoreLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell *cell = table->dequeueCell();
	if (cell)
	{
		cell->removeAllChildren();
	}
	else
	{
		cell = new CCTableViewCell;
		cell->autorelease();
	}

	//根据标签来初创建不同的cell
	CCSprite* pDaoju = NULL;
	if (table->getTag() == kaibaoxiang_node_tableview_tag)
	{
		if (idx >= 0 && (int)idx < m_iBaoxiangCellCount)
		{
			pDaoju = this->createCellSpr(idx, &m_baoxiangInfoVector.at(idx));
		}
	}
	else
	{
		if (idx >= 0 && (int)idx < m_iBuyStarCellCount)
		{
			pDaoju = this->createCellSpr(idx, &m_buystarInfoVector.at(idx));
		}
	}

	if (pDaoju)
	{
		pDaoju->setPosition(CCPointZero);
		if (idx == 0)
		{
			pDaoju->setPosition(ccp(10, 0));
		}
		pDaoju->setAnchorPoint(ccp(0,0));
		cell->addChild(pDaoju);
	}

	return cell;
}

void IOSStoreLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	int iTag = table->getTag();
	if (iTag == kaibaoxiang_node_tableview_tag)
	{
		return;
	}
	CCLog("%d", cell->getIdx());

}


CCSize IOSStoreLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return CCSizeMake(230, 396);
}

unsigned int IOSStoreLayer::numberOfCellsInTableView(CCTableView *table)
{
	int	iRet = 0; 
	int iTag = table->getTag();
	if (iTag == kaibaoxiang_node_tableview_tag)
	{
		iRet = m_iBaoxiangCellCount;
	}
	else
	{
		iRet = m_iBuyStarCellCount;
	}
	return iRet;
}

CCSprite* IOSStoreLayer::createCellSpr(int index, CellInfo* daojuInfo)
{
	CCSprite* pDaoju = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg.png").c_str());

	//显示道具的名字
	CCLabelTTF* pTitle = CCLabelTTF::create(daojuInfo->nameStr.c_str(), fontStr_katong, 24);
	pDaoju->addChild(pTitle);
	pTitle->setColor(fontColor_Store);
	pTitle->setPosition(ccp(pDaoju->getContentSize().width/2, pDaoju->getContentSize().height - 40));

	//显示道具的描述
	CCLabelTTF* pDescribe = CCLabelTTF::create(daojuInfo->describeStr.c_str(), fontStr_katong, 24, CCSizeMake(200, 60), kCCTextAlignmentLeft);
	pDaoju->addChild(pDescribe);
	pDescribe->setAnchorPoint(ccp(0, 0.5f));
	pDescribe->setHorizontalAlignment(kCCTextAlignmentLeft);
	pDescribe->setPosition(ccp(10, 85));

	if (daojuInfo->bBaoxiang)
	{
		pDescribe->setPosition(ccp(10, 65));
	}

	//显示图标
	CCSprite* pIcon = CCSprite::create(daojuInfo->iconNameStr.c_str());
//	if (pIcon)
	{
		pIcon->setPosition(ccp(pDaoju->getContentSize().width/2, pDaoju->getContentSize().height/2+20));
		pDaoju->addChild(pIcon, 1);

		CCSprite *mask = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_fugai.png").c_str());
		pDaoju->addChild(mask, 2);
		mask->setPosition(ccp(pIcon->getPositionX(), pIcon->getPositionY()+20));
	}

	//宝箱需要设置缩放为35%
	if (daojuInfo->bBaoxiang)
	{
		pIcon->setScale(0.35f);
	}

	//底部按钮
	CCMenuItemImage* pMenuItem = CCMenuItemImage::create(
		daojuInfo->btnIconNorNameStr.c_str(), 
		daojuInfo->btnIconSelNameStr.c_str(), 
		this, 
		menu_selector(IOSStoreLayer::menuBtnCallBack));

	//星星数不够就显示文字,够的话就显示开宝箱按钮
	if (pMenuItem)
	{
		bool bMore = false;
		int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
		if (daojuInfo->bBaoxiang)
		{
			if (daojuInfo->iBaoxiangCount == 10)
			{
				if (iStarCount >= 95)
				{
					bMore = true;
				}
			}
			else if (daojuInfo->iBaoxiangCount == 1)
			{
				if (iStarCount >= 10)
				{
					bMore = true;
				}
			}
		}
		else
		{
			bMore = true;
		}

		CCMenu* pMenu = CCMenu::create();
		pMenu->setPosition(CCPointZero);
		pDaoju->addChild(pMenu, 1);
		pMenu->addChild(pMenuItem);
		pMenuItem->setPosition(ccp(pDaoju->getContentSize().width/2,  pMenuItem->getContentSize().height/2));
		pMenu->setTouchPriority(kCCMenuHandlerPriority-3);
		pMenuItem->setVisible(false);

		if (bMore)
		{
			pMenuItem->setVisible(true);
		}
		else if (daojuInfo->bBaoxiang)
		{
			//星星不足时,显示的字符串
			CCLabelTTF* pTip = CCLabelTTF::create(LFStrings::getValue("Starless").c_str(), fontStr_katong, 24, CCSizeMake(200, 60), kCCTextAlignmentCenter);
			pDaoju->addChild(pTip,1);
			pTip->setPosition(ccp(pDaoju->getContentSize().width/2,  pMenuItem->getContentSize().height/2+5));
		}
	}

	//宝箱和购买星星区别加载
	if (daojuInfo->bBaoxiang)
	{
		//十个宝箱
		if (daojuInfo->iBaoxiangCount == 10)
		{
			pIcon = CCSprite::create(daojuInfo->iconBgNameStr.c_str());
			if (pIcon)
			{
				pIcon->setPosition(ccp(pDaoju->getContentSize().width/2, pDaoju->getContentSize().height/2+20));
				pDaoju->addChild(pIcon);
				pIcon->setScale(0.35f);
			}
			//十次
			pMenuItem->setTag(tentimes_btn_tag);
		}
		else if (daojuInfo->iBaoxiangCount == 1)
		{
			//一次
			pMenuItem->setTag(onetimes_btn_tag);
		}
	}
	else
	{
		//按照价格来设置购买按钮的tag
		pMenuItem->setTag(daojuInfo->iPrice);
	}

	return pDaoju;
}

void IOSStoreLayer::menuBtnCallBack(CCObject* pObj)
{
	CCLog("menubtn call back");
	CCMenuItemImage* pItme = (CCMenuItemImage*)pObj;
	int iTag = pItme->getTag();
	switch (iTag)
	{
	case close_btn_tag:
		{
			m_pBg->runAction(CCSequence::create(CCScaleTo::create(0.2f, 0.0f), CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::removeCurLayer)), NULL));
		}
		break;

	case onetimes_btn_tag:
		{
			CCLog("one times btn click....");
			m_bBaoxiangBuyTenTimes = false;
			m_pBg->setVisible(false);
			this->showBoxAniLayer();

			//CmdHelper::getHelper()->cmdOpenBox(1, m_enumComeFrom);
		}
		break;

	case tentimes_btn_tag:
		{
			CCLog("ten times btn click....");
			m_pBg->setVisible(false);
			m_bBaoxiangBuyTenTimes = true;
			this->showBoxAniLayer();

			//CmdHelper::getHelper()->cmdOpenBox(10, m_enumComeFrom);
		}
		break;
	case buy_one_again:
		{
			CCLog("buy_one_again click....");
			m_bBaoxiangBuyTenTimes = false;
			this->showBoxAniLayer();

			//CmdHelper::getHelper()->cmdOpenBox(1, m_enumComeFrom);
		}
		break;

	case buy_ten_again:
		{
			CCLog("buy_ten_again click....");
			m_bBaoxiangBuyTenTimes = true;
			this->showBoxAniLayer();

			//CmdHelper::getHelper()->cmdOpenBox(10, m_enumComeFrom);
		}
		break;

	case ok_btn_tag:
		{
			CCLog("ok_btn_tag click....");
			if (m_enumComeFrom == from_MarketStore)
			{
				this->backMainNode();
				this->setLeftTopTipStr("");
				this->setTipBgVisible(false);
			}
			else if (m_enumComeFrom == from_GameInSuperTool)
			{
				this->setLeftTopTipStr("");
				this->setTipBgVisible(false);
				bool bSuperTool = false;
				for (size_t i = 0; i < m_kaiBaoxiangRspVector.size(); ++i)
				{
					if (m_kaiBaoxiangRspVector.at(i).item == BuyItem::itemtype_SuperTools)
					{
						bSuperTool = true;
						break;
					}
				}
				if (bSuperTool)
				{
					//已抽到超能道具，跳转到超能道具使用界面
					IOSMarketUseSuperToolLayer* pLayer = new IOSMarketUseSuperToolLayer();
					CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 100);
					pLayer->release();
					this->removeFromParent();
				}
				else
				{
					//未抽到超能道具，跳转到开宝箱界面
					this->backMainNode();
				}
			}
			else if (m_enumComeFrom == from_GameInHelpMap)
			{
				//bool be = false;
				//if (History::getHistory()->getFirstOpenBoxThing(Openbox_firstFromHelpMap, be) && be)
				{
					this->removeCurLayer();
					CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BuyHelpMapSuccess, NULL);
				}
				
			}
			else if (m_enumComeFrom == from_SeasonSelector)
			{
				this->setLeftTopTipStr("");
				this->setTipBgVisible(false);
				bool bBack = false;
				std::vector<JuqingDaoJu> daojuVector = JuqingDaoJuManager::getManager()->getAllItemsInSeason(GameInforEditer::getGameInfor()->getLastPlayedSeasonId());
				for (size_t i = 0; i < daojuVector.size(); ++i)
				{
					if (daojuVector.at(i).state == DJS_NotGetted)
					{
						bBack = true;
						break;
					}
				}

				if (bBack)
				{
					//未收集齐解锁的4个剧情道具,跳转到开宝箱界面
					this->backMainNode();
				}
				else
				{
					//已收集齐解锁的4个剧情道具，跳转至剧情道具和插画动画显示流程
					if (BaseScense* pScene = BaseScense::getCurrentScene())
					{
						SeasonSelectorLayerGameGoods* pgamegoods = dynamic_cast<SeasonSelectorLayerGameGoods*>(pScene->getChildByTag(LevelManager::gamegoods_tag));
						SeasonInfor * _seasonInfo = GameInforEditer::getGameInfor()->getSeasonInfor(GameInforEditer::getGameInfor()->getLastPlayedSeasonId());
						if (pgamegoods && _seasonInfo && _seasonInfo->getSectionInfor(20) && _seasonInfo->getSectionInfor(20)->iStarNum > 0)
						{
							pgamegoods->showOpenUnlock();
						}
					}
					this->removeCurLayer();
				}
			}
			else
			{
				this->removeCurLayer();
			}
		}
		break;

	default:
		break;
	}
}

void IOSStoreLayer::backMainNode()
{
	if (m_pGetItemNode)
	{
		m_pGetItemNode->removeFromParent();

		m_pBg->setVisible(true);
//		this->initKaibaoxiangNode();
		CCTableView* p = (CCTableView*)m_pKaibaoxiangNode->getChildByTag(kaibaoxiang_node_tableview_tag);
		if (p)
		{
			p->reloadData();
		}
	}

	this->setComefrom(m_enumComeFrom);

	int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
	this->setStarCount(iStarCount);
}

void IOSStoreLayer::showBoxAniLayer()
{
	m_pGetItemNode = this->getChildByTag(getitme_node_tag);
	if (m_pGetItemNode)
	{
		m_pGetItemNode->removeFromParent();
	}
	m_pGetItemNode = CCNode::create();
	this->addChild(m_pGetItemNode, 2, getitme_node_tag);

	CCSprite* pBox = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang.png").c_str());
	m_pGetItemNode->addChild(pBox, 4, box_tag);
	pBox->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));

#define ROTATE 10
#define TIME 0.025f
	pBox->runAction(CCSequence::create(
		CCScaleTo::create(0.1f, 1.05f),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCScaleTo::create(0.1f, 1.0f),
		
		CCSpawn::create(
		CCScaleTo::create(0.1f, 1.2f),
		CCMoveBy::create(0.1f, ccp(0, 50)),
	
		NULL),
			CCScaleTo::create(0.1f, 1.0f),
		CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::showOpenBoxAni)),
		NULL));
	CCSprite* pGuangquan = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang_xiaoguo_1.png").c_str());
	m_pGetItemNode->addChild(pGuangquan, -1, guanquan_tag);
	pGuangquan->setPosition(pBox->getPosition());
	CCRotateBy* rotate = CCRotateBy::create(0.5f, 30);
	pGuangquan->runAction(CCSequence::create(CCFadeIn::create(0.65f), NULL));
	pGuangquan->runAction(CCRepeatForever::create(rotate));
	pGuangquan->setScale(0.9f);
}

void IOSStoreLayer::showOpenBoxAni(CCNode* pNode)
{
	//播放打开动画
	CCSprite* pBox = (CCSprite*)pNode;
	CCAnimation* pAnimation = CCAnimation::create();
	char filename[64] = {};
	for (size_t i = 0; i < 2; ++i)
	{
		sprintf(filename, "daoju_baoxiang%d.png", i+2);
		pAnimation->addSpriteFrameWithFileName(ResManager::getManager()->getSharedFilePath(g_storelayerPath+filename).c_str());
	}
	pAnimation->setDelayPerUnit(0.05f);
	pAnimation->setLoops(1);
	pBox->runAction(CCSequence::create(
		CCAnimate::create(pAnimation),
		CCDelayTime::create(0.15f),
		CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::boxStartMove)),
		NULL));
	pBox->runAction(CCSequence::create(
		CCDelayTime::create(0.05f), 
		CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::showRandStars)),
		NULL));
// 	//出现大的星星
// 	CCSprite* pBigStar = CCSprite::create("daoju_baoxiang_xiaoguo_2.png");
// 	pBox->addChild(pBigStar, 1, box_big_star_tag);
// 	pBigStar->setPosition(ccp(pBox->getContentSize().width/2 - 20, pBox->getContentSize().height/2));
// 	pBigStar->setScale(0.1f);
// 	pBigStar->runAction(CCSequence::create(
// 		CCScaleTo::create(0.2f, 1.0f),
// 		CCRemoveSelf::create(),
// 		NULL));
}

void IOSStoreLayer::boxStartMove(CCNode* pBox)
{
	float dt = 0.2f;
	pBox->runAction(CCSequence::create(
		CCSpawn::create(CCMoveTo::create(dt, ccp(this->getContentSize().width/2, this->getContentSize().height - 170)), 
			CCScaleTo::create(dt, 0.5f),
			NULL),
		CCCallFuncN::create(this, callfuncN_selector(IOSStoreLayer::showStarsFadeEffect)),
		NULL));

	CCNode* pnode = m_pGetItemNode->getChildByTag(guanquan_tag);
	if (pnode)
	{
		pnode->runAction(CCSpawn::create(
			CCMoveTo::create(dt, ccp(this->getContentSize().width/2, this->getContentSize().height - 170)), 
			CCScaleTo::create(dt, 0.5f),
			NULL));
	}
}

void IOSStoreLayer::showItemsNode()
{
	int iTotalCount = 1;
	int iPrice = 10;
	int xcount = 1;
	int ycount = 1;
	CCSize size = CCSizeMake(120, 190);

	if (m_bBaoxiangBuyTenTimes)
	{
		iTotalCount = 10;
		size.width = 820;
		size.height = 350;
		xcount = 5;
		ycount = 2;
		iPrice = 95;
	}

	iTotalCount = this->getKaiBaoxiangRsqData().size();
	if (iTotalCount >= 11)
	{
		xcount = 6;
	}

	if (iTotalCount == 2)
	{
		xcount = 2;
		size = CCSizeMake(140*2, 190);
	}

	IOSStoreLayerScrollView* pView = IOSStoreLayerScrollView::createView(size, iTotalCount, xcount, ycount);
	m_pGetItemNode->addChild(pView);
	pView->ignoreAnchorPointForPosition(false);
	pView->setAnchorPoint(ccp(0.5f, 0.5f));
	pView->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2 - 60));
	pView->setClickableCreater(pView);
	pView->prepareToDisplay();
	pView->setTouchEnabled(false);
	pView->setTag(getitem_scrollview_tag);

	//显示
	CCSprite* pSmallbg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_anniu_zaikaishici_fujia.png").c_str());
	m_pGetItemNode->addChild(pSmallbg, 1);
	pSmallbg->setPosition(ccp(200, 100));

	//添加菜单
	CCMenu* pMenu = CCMenu::create();
	pMenu->setPosition(CCPointZero);
	m_pGetItemNode->addChild(pMenu, 2);
	
	//再开按钮
	CCMenuItemImage* pItem = NULL;
	int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
	if (m_bBaoxiangBuyTenTimes)
	{
		pItem = CCMenuItemImage::create(
			ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_zaikaishici.png").c_str(),
			ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_zaikaishici_select.png").c_str(),
			this, menu_selector(IOSStoreLayer::menuBtnCallBack));
		pItem->setTag(buy_ten_again);
	}
	else
	{
		pItem = CCMenuItemImage::create(
			ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_zaikaiyici.png").c_str(),
			ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_zaikaiyici_select.png").c_str(),
			this, menu_selector(IOSStoreLayer::menuBtnCallBack));
		pItem->setTag(buy_one_again);
	}
	pItem->setPosition(ccp(pSmallbg->getPositionX() + pSmallbg->getContentSize().width/2 + pItem->getContentSize().width/2 - 30, 
		pSmallbg->getPositionY()));
	pMenu->addChild(pItem);

	//星星数量
	CCLabelAtlas* lable = CCLabelAtlas::create(
		"0123456789",
		ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_star_amount.png").c_str(),
		30, 47,//24
		'0');
	m_pGetItemNode->addChild(lable, 1, star_count_str_tag);
	lable->setAnchorPoint(ccp(0, 0.5));
	lable->setScale(0.6f);
	int iCount = 10;
	if (m_bBaoxiangBuyTenTimes)
	{
		iCount = 95;
	}
	char charStarCount[10] = {}; 
	sprintf(charStarCount, "%d", iCount);
	lable->setString(charStarCount);
	lable->setPosition(ccp(pItem->getPositionX()-pItem->getContentSize().width/2-lable->getContentSize().width/2 - 40, pSmallbg->getPositionY() - 3));

	//右边星星图标
	CCSprite* pXingxing = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_xiaogk+"YX_start.png").c_str());
	m_pGetItemNode->addChild(pXingxing, 1);
	pXingxing->setPosition(ccp(lable->getPositionX() - 30, pSmallbg->getPositionY()));

	//星星数不够时,只显示确定按钮
	//确定按钮
	CCMenuItemImage* pOKItem = CCMenuItemImage::create(
		ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_quedingi.png").c_str(),
		ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_quedingi_select.png").c_str(),
		this, menu_selector(IOSStoreLayer::menuBtnCallBack));
	pOKItem->setTag(ok_btn_tag);
	pOKItem->setPosition(ccp(this->getContentSize().width/2 + pOKItem->getContentSize().width, pSmallbg->getPositionY()));
	pMenu->addChild(pOKItem);

	if (iStarCount < iPrice)
	{
		lable->setVisible(false);
		pXingxing->setVisible(false);
		pSmallbg->setVisible(false);
		pItem->setVisible(false);

		pOKItem->setPosition(ccp(this->getContentSize().width/2, pSmallbg->getPositionY()));
	}
	pMenu->setTouchPriority(kCCMenuHandlerPriority-5);
}

void IOSStoreLayer::showStarsFadeEffect(CCNode* pBox)
{
	int iStarCount = 8;
	CCPointArray* pPointArray = randStarPoint(iStarCount);
	for (int i = 0; i < iStarCount; ++i)
	{
		CCSprite* pStar = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang_xiaoguo_2.png").c_str());
		pStar->setScale(this->randScale());
		pStar->setRotation(this->randRotation());
		pBox->addChild(pStar);
		CCPoint point = pPointArray->getControlPointAtIndex(i);
		pStar->setPosition(ccp(point.x , point.y));
		pStar->setTag(box_star_tag);

		float dt = CCRANDOM_0_1() + 1.0f;
		float scale = CCRANDOM_0_1() + 0.5f;
		pStar->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(dt), CCFadeIn::create(dt), NULL)));
		CCScaleBy* pscaleby = CCScaleBy::create(dt, scale);
		pStar->runAction(CCRepeatForever::create(CCSequence::create(pscaleby, pscaleby->reverse(), NULL)));
	}
}

void IOSStoreLayer::showRandStars(CCNode* pParent)
{
	//设置随机种子
	srand((size_t)time(NULL));
	int iStarCount = 11;//this->randStarCount();

	CCPoint ppoint = pParent->getPosition();

	std::vector<int> angleVector;
	angleVector.push_back(20);
	angleVector.push_back(-45);
	angleVector.push_back(90);
	angleVector.push_back(-360);
	angleVector.push_back(123);
	angleVector.push_back(-164);
	angleVector.push_back(209);
	angleVector.push_back(294);
	angleVector.push_back(-279);
	angleVector.push_back(329);
	angleVector.push_back(340);

	for (int i = 0; i < iStarCount; ++i)
	{
		CCSprite* pStar = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"daoju_baoxiang_xiaoguo_2.png").c_str());
		pStar->setScale(this->randScale());
		pStar->setRotation(this->randRotation());
		m_pGetItemNode->addChild(pStar, 15);
		pStar->setPosition(ccp(ppoint.x , ppoint.y+50));
		pStar->setTag(box_star_tag);

		CCPoint point;

		float angle = angleVector.at(i);//CC_RADIANS_TO_DEGREES(angleVector.at(i));
 		float ds = 768;
  		point.x = sin(angle)*ds;
 		point.y = cos(angle)*ds;

		float dt = 0.2f;
		pStar->runAction(CCRepeatForever::create(CCRotateBy::create(0.035f, 180)));
		pStar->runAction(CCSequence::create(
			CCSpawn::create(
			CCEaseIn::create(CCMoveBy::create(0.9f, point), 0.25f),
			NULL),
			CCRemoveSelf::create(),
			NULL));
	}

	this->showItemsNode();
}

int IOSStoreLayer::randStarCount()
{
	int iRet = 8;

	int max = 15;
	int min = iRet;
	iRet = rand()%(max-min) + min;

	return iRet;
}

float IOSStoreLayer::randScale()
{
	float fRet = 0.65f;
	float maxScale = 0.8f;
	float minScale = 0.5f;
	fRet = CCRANDOM_0_1()*(maxScale-minScale) + minScale;

	return fRet;
}

float IOSStoreLayer::randRotation()
{
	float fRet = 0.0f;
	float maxScale = 360.0f;
	float minScale = 0.0f;
	fRet = CCRANDOM_0_1()*(maxScale-minScale) + minScale;

	return fRet;
}

CCPointArray* IOSStoreLayer::randStarPoint(int iCount)
{
	CCPointArray* pointRet = CCPointArray::create(iCount);

	pointRet->addControlPoint(ccp(60, 210));
	pointRet->addControlPoint(ccp(80, 330));
	pointRet->addControlPoint(ccp(115, 450));
	pointRet->addControlPoint(ccp(270, 570));
	pointRet->addControlPoint(ccp(230, 100));
	pointRet->addControlPoint(ccp(425, 550));
	pointRet->addControlPoint(ccp(550, 410));
	pointRet->addControlPoint(ccp(540, 180));

	return pointRet;
}

void IOSStoreLayer::removeCurLayer(CCNode* pNode /* = NULL*/)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MarketClosed, NULL);
	this->removeFromParent();
}

void IOSStoreLayer::clickToggleCallBack(CCObject* pObj)
{
	CCLog("menutoggle call back");
	CCMenuItemToggle* pToggle = (CCMenuItemToggle*)pObj;
	int iTag = pToggle->getTag();
	int index = pToggle->getSelectedIndex();
	switch (iTag)
	{
	case kaibaoxiang_toggle_tag:
		{
			m_iCurShowNodeIndex = kaibaoxiang_node_index;

			if (m_enumComeFrom == from_GameInSuperTool)
			{
				this->setComefrom(m_enumComeFrom);
			}
			else if (m_enumComeFrom == from_SeasonSelector)
			{
				this->setComefrom(m_enumComeFrom);
			}
			else if (m_enumComeFrom == from_MarketStore)
			{
				this->setComefrom(m_enumComeFrom);
			}
			else if (m_enumComeFrom == from_GameInHelpMap)
			{
				this->setTipBgVisible(true);
			}
			this->setLeftTopTipStrVisible(true);
			this->setADVisible(false);
		}
		break;

	case buystar_toggle_tag:
		{
			m_iCurShowNodeIndex = buystar_node_index;
			this->setLeftTopTipStrVisible(false);
			this->setTipBgVisible(false);
			this->setADVisible(true);
		}
		break;

	default:
		break;
	}
	
	this->setToggleStateByNodeIndex(m_iCurShowNodeIndex);

	if (index == 1)
	{
		if (m_iCurShowNodeIndex == buystar_node_index)
		{
			this->replaceLayer(m_pKaibaoxiangNode, m_pBuyStarNode, this, SEL_CallFuncN(&IOSStoreLayer::setTableViewVisble));
		}
		else
		{
			this->replaceLayer(m_pBuyStarNode, m_pKaibaoxiangNode, this, SEL_CallFuncN(&IOSStoreLayer::setTableViewVisble));
		}
	}
}

void IOSStoreLayer::setToggleStateByNodeIndex(int iNodeIndex)
{
	CCMenuItemToggle* pToggle = NULL;
	CCNode* pNode = NULL;
	for (int i = 0; i < 2; ++i)
	{
		pNode = m_pBg->getChildByTag(kaibaoxiang_node_tag+i);
		pToggle = (CCMenuItemToggle*)m_pBgMenu->getChildByTag(kaibaoxiang_toggle_tag+i);
		if (iNodeIndex == i)
		{
			if (pNode)
			{
//				pNode->setVisible(true);
//				this->setTableViewVisble(pNode, (void*)true);
				pToggle->setSelectedIndex(1);
			}
		}
		else
		{
			if (pNode)
			{
//				pNode->setVisible(false);
//				this->setTableViewVisble(pNode, (void*)false);
				pToggle->setSelectedIndex(0);
			}
		}
	}
}


// void IOSStoreLayer::setToggleStateByNodeIndex(int iNodeIndex)
// {
// 	CCMenuItemToggle* pToggle = NULL;
// 	CCNode* pNode = NULL;
// 	for (int i = 0; i < 2; ++i)
// 	{
// 		pNode = m_pBg->getChildByTag(kaibaoxiang_node_tag+i);
// 		pToggle = (CCMenuItemToggle*)m_pBgMenu->getChildByTag(kaibaoxiang_toggle_tag+i);
// 		if (iNodeIndex == i)
// 		{
// 			if (pNode)
// 			{
// // 				pNode->runAction(CCSequence::create(
// // 					CCFadeIn::create(0.2f), CCHide::create(), 
// // 					CCCallFuncND::create(this, callfuncND_selector(IOSStoreLayer::setTableViewVisble), 
// // 					(void*)true), 
// // 					NULL));
// 				this->setTableViewVisble(pNode, (void*)true);
// 				pToggle->setSelectedIndex(1);
// 			}
// 		}
// 		else
// 		{
// 			if (pNode)
// 			{
// // 				pNode->runAction(CCSequence::create(
// // 					CCShow::create(),
// // 					CCFadeOut::create(0.2f), 
// // 					CCCallFuncND::create(this, callfuncND_selector(IOSStoreLayer::setTableViewVisble), 
// // 					(void*)false), NULL));
// 				this->setTableViewVisble(pNode, (void*)false);
// 				pToggle->setSelectedIndex(0);
// 			}
// 		}
// 	}
// }


void IOSStoreLayer::setTableViewVisble(CCNode* pNode, bool e)
{
	int iTag = pNode->getTag();
	if (iTag == kaibaoxiang_node_tag)
	{
		if (CCNode* p = pNode->getChildByTag(kaibaoxiang_node_tableview_tag))
		{
			p->setVisible(e);

			if (p = pNode->getChildByTag(leftbg_tag))
			{
				if (p->isVisible() != e)
				{
					p->setVisible(e);
				}
			}
		}
	}
	else if (iTag == buystar_node_tag)
	{
		if (CCNode* p = pNode->getChildByTag(buystar_node_tableview_tag))
		{
			if (p->isVisible() != e)
			{
				p->setVisible(e);
			}
		}
	}
}

void IOSStoreLayer::setTableViewVisble(CCNode* pNode)
{
	bool e = false;
	if (m_iCurShowNodeIndex == buystar_node_index)
	{
		e = true;
	}
	this->setTableViewVisble(m_pBuyStarNode, e);
	this->setTableViewVisble(m_pKaibaoxiangNode, !e);
}


bool IOSStoreLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchPoint = pTouch->getLocation();
	return true;
}


void IOSStoreLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void IOSStoreLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void IOSStoreLayer::payEnd(CCObject* pObj)
{
// 	NetworkWaitingUI::hideWaiting();
// 	SPCmd_PayEndRsp* rsp = (SPCmd_PayEndRsp*)((MSG_Rsp*)pObj)->getData();
// 	if (rsp->err_code != Success)
// 	{
// 		CCLog("%s ---> error code = %d", __FUNCTION__, rsp->err_code);
// 		if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 		{
// 			CCMessageBox(LFStrings::getValue("wangluoguzhang").c_str(), LFStrings::getValue("cuowu").c_str());
// 		}
// 		return;
// 	}
// 	else if (rsp->err_code == Success && rsp->status)
// 	{
// 		if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 		{
// 			CCMessageBox(LFStrings::getValue("buy_suc").c_str(), LFStrings::getValue("input_ok").c_str());
// 		}
// 		int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
// 		this->setStarCount(iStarCount);
// 		this->setADVisible(false, true);
// 	}
}

void IOSStoreLayer::openBoxRsp(CCObject* pObj)
{
// 	NetworkWaitingUI::hideWaiting();
// 	SPCmd_OpenBoxRsp* rsp = (SPCmd_OpenBoxRsp*)((MSG_Rsp*)pObj)->getData();
// 	if (rsp->err_code != Success)
// 	{
// 		CCLog("%s ---> error code = %d", __FUNCTION__, rsp->err_code);
// 		CCMessageBox(LFStrings::getValue("wangluoguzhang").c_str(), LFStrings::getValue("cuowu").c_str());
// 		return;
// 	}
// 	else if (rsp->err_code == Success)
// 	{
// 		m_kaiBaoxiangRspVector = OpenBoxHelper::getHelper()->getBoxData();
// 	}
}