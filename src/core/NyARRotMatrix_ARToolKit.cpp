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
#include "NyARRotMatrix_ARToolKit.h"
#include "NyARRotMatrix.h"
#include "NyARRotVector.h"
#include "nyarcore.h"
#include <cmath>
#include <cstdlib>
namespace NyARToolkitCPP
{
	NyARRotMatrix_ARToolKit::NyARRotMatrix_ARToolKit(const NyARPerspectiveProjectionMatrix* i_matrix):NyARRotMatrix(i_matrix)
	{
		return;
	}
	NyARRotMatrix_ARToolKit::~NyARRotMatrix_ARToolKit()
	{
	}
	void NyARRotMatrix_ARToolKit::initRotByPrevResult(const NyARTransMatResult& i_prev_result)
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
	void NyARRotMatrix_ARToolKit::initRotBySquare(const TNyARLinear i_linear[],const TNyARDoublePoint2d i_sqvertex[])
	{
		NyARRotMatrix::initRotBySquare(i_linear,i_sqvertex);
		//Matrixからangleをロード
		this->updateAngleFromMatrix();
		return;
	}
	const TNyARDoublePoint3d& NyARRotMatrix_ARToolKit::refAngle()const
	{
		return this->_angle;
	}


	/**
	* 回転角から回転行列を計算してセットします。
	* @param i_x
	* @param i_y
	* @param i_z
	*/
	void NyARRotMatrix_ARToolKit::setAngle(double i_x,double i_y,double i_z)
	{
		const double sina = sin(i_x);
		const double cosa = cos(i_x);
		const double sinb = sin(i_y);
		const double cosb = cos(i_y);
		const double sinc = sin(i_z);
		const double cosc = cos(i_z);
		// Optimize
		const double CACA = cosa * cosa;
		const double SASA = sina * sina;
		const double SACA = sina * cosa;
		const double SASB = sina * sinb;
		const double CASB = cosa * sinb;
		const double SACACB = SACA * cosb;

		this->m00 = CACA * cosb * cosc + SASA * cosc + SACACB * sinc - SACA * sinc;
		this->m01 = -CACA * cosb * sinc - SASA * sinc + SACACB * cosc - SACA * cosc;
		this->m02 = CASB;
		this->m10 = SACACB * cosc - SACA * cosc + SASA * cosb * sinc + CACA * sinc;
		this->m11 = -SACACB * sinc + SACA * sinc + SASA * cosb * cosc + CACA * cosc;
		this->m12 = SASB;
		this->m20 = -CASB * cosc - SASB * sinc;
		this->m21 = CASB * sinc - SASB * cosc;
		this->m22 = cosb;
		//angleは逆算する。
		updateAngleFromMatrix();
		return;
	}

	/*	バグ：m22はセット時に確定できるんじゃないの？
	
	*/
	void NyARRotMatrix_ARToolKit::updateAngleFromMatrix()
	{
		double a,b,c;
		double sina, cosa, sinb,cosb,sinc, cosc;

		if (this->m22 > 1.0) {// <Optimize/>if( rot[2][2] > 1.0 ) {
			cosb = 1.0;// <Optimize/>rot[2][2] = 1.0;
		} else if (this->m22 < -1.0) {// <Optimize/>}else if( rot[2][2] < -1.0 ) {
			cosb = -1.0;// <Optimize/>rot[2][2] = -1.0;
		}else{
			cosb =this->m22;// <Optimize/>cosb = rot[2][2];
		}
		b = acos(cosb);
		sinb =sin(b);
		const double rot02=this->m02;
		const double rot12=this->m12;
		if (b >= 0.000001 || b <= -0.000001) {
			cosa = rot02 / sinb;// <Optimize/>cosa = rot[0][2] / sinb;
			sina = rot12 / sinb;// <Optimize/>sina = rot[1][2] / sinb;
			if (cosa > 1.0) {
				cosa = 1.0;
				sina = 0.0;
			}
			if (cosa < -1.0) {
				cosa = -1.0;
				sina = 0.0;
			}
			if (sina > 1.0) {
				sina = 1.0;
				cosa = 0.0;
			}
			if (sina < -1.0) {
				sina = -1.0;
				cosa = 0.0;
			}
			a = acos(cosa);
			if (sina < 0) {
				a = -a;
			}
			// <Optimize>
			// sinc = (rot[2][1]*rot[0][2]-rot[2][0]*rot[1][2])/(rot[0][2]*rot[0][2]+rot[1][2]*rot[1][2]);
			// cosc = -(rot[0][2]*rot[2][0]+rot[1][2]*rot[2][1])/(rot[0][2]*rot[0][2]+rot[1][2]*rot[1][2]);
			const double tmp = (rot02 * rot02 + rot12 * rot12);
			sinc = (this->m21 * rot02 - this->m20 * rot12) / tmp;
			cosc = -(rot02 * this->m20 + rot12 * this->m21) / tmp;
			// </Optimize>

			if (cosc > 1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = 1.0;
				sinc = 0.0;
			}
			if (cosc < -1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = -1.0;
				sinc = 0.0;
			}
			if (sinc > 1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = 1.0;
				cosc = 0.0;
			}
			if (sinc < -1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = -1.0;
				cosc = 0.0;
			}
			c = acos(cosc);
			if (sinc < 0) {
				c = -c;
			}
		} else {
			a = b = 0.0;
			cosa = cosb = 1.0;
			sina = sinb = 0.0;
			cosc=this->m00;//cosc = rot[0];// <Optimize/>cosc = rot[0][0];
			sinc=this->m01;//sinc = rot[1];// <Optimize/>sinc = rot[1][0];
			if (cosc > 1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = 1.0;
				sinc = 0.0;
			}
			if (cosc < -1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = -1.0;
				sinc = 0.0;
			}
			if (sinc > 1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = 1.0;
				cosc = 0.0;
			}
			if (sinc < -1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = -1.0;
				cosc = 0.0;
			}
			c = acos(cosc);
			if (sinc < 0) {
				c = -c;
			}
		}
		//angleの更新
		this->_angle.x = a;// wa.value=a;//*wa = a;
		this->_angle.y = b;// wb.value=b;//*wb = b;
		this->_angle.z = c;// wc.value=c;//*wc = c;
		return;
	}


}
