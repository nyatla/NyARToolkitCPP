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
#include "NyARRotVector.h"
#include "NyARPerspectiveProjectionMatrix.h"
#include "NyARMat.h"
#include "NyAR_types.h"

namespace NyARToolkitCPP
{
	class NyARRotVector:public NyARBaseClass
	{
	public:
		NyARRotVector(const NyARPerspectiveProjectionMatrix* i_cmat);
		~NyARRotVector();
	public:
		double v1;
		double v2;
		double v3;
	private:
		const NyARPerspectiveProjectionMatrix* _projection_mat_ref;
		NyARMat* _mat_a;
	public:
		void exteriorProductFromLinear(const TNyARLinear& i_linear1, const TNyARLinear& i_linear2);
		void checkVectorByVertex(const TNyARDoublePoint2d& i_start_vertex,const TNyARDoublePoint2d& i_end_vertex);
		static void checkRotation(NyARRotVector& io_vec1,NyARRotVector& io_vec2);
	};

}
