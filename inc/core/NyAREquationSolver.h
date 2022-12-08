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
#include "nyarcore.h"
#include <cmath>
namespace NyARToolkitCPP
{
	class NyAREquationSolver:public NyARBaseClass
	{
	public:

		//o_resultは要素数2以上であること。
		static int solve2Equation(double i_a, double i_b, double i_c,double* o_result);
		//o_resultは要素数2以上であること。
		static int solve2Equation(double i_b, double i_c,double* o_result);
		//
		static int solve2Equation(double i_b, double i_c,double* o_result,int i_result_st);
		/**
		 * ３次方程式 a*x^3+b*x^2+c*x+d=0の実根を求める。	 
		 * http://aoki2.si.gunma-u.ac.jp/JavaScript/src/3jisiki.html
		 * のコードを基にしてます。
		 * @param i_a
		 * X^3の係数
		 * @param i_b
		 * X^2の係数
		 * @param i_c
		 * X^1の係数
		 * @param i_d
		 * X^0の係数
		 * @param o_result
		 * 実根。double[3]を指定すること。
		 * @return
		 */
		static int solve3Equation(double i_a, double i_b, double i_c, double i_d,double* o_result);
		/**
		 * ３次方程式 x^3+b*x^2+c*x+d=0の実根を求める。
		 * だけを求める。
		 * http://aoki2.si.gunma-u.ac.jp/JavaScript/src/3jisiki.html
		 * のコードを基にしてます。
		 * @param i_b
		 * X^2の係数
		 * @param i_c
		 * X^1の係数
		 * @param i_d
		 * X^0の係数
		 * @param o_result
		 * 実根。double[1]以上を指定すること。
		 * @return
		 */
		static int solve3Equation(double i_b, double i_c, double i_d,double* o_result);
		/**
		 * ４次方程式の実根だけを求める。
		 * @param i_a
		 * X^3の係数
		 * @param i_b
		 * X^2の係数
		 * @param i_c
		 * X^1の係数
		 * @param i_d
		 * X^0の係数
		 * @param o_result
		 * 実根。double[3]を指定すること。
		 * @return
		 */
		static int solve4Equation(double i_a, double i_b, double i_c, double i_d,double i_e,double* o_result);
		/**
		 * 3乗根を求められないシステムで、３乗根を求めます。
		 * http://aoki2.si.gunma-u.ac.jp/JavaScript/src/3jisiki.html
		 * @param i_in
		 * @return
		 */
	private:
		static double cuberoot(double i_in);
		/**
		 * 3次方程式の実根を１個だけ求める。
		 * 4字方程式で使う。
		 * @param i_b
		 * @param i_c
		 * @param i_d
		 * @param o_result
		 * @return
		 */
		static double solve3Equation_1(double i_b, double i_c, double i_d);
	};
}
