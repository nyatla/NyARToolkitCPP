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
#include "NyARBaseClass.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class INyARRaster:public NyARBaseClass
	{
	public:
		virtual int getWidth()const=0;
		virtual int getHeight()const=0;
		virtual const TNyARIntSize& getSize()const=0;
		virtual void* getBuffer()const=0;
		virtual int getBufferType()const=0;
		virtual bool isEqualBufferType(int i_type_value)const=0;
		virtual bool hasBuffer()const=0;
		virtual void wrapBuffer(void* i_ref_buf)=0;

	};
}
