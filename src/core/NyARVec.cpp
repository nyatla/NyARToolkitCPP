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
#include "NyARVec.h"
#include "nyarcore.h"
#include <cstring>
namespace NyARToolkitCPP
{
	NyARVec::NyARVec(int i_clm)
	{
		this->v=new double[i_clm];
		this->v_length=i_clm;
		this->clm=i_clm;
	}
	NyARVec::~NyARVec()
	{
		NyAR_SAFE_ARRAY_DELETE(this->v);
	}
	void NyARVec::realloc(int i_clm)
	{
		if (i_clm <= this->v_length) {
			// 十分な配列があれば何もしない。
		} else {
			// 不十分なら取り直す。
			NyAR_SAFE_ARRAY_DELETE(this->v);
			this->v = new double[i_clm];
		}
		this->clm = i_clm;
		return;
	}
	int NyARVec::getClm()
	{
		return this->clm;
	}
	double* NyARVec::getArray()
	{
		return this->v;
	}
	void NyARVec::setNewArray(const double i_array[], int i_clm)
	{
		this->realloc(i_clm);
		memcpy(this->v,i_array,sizeof(double)*i_clm);
		this->clm = i_clm;
		return;
	}
}
