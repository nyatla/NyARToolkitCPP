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
#include "INyARPca2d.h"
namespace NyARToolkitCPP
{
	class NyARPca2d_MatrixPCA_O2 :public INyARPca2d
	{
	private:
		 static void PCA_QRM(NyARDoubleMatrix22& o_matrix, double dv[]);
		 void PCA_PCA(const double* i_x,const double* i_y,int i_number_of_data,NyARDoubleMatrix22& o_matrix,double o_ev[],double o_mean[])const;
	public:
		void pca(const double i_v1[],const double i_v2[],int i_number_of_point,NyARDoubleMatrix22& o_evec, double o_ev[],double o_mean[])const;

	};
}
