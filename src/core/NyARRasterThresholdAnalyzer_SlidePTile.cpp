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
#include "NyARRasterThresholdAnalyzer_SlidePTile.h"
#include "NyAR_types.h"
#include "NyARException.h"
#include "nyarcore.h"
namespace NyARToolkitCPP
{
	void NyARRasterThresholdAnalyzer_SlidePTile::setVerticalInterval(int i_step)
	{
		this->_raster_analyzer->setVerticalInterval(i_step);
		return;
	}
	NyARRasterThresholdAnalyzer_SlidePTile::NyARRasterThresholdAnalyzer_SlidePTile(int i_persentage,int i_raster_format,int i_vertical_interval)
	{
		NyAR_ASSERT(0 <= i_persentage && i_persentage <= 50);
		//‰Šú‰»
		if(!initInstance(i_raster_format,i_vertical_interval)){
			throw NyARException();
		}
		this->_sptile=new NyARHistogramAnalyzer_SlidePTile(i_persentage);
		this->_histogram=new NyARHistogram(256);
	}
	NyARRasterThresholdAnalyzer_SlidePTile::~NyARRasterThresholdAnalyzer_SlidePTile()
	{
		NyAR_SAFE_DELETE(this->_sptile);
		NyAR_SAFE_DELETE(this->_histogram);
		NyAR_SAFE_DELETE(this->_raster_analyzer);
	}

	bool NyARRasterThresholdAnalyzer_SlidePTile::initInstance(int i_raster_format,int i_vertical_interval)
	{
		this->_raster_analyzer=new NyARRasterAnalyzer_Histogram(i_raster_format,i_vertical_interval);
		return true;
	}
	int NyARRasterThresholdAnalyzer_SlidePTile::analyzeRaster(const INyARRaster& i_input)
	{
		this->_raster_analyzer->analyzeRaster(i_input, *this->_histogram);
		return this->_sptile->getThreshold(*this->_histogram);
	}


}

