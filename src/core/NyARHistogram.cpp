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

#include "NyARHistogram.h"
#include <assert.h>
#include "nyarcore.h"
namespace NyARToolkitCPP
{
	/**
	 * ヒストグラムを格納するクラスです。
	 */
	NyARHistogram::NyARHistogram(int i_length)
	{
		this->data=new int[i_length];
		this->length=i_length;
		this->total_of_data=0;
	}
	NyARHistogram::~NyARHistogram()
	{
		NyAR_SAFE_ARRAY_DELETE(this->data);
	}
	int NyARHistogram::getTotal(int i_st,int i_ed)
	{
		NyAR_ASSERT(i_st<i_ed && i_ed<this->length);
		int result=0;
		int* s=this->data;
		for(int i=i_st;i<=i_ed;i++){
			result+=s[i];
		}
		return result;
	}
	/**
	 * 指定したi_pos未満サンプルを０にします。
	 * @param i_pos
	 */
	void NyARHistogram::lowCut(int i_pos)
	{
		int s=0;
		for(int i=0;i<i_pos;i++){
			s+=this->data[i];
			this->data[i]=0;
		}
		this->total_of_data-=s;
	}
	/**
	 * 指定したi_pos以上のサンプルを０にします。
	 * @param i_pos
	 */
	void NyARHistogram::highCut(int i_pos)
	{
		int s=0;
		for(int i=this->length-1;i>=i_pos;i--){
			s+=this->data[i];
			this->data[i]=0;
		}
		this->total_of_data-=s;
	}
	/**
	 * 最小の値が格納されているサンプル番号を返します。
	 */
	int NyARHistogram::getMinSample()
	{
		int* data=this->data;
		int ret=this->length-1;
		int min=data[ret];
		for(int i=this->length-2;i>=0;i--)
		{
			if(data[i]<min){
				min=data[i];
				ret=i;
			}
		}
		return ret;
	}
	/**
	 * サンプルの中で最小の値を返します。
	 * @return
	 */
	int NyARHistogram::getMinData()
	{
		return this->data[this->getMinSample()];
	}
	/**
	 * 平均値を計算します。
	 * @return
	 */
	int NyARHistogram::getAverage()
	{
		long sum=0;
		for(int i=this->length-1;i>=0;i--)
		{
			sum+=this->data[i]*i;
		}
		return (int)(sum/this->total_of_data);
	}

}
