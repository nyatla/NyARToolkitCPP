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
#include "NyARDoubleMatrix33.h"
#include "NyARTransMatResult.h"
#include "NyARPerspectiveProjectionMatrix.h"
#include "NyARRotVector.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARRotMatrix :public NyARDoubleMatrix33
	{	
	public:
		NyARRotMatrix(const NyARPerspectiveProjectionMatrix* i_matrix);
		virtual ~NyARRotMatrix();
	private:
		NyARRotVector* __initRot_vec1;
		NyARRotVector* __initRot_vec2;
	/**
	 * NyARTransMatResultÇÃì‡óeÇ©ÇÁNyARRotMatrixÇïúå≥ÇµÇ‹Ç∑ÅB
	 * @param i_prev_result
	 */
	public:
		virtual void initRotByPrevResult(const NyARTransMatResult& i_prev_result);
		virtual void initRotBySquare(const TNyARLinear i_linear[],const TNyARDoublePoint2d i_sqvertex[]);
		void getPoint3d(const TNyARDoublePoint3d& i_in_point,TNyARDoublePoint3d& i_out_point)const;
		void getPoint3dBatch(const TNyARDoublePoint3d i_in_point[],TNyARDoublePoint3d i_out_point[],int i_number_of_vertex)const;
	};
}
