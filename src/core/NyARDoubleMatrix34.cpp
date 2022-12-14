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
#include "NyARDoubleMatrix34.h"

namespace NyARToolkitCPP
{
	 void NyARDoubleMatrix34::setValue(const double i_value[])
	{
		this->m00=i_value[0];
		this->m01=i_value[1];
		this->m02=i_value[2];
		this->m03=i_value[3];
		this->m10=i_value[4];
		this->m11=i_value[5];
		this->m12=i_value[6];
		this->m13=i_value[7];
		this->m20=i_value[8];
		this->m21=i_value[9];
		this->m22=i_value[10];
		this->m23=i_value[11];
		return;
	}
	void NyARDoubleMatrix34::getValue(double o_value[])const
	{
		o_value[0]=this->m00;
		o_value[1]=this->m01;
		o_value[2]=this->m02;
		o_value[3]=this->m03;
		o_value[4]=this->m10;
		o_value[5]=this->m11;
		o_value[6]=this->m12;
		o_value[7]=this->m13;
		o_value[8]=this->m20;
		o_value[9]=this->m21;
		o_value[10]=this->m22;
		o_value[11]=this->m23;
		return;
	}
}
