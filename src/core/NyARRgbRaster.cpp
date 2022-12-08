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
#include "NyARRgbRaster.h"
#include "nyarcore.h"
#include "NyARRgbPixelReader_INT1D_X8R8G8B8_32.h"
#include "NyARRgbPixelReader_BYTE1D_B8G8R8X8_32.h"
#include "NyARRgbPixelReader_BYTE1D_R8G8B8_24.h"
namespace NyARToolkitCPP
{
	NyARRgbRaster::NyARRgbRaster(int i_width, int i_height,int i_raster_type,bool i_is_alloc)
		:NyARRgbRaster_BasicClass(i_width,i_height,i_raster_type)
		{
			if(!initInstance(this->_size,i_raster_type,i_is_alloc)){
				throw NyARException();
			}
		}
	/**
	 * 
	 * @param i_width
	 * @param i_height
	 * @param i_raster_type
	 * NyARBufferTypeに定義された定数値を指定してください。
	 * @throws NyARException
	 */
	NyARRgbRaster::NyARRgbRaster(int i_width, int i_height,int i_raster_type)
		:NyARRgbRaster_BasicClass(i_width,i_height,i_raster_type)
	{
		if(!initInstance(this->_size,i_raster_type,true)){
			throw NyARException();
		}
	}
	NyARRgbRaster::~NyARRgbRaster()
	{
		NyAR_SAFE_DELETE(this->_reader);
		if(this->_is_attached_buffer){
			NyAR_SAFE_FREE(this->_buf);
		}
	}
	bool NyARRgbRaster::initInstance(const TNyARIntSize& i_size,int i_raster_type,bool i_is_alloc)
	{
		switch(i_raster_type)
		{
		case NyARBufferType::INT1D_X8R8G8B8_32:
			this->_buf=i_is_alloc?NyAR_MALLOC(sizeof(int)*i_size.w*i_size.h):NULL;
			this->_reader=new NyARRgbPixelReader_INT1D_X8R8G8B8_32(&i_size,(int*)this->_buf);
			break;
		case NyARBufferType::BYTE1D_B8G8R8X8_32:
			this->_buf=i_is_alloc?NyAR_MALLOC(sizeof(NyAR_BYTE_t)*i_size.w*i_size.h*4):NULL;
			this->_reader=new NyARRgbPixelReader_BYTE1D_B8G8R8X8_32(&i_size,(NyAR_BYTE_t*)this->_buf);
			break;
		case NyARBufferType::BYTE1D_R8G8B8_24:
			this->_buf=i_is_alloc?NyAR_MALLOC(sizeof(NyAR_BYTE_t)*i_size.w*i_size.h*3):NULL;
			this->_reader=new NyARRgbPixelReader_BYTE1D_R8G8B8_24(&i_size,(NyAR_BYTE_t*)this->_buf);
			break;
		default:
			return false;
		}
		this->_is_attached_buffer=i_is_alloc;
		return true;
	}
	INyARRgbPixelReader& NyARRgbRaster::getRgbPixelReader()const
	{
		return *this->_reader;
	}
	void* NyARRgbRaster::getBuffer()const
	{
		return this->_buf;
	}
	bool NyARRgbRaster::hasBuffer()const
	{
		return this->_buf!=NULL;
	}
	void NyARRgbRaster::wrapBuffer(void* i_ref_buf)
	{
		NyAR_ASSERT(!this->_is_attached_buffer);//バッファがアタッチされていたら機能しない。
		this->_buf=i_ref_buf;
		//ピクセルリーダーの参照バッファを切り替える。
		this->_reader->switchBuffer(i_ref_buf);
	}
}
