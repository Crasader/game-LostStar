/*
 *  Created on: 13 июля 2012
 *      Ported to C++: Alexandr Zhdanov
 *
 * AWSuite: http://forzefield.com
 *
 * Copyright (c) 2010 ForzeField Studios S.L.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


//道具点击模糊


#ifndef AWTEXTUREFILTER_H_
#define AWTEXTUREFILTER_H_

#include "CCTexture2DMutable.h"
#include "cocos2d.h"

USING_NS_CC;


class AWTextureFilter {

/**
//	@param input: Original texture data
//	@param output: Empty (or not) buffer
//  @param format: Pixel format of the data
//	@param width: Real width (is a power of two)
//	@param height: Real height (is a power of two)
//	@param position: Top left vertex of the blur effect
//	@param size: The size of the blur effect
//	@param contentSize:
//	@param radios: It's the radius of the blur effect
**/
public:
	AWTextureFilter() {};
	~AWTextureFilter() {};

	void blurInput(void* input, void* output, CCTexture2DPixelFormat format, int width, int height, CCSize contentSize, int radius, CCRect rect);
	CCTexture2DMutable* blur(CCTexture2DMutable* texture, int radius);
	CCTexture2DMutable* blur(CCTexture2DMutable* texture, int radius, CCRect rect);
};


#endif /* AWTEXTUREFILTER_H_ */
