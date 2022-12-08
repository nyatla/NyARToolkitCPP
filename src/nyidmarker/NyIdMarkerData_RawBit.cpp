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
#include "NyIdMarkerData_RawBit.h"
#include <string.h>

namespace NyARToolkitCPP
{
	bool NyIdMarkerData_RawBit::isEqual(const INyIdMarkerData& i_target)const
	{
		const NyIdMarkerData_RawBit &s=(NyIdMarkerData_RawBit&)i_target;
		if(s.length!=this->length){
			return false;
		}
		for(int i=s.length-1;i>=0;i--){
			if(s.packet[i]!=s.packet[i]){
				return false;
			}
		}
		return true;
	}
	void NyIdMarkerData_RawBit::copyFrom(const INyIdMarkerData& i_source)
	{
		const NyIdMarkerData_RawBit &s=(const NyIdMarkerData_RawBit&)i_source;
		memcpy(this->packet,s.packet,sizeof(int)*s.length);
		this->length=s.length;
		return;
	}
}
