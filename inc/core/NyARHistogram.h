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
#pragma once
#include "NyARBaseClass.h"

namespace NyARToolkitCPP
{
	/**
	 * ヒストグラムを格納するクラスです。
	 */
	class NyARHistogram:public NyARBaseClass
	{
	public:
		int* data;
		int length;
		int total_of_data;
	public:
		NyARHistogram(int i_length);
		~NyARHistogram();
		int getTotal(int i_st,int i_ed);
		/**
		 * 指定したi_pos未満サンプルを０にします。
		 * @param i_pos
		 */
		void lowCut(int i_pos);
		/**
		 * 指定したi_pos以上のサンプルを０にします。
		 * @param i_pos
		 */
		void highCut(int i_pos);
		/**
		 * 最小の値が格納されているサンプル番号を返します。
		 */
		int getMinSample();
		/**
		 * サンプルの中で最小の値を返します。
		 * @return
		 */
		int getMinData();
		/**
		 * 平均値を計算します。
		 * @return
		 */
		int getAverage();
	};
}
