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
#include "NyARDoubleMatrix22.h"
#include "INyARPca2d.h"
#include "NyARLabelingImage.h"
#include "NyARBinRaster.h"

namespace NyARToolkitCPP
{
	class NyARContourPickup:public NyARBaseClass
	{
	public:
		int getContour(const NyARBinRaster& i_raster,int i_entry_x,int i_entry_y,int i_array_size,int* o_coord_x,int* o_coord_y)const;
		int getContour(const NyARLabelingImage& i_raster,int i_entry_x,int i_entry_y,int i_array_size,int* o_coord_x,int* o_coord_y)const;

	};
}

