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
#include "NyARSquare.h"

namespace NyARToolkitCPP
{
	void NyARSquare::getCenter2d(TNyARDoublePoint2d& o_out)const
    {
    	o_out.x=(this->sqvertex[0].x+this->sqvertex[1].x+this->sqvertex[2].x+this->sqvertex[3].x)/4;
    	o_out.y=(this->sqvertex[0].y+this->sqvertex[1].y+this->sqvertex[2].y+this->sqvertex[3].y)/4;
    	return;
    }
}
