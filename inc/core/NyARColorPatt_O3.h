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
#pragma once
#include "INyARColorPatt.h"
#include "NyARBufferType.h"
#include "NyARMat.h"
#include "NyARSquare.h"
#include "NyAR_types.h"
#include "NyARRgbPixelReader_INT1D_X8R8G8B8_32.h"
namespace NyARToolkitCPP
{
	class NyARColorPatt_O3 :public INyARColorPatt
	{
	private:
		const static int BUFFER_FORMAT=NyARBufferType::INT1D_X8R8G8B8_32;
		int* _patdata;
		TNyARIntSize _size;
		NyARRgbPixelReader_INT1D_X8R8G8B8_32* _pixelreader;
	public:	
		NyARColorPatt_O3(int i_width, int i_height);
		virtual ~NyARColorPatt_O3();
		int getWidth()const;
		int getHeight()const;
		const TNyARIntSize& getSize()const;
		INyARRgbPixelReader& getRgbPixelReader();
		void* getBuffer()const;
		int getBufferType()const;
		bool isEqualBufferType(int i_type_value)const;
		bool hasBuffer()const;
		void wrapBuffer(void* i_ref_buf);

	private:
		NyARMat* __get_cpara_a;
		NyARMat* __get_cpara_b;
		NyARMat* __pickFromRaster_cpara;
	private:
		bool get_cpara(const TNyARIntPoint2d i_vertex[], NyARMat& o_para);
		void updateExtpat(const INyARRgbRaster& image, const NyARMat& i_cpara, int i_xdiv2,int i_ydiv2);
	public:
		bool pickFromRaster(const INyARRgbRaster& image,const TNyARIntPoint2d i_vertexs[]);

	private:
		int* __updateExtpat_rgbset;
		int* __updateExtpat_xc;
		int* __updateExtpat_yc;
		double* __updateExtpat_xw;
		double* __updateExtpat_yw;
		int _last_pix_resolution_x;
		int _last_pix_resolution_y;
		void reservWorkBuffers(int i_xdiv,int i_ydiv);

	};

}
