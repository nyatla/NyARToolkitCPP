/* 
 * PROJECT: NyARToolkitCPP
 * --------------------------------------------------------------------------------
 *
 * The NyARToolkitCS is C++ version NyARToolkit class library.
 * 
 * Copyright (C)2008 R.Iizuka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For further information please contact.
 *	http://nyatla.jp/nyatoolkit/
 *	<airmail(at)ebony.plala.or.jp>
 * 
 */
#pragma once

#include <cmath>
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyMath
	{
	public:
		const static NyAR_INT64_t FIXEDFLOAT24_1=0x1000000L;
		const static NyAR_INT64_t FIXEDFLOAT24_0_25=FIXEDFLOAT24_1/4;
		const static NyAR_INT64_t FIXEDFLOAT16_1=0x10000L;	
		const static NyAR_INT64_t FIXEDFLOAT16_0_25=FIXEDFLOAT16_1/4;	
		const static NyAR_INT64_t FIXEDFLOAT8_1=0x100L;
	private:
		const static int FIXEDFLOAT16I_1=(int)FIXEDFLOAT16_1;	
		const static int FIXEDFLOAT16I_0_25=(int)FIXEDFLOAT16_1/4;

		const static int FF16_PI=(int)(3.1415926535897932384626433832795*FIXEDFLOAT16_1);
		const static int FF16_2PI=(int)(2 *FF16_PI);
		const static int FF16_05PI=(int)(FF16_PI/2);
		/* sinテーブルは0-2PIを1024分割
		* acosテーブルは0-1を256分割
		*/
		static int sin_table[339];
		static int acos_table [1537];
		const static int SQRT_LOOP=10;
		/**
		* http://www.geocities.co.jp/SiliconValley-PaloAlto/5438/
		* 参考にしました。
		* 少数点部が16bitの変数の平方根を求めます。
		* 戻り値の小数点部分は16bitです。
		* @param i_v
		* @return
		*/
	public:
		static NyAR_INT64_t sqrtFixdFloat16(NyAR_INT64_t i_ff16);
		static NyAR_INT64_t sqrtFixdFloat(NyAR_INT64_t i_ff,int i_bit);
		static int acosFixedFloat16(int i_ff24);
		static int sinFixedFloat24(int i_ff16);
		static int cosFixedFloat24(int i_ff16);
		static void initialize();
	};
}
