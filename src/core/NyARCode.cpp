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
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;


#include "NyARCode.h"
#include "nyarcore.h"
#include "NyARRaster.h"
#include "NyAR_types.h"
#include "NyARMatchPattDeviationColorData.h"
#include "NyARBufferType.h"
namespace NyARToolkitCPP
{
	class NyARCodeFileReader
	{
	public:
		static void loadFromARToolKitFormFile(const unsigned char* i_data,NyARRaster* o_raster[])
		{
			//GBRAで一度読みだす。
			for (int h = 0; h < 4; h++) {
				NyAR_ASSERT(o_raster[h]->isEqualBufferType(NyARBufferType::INT1D_X8R8G8B8_32));
				NyARRaster& ra=*o_raster[h];
				readBlock(i_data,ra.getWidth(),ra.getHeight(),(int*)ra.getBuffer());
			}
			return;
		}


		static void loadFromARToolKitFormFile(const unsigned char* i_data,NyARCode& o_code)
		{
			const int width=o_code.getWidth();
			const int height=o_code.getHeight();
			NyARRaster tmp_raster(width,height,NyARBufferType::INT1D_X8R8G8B8_32);
			//4個の要素をラスタにセットする。
			int* buf=(int*)tmp_raster.getBuffer();
			//GBRAで一度読みだす。
			for (int h = 0; h < 4; h++){
				readBlock(i_data+h*width*height*3,width,height,buf);
				//ARCodeにセット(カラー)
				o_code._color_pat[h]->setRaster(tmp_raster);
			}
			return;
		}
		/**
		* 1ブロック分のXRGBデータをi_inからo_bufへ読みだします。
		* @param i_st
		* @param o_buf
		*/
	private:
		static void readBlock(const unsigned char* i_in,int i_width,int i_height,int* o_buf)
		{
			const int pixels=i_width*i_height;
			//プレーン→24bit値
			for(int i=0;i<3;i++){
				for(int i3=0;i3<pixels;i3++){
					o_buf[i3]=(o_buf[i3]<<8)|((unsigned int)i_in[i3+pixels*i]);
				}
			}
			//順番を入れ替えて下位24bitの有効化
			for(int i3=0;i3<pixels;i3++){
				o_buf[i3]=((o_buf[i3]<<16)&0xff0000)|(o_buf[i3]&0x00ff00)|((o_buf[i3]>>16)&0x0000ff);
			}
			return;
		}
	};

	const NyARMatchPattDeviationColorData* NyARCode::getColorData(int i_index)const
	{
		return this->_color_pat[i_index];
	}

	int NyARCode::getWidth()const
	{
		return _width;
	}

	int NyARCode::getHeight()const
	{
		return _height;
	}
	NyARCode::NyARCode(int i_width, int i_height)
	{
		this->_width = i_width;
		this->_height = i_height;
		//空のラスタを4個作成
		for(int i=0;i<4;i++){
			this->_color_pat[i]=new NyARMatchPattDeviationColorData(i_width,i_height);
			//				this->_bw_pat[i]=new NyARMatchPattDeviationBlackWhiteData(i_width,i_height);
		}
		return;
	}
	NyARCode::~NyARCode()
	{
		for(int i=0;i<4;i++){
			NyAR_SAFE_DELETE(this->_color_pat[i]);
		}
		return;
	}

	void NyARCode::loadARPattFromFile(const char* i_filename)
	{
		ifstream inf;
		const int w=this->_width;
		const int h=this->_height;

		unsigned char* data_array=NULL;
		try {
			inf.open(i_filename);
			data_array=new unsigned char[4*h*w*3];
			for(int i=0;i<4*h*w*3;i++){
				int r;
				inf>>r;
				data_array[i]=(unsigned char)(r&0xff);
			}
			loadARPatt(data_array);
			NyAR_SAFE_ARRAY_DELETE(data_array);
			data_array=NULL;
		} catch(NyARException e){
			NyAR_SAFE_ARRAY_DELETE(data_array);
			throw;
		} catch (...){
			NyAR_SAFE_ARRAY_DELETE(data_array);
			throw NyARException();
		}
		return;
	}
	void NyARCode::setRaster(const NyARRaster i_raster[])
	{
		//ラスタにパターンをロードする。
		for(int i=0;i<4;i++){
			this->_color_pat[i]->setRaster(i_raster[i]);				
		}
		return;
	}

	void NyARCode::loadARPatt(const unsigned char* i_stream)
	{
		//ラスタにパターンをロードする。
		NyARCodeFileReader::loadFromARToolKitFormFile(i_stream,*this);
		return;
	}
}
