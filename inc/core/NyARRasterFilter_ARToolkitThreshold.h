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
#include "NyAR_types.h"
#include "INyARRasterFilter_Rgb2Bin.h"
namespace NyARToolkitCPP
{
	class IdoThFilterImpl;

	class NyARRasterFilter_ARToolkitThreshold:public INyARRasterFilter_Rgb2Bin
	{
	protected:
		int _threshold;
	private:
		IdoThFilterImpl* _do_threshold_impl;
	public:
		NyARRasterFilter_ARToolkitThreshold(int i_threshold,int i_in_raster_type);
		NyARRasterFilter_ARToolkitThreshold(int i_threshold,int i_in_raster_type,int i_out_raster_type);
		bool initInstance(int i_threshold,int i_in_raster_type,int i_out_raster_type);
		virtual ~NyARRasterFilter_ARToolkitThreshold();
		void setThreshold(int i_threshold);
		void doFilter(const INyARRgbRaster& i_input, NyARBinRaster& i_output)const;
	private:
		bool checkInputType(int i_input_type) const;
	};
}
