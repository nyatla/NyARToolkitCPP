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
#include "NyARBinRaster.h"
#include "NyAR_types.h"
#include "INyARRgbPixelReader.h"
namespace NyARToolkitCPP
{
	class NyARRgbPixelReader_INT1D_X8R8G8B8_32 :public INyARRgbPixelReader
	{
	protected:
		int* _ref_buf;
	private:
		const TNyARIntSize* _size;
	public:
		NyARRgbPixelReader_INT1D_X8R8G8B8_32(const TNyARIntSize* i_size,int* i_buf);
		void getPixel(int i_x, int i_y, int* o_rgb)const;
		void getPixelSet(const int* i_x,const int* i_y, int i_num, int* o_rgb)const;
		void setPixel(int i_x, int i_y, const int* i_rgb);
		void setPixels(const int* i_x, const int* i_y, int i_num, const int* i_intrgb);
		void switchBuffer(void* i_ref_buffer);
	};
}
