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
#include "NyARRotMatrix.h"
#include "NyARRotVector.h"

namespace NyARToolkitCPP
{
	class NyARRotMatrix_ARToolKit :public NyARRotMatrix
	{	
	public:
		NyARRotMatrix_ARToolKit(const NyARPerspectiveProjectionMatrix* i_matrix);
		virtual ~NyARRotMatrix_ARToolKit();
		void updateAngleFromMatrix();
	private:
		NyARRotVector* __initRot_vec1;
		NyARRotVector* __initRot_vec2;
	protected:
		TNyARDoublePoint3d _angle;
	private:
	public:
		void initRotByPrevResult(const NyARTransMatResult& i_prev_result);
		void initRotBySquare(const TNyARLinear i_linear[],const TNyARDoublePoint2d i_sqvertex[]);
		const TNyARDoublePoint3d& refAngle()const;
	public:
		virtual void setAngle(double i_x,double i_y,double i_z);

	};
}
