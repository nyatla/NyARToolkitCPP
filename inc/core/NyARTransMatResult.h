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
#include "NyARDoubleMatrix34.h"
#include "NyAR_types.h"

namespace NyARToolkitCPP
{
	class NyARTransMatResult :public NyARDoubleMatrix34
	{
	/**
	 * エラーレート。この値はINyARTransMatの派生クラスが使います。
	 */
	public:
		double error;	
		bool has_value;
	public:
		NyARTransMatResult();
		void getZXYAngle(TNyARDoublePoint3d& o_out);
		void transformVertex(const TNyARDoublePoint3d& i_in,TNyARDoublePoint3d& o_out);
		void transformVertex(double i_x,double i_y,double i_z,TNyARDoublePoint3d& o_out);

	};
}
