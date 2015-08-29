#include "rankingLayer.h"

#include "UserDataManager.h"
#include "font.h"
#include "GameInforEditer.h"
#include "LFStrings.h"
#include "VersionControl.h"


USING_NS_CC;

rankingLayer::rankingLayer(void)
{
// 	vector<RankList_PerfectPassOrEvaluate> infoVec;
// 	RankList_PerfectPassOrEvaluate temp;
// 
// 	for (int i = 0; i < 20; ++i)
// 	{
// 		temp.order = 1;
// 		temp.name = "sdf";
// 		temp.val = rand()%300;
// 		temp.time = rand()%10000;
// 		infoVec.push_back(temp);
// 	}
// 	temp = infoVec.at(2);
// 	temp.time = 100;
// 	infoVec.push_back(temp);
// 
// 	temp = infoVec.at(2);
// 	temp.time = 1000;
// 	infoVec.push_back(temp);
// 
// 	temp = infoVec.at(2);
// 	temp.time = 1001;
// 	infoVec.push_back(temp);
// 
// 	temp = infoVec.at(4);
// 	infoVec.push_back(temp);
// 	
// 	for (int i = 0; i < infoVec.size(); i++)
// 	{
// 		CCLog("addr is %p", &infoVec.at(i).name);
// 	}
// 
// 	RankingListManager::getInstance()->setOrderEvaluate(infoVec);
}

rankingLayer::~rankingLayer(void)
{
}

bool rankingLayer::init()
{
	ccColor4B color = {0, 0, 0, 125};
	if (!CCLayerColor::initWithColor(color))
	{
		return false;
	}

	this->initMemeber();
	this->initUI();
	this->initBtn();
//	this->initTableView(sanxiang_tableview_type);

//	NetworkWaitingUI::showWating();
// 	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(rankingLayer::receiveMsg), MSG_queryRanklistRsp, NULL);
// 	CmdHelper::getHelper()->cmdQueryRanklist();

	return true;
}

void rankingLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void rankingLayer::onEnter()
{
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	CCLayerColor::onEnter();
}

void rankingLayer::onExit()
{
	CCLayerColor::onExit();
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_queryRanklistRsp);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCTableView* pView = (CCTableView*)m_pBg->getChildByTag(sanxiang_tableview_tag);
	if (pView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	}

	pView = (CCTableView*)m_pBg->getChildByTag(chengjiu_tableview_tag);
	if (pView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	}
}

void rankingLayer::initMemeber()
{
	m_iCellCount = 0;
	m_iTableViewType = sanxiang_tableview_type;
}

void rankingLayer::initUI()
{
	//初始化背景
	m_pBg = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_bg.png").c_str());
	this->addChild(m_pBg, 1, bg_tag);
	m_pBg->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));

	m_pMenu = CCMenu::create();
	m_pMenu->setPosition(CCPointZero);
	m_pBg->addChild(m_pMenu, 2);
}

void rankingLayer::initBtn()
{
	CCNode* pBg = this->getChildByTag(bg_tag);
	if (!pBg)
	{
		return;
	}
	//关闭按钮
	CCMenuItemImage* pClose = CCMenuItemImage::create(ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi.png").c_str(), 
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi_select.png").c_str(), 
		this,
		menu_selector(rankingLayer::menuBtnCallBack));
	pClose->setTag(close_btn_tag);
	m_pMenu->addChild(pClose);
	pClose->setPosition(ccp(pBg->getContentSize().width - pClose->getContentSize().width/2 + 20, 
		pBg->getContentSize().height - pClose->getContentSize().height/2 - 40));

	//三星通关按钮
	CCMenuItemSprite* pNorItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_sanxingtongguan.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_sanxingtongguan.png").c_str()));
	CCMenuItemSprite* pPreItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_sanxingtongguan_select.png").c_str()),  
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_sanxingtongguan_select.png").c_str()));

	CCMenuItemToggle* pSanxiangToggle = CCMenuItemToggle::createWithTarget(this, menu_selector(rankingLayer::menuToggleCallBack), pNorItem, pPreItem, NULL);
	pSanxiangToggle->setTag(sanxing_btn_tag);
	m_pMenu->addChild(pSanxiangToggle);
	pSanxiangToggle->setPosition(ccp(m_pBg->getContentSize().width/2 - pSanxiangToggle->getContentSize().width/2 - 30, 
		m_pBg->getContentSize().height - pSanxiangToggle->getContentSize().height - 100));
	pSanxiangToggle->setSelectedIndex(0);

	//成就数量按钮
	pNorItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_chengjiushuliang.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_chengjiushuliang.png").c_str()));
	pPreItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_chengjiushuliang_select.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_chengjiushuliang_select.png").c_str()));
	CCMenuItemToggle* pChengjiuToggle = CCMenuItemToggle::createWithTarget(this, menu_selector(rankingLayer::menuToggleCallBack), pNorItem, pPreItem, NULL);
	pChengjiuToggle->setTag(chengjiu_btn_tag);
	m_pMenu->addChild(pChengjiuToggle);
	pChengjiuToggle->setPosition(ccp(m_pBg->getContentSize().width/2 + pChengjiuToggle->getContentSize().width/2 + 30, 
		pSanxiangToggle->getPositionY()));
	pChengjiuToggle->setSelectedIndex(0);

	//分享按钮
	CCMenuItemImage* pShare= CCMenuItemImage::create(ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_fenxiangchengji.png").c_str(), 
		ResManager::getManager()->getFilePathByLanguageType(g_rankinglayerPath+"main_paihangbang_anniu_fenxiangchengji_select.png").c_str(), 
		this,
		menu_selector(rankingLayer::menuBtnCallBack));
	pShare->setTag(share_btn_tag);
	m_pMenu->addChild(pShare);
	pShare->setPosition(ccp(m_pBg->getContentSize().width/2, 
		pShare->getContentSize().height));
	pShare->setVisible(false);
}

void rankingLayer::receiveMsg(CCObject* pMsg)
{
	//收到消息后的处理
// 	NetworkWaitingUI::hideWaiting();
// 	SPCmd_QueryRankListRsp *data  = (SPCmd_QueryRankListRsp*)((MSG_Rsp*)pMsg)->getData();
// 	if (data->err_code != Success)
// 	{
// //		CCMessageBox(LFStrings::getValue("cuowu").c_str(), LFStrings::getValue("cuowu").c_str());
// //		return;
// 	}

	//先获取自己的成就，因为这里会和更新最新的信息
	m_OwnerRankingSanxingInfo = RankingListManager::getInstance()->getMyPerfectPass();
	m_OwnerRankingChengjiuInfo = RankingListManager::getInstance()->getMyEvaluate();

	m_rankSanxingInfoVector = RankingListManager::getInstance()->getOrderPerfectPass();
	m_rankChengjiuInfoVector = RankingListManager::getInstance()->getOrderEvaluate();


	m_iCellCount = m_rankSanxingInfoVector.size()-1;

	size_t id = GameInforEditer::getGameInfor()->getClientId();
	for (size_t i = 0; i < m_rankSanxingInfoVector.size(); i++)
	{
		if (m_rankSanxingInfoVector.at(i).id == id)
		{
			if ((int)i >= m_iCellCount)
			{
				m_rankSanxingInfoVector.erase(m_rankSanxingInfoVector.begin()+i);
			}
			else
			{
				m_rankSanxingInfoVector.erase(m_rankSanxingInfoVector.begin()+m_rankSanxingInfoVector.size()-1);
			}
			break;
		}
	}

	for (size_t i = 0; i < m_rankChengjiuInfoVector.size(); i++)
	{
		if (m_rankChengjiuInfoVector.at(i).id == id)
		{
			if ((int)i >= m_iCellCount)
			{
				m_rankChengjiuInfoVector.erase(m_rankChengjiuInfoVector.begin()+i);
			}
			else
			{
				m_rankChengjiuInfoVector.erase(m_rankChengjiuInfoVector.begin()+m_rankChengjiuInfoVector.size()-1);
			}
			
			break;
		}
	}


	if (CCNode* pnode = m_pMenu->getChildByTag(share_btn_tag))
	{

	}

	this->initTableView(sanxiang_tableview_type);
}

void rankingLayer::initTableView(_TableviewType tableviewType)
{
	m_iTableViewType = tableviewType;
	int iTag = sanxiang_tableview_tag;
	m_iCellCount = m_rankSanxingInfoVector.size();
	this->setToggleState();
	if (tableviewType == chengjiu_tableview_type)
	{
		m_iCellCount = m_rankChengjiuInfoVector.size();
		iTag = chengjiu_tableview_tag;
	}

	CCTableView* pTableView = (CCTableView*)m_pBg->getChildByTag(sanxiang_tableview_tag);
	if (pTableView)
	{
		pTableView->removeFromParent();
	}

	pTableView = (CCTableView*)m_pBg->getChildByTag(chengjiu_tableview_tag);
	if (pTableView)
	{
		pTableView->removeFromParent();
	}

	if (m_iCellCount <= 0)
	{
		return;
	}
	pTableView = CCTableView::create(this, CCSizeMake(540, 56*m_iCellCount));
	pTableView->setViewSize(CCSizeMake(540, 280));
	pTableView->setDirection(kCCScrollViewDirectionVertical);
	pTableView->setTag(iTag);
	pTableView->setPosition(ccp(36, 190));
	m_pBg->addChild(pTableView, 2);
	pTableView->setContentOffset(ccp(0, 280-56*(m_iCellCount)));

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pTableView);
	pTableView->runAction(CCSequence::create(
		CCDelayTime::create(0.1f),
		CCCallFuncN::create(this, callfuncN_selector(rankingLayer::scheduleUpTableViewPrioority)),
		NULL));

	//显示自己的信息
	CCSprite* pOwner = (CCSprite*)m_pBg->getChildByTag(owner_tag);
	if (!pOwner)
	{
		if (m_iTableViewType == sanxiang_tableview_type)
		{
			pOwner = this->createCell(-1, m_OwnerRankingSanxingInfo);
		}
		else
		{
			pOwner = this->createCell(-1, m_OwnerRankingChengjiuInfo);
		}
		m_pBg->addChild(pOwner, 1, owner_tag);
		pOwner->setPosition(ccp(m_pBg->getContentSize().width/2, pTableView->getPositionY()-pOwner->getContentSize().height/2));
	}
}

void rankingLayer::setOwnerInfo(RankList_PerfectPassOrEvaluate info)
{
	CCSprite* pOwner = (CCSprite*)m_pBg->getChildByTag(owner_tag);
	if (pOwner)
	{
		std::string numStr;
		//设置名次
		CCLabelTTF* pTTF = (CCLabelTTF*)pOwner->getChildByTag(mingci_tag);
		if (pTTF)
		{
			if ((int)info.order > m_iCellCount)
			{
				this->covertIntToString(m_iCellCount, numStr);
				numStr += "+";
			}
			else
			{
				this->covertIntToString(info.order, numStr);
			}
			pTTF->setString(numStr.c_str());
		}
		//设置名字
		pTTF = (CCLabelTTF*)pOwner->getChildByTag(username_tag);
		if (pTTF)
		{
			pTTF->setString(info.name.c_str());
		}
		//设置数量
		pTTF = (CCLabelTTF*)pOwner->getChildByTag(numstr_tag);
		if (pTTF)
		{
			this->covertIntToString(info.val, numStr);
			pTTF->setString(numStr.c_str());
		}
	}
}

void rankingLayer::scheduleUpTableViewPrioority(CCNode* pNode)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((CCTableView*)pNode, kCCMenuHandlerPriority-1, false);
}

CCTableViewCell* rankingLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

	std::vector<RankList_PerfectPassOrEvaluate> infoList;

	if (m_iTableViewType == sanxiang_tableview_type)
	{
		infoList = m_rankSanxingInfoVector;
	}
	else
	{
		infoList = m_rankChengjiuInfoVector;
	}

	if (idx >= 0 && idx < infoList.size())
	{
		RankList_PerfectPassOrEvaluate info = infoList.at(m_iCellCount-idx-1);

		CCSprite* pCell = this->createCell(m_iCellCount-idx-1, info);
		cell->addChild(pCell);
		pCell->setAnchorPoint(CCPointZero);
	}

	return cell;
}


CCSize rankingLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return CCSizeMake(540, 56);
}

unsigned int rankingLayer::numberOfCellsInTableView(CCTableView *table)
{
	return m_iCellCount;
}

CCSprite* rankingLayer::createCell(int iIndex, RankList_PerfectPassOrEvaluate rankingInfo)
{
	CCSprite* pRet = NULL;
	if (iIndex%2 == 0)
	{
		pRet = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_rankinglayerPath+"main_paihangbang_wanjiaxinxi_1.png").c_str());
	}
	else
	{
		pRet = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_rankinglayerPath+"main_paihangbang_wanjiaxinxi_2.png").c_str());
	}

	if (iIndex == -1)
	{
		pRet = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_rankinglayerPath+"main_paihangbang_wanjiaxinxi_3.png").c_str());
	}
	if (pRet)
	{
		if (iIndex == -1)
		{
			iIndex = 20;
		}
		std::string numStr;
		if ((int)rankingInfo.order > m_iCellCount)
		{
			this->covertIntToString(m_iCellCount, numStr);
			numStr += "+";
		}
		else
		{
			this->covertIntToString(rankingInfo.order, numStr);
		}
		
		//名次字符串
		CCLabelTTF* pRankingTTF = CCLabelTTF::create(numStr.c_str(), fontStr_katong, 24);
		pRankingTTF->setPosition(ccp(60, pRet->getContentSize().height/2));
		pRet->addChild(pRankingTTF, 1, mingci_tag);

		//名字字符串
		CCLabelTTF* pNameTTF = CCLabelTTF::create(rankingInfo.name.c_str(), fontStr_katong, 24);
		pNameTTF->setPosition(ccp(pRet->getContentSize().width/2, pRet->getContentSize().height/2));
		pRet->addChild(pNameTTF, 1, username_tag);

		this->covertIntToString(rankingInfo.val, numStr);
 		//数量字符串
		CCLabelTTF* pNameTTF1 = CCLabelTTF::create(numStr.c_str(), fontStr_katong, 24);
		pNameTTF1->setPosition(ccp(pRet->getContentSize().width - 50, pRet->getContentSize().height/2));
		pRet->addChild(pNameTTF1, 1, numstr_tag);
	}

	return pRet;
}

void rankingLayer::covertIntToString(int iNum, std::string& outString)
{
	stringstream strRet;
	strRet << iNum;
	outString = strRet.str();
}


void rankingLayer::menuToggleCallBack(CCObject* pObj)
{
	CCLog("menutoggle call back");
	CCMenuItemToggle* pItme = (CCMenuItemToggle*)pObj;
	int iTag = pItme->getTag();
	int index = pItme->getSelectedIndex();
	switch (iTag)
	{
	case sanxing_btn_tag:
		{
			if (index == 1)
			{
				this->initTableView(sanxiang_tableview_type);
				this->setOwnerInfo(m_OwnerRankingSanxingInfo);
			}
		}
		break;

	case chengjiu_btn_tag:
		{
			if (index == 1)
			{
				this->initTableView(chengjiu_tableview_type);
				this->setOwnerInfo(m_OwnerRankingChengjiuInfo);
			}
		}
		break;
	}
}

void rankingLayer::menuBtnCallBack(CCObject* pObj)
{
	CCLog("menubtn call back");
	CCMenuItemImage* pItme = (CCMenuItemImage*)pObj;
	int iTag = pItme->getTag();
	switch (iTag)
	{
	case close_btn_tag:
		{
			m_pBg->runAction(CCSequence::create(CCScaleTo::create(0.2f, 0.1f), CCCallFuncN::create(this, callfuncN_selector(rankingLayer::removeCurLayer)), NULL));
		}
		break;

// 	case sanxing_btn_tag:
// 		{
// 			this->initTableView(sanxiang_tableview_type);
// 			this->setOwnerInfo(m_OwnerRankingSanxingInfo);
// 		}
// 		break;
// 
// 	case chengjiu_btn_tag:
// 		{
// 			this->initTableView(chengjiu_tableview_type);
// 			this->setOwnerInfo(m_OwnerRankingChengjiuInfo);
// 		}
// 		break;

	case share_btn_tag:
		{
            string url = "https://itunes.apple.com/us/app/fang-kai-na-xing-xing/id869763433?l=zh&ls=1&mt=8";
			int order = m_OwnerRankingSanxingInfo.order;
			std::string text = LFStrings::getValue("Ranking_sanxing_shareText");
			if (m_iTableViewType == chengjiu_tableview_type)
			{
				order = m_OwnerRankingChengjiuInfo.order;
				text = LFStrings::getValue("Ranking_chengjiu_shareText");
			}
            CCDictionary* pDict = CCDictionary::create();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS  && (Version_Platform == 8 || Version_Platform == 0)
            text += url;
#endif
			CCLog("share.....");
		}
		break;

	default:
		break;
	}
}

void rankingLayer::setToggleState()
{
	CCMenuItemToggle* pToggle = (CCMenuItemToggle*)m_pMenu->getChildByTag(sanxing_btn_tag);
	CCMenuItemToggle* pToggle1 = (CCMenuItemToggle*)m_pMenu->getChildByTag(chengjiu_btn_tag);
	if (m_iTableViewType == sanxiang_tableview_type)
	{
		if (pToggle)
		{
			pToggle->setSelectedIndex(1);
			pToggle->setEnabled(false);
		}
		if (pToggle1)
		{
			pToggle1->setSelectedIndex(0);
			pToggle1->setEnabled(true);
		}
	}
	else
	{
		if (pToggle)
		{
			pToggle->setSelectedIndex(0);
			pToggle->setEnabled(true);
		}
		if (pToggle1)
		{
			pToggle1->setSelectedIndex(1);
			pToggle1->setEnabled(false);
		}
	}
}

void rankingLayer::removeCurLayer(CCNode* pNode)
{
	this->removeFromParent();
}

bool rankingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchPoint = pTouch->getLocation();

	return true;
}

void rankingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void rankingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}
