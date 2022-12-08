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
	class INyARCameraDistortionFactor:public NyARBaseClass
	{
	public:
		virtual void ideal2Observ(const TNyARDoublePoint2d& i_in, TNyARDoublePoint2d& o_out)const=0;
		virtual void ideal2ObservBatch(const TNyARDoublePoint2d i_in[],TNyARDoublePoint2d o_out[], int i_size)const=0;
		virtual void observ2Ideal(double ix, double iy,TNyARDoublePoint2d& o_point)const=0;
		virtual void observ2IdealBatch(const int i_x_coord[],const int i_y_coord[],int i_start, int i_num, double o_x_coord[],double o_y_coord[])const=0;
	};
}
