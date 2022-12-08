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
#include "NyARDoubleMatrix33.h"
#include "NyARPerspectiveProjectionMatrix.h"
#include "NyAREquationSolver.h"
namespace NyARToolkitCPP
{
	typedef struct TSinCosValue_t 
	{
		double cos_val;
		double sin_val;
	}TSinCosValue;

	/**
	* 基本姿勢と実画像を一致するように、角度を微調整→平行移動量を再計算 を繰り返して、変換行列を最適化する。
	* 
	*/
	class NyARPartialDifferentiationOptimize:public NyARBaseClass
	{
	private:
		const NyARPerspectiveProjectionMatrix* _projection_mat_ref;

	public:
		NyARPartialDifferentiationOptimize(const NyARPerspectiveProjectionMatrix* i_projection_mat_ref);
		void sincos2Rotation_ZXY(const TSinCosValue (&i_sincos)[3],NyARDoubleMatrix33& i_rot_matrix);
	private:
		void rotation2Sincos_ZXY(const NyARDoubleMatrix33& i_rot_matrix, TSinCosValue (&o_out)[3],TNyARDoublePoint3d& o_ang);
	private:
		double optimizeParamX(const TSinCosValue& i_angle_y, const TSinCosValue& i_angle_z, const TNyARDoublePoint3d& i_trans, const TNyARDoublePoint3d i_vertex3d[],const TNyARDoublePoint2d i_vertex2d[], int i_number_of_vertex, double i_hint_angle);
		double optimizeParamY(const TSinCosValue& i_angle_x, const TSinCosValue& i_angle_z,const TNyARDoublePoint3d& i_trans,const TNyARDoublePoint3d i_vertex3d[],const TNyARDoublePoint2d i_vertex2d[], int i_number_of_vertex, double i_hint_angle);
		double optimizeParamZ(const TSinCosValue& i_angle_x, const TSinCosValue& i_angle_y,const TNyARDoublePoint3d& i_trans,const TNyARDoublePoint3d i_vertex3d[],const TNyARDoublePoint2d i_vertex2d[], int i_number_of_vertex, double i_hint_angle);
	public:
		void modifyMatrix(NyARDoubleMatrix33& io_rot,const TNyARDoublePoint3d& i_trans,const TNyARDoublePoint3d i_vertex3d[],const TNyARDoublePoint2d i_vertex2d[], int i_number_of_vertex);
	private:
		double getMinimumErrorAngleFromParam(double iL,double iJ, double iK, double iM, double iN, double iO, double i_hint_angle);

	};
}
