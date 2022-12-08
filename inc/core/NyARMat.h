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
#include "NyARVec.h"
namespace NyARToolkitCPP
{


	class NyARMat:public NyARBaseClass
	{
	public:
		NyARMat(void);
		NyARMat(int i_row, int i_clm);
		virtual ~NyARMat(void);
	private:
		double* m;
		int m_length;
		int clm,row;
		NyARVec* wk_PCA_QRM_ev;// = new NyARVec(1);
		NyARMat* wk_work_matrixPCA;// = null;
		NyARMat* wk_PCA_PCA_u;// = null;
	private:

	public:
		void realloc(int i_row, int i_clm);
		int getClm()const;
		int getRow()const;
		void zeroClear();
		void copyFrom(const NyARMat& i_copy_from);
		double* getArray()const;
		void matrixMul(const NyARMat& a,const NyARMat& b);
		bool matrixSelfInv();
		void dump();
	};
}
