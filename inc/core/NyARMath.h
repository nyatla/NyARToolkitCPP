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
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARMath
	{
		/**
		 * p2-p1ベクトルのsquare normを計算する。
		 * @param i_p1
		 * @param i_p2
		 * @return
		 */
	public:
		static double sqNorm(const TNyARDoublePoint2d& i_p1,const TNyARDoublePoint2d& i_p2);
		static double sqNorm(double i_p1x,double i_p1y,double i_p2x,double i_p2y);
		/**
		 * p2-p1ベクトルのsquare normを計算する。
		 * @param i_p1
		 * @param i_p2
		 * @return
		 */	
		static double sqNorm(const TNyARDoublePoint3d& i_p1,const TNyARDoublePoint3d& i_p2);
		/**
		 * 3乗根を求められないシステムで、３乗根を求めます。
		 * http://aoki2.si.gunma-u.ac.jp/JavaScript/src/3jisiki.html
		 * @param i_in
		 * @return
		 */
		static double cubeRoot(double i_in);
	};
}
