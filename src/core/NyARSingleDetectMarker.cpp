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
#include "NyARSingleDetectMarker.h"
#include "NyARParam.h"
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "NyARSquareContourDetector.h"
#include "INyARTransMat.h"
#include "NyARTransMat.h"
#include "NyARRasterFilter_ARToolkitThreshold.h"
#include "NyARTransMatResult.h"
#include "NyARColorPatt_O3.h"
#include "NyARSquareContourDetector_ARToolKit.h"
#include "NyARSquareContourDetector_Rle.h"
#include "NyARColorPatt_Perspective_O2.h"
#include "NyARSquareStack.h"
#include "NyAR_types.h"
#define AR_SQUARE_MAX 100
namespace NyARToolkitCPP
{
	NyARSingleDetectMarker::NyARSingleDetectMarker(const NyARParam* i_param,const NyARCode* i_code, double i_marker_width,int i_input_raster_type,int i_profile_id)
	{
		initInstance(i_param,i_code,i_marker_width,i_input_raster_type,i_profile_id);
		return;
	}

	NyARSingleDetectMarker::~NyARSingleDetectMarker()
	{
		NyAR_SAFE_DELETE(this->_threshold);
		return;
	}
	void NyARSingleDetectMarker::initInstance(
		const NyARParam* i_ref_param,
		const NyARCode* i_ref_code,
		double i_marker_width,
		int i_input_raster_type,
		int i_profile_id)
	{
		//この3インスタンスは、Custom側で開放する。
		INyARColorPatt* patt_inst;
		NyARSquareContourDetector* sqdetect_inst;
		INyARTransMat* transmat_inst;

		this->_threshold=new NyARRasterFilter_ARToolkitThreshold(100,i_input_raster_type);
		switch(i_profile_id){
		case PF_NYARTOOLKIT_ARTOOLKIT_FITTING:
		case PF_ARTOOLKIT_COMPATIBLE:
			//not supprted!
			throw NyARException();
			break;
		case PF_NYARTOOLKIT:
			patt_inst=new NyARColorPatt_Perspective_O2(i_ref_code->getWidth(), i_ref_code->getHeight(),4,25);
			sqdetect_inst=new NyARSquareContourDetector_Rle(i_ref_param->getScreenSize());			
			transmat_inst=new NyARTransMat(i_ref_param);
			break;
		default:
			throw NyARException();
		}
		NyARCustomSingleDetectMarker::initInstance(patt_inst,sqdetect_inst,transmat_inst,this->_threshold,*i_ref_param,i_ref_code,i_marker_width);
		return;
	}

	bool NyARSingleDetectMarker::detectMarkerLite(const INyARRgbRaster &i_raster,int i_threshold)
	{
		((NyARRasterFilter_ARToolkitThreshold*)this->_tobin_filter)->setThreshold(i_threshold);
		return NyARCustomSingleDetectMarker::detectMarkerLite(i_raster);
	}
}
