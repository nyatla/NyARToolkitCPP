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
#include "INyARRasterThresholdAnalyzer.h" 
#include "INyARRaster.h"
#include "NyARRasterAnalyzer_Histogram.h"
#include "NyARHistogramAnalyzer_SlidePTile.h"
#include "NyARHistogram.h"


namespace NyARToolkitCPP
{
	class NyARRasterThresholdAnalyzer_SlidePTile : public INyARRasterThresholdAnalyzer
	{
	protected:
		NyARRasterAnalyzer_Histogram* _raster_analyzer;
	private:
		NyARHistogramAnalyzer_SlidePTile* _sptile;
		NyARHistogram* _histogram;
	public:
		void setVerticalInterval(int i_step);
	public:
		NyARRasterThresholdAnalyzer_SlidePTile(int i_persentage,int i_raster_format,int i_vertical_interval);
		~NyARRasterThresholdAnalyzer_SlidePTile();
	protected:
		bool initInstance(int i_raster_format,int i_vertical_interval);
	public:
		int analyzeRaster(const INyARRaster& i_input);
	};
}

