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
#include "NyARRotMatrix_ARToolKit_O2.h"
#include <cmath>
namespace NyARToolkitCPP
{
	NyARRotMatrix_ARToolKit_O2::NyARRotMatrix_ARToolKit_O2(const NyARPerspectiveProjectionMatrix* i_matrix):NyARRotMatrix_ARToolKit(i_matrix)
	{
		return;
	}
	/*	ŠÖ”·‚µ‘Ö‚¦
	*/
	void NyARRotMatrix_ARToolKit_O2::setAngle(double i_x,double i_y,double i_z)
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
		//angle‚ð‹tŒvŽZ‚¹‚¸‚É’¼Ú‘ã“ü
		this->_angle.x=i_x;
		this->_angle.y=i_y;
		this->_angle.z=i_z;
		return;
	}

}
