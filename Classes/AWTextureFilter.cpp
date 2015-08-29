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
#include "AWTextureFilter.h"


void AWTextureFilter::blurInput(void* input, void* output, CCTexture2DPixelFormat format, int width, int height, CCSize contentSize, int radius, CCRect rect) {
    int read, i, xl, yl, yi, ym, ri, riw;
	const int wh = width*height;
	CCSize size = CCSizeMake(rect.size.width, rect.size.height);

	size.width = (size.width==0) ? contentSize.width : size.width;
	size.height = (size.height==0) ? contentSize.height : size.height;

	//Check data
	CCSize position = CCSizeMake(MAX(0, rect.origin.x), MAX(0, rect.origin.y));
    size.width = position.width+size.width-MAX(0, (position.width+size.width)-width);
	size.height = position.height+size.height-MAX(0, (position.height+size.height)-height);
    yi = position.height*width;

	//Generate Gaussian kernel
    radius = MIN(MAX(1,radius), 248);
    int kernelSize = 1+radius*2;
    int* kernel = new int[kernelSize];
    int g = 0, sum = 0;

	//Gaussian filter
    for (i = 0; i<radius;i++)
	{
		g = i*i*i+1;
		kernel[i] = kernel[kernelSize-i-1] = g;
		sum+=g*2;
    }

	g = radius*radius;
    kernel[radius] = g;
    sum+=g;

    if(format == kCCTexture2DPixelFormat_RGBA8888) {
		int cr, cg, cb, ca;

		//const
        //ccColor4B *originalData = (ccColor4B*)input;
        //Ryan wu 修改内存操作的bug 2012-9-11
        ccColor4B *originalDataBuff = new ccColor4B[(width)*(height)+radius*2];
        ccColor4B* originalData = originalDataBuff+radius;
        memcpy(originalData, input, width*height*sizeof(originalData));
        
		ccColor4B *data = (ccColor4B*)output;
		ccColor4B *temp = new ccColor4B[wh*4];//malloc(wh*4);
		ccColor4B *pixel;


		//Horizontal blur
		for (yl = position.height; yl<size.height; yl++) {
			for (xl = position.width; xl<size.width; xl++) {
				cb = cg = cr = ca = 0;
				ri = xl-radius;

				for (i = 0; i<kernelSize; i++){
					read = yi + ri + i;
                    
					pixel = &originalData[read];
                        cr+= pixel->r*kernel[i];
                        cg+= pixel->g*kernel[i];
                        cb+= pixel->b*kernel[i];
                        ca+= pixel->a*kernel[i];
                    

					
				}
				pixel = &temp[yi+xl];
				pixel->r = cr/sum;
				pixel->g = cg/sum;
				pixel->b = cb/sum;
				pixel->a = ca/sum;
			}
			yi+=width;
		}
		yi = (position.height)*width;

		//Vertical blur
		for (yl = position.height; yl<size.height; yl++){
			for (xl = position.width; xl<size.width; xl++){
				cb = cg = cr = ca = 0;
				ri = yl-radius;

				for (i = 0; i<kernelSize; i++){
					if((ri + i) >=position.height && (ri + i) < size.height)
						read = (ri + i) * width + xl;
					else
						read = yl * width + xl;

					pixel = &temp[read];
					cr+= pixel->r * kernel[i];
					cg+= pixel->g * kernel[i];
					cb+= pixel->b * kernel[i];
					ca+= pixel->a * kernel[i];
				}
				pixel = &data[yi+xl];
				pixel->r = cr/sum;
				pixel->g = cg/sum;
				pixel->b = cb/sum;
				pixel->a = ca/sum;
			}
			yi+=width;
		}

		//Free temp data
		free(temp);
        //Ryan wu 修改内存操作的bug 2012-9-11
        delete originalDataBuff;
    }
    else  if(format == kTexture2DPixelFormat_A8) {
		int ca;

		const unsigned char *originalData = (const unsigned char*) input;
		unsigned char *data = (unsigned char*) output;
		unsigned char *temp = new unsigned char [wh];//malloc(wh);

		//Horizontal blur
		for (yl = position.height; yl<size.height; yl++){
			for (xl = position.width; xl<size.width; xl++){
				ca = 0;
				ri = xl-radius;
				for (i = 0; i<kernelSize; i++){
					read = ri+i;
					if (read>=position.width && read<size.width){
						read+=yi;
						ca+=originalData[read] * kernel[i];
					}
				}
				ri = yi+xl;
				temp[ri] = ca/sum;;
			}
			yi+=width;
		}
		yi = position.height*width;

		//Vertical blur
		for (yl = position.height; yl<size.height; yl++){
			ym = yl-radius;
			riw = ym*width;
			for (xl = position.width; xl<size.width; xl++){
				ca = 0;
				ri = ym;
				read = xl+riw;
				for (i = 0; i<kernelSize; i++){
					if (ri<size.height && ri>=position.height)
						ca+=temp[read] * kernel[i];

					ri++;
					read+=width;
				}
				data[xl+yi] = ca/sum;;

			}
			yi+=width;
		}

		//Free temp data
		free(temp);

	}
}


CCTexture2DMutable* AWTextureFilter::blur(CCTexture2DMutable* texture, int radius) {
	return this->blur(texture, radius, CCRectZero);
}

CCTexture2DMutable* AWTextureFilter::blur(CCTexture2DMutable* texture, int radius, CCRect rect) {
	if(!texture)
		return NULL;

#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	void *input = texture->getOriginalTexData();
#else
	void *input = texture->getTexData();
#endif
    
	//Apply the effect to the texture
    {
        //this->blurInput(input, texture->getTexData(), texture->getPixelFormat(), texture->getPixelsWide(), texture->getPixelsHigh(), texture->getContentSize(), radius, rect); //?
    }
    {
        //Ryan wu 2012-9-11 支持retina
        //CCLog("texture->getPixelsWide()=%d, texture->getPixelsHigh()=%d, texture->getContentSize()=[%f, %f]"
        //      , texture->getPixelsWide(), texture->getPixelsHigh(), texture->getContentSize().width, texture->getContentSize().height);
        CCDirector *pDirector = CCDirector::sharedDirector();
        CCSize contentSize;
        if (pDirector->getWinSizeInPixels().width/pDirector->getWinSize().width == 2) {
            contentSize = CCSizeMake(texture->getContentSize().width*2, texture->getContentSize().height*2);
        }else{
            contentSize = texture->getContentSize();
        }
        
        this->blurInput(input, texture->getTexData(), texture->getPixelFormat(), texture->getPixelsWide(), texture->getPixelsHigh(), contentSize, radius, rect); //?
    }

	//Update the GPU data
	texture->apply();

	return texture;
}
