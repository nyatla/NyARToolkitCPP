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
#include "NyARRotMatrix.h"
#include "nyarcore.h"
namespace NyARToolkitCPP
{
	NyARRotMatrix::NyARRotMatrix(const NyARPerspectiveProjectionMatrix* i_matrix)
	{
		this->__initRot_vec1=new NyARRotVector(i_matrix);
		this->__initRot_vec2=new NyARRotVector(i_matrix);
		return;
	}
	NyARRotMatrix::~NyARRotMatrix()
	{
		NyAR_SAFE_DELETE(this->__initRot_vec1);
		NyAR_SAFE_DELETE(this->__initRot_vec2);
	}
	void NyARRotMatrix::initRotByPrevResult(const NyARTransMatResult& i_prev_result)
	{
		this->m00=i_prev_result.m00;
		this->m01=i_prev_result.m01;
		this->m02=i_prev_result.m02;

		this->m10=i_prev_result.m10;
		this->m11=i_prev_result.m11;
		this->m12=i_prev_result.m12;

		this->m20=i_prev_result.m20;
		this->m21=i_prev_result.m21;
		this->m22=i_prev_result.m22;
		return;
	}
	void NyARRotMatrix::initRotBySquare(const TNyARLinear i_linear[],const TNyARDoublePoint2d i_sqvertex[])
	{
		NyARRotVector& vec1=*this->__initRot_vec1;
		NyARRotVector& vec2=*this->__initRot_vec2;

		//Œü‚©‚¢‡‚Á‚½•Ó‚©‚çA‚Q–{‚ÌƒxƒNƒgƒ‹‚ðŒvŽZ

		//Ž²‚P
		vec1.exteriorProductFromLinear(i_linear[0], i_linear[2]);
		vec1.checkVectorByVertex(i_sqvertex[0], i_sqvertex[1]);

		//Ž²‚Q
		vec2.exteriorProductFromLinear(i_linear[1], i_linear[3]);
		vec2.checkVectorByVertex(i_sqvertex[3], i_sqvertex[0]);

		//‰ñ“]‚ÌÅ“K‰»H
		NyARRotVector::checkRotation(vec1,vec2);

		this->m00 =vec1.v1;
		this->m10 =vec1.v2;
		this->m20 =vec1.v3;
		this->m01 =vec2.v1;
		this->m11 =vec2.v2;
		this->m21 =vec2.v3;

		//ÅŒã‚ÌŽ²‚ðŒvŽZ
		double w02 = vec1.v2 * vec2.v3 - vec1.v3 * vec2.v2;
		double w12 = vec1.v3 * vec2.v1 - vec1.v1 * vec2.v3;
		double w22 = vec1.v1 * vec2.v2 - vec1.v2 * vec2.v1;
		double w = sqrt(w02 * w02 + w12 * w12 + w22 * w22);
		this->m02 = w02/w;
		this->m12 = w12/w;
		this->m22 = w22/w;
		return;
	}
	void NyARRotMatrix::getPoint3d(const TNyARDoublePoint3d& i_in_point,TNyARDoublePoint3d& i_out_point)const
	{
		double x=i_in_point.x;
		double y=i_in_point.y;
		double z=i_in_point.z;
		i_out_point.x=this->m00 * x + this->m01 * y + this->m02 * z;
		i_out_point.y=this->m10 * x + this->m11 * y + this->m12 * z;
		i_out_point.z=this->m20 * x + this->m21 * y + this->m22 * z;
		return;
	}
	/**
	* •¡”‚Ì’¸“_‚ðˆêŠ‡‚µ‚Ä•ÏŠ·‚·‚é
	* @param i_in_point
	* @param i_out_point
	* @param i_number_of_vertex
	*/
	void NyARRotMatrix::getPoint3dBatch(const TNyARDoublePoint3d i_in_point[],TNyARDoublePoint3d i_out_point[],int i_number_of_vertex)const
	{
		for(int i=i_number_of_vertex-1;i>=0;i--){
			TNyARDoublePoint3d& out_ptr=i_out_point[i];
			const TNyARDoublePoint3d& in_ptr=i_in_point[i];
			double x=in_ptr.x;
			double y=in_ptr.y;
			double z=in_ptr.z;
			out_ptr.x=this->m00 * x + this->m01 * y + this->m02 * z;
			out_ptr.y=this->m10 * x + this->m11 * y + this->m12 * z;
			out_ptr.z=this->m20 * x + this->m21 * y + this->m22 * z;
		}
		return;
	}
}
