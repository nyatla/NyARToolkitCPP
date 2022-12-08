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
#include "NyARRgbPixelReader_BYTE1D_B8G8R8X8_32.h"
#include "NyARException.h"

namespace NyARToolkitCPP
{
	NyARRgbPixelReader_BYTE1D_B8G8R8X8_32::NyARRgbPixelReader_BYTE1D_B8G8R8X8_32(const TNyARIntSize* i_size,NyAR_BYTE_t* i_buf)
	{
		this->_ref_buf = i_buf;
		this->_ref_size = i_size;
	}
	void NyARRgbPixelReader_BYTE1D_B8G8R8X8_32::getPixel(int i_x, int i_y, int* o_rgb)const
	{
		const NyAR_BYTE_t* ref_buf =this->_ref_buf;
		int bp = (i_x + i_y * this->_ref_size->w) * 4;
		o_rgb[0] = (ref_buf[bp + 2] & 0xff);// R
		o_rgb[1] = (ref_buf[bp + 1] & 0xff);// G
		o_rgb[2] = (ref_buf[bp + 0] & 0xff);// B
		return;
	}

	void NyARRgbPixelReader_BYTE1D_B8G8R8X8_32::getPixelSet(const int* i_x,const int* i_y, int i_num, int* o_rgb)const
	{
		int bp;
		int width = this->_ref_size->w;
		const NyAR_BYTE_t* ref_buf =this->_ref_buf;
		for (int i = i_num - 1; i >= 0; i--) {
			bp = (i_x[i] + i_y[i] * width) * 4;
			o_rgb[i * 3 + 0] = (ref_buf[bp + 2] & 0xff);// R
			o_rgb[i * 3 + 1] = (ref_buf[bp + 1] & 0xff);// G
			o_rgb[i * 3 + 2] = (ref_buf[bp + 0] & 0xff);// B
		}
		return;
	}
	void NyARRgbPixelReader_BYTE1D_B8G8R8X8_32::setPixel(int i_x, int i_y, const int* i_rgb)
	{
		NyAR_BYTE_t* ref_buf =this->_ref_buf;
		int bp = (i_x + i_y * this->_ref_size->w) * 4;
		ref_buf[bp+0] = (NyAR_BYTE_t)i_rgb[0];// R
		ref_buf[bp+1] = (NyAR_BYTE_t)i_rgb[1];// G
		ref_buf[bp+2] = (NyAR_BYTE_t)i_rgb[2];// B	
	}
	void NyARRgbPixelReader_BYTE1D_B8G8R8X8_32::setPixels(const int* i_x, const int* i_y, int i_num, const int* i_intrgb)
	{
		NyARException::notImplement();
	}
	void NyARRgbPixelReader_BYTE1D_B8G8R8X8_32::switchBuffer(void* i_ref_buffer)
	{
		this->_ref_buf=(NyAR_BYTE_t*)i_ref_buffer;
	}
}

