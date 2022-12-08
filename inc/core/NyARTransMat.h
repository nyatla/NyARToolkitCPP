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
#include "INyARTransMat.h"
#include "INyARRotTransOptimize.h"
#include "INyARTransportVectorSolver.h"
#include "NyARPartialDifferentiationOptimize.h"
#include "NyARParam.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARTransMat:public INyARTransMat
	{
	private:
		NyARDoubleMatrix33 __rot;
		const NyARCameraDistortionFactor* _ref_dist_factor;
		const NyARPerspectiveProjectionMatrix* _projection_mat_ref;
		double errRate(const NyARDoubleMatrix33& io_rot,const TNyARDoublePoint3d& i_trans,const TNyARDoublePoint3d i_vertex3d[], TNyARDoublePoint2d i_vertex2d[],int i_number_of_vertex,TNyARDoublePoint3d o_rot_vertex[]);
		double optimize(NyARRotMatrix& io_rotmat,TNyARDoublePoint3d& io_transvec,const INyARTransportVectorSolver& i_solver,const TNyARDoublePoint3d i_offset_3d[],TNyARDoublePoint2d i_2d_vertex[],double i_err_threshold);
	protected:
		NyARRotMatrix* _rotmatrix;
		NyARPartialDifferentiationOptimize* _mat_optimize;
	protected:
		INyARTransportVectorSolver* _transsolver;
	protected:
		NyARTransMat();
	public:
		NyARTransMat(const NyARParam* i_param);
		virtual ~NyARTransMat();
		void transMat(const NyARSquare& i_square,const NyARRectOffset& i_offset, NyARTransMatResult& o_result);
		void transMatContinue(const NyARSquare& i_square,const NyARRectOffset& i_offset, NyARTransMatResult& io_result_conv);
	private:
		double makeErrThreshold(const TNyARDoublePoint2d i_vertex[]);
		void initVertexOrder(const NyARSquare& i_square, int i_direction,const TNyARDoublePoint2d* o_sqvertex_ref[],const TNyARLinear* o_liner_ref[])const;
	private:
		void updateMatrixValue(const NyARRotMatrix& i_rot, const TNyARDoublePoint3d& i_trans,NyARTransMatResult& o_result);
	};

}
