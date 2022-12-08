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
#include "../utils/NyArray.h"
#include "INyARDoubleMatrix.h"
namespace NyARToolkitCPP
{
	class NyARDoubleMatrix44 : INyARDoubleMatrix
	{
	public:
		double m00;
		double m01;
		double m02;
		double m03;
		double m10;
		double m11;
		double m12;
		double m13;
		double m20;
		double m21;
		double m22;
		double m23;
		double m30;
		double m31;
		double m32;
		double m33;
	public:
		void setValue(const double i_value[]);
		void getValue(double o_value[])const;
		static NyArray<NyARDoubleMatrix44>* createArray(int i_number);
		bool inverse(const NyARDoubleMatrix44& i_src);
	};
}
