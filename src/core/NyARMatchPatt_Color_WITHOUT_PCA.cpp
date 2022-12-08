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
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "INyARColorPatt.h"
#include "INyARMatchPatt.h"
#include "../utils/NyArray.h"
#include "nyarcore.h"
#include <cmath>
namespace NyARToolkitCPP
{

	NyARMatchPatt_Color_WITHOUT_PCA::NyARMatchPatt_Color_WITHOUT_PCA(const NyARCode* i_code_ref)
	{
		const int w=i_code_ref->getWidth();
		const int h=i_code_ref->getHeight();
		//最適化定数の計算
		this->_rgbpixels=w*h*3;
		this->_optimize_for_mod=this->_rgbpixels-(this->_rgbpixels%16);
		this->setARCode(i_code_ref);
		return;
	}
	NyARMatchPatt_Color_WITHOUT_PCA::NyARMatchPatt_Color_WITHOUT_PCA(int i_width, int i_height)
	{
		//最適化定数の計算
		this->_rgbpixels=i_height*i_width*3;
		this->_optimize_for_mod=this->_rgbpixels-(this->_rgbpixels%16);		
		return;
	}
	/**
	* 比較対象のARCodeをセットします。
	* @throws NyARException
	*/
	void NyARMatchPatt_Color_WITHOUT_PCA::setARCode(const NyARCode* i_code_ref)
	{
		this->_code_patt=i_code_ref;
		return;
	}
	/**
	* 現在セットされているARコードとi_pattを比較します。
	*/
	bool NyARMatchPatt_Color_WITHOUT_PCA::evaluate(const NyARMatchPattDeviationColorData& i_patt,TNyARMatchPattResult& o_result)const
	{
		NyAR_ASSERT(this->_code_patt!=NULL);
		//
		const int* linput = i_patt.refData();
		int sum;
		double max = 0.0;
		int res = TNyARMatchPattResult::DIRECTION_UNKNOWN;
		const int for_mod=this->_optimize_for_mod;
		for (int j = 0; j < 4; j++) {
			//合計値初期化
			sum=0;
			const NyARMatchPattDeviationColorData &code_patt=*this->_code_patt->getColorData(j);
			const int* pat_j = code_patt.refData();
			//<全画素について、比較(FORの1/16展開)>
			int i;
			for(i=this->_rgbpixels-1;i>=for_mod;i--){
				sum += linput[i] * pat_j[i];
			}
			for (;i>=0;) {
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
				sum += linput[i] * pat_j[i];i--;
			}
			//<全画素について、比較(FORの1/16展開)/>
			const double sum2 = sum / code_patt.getPow();// sum2 = sum / patpow[k][j]/ datapow;
			if (sum2 > max) {
				max = sum2;
				res = j;
			}
		}
		o_result.direction = res;
		o_result.confidence= max/i_patt.getPow();
		return true;		
	}



}
