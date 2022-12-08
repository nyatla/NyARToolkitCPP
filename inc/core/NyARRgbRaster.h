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
#include "NyARRaster.h"
#include "nyarcore.h"
#include "NyARRgbRaster_BasicClass.h"
#include "NyAR_types.h"
#include "NyARException.h"
#include "NyARBufferType.h"
namespace NyARToolkitCPP
{
	/**このクラスは、単機能のNyARRasterです。
	* 特定タイプのバッファをラップする、INyARBufferReaderインタフェイスを提供します。
	*
	*/
	class NyARRgbRaster :public NyARRgbRaster_BasicClass
	{
	protected:
		void* _buf;
		INyARRgbPixelReader* _reader;
		/**
		 * バッファオブジェクトがアタッチされていればtrue
		 */
		bool _is_attached_buffer;
		/**
		 * 
		 * @param i_width
		 * @param i_height
		 * @param i_raster_type
		 * NyARBufferTypeに定義された定数値を指定してください。
		 * @param i_is_alloc
		 * @throws NyARException
		 */
	public:
		NyARRgbRaster(int i_width, int i_height,int i_raster_type,bool i_is_alloc);
		NyARRgbRaster(int i_width, int i_height,int i_raster_type);
		~NyARRgbRaster();
	protected:
		bool initInstance(const TNyARIntSize& i_size,int i_raster_type,bool i_is_alloc);
	public:
		INyARRgbPixelReader& getRgbPixelReader()const;
		void* getBuffer()const;
		bool hasBuffer()const;
		virtual void wrapBuffer(void* i_ref_buf);
	};
}