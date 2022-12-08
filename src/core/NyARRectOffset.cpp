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
#include "NyARRectOffset.h"

namespace NyARToolkitCPP
{
	void NyARRectOffset::setSquare(double i_width)
	{
		const double w_2 = i_width / 2.0;

		TNyARDoublePoint3d *vertex3d_ptr= this->vertex;
		vertex3d_ptr->x = -w_2;
		vertex3d_ptr->y =  w_2;
		vertex3d_ptr->z = 0.0;
		vertex3d_ptr++;
		vertex3d_ptr->x = w_2;
		vertex3d_ptr->y = w_2;
		vertex3d_ptr->z = 0.0;
		vertex3d_ptr++;
		vertex3d_ptr->x =  w_2;
		vertex3d_ptr->y = -w_2;
		vertex3d_ptr->z = 0.0;
		vertex3d_ptr++;
		vertex3d_ptr->x = -w_2;
		vertex3d_ptr->y = -w_2;
		vertex3d_ptr->z = 0.0;
		return;
	}
}
