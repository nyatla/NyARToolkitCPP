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
#include "NyARObserv2IdealMap.h"
#include "NyARDoubleMatrix22.h"
#include "NyARPca2d_MatrixPCA_O2.h"
#include "NyARCameraDistortionFactor.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{

	class NyARCoord2Linear : NyARBaseClass
	{
	private:
		double* _xpos;
		double* _ypos;	
		INyARPca2d* _pca;
		NyARObserv2IdealMap* _dist_factor;
		NyARDoubleMatrix22 __getSquareLine_evec;
	public:
		NyARCoord2Linear(const TNyARIntSize& i_size,const NyARCameraDistortionFactor& i_distfactor_ref);
		~NyARCoord2Linear();
		/**
		* 輪郭点集合からay+bx+c=0の直線式を計算します。
		* @param i_st
		* @param i_ed
		* @param i_xcoord
		* @param i_ycoord
		* @param i_cood_num
		* @param o_line
		* @return
		* @throws NyARException
		*/
		bool coord2Line(int i_st,int i_ed,int i_xcoord[], int i_ycoord[],int i_cood_num,TNyARLinear& o_line);
	};
}




