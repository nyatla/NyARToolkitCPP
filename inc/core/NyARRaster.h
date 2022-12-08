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
#include "NyARRaster_BasicClass.h"
#include "NyAR_types.h"
#include "NyARException.h"
#include "NyARBufferType.h"
namespace NyARToolkitCPP
{
	/**このクラスは、単機能のNyARRasterです。
	* 特定タイプのバッファをラップする、INyARBufferReaderインタフェイスを提供します。
	*
	*/
	class NyARRaster :public NyARRaster_BasicClass
	{
	protected:
		void* _buf;
		int _buf_type;
		/**
		* バッファオブジェクトがアタッチされていればtrue
		*/
		bool _is_attached_buffer;
	public:
		NyARRaster(int i_width, int i_height,int i_buffer_type,bool i_is_alloc);
		NyARRaster(int i_width, int i_height,int i_buffer_type);
		~NyARRaster();
	protected:
		bool initInstance(TNyARIntSize& i_size,int i_buf_type,bool i_is_alloc);
	public:
		void* getBuffer()const;
		bool hasBuffer()const;
		void wrapBuffer(void* i_ref_buf);
	};
}

