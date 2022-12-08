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

#include "NyARRgbPixelReader_INT1D_GRAY_8.h"
#include "NyARException.h"
namespace NyARToolkitCPP
{



	NyARRgbPixelReader_INT1D_GRAY_8::NyARRgbPixelReader_INT1D_GRAY_8(int* i_buf,const TNyARIntSize* i_size)
	{
		this->_ref_buf = i_buf;
		this->_size = i_size;
	}
	void NyARRgbPixelReader_INT1D_GRAY_8::getPixel(int i_x, int i_y, int o_rgb[])const
	{
		o_rgb[0]=o_rgb[1]=o_rgb[2]=this->_ref_buf[i_x + i_y * this->_size->w];
		return;
	}

	void NyARRgbPixelReader_INT1D_GRAY_8::getPixelSet(const int i_x[],const int i_y[], int i_num, int o_rgb[])const
	{
		const int width = this->_size->w;
		const int* ref_buf = this->_ref_buf;
		for (int i = i_num - 1; i >= 0; i--){
			o_rgb[i * 3 + 0] = o_rgb[i * 3 + 1]=o_rgb[i * 3 + 2]=ref_buf[i_x[i] + i_y[i] * width];
		}
		return;
	}
	void NyARRgbPixelReader_INT1D_GRAY_8::setPixel(int i_x, int i_y, const int* i_rgb)
	{
		NyARException::notImplement();
	}
	void NyARRgbPixelReader_INT1D_GRAY_8::setPixels(const int* i_x, const int* i_y, int i_num, const int* i_intrgb)
	{
		NyARException::notImplement();
	}
	void NyARRgbPixelReader_INT1D_GRAY_8::switchBuffer(void* i_ref_buffer)
	{
		this->_ref_buf=(int*)i_ref_buffer;
	}
}
