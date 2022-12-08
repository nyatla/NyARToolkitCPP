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

#include "nyarcore.h"
#include "NyARBinRaster.h"
#include "NyARSquareContourDetector.h"

namespace NyARToolkitCPP
{
	class NyARSquareContourDetector:public NyARBaseClass
	{
	public:
		class IDetectMarkerCallback:public NyARBaseClass
		{
		public:
			virtual void onSquareDetect(const NyARSquareContourDetector& i_sender,int i_coordx[],int i_coordy[],int i_coor_num,int i_vertex_index[])=0;
		};
		/**
		 *
		 * @param i_raster
		 * @param o_square_stack
		 * @throws NyARException
		 */
		virtual void detectMarkerCB(const NyARBinRaster& i_raster,IDetectMarkerCallback& i_callback)=0;
	};
}

