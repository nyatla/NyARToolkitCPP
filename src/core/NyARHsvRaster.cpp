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

#include "NyARHsvRaster.h"
#include "NyARBufferType.h"
#include "nyarcore.h"
#include <cstdlib>
#include <assert.h>

namespace NyARToolkitCPP
{
	NyARHsvRaster::NyARHsvRaster(int i_width, int i_height)
		:NyARRaster_BasicClass(i_width,i_height,NyARBufferType::INT1D_X7H9S8V8_32)
	{
		//このクラスは外部参照バッファ/形式多重化が使えない簡易実装です。
		this->_ref_buf = (int*)NyAR_MALLOC(sizeof(int)*i_height*i_width);
	}
	NyARHsvRaster::~NyARHsvRaster()
	{
		NyAR_SAFE_FREE(this->_ref_buf);
	}
	void* NyARHsvRaster::getBuffer()const
	{
		return this->_ref_buf;
	}
	bool NyARHsvRaster:: hasBuffer()const
	{
		return true;
	}
	void NyARHsvRaster::wrapBuffer(void* i_ref_buf)
	{
		NyARException::notImplement();
	}

}
