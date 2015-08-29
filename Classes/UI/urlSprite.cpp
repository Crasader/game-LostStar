#include "urlSprite.h" 

 

urlSprite* urlSprite::create(const char* defaultImagePath, const char* url){
	urlSprite* pSprite = new urlSprite();
	if (pSprite && pSprite->init(defaultImagePath,url))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

bool urlSprite::init(const char* defaultImagePath, const char* url){

	m_url = url;
	CCTexture2D* pTexture = NULL;

	// 图片是否存在，如果不存在使用默认纹理并下载图片
	if (isExist(url))
	{
		string path = getImagePath(url).c_str();
		pTexture = CCTextureCache::sharedTextureCache()->addImage(path.c_str());
	}else{
		pTexture = CCTextureCache::sharedTextureCache()->addImage(defaultImagePath);
		// 下载图片
		downloadImage(url);
	}

	return CCSprite::initWithTexture(pTexture);
}

//判断图片是否已经存在
bool urlSprite::isExist(const char* url){	
	if (url == NULL || url[0] == '\0') return false;

	std::string path = getImagePath(url);
	return CCFileUtils::sharedFileUtils()->isFileExist(path);
}

//获取图片全路径
std::string urlSprite::getImagePath(const char* url)
{
	std::string urlStr = url;
	//1. 先检测是否本地文件
	if (urlStr.find(NATIVE_PICTURE_URL) < urlStr.length()){
		return urlStr;
	}

	//2. 
	urlStr.clear();
	for (int i=0; ; ++i)
	{
		if (url[i] == '\0') break;
		if (   url[i] >= '0' && url[i] <= '9'
			|| url[i] >= 'a' && url[i] <= 'z'
			|| url[i] >= 'A' && url[i] <= 'Z')
			urlStr.push_back(url[i]);
	}

	return CCFileUtils::sharedFileUtils()->getWritablePath().append(urlStr);
}

// 下载图片
void urlSprite::downloadImage(const char* url){
	if (url == nullptr || url[0] == '\0')
	{
		return;
	}

	// 发起http请求，下载图片
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(url);
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(urlSprite::onDownloadCompleted));	
	CCHttpClient::getInstance()->send(request);
	request->release();	
}

// 请求回调
void urlSprite::onDownloadCompleted(CCHttpClient *sender, CCHttpResponse *response){
	if (!response)
	{
		return;
	}
	// 返回码
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d", statusCode);	
	CCLog("response code: %d", statusCode);

	// 请求失败
	if (!response->isSucceed()) 
	{
		CCLog("response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string data (buffer->begin(),buffer->end());
	std::string path =saveImage(data);
	//如果保存图片成功，更新纹理
	if (path != "")
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(path.c_str());
		if (pTexture)
		{
			setTexture(pTexture);
		}		
	}
}

// 保存图片
std::string urlSprite::saveImage(const std::string& data){

	std::string path = this->getImagePath(m_url.c_str());	
	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		// 1.buff
		// 2.每次写的字节数
		// 3.写多少次结束
		// 4.文件句柄
		fwrite(data.c_str(), 1, data.length(), file);
		fclose(file);
		return path;
	}
	return "";
}
