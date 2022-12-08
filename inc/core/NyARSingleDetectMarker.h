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
#include "NyARCustomSingleDetectMarker.h"
#include "NyARRasterFilter_ARToolkitThreshold.h"
namespace NyARToolkitCPP
{
	class NyARSingleDetectMarker:public NyARCustomSingleDetectMarker
	{
	public:
		const static int PF_ARTOOLKIT_COMPATIBLE=1;
		const static int PF_NYARTOOLKIT=2;
		const static int PF_NYARTOOLKIT_ARTOOLKIT_FITTING=100;
		const static int PF_TEST2=201;
	private:
		NyARRasterFilter_ARToolkitThreshold* _threshold;
		void initInstance(
			const NyARParam* i_ref_param,
			const NyARCode* i_ref_code,
			double i_marker_width,
			int i_input_raster_type,
			int i_profile_id);
	public:
		NyARSingleDetectMarker(const NyARParam* i_param,const NyARCode* i_code, double i_marker_width,int i_input_raster_type,int i_profile_id=PF_NYARTOOLKIT);
	public:
		virtual ~NyARSingleDetectMarker();
		bool detectMarkerLite(const INyARRgbRaster &i_raster,int i_threshold);
	};
}
