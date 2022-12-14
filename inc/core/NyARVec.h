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
namespace NyARToolkitCPP
{
	class NyARVec:public NyARBaseClass
	{
	public:
		NyARVec(int i_clm);
		virtual ~NyARVec();
	private:
		int clm;
		double* v;
		int v_length;
	public:
		void realloc(int i_clm);
		int getClm();
		double* getArray();
		int arVecDisp();
/*
		double vecInnerproduct(NyARVec &y, int i_start);
		double vecHousehold(int i_start);
*/
		void setNewArray(const double i_array[], int i_clm);

	};
}
