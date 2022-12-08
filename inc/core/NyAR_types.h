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
#include "cmath"
#include "string.h"

namespace NyARToolkitCPP
{
	/*	constant
		--定数
	*/
	const static double NyAR_PI=3.1415926535897932384626433832795;

	/*	basic type
		--基本型
	*/
	typedef unsigned char NyAR_BYTE_t;

	typedef long long NyAR_INT64_t;



	/*	complex type
		--複合型
	*/
	typedef struct TNyARDoublePoint2d_t TNyARDoublePoint2d;
	typedef struct TNyARDoublePoint3d_t TNyARDoublePoint3d;
	typedef struct TNyARIntPoint2d_t TNyARIntPoint2d;
	typedef struct TNyARIntRect_t TNyARIntRect;
	typedef struct TNyARIntSize_t TNyARIntSize;


	struct TNyARIntPoint2d_t
	{
		int x;
		int y;
		inline void getValue(int (&o_value)[2])
		{
			o_value[0]=this->x;
			o_value[1]=this->y;
			return;
		}
	};


	struct TNyARDoublePoint2d_t{
		double x;
		double y;
		inline void setValue(const TNyARDoublePoint2d& i_src)
		{
			this->x=i_src.x;
			this->y=i_src.y;
			return;
		}
		inline void setValue(const TNyARIntPoint2d& i_src)
		{
			this->x=(double)i_src.x;
			this->y=(double)i_src.y;
			return;
		}
		inline double dist()
		{
			return sqrt(this->x*this->x+this->y+this->y);
		}
	};


	struct TNyARDoublePoint3d_t
	{
		double x;
		double y;
		double z;
		inline void setValue(const TNyARDoublePoint3d& i_in)
		{
			this->x=i_in.x;
			this->y=i_in.y;
			this->z=i_in.z;
			return;
		}
		/**
		 * i_pointとのベクトルから距離を計算します。
		 * @return
		 */
		inline double dist(const TNyARDoublePoint3d& i_point)
		{
			double x,y,z;
			x=this->x-i_point.x;
			y=this->y-i_point.y;
			z=this->z-i_point.z;
			return sqrt(x*x+y*y+z*z);
		}
	};




/*
	typedef struct TNyARIntPoint_t TNyARIntPoint;

	struct TNyARIntPoint_t
	{
		int x;
		int y;
	};
*/

	struct TNyARIntRect_t
	{
		int l;
		int t;
		int w;
		int h;
	};

	struct TNyARIntSize_t
	{
		int h;
		int w;
		inline bool isEqualSize(const TNyARIntSize& a)const
		{
			if (a.w == this->w && a.h == this->h){
				return true;
			}
			return false;
		}
		inline bool isEqualSize(const TNyARIntSize* a)const
		{
			if (a->w == this->w && a->h == this->h){
				return true;
			}
			return false;
		}
		inline bool isEqualSize(int i_width, int i_height)const
		{
			if (i_width == this->w && i_height == this->h){
				return true;
			}
			return false;
		}
	};

	/*	線分のパラメータ
	*/
	typedef struct TNyARLinear_t TNyARLinear;
	struct TNyARLinear_t
	{
		double dx;//x軸の増加量
		double dy;//y軸の増加量
		double c;//切片
		/**
		 * 2直線の交点を計算します。
		 * @param l_line_i
		 * @param l_line_2
		 * @param o_point
		 * @return
		 */
		static inline bool crossPos(const TNyARLinear& l_line_i,const TNyARLinear& l_line_2,TNyARDoublePoint2d& o_point)
		{
			double w1 = l_line_2.dy * l_line_i.dx - l_line_i.dy * l_line_2.dx;
			if (w1 == 0.0) {
				return false;
			}
			o_point.x = (l_line_2.dx * l_line_i.c - l_line_i.dx * l_line_2.c) / w1;
			o_point.y = (l_line_i.dy * l_line_2.c - l_line_2.dy * l_line_i.c) / w1;
			return true;
		}
	};


	/*	configulation type
		--コンフィグ型
	*/
	enum TNyAREndian{
		TNyAREndian_LITTLE,
		TNyAREndian_BIG
	};
}
