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
#include "NyARLabelingLabelStack.h"
#include "NyARRaster_BasicClass.h"
#include "NyARBufferType.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARLabelingImage:public NyARRaster_BasicClass
	{
	/*INyARLabelingImage*/
	protected:
		int* _ref_buf;
	protected:
		NyARLabelingLabelStack* _label_list;
		NyArray<int>* _index_table;
		bool _is_index_table_enable;
	public:
		const NyArray<int>* getIndexArray()const;
		NyARLabelingLabelStack& getLabelStack()const;
	/*NyARLabelingImage*/
	public:
		NyARLabelingImage(int i_width, int i_height);
		virtual ~NyARLabelingImage();
	protected:
	public:
		bool hasBuffer()const;
		void* getBuffer()const;
		void wrapBuffer(void* i_ref_buf);
		int getTopClipTangentX(const NyARLabelingLabel& i_label)const;
		void reset(bool i_label_index_enable);
		void drawFrameEdge();
	};
}
