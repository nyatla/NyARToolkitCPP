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
#include "NyARRaster.h"
#include "nyarcore.h"
namespace NyARToolkitCPP
{
	NyARRaster::NyARRaster(int i_width, int i_height,int i_buffer_type,bool i_is_alloc)
		:NyARRaster_BasicClass(i_width,i_height,i_buffer_type)
	{
		if(!initInstance(this->_size,i_buffer_type,i_is_alloc)){
			throw NyARException();
		}
		return;
	}	

	NyARRaster::NyARRaster(int i_width, int i_height,int i_buffer_type)
		:NyARRaster_BasicClass(i_width,i_height,i_buffer_type)
	{
		if(!initInstance(this->_size,i_buffer_type,true)){
			throw NyARException();
		}
		return;
	}

	bool NyARRaster::initInstance(TNyARIntSize& i_size,int i_buf_type,bool i_is_alloc)
	{
		switch(i_buf_type)
		{
		case NyARBufferType::INT1D_X8R8G8B8_32:
			this->_buf=i_is_alloc?NyAR_MALLOC(sizeof(int)*i_size.w*i_size.h):NULL;
			break;
		default:
			return false;
		}
		this->_is_attached_buffer=i_is_alloc;
		return true;
	}
	void* NyARRaster::getBuffer()const
	{
		return this->_buf;
	}
	bool NyARRaster::hasBuffer()const
	{
		return this->_buf!=NULL;
	}
	void NyARRaster::wrapBuffer(void* i_ref_buf)
	{
		assert(!this->_is_attached_buffer);//バッファがアタッチされていたら機能しない。
		this->_buf=i_ref_buf;
	}
	NyARRaster::~NyARRaster()
	{
		if(this->_is_attached_buffer){
			NyAR_SAFE_FREE(this->_buf);
		}
		return;
	}
}
