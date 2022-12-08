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
#include "NyStdLib.h"

namespace NyARToolkitCPP
{
	int NyStdLib::byteSwap(int i_src)
	{
		int result;
		const unsigned char* r=(const unsigned char*)&i_src;
		unsigned char* w=(unsigned char*)&result;
		for(int i = 0; i < 4; i++ ) {
			*(w+i)=*(r+3-i);
		}
		return result;
	}

	double NyStdLib::byteSwap(double i_src)
	{
		double result;
		const unsigned char* r=(const unsigned char*)&i_src;
		unsigned char* w=(unsigned char*)&result;
		for(int i = 0; i < 8; i++ ) {
			*(w+i)=*(r+7-i);
		}
		return result;
	}
}
