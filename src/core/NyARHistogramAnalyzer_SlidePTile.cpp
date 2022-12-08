/* 
* PROJECT: NyARToolkitCPP (Extension)
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
#include "NyARHistogramAnalyzer_SlidePTile.h"
#include "nyarcore.h"

namespace NyARToolkitCPP
{
	NyARHistogramAnalyzer_SlidePTile::NyARHistogramAnalyzer_SlidePTile(int i_persentage)
	{
		NyAR_ASSERT(0 <= i_persentage && i_persentage <= 50);
		//初期化
		this->_persentage=i_persentage;
	}
	int NyARHistogramAnalyzer_SlidePTile::getThreshold(const NyARHistogram& i_histogram)const
	{
		//総ピクセル数を計算
		int n=i_histogram.length;
		int sum_of_pixel=i_histogram.total_of_data;
		int* hist=i_histogram.data;
		// 閾値ピクセル数確定
		int th_pixcels = sum_of_pixel * this->_persentage / 100;
		int th_wk;
		int th_w, th_b;

		// 黒点基準
		th_wk = th_pixcels;
		for (th_b = 0; th_b < n-2; th_b++) {
			th_wk -= hist[th_b];
			if (th_wk <= 0) {
				break;
			}
		}
		// 白点基準
		th_wk = th_pixcels;
		for (th_w = n-1; th_w > 1; th_w--) {
			th_wk -= hist[th_w];
			if (th_wk <= 0) {
				break;
			}
		}
		// 閾値の保存
		return (th_w + th_b) / 2;
	}

}