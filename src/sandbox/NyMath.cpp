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
#include "NyMath.h"
#include <cmath>
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	NyAR_INT64_t NyMath::sqrtFixdFloat16(NyAR_INT64_t i_ff16)
	{
		NyAR_INT64_t t=0,s;
		s=i_ff16>0?i_ff16:-i_ff16;
		if(i_ff16==0){
			return 0;
		}
		for(int i=SQRT_LOOP;i>0;i--){
			t = s;
			s = (t+((i_ff16<<16)/t))>>1;
			if(s==t){
				break;
			}
		};
		return t;
	}
	NyAR_INT64_t NyMath::sqrtFixdFloat(NyAR_INT64_t i_ff,int i_bit)
	{
		NyAR_INT64_t t=0,s;
		s=i_ff>0?i_ff:-i_ff;
		if(i_ff==0){
			return 0;
		}
		for(int i=SQRT_LOOP;i>0;i--){
			t = s;
			s = (t+((i_ff<<i_bit)/t))>>1;
			if(s==t){
				break;
			}
		}
		return t;
	}
	int NyMath::acosFixedFloat16(int i_ff24)
	{/*	
	 long x=i_ff24>>8;
	 long x2=(x*x)>>16;
	 long x3=(x2*x)>>16;
	 long x4=(x2*x2)>>16;
	 //		return FF16_05PI-(int)(x+x3/6+(((3*x3*x2/(2*4*5)+(3*5*x4*x3)/(2*4*6*7)))>>16));
	 */		
		int result;
		int abs_ff24=i_ff24>0?i_ff24:-i_ff24;
		//(0<=n<=0.25) 0<=0-16384(65536/4)まではy=PI/2-xので近似
		//(0.25<n<=1/2PI) 16385-65536までは(128ステップ単位)のテーブルを使用				

		if(abs_ff24<FIXEDFLOAT24_0_25){
			//0.25までの範囲は、2次の近似式
			result=(i_ff24>>8);
			return FF16_05PI-result+((((result*result)>>16)*result)>>16)/6;
		}else{
			result=acos_table[((abs_ff24>>8)-FIXEDFLOAT16I_0_25)>>5];
			if (i_ff24 < 0) {
				return FF16_PI-result;
			}else{
				return result;
			}
		}
		//		return (int)(Math.acos((double)i_ff24/0x1000000)*0x10000);
	}

	int NyMath::sinFixedFloat24(int i_ff16)
	{
		int result;
		//i_ff16を0-2πに制限
		int rad=i_ff16%FF16_2PI;
		if(rad<0){
			rad=rad+FF16_2PI;
		}
		//4ブロックに分割
		int dv=rad/FF16_05PI;
		//radを0-0.5PIに制限
		rad=rad-dv*FF16_05PI;
		//radをdvにより補正
		if(dv==1 || dv==3){
			rad=FF16_05PI-rad;
		}
		//(0<=n<=0.25) 0<=0-16384(65536/4)まではy=xので近似
		//(0.25<n<=1/2PI) 16385-102944までは(256ステップ単位)のテーブルを使用				
		//負にする
		if(rad<FIXEDFLOAT16_0_25){
			result=rad<<8;
		}else{
			result=sin_table[(rad-FIXEDFLOAT16I_0_25)>>8];
		}
		if(dv>=2){
			result=-result;
		}
		return result;
		//		return (int)(Math.sin((double)i_ff16/0x10000)*0x1000000);
	}
	int NyMath::cosFixedFloat24(int i_ff16)
	{
		int result;
		//i_ff16を0-2πに制限
		int rad=(i_ff16+FF16_05PI)%FF16_2PI;
		if(rad<0){
			rad=rad+FF16_2PI;
		}
		//4ブロックに分割
		int dv=rad/FF16_05PI;
		//radを0-0.5PIに制限
		rad=rad-dv*FF16_05PI;
		//radをdvにより補正
		if(dv==1 || dv==3){
			rad=FF16_05PI-rad;
		}
		//(0<=n<=0.25) 0<=0-16384(65536/4)まではy=xので近似
		//(0.25<n<=1/2PI) 16385-102944までは(256ステップ単位)のテーブルを使用				
		//負にする
		if(rad<FIXEDFLOAT16_0_25){
			result=rad<<8;
		}else{
			result=sin_table[(rad-FIXEDFLOAT16I_0_25)>>8];
		}
		if(dv>=2){
			result=-result;
		}
		return result;
		//		return (int)(Math.cos((double)i_ff16/0x10000)*0x1000000);
	}
	void NyMath::initialize()
	{

		int step;
		step=FIXEDFLOAT16I_0_25+256;
		for(int i=0;i<339;i++){
			sin_table[i]=(int)((sin((double) step / (double) FIXEDFLOAT16I_1))*FIXEDFLOAT24_1);
			step+=256;
		}
		//acosテーブル初期化
		step=FIXEDFLOAT16I_0_25+32;
		for (int i = 0; i < 1537; i++) {
			acos_table[i] =(int)((acos((double) step/(double) FIXEDFLOAT16I_1))*FIXEDFLOAT16_1);
			step+=32;
		}
		return;
	}

}