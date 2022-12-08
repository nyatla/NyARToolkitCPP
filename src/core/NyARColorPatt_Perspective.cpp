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
#include "INyARColorPatt.h"
#include "NyAR_types.h"
#include "NyARPerspectiveParamGenerator_O1.h"
#include "NyARRgbPixelReader_INT1D_X8R8G8B8_32.h"

#include "NyARColorPatt_Perspective.h"
#include "nyarcore.h"


namespace NyARToolkitCPP
{
	/**
	* 遠近法を使ったパースペクティブ補正をかけて、ラスタ上の四角形から
	* 任意解像度の矩形パターンを作成します。
	*
	*/

#define LOCAL_LT 1

	void NyARColorPatt_Perspective::initializeInstance(int i_width, int i_height,int i_point_per_pix)
	{
		NyAR_ASSERT( i_width>2 && i_height>2);
		this->_resolution=i_point_per_pix;
		this->_size.w=i_width;
		this->_size.h=i_height;
		this->_patdata = new int[i_height*i_width];
		this->_pixelreader=new NyARRgbPixelReader_INT1D_X8R8G8B8_32(&(this->_size),this->_patdata);
		this->_perspective_gen=NULL;
		return;
	}

	NyARColorPatt_Perspective::NyARColorPatt_Perspective(int i_width, int i_height,int i_point_per_pix)
	{
		initializeInstance(i_width,i_height,i_point_per_pix);
		setEdgeSize(0,0,i_point_per_pix);
		return;
	}
	NyARColorPatt_Perspective::~NyARColorPatt_Perspective()
	{
		NyAR_SAFE_ARRAY_DELETE(this->_patdata);
		NyAR_SAFE_DELETE(this->_pixelreader);
		if(this->_perspective_gen!=NULL){
			NyAR_SAFE_DELETE(this->_perspective_gen);
		}
		return;
	}

	NyARColorPatt_Perspective::NyARColorPatt_Perspective(int i_width, int i_height,int i_resolution,int i_edge_percentage)
	{
		//入力制限
		initializeInstance(i_width,i_height,i_resolution);
		setEdgeSizeByPercent(i_edge_percentage,i_edge_percentage,i_resolution);
		return;
	}	


	void NyARColorPatt_Perspective::setEdgeSize(int i_x_edge,int i_y_edge,int i_resolution)
	{
		NyAR_ASSERT(i_x_edge>=0);
		NyAR_ASSERT(i_y_edge>=0);
		//Perspectiveパラメタ計算器を作成
		if(this->_perspective_gen!=NULL){
			NyAR_SAFE_DELETE(this->_perspective_gen);
		}
		this->_perspective_gen=new NyARPerspectiveParamGenerator_O1(
			LOCAL_LT,LOCAL_LT,
			(i_x_edge*2+this->_size.w)*i_resolution,
			(i_y_edge*2+this->_size.h)*i_resolution);
		//ピックアップ開始位置を計算
		this->_pickup_lt.x=i_x_edge*i_resolution+LOCAL_LT;
		this->_pickup_lt.y=i_y_edge*i_resolution+LOCAL_LT;
		return;
	}
	void NyARColorPatt_Perspective::setEdgeSizeByPercent(int i_x_percent,int i_y_percent,int i_resolution)
	{
		NyAR_ASSERT(i_x_percent>=0);
		NyAR_ASSERT(i_y_percent>=0);
		setEdgeSize(this->_size.w*i_x_percent/50,this->_size.h*i_y_percent/50,i_resolution);
		return;
	}


	int NyARColorPatt_Perspective::getWidth()const
	{
		return this->_size.w;
	}
	int NyARColorPatt_Perspective::getHeight()const
	{
		return this->_size.h;
	}
	const TNyARIntSize& NyARColorPatt_Perspective::getSize()const
	{
		return 	this->_size;
	}
	INyARRgbPixelReader& NyARColorPatt_Perspective::getRgbPixelReader()const
	{
		return *(this->_pixelreader);
	}
	void* NyARColorPatt_Perspective::getBuffer()const
	{
		return this->_patdata;
	}
	int NyARColorPatt_Perspective::getBufferType()const
	{
		return BUFFER_FORMAT;
	}

	bool NyARColorPatt_Perspective::isEqualBufferType(int i_type_value)const
	{
		return BUFFER_FORMAT==i_type_value;
	}

	bool NyARColorPatt_Perspective::hasBuffer()const
	{
		return this->_patdata!=NULL;
	}

	void NyARColorPatt_Perspective::wrapBuffer(void* i_ref_buf)
	{
		NyARException::notImplement();
	}

	bool NyARColorPatt_Perspective::pickFromRaster(const INyARRgbRaster& image,const TNyARIntPoint2d i_vertexs[])
	{
		//遠近法のパラメータを計算
		double cpara[8];
		if (!this->_perspective_gen->getParam(i_vertexs, cpara)) {
			return false;
		}

		int resolution=this->_resolution;
		int img_x = image.getWidth();
		int img_y = image.getHeight();
		int res_pix=resolution*resolution;

		int rgb_tmp[3];

		//ピクセルリーダーを取得
		const INyARRgbPixelReader& reader=image.getRgbPixelReader();
		int p=0;
		for(int iy=0;iy<this->_size.h*resolution;iy+=resolution){
			//解像度分の点を取る。
			for(int ix=0;ix<this->_size.w*resolution;ix+=resolution){
				int r,g,b;
				r=g=b=0;
				for(int i2y=iy;i2y<iy+resolution;i2y++){
					int cy=this->_pickup_lt.y+i2y;
					for(int i2x=ix;i2x<ix+resolution;i2x++){
						//1ピクセルを作成
						int cx=this->_pickup_lt.x+i2x;
						const double d=cpara[6]*cx+cpara[7]*cy+1.0;
						int x=(int)((cpara[0]*cx+cpara[1]*cy+cpara[2])/d);
						int y=(int)((cpara[3]*cx+cpara[4]*cy+cpara[5])/d);
						if(x<0){x=0;}
						if(x>=img_x){x=img_x-1;}
						if(y<0){y=0;}
						if(y>=img_y){y=img_y-1;}

						reader.getPixel(x, y, rgb_tmp);
						r+=rgb_tmp[0];
						g+=rgb_tmp[1];
						b+=rgb_tmp[2];
					}
				}
				r/=res_pix;
				g/=res_pix;
				b/=res_pix;
				this->_patdata[p]=((r&0xff)<<16)|((g&0xff)<<8)|((b&0xff));
				p++;
			}
		}
		//ピクセル問い合わせ
		//ピクセルセット
		return true;
	}
}
