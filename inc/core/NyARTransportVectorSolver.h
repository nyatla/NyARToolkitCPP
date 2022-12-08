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
#include "INyARTransportVectorSolver.h"
#include "NyARPerspectiveProjectionMatrix.h"
namespace NyARToolkitCPP
{
	class NyARTransportVectorSolver :public INyARTransportVectorSolver
	{
	private:
		double* _cx;
		double* _cy;	
	private:
		const NyARPerspectiveProjectionMatrix* _projection_mat;
		int _nmber_of_vertex;
	public:
		NyARTransportVectorSolver(const NyARPerspectiveProjectionMatrix* i_projection_mat_ref,int i_max_vertex);
		virtual ~NyARTransportVectorSolver();
	private:
		double _a00,_a01_10,_a02_20,_a11,_a12_21,_a22;
	public:
		void virtual set2dVertex(const TNyARDoublePoint2d i_ref_vertex_2d[],int i_number_of_vertex);
		void virtual solveTransportVector(const TNyARDoublePoint3d i_vertex3d[],TNyARDoublePoint3d& o_transfer)const;
	};
}
