/* 
* PROJECT: NyARToolkitCPP
* --------------------------------------------------------------------------------
*
* The NyARToolkitCPP is C++ version NyARToolkit class library.
* Copyright (C)2008-2009 Ryo Iizuka
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
* For further information please contact.
*	http://nyatla.jp/nyatoolkit/
*	<airmail(at)ebony.plala.or.jp> or <nyatla(at)nyatla.jp>
* 
*/
#include "NyARRasterFilter_ARToolkitThreshold.h"
#include "INyARRgbRaster.h"
#include "NyARBinRaster.h"
#include "nyarcore.h"
#include "NyAR_types.h"

namespace NyARToolkitCPP
{
	class IdoThFilterImpl
	{
	public:
		virtual void doThFilter(const INyARRaster& i_input,const INyARRaster& i_output,const TNyARIntSize& i_size,int i_threshold)const=0;
	};

	class doThFilterImpl_BUFFERFORMAT_BYTE1D_RGB_24:public IdoThFilterImpl
	{
	public:
		void doThFilter(const INyARRaster& i_input,const INyARRaster& i_output,const TNyARIntSize& i_size,int i_threshold)const
		{
			int* out_buf = (int*) i_output.getBuffer();
			unsigned char* in_buf = (unsigned char*) i_input.getBuffer();
			
			const int th=i_threshold*3;
			int bp =(i_size.w*i_size.h-1)*3;
			int w;
			int xy;
			const int pix_count   =i_size.h*i_size.w;
			const int pix_mod_part=pix_count-(pix_count%8);
			for(xy=pix_count-1;xy>=pix_mod_part;xy--){
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
			}
			//タイリング
			for (;xy>=0;) {
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 3;
				xy--;
			}
			return;			
		}
		
	};

	class doThFilterImpl_BUFFERFORMAT_BYTE1D_B8G8R8X8_32:public IdoThFilterImpl
	{
	public:
		void doThFilter(const INyARRaster& i_input,const INyARRaster& i_output,const TNyARIntSize& i_size,int i_threshold)const
		{
			int* out_buf = (int*) i_output.getBuffer();
			unsigned char* in_buf = (unsigned char*) i_input.getBuffer();
			
			const int th=i_threshold*3;
			int bp =(i_size.w*i_size.h-1)*4;
			int w;
			int xy;
			const int pix_count   =i_size.h*i_size.w;
			const int pix_mod_part=pix_count-(pix_count%8);
			for(xy=pix_count-1;xy>=pix_mod_part;xy--){
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
			}
			//タイリング
			for (;xy>=0;) {
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp] & 0xff) + (in_buf[bp + 1] & 0xff) + (in_buf[bp + 2] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
			}			
		}		
	};


	class doThFilterImpl_BUFFERFORMAT_BYTE1D_X8R8G8B8_32 : public IdoThFilterImpl
	{
		void doThFilter(const INyARRaster& i_input,const INyARRaster& i_output,const TNyARIntSize& i_size,int i_threshold)const
		{
			int* out_buf = (int*) i_output.getBuffer();
			unsigned char* in_buf = (unsigned char*) i_input.getBuffer();
			
			const int th=i_threshold*3;
			int bp =(i_size.w*i_size.h-1)*4;
			int w;
			int xy;
			const int pix_count   =i_size.h*i_size.w;
			const int pix_mod_part=pix_count-(pix_count%8);
			for(xy=pix_count-1;xy>=pix_mod_part;xy--){
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
			}
			//タイリング
			for (;xy>=0;) {
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
				w= ((in_buf[bp+1] & 0xff) + (in_buf[bp + 2] & 0xff) + (in_buf[bp + 3] & 0xff));
				out_buf[xy]=w<=th?0:1;
				bp -= 4;
				xy--;
			}
			return;			
		}
		
	};


	class doThFilterImpl_BUFFERFORMAT_INT1D_X8R8G8B8_32 :public IdoThFilterImpl
	{
	public:
		void doThFilter(const INyARRaster& i_input,const INyARRaster& i_output,const TNyARIntSize& i_size,int i_threshold)const
		{
			int* out_buf = (int*)i_output.getBuffer();
			const int* in_buf = (const int*)i_input.getBuffer();
			
			const int th=i_threshold*3;
			int w;
			int xy;
			const int pix_count   =i_size.h*i_size.w;
			const int pix_mod_part=pix_count-(pix_count%8);

			for(xy=pix_count-1;xy>=pix_mod_part;xy--){
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
			}
			//タイリング
			for (;xy>=0;) {
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
				w=in_buf[xy];
				out_buf[xy]=(((w>>16)&0xff)+((w>>8)&0xff)+(w&0xff))<=th?0:1;
				xy--;
			}			
		}		
	};

	class doThFilterImpl_BUFFERFORMAT_WORD1D_R5G6B5_16LE :public IdoThFilterImpl
	{
		void doThFilter(const INyARRaster& i_input,const INyARRaster& i_output,const TNyARIntSize& i_size,int i_threshold)const
		{
			int* out_buf = (int*) i_output.getBuffer();
			unsigned short* in_buf = (unsigned short*) i_input.getBuffer();
			
			const int th=i_threshold*3;
			unsigned short w;
			int xy;
			const int pix_count   =i_size.h*i_size.w;
			const int pix_mod_part=pix_count-(pix_count%8);

			for(xy=pix_count-1;xy>=pix_mod_part;xy--){				
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
			}
			//タイリング
			for (;xy>=0;) {
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
                w =in_buf[xy];
                w = ((w & 0xf800) >> 8) + ((w & 0x07e0) >> 3) + ((w & 0x001f) << 3);
                out_buf[xy] = w <= th ? 0 : 1;
				xy--;
			}
		}		
	};


	NyARRasterFilter_ARToolkitThreshold::NyARRasterFilter_ARToolkitThreshold(int i_threshold,int i_in_raster_type)
	{
		if(!initInstance(i_threshold,i_in_raster_type,NyARBufferType::INT1D_BIN_8)){
			throw NyARException();
		}
	}
	NyARRasterFilter_ARToolkitThreshold::NyARRasterFilter_ARToolkitThreshold(int i_threshold,int i_in_raster_type,int i_out_raster_type)
	{
		if(!initInstance(i_threshold,i_in_raster_type,i_out_raster_type)){
			throw NyARException();
		}
	}
	bool NyARRasterFilter_ARToolkitThreshold::initInstance(int i_threshold,int i_in_raster_type,int i_out_raster_type)
	{
		switch(i_out_raster_type){
	case NyARBufferType::INT1D_BIN_8:
			switch (i_in_raster_type){
			case NyARBufferType::BYTE1D_B8G8R8_24:
			case NyARBufferType::BYTE1D_R8G8B8_24:
				this->_do_threshold_impl=new doThFilterImpl_BUFFERFORMAT_BYTE1D_RGB_24();
				break;
			case NyARBufferType::BYTE1D_B8G8R8X8_32:
				this->_do_threshold_impl=new doThFilterImpl_BUFFERFORMAT_BYTE1D_B8G8R8X8_32();
				break;
			case NyARBufferType::BYTE1D_X8R8G8B8_32:
				this->_do_threshold_impl=new doThFilterImpl_BUFFERFORMAT_BYTE1D_X8R8G8B8_32();
				break;
			case NyARBufferType::INT1D_X8R8G8B8_32:
				this->_do_threshold_impl=new doThFilterImpl_BUFFERFORMAT_INT1D_X8R8G8B8_32();
				break;
			case NyARBufferType::WORD1D_R5G6B5_16LE:
				this->_do_threshold_impl=new doThFilterImpl_BUFFERFORMAT_WORD1D_R5G6B5_16LE();
				break;
			default:
				return false;//サポートしない組み合わせ
			}
			break;
		default:
			return false;//サポートしない組み合わせ
		}
		this->_threshold = i_threshold;
		return true;
	}




	NyARRasterFilter_ARToolkitThreshold::~NyARRasterFilter_ARToolkitThreshold()
	{
		NyAR_SAFE_DELETE(this->_do_threshold_impl);
		return;
	}

	void NyARRasterFilter_ARToolkitThreshold::setThreshold(int i_threshold)
	{
		this->_threshold = i_threshold;
	}
	void NyARRasterFilter_ARToolkitThreshold::doFilter(const INyARRgbRaster& i_input, NyARBinRaster& i_output)const
	{
		NyAR_ASSERT(i_output.isEqualBufferType(NyARBufferType::INT1D_BIN_8));
		NyAR_ASSERT(i_input.getSize().isEqualSize(i_output.getSize()));
		const TNyARIntSize& size = i_output.getSize();
		this->_do_threshold_impl->doThFilter(i_input,i_output,size,this->_threshold);
		return;
	}


}

#define COMMENT
#ifndef COMMENT
#include <cstdio>
#include <cstdlib>
#include <Winbase.h>
#include "NyARRgbRaster_BGRA.h"
#include "NyARGrayscaleRaster.h"
using namespace NyARToolkitCPP;
void test()
{
	FILE *fp;
	static unsigned char s[320*240*4];
	
	if ((fp = fopen("\\temp\\320x240ABGR.raw", "r")) == NULL) {
		printf("file open error!!\n");
		exit(-1);
	}
	fread(s,1,320*240*4,fp);
	fclose(fp);
	NyARRgbRaster_BGRA* in_raster=NyARRgbRaster_BGRA::wrap(s,320,240);
	NyARBinRaster gs(320,240);
	NyARRasterFilter_ARToolkitThreshold th(100);
	DWORD t=GetTickCount();
	for(int i=0;i<100;i++){
		th.doFilter(in_raster,&gs);
	}
	t=GetTickCount()-t;
	TCHAR w[256];
	wsprintf(w,L"%ums",t);
	OutputDebugString(w);
	const int* p;
	p=(const int*)gs.getBufferReader()->getBuffer();
	int sum=0;
	for(int i=0;i<320*240;i++){
		sum+=*(p+i);
	}
	delete in_raster;
	return;
}

#endif

