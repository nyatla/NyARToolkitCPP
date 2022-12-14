/* 
 * PROJECT: NyARToolkitCPP Alchemy bind
 * --------------------------------------------------------------------------------
 * The NyARToolkitCPP Alchemy bind is stub/proxy classes for NyARToolkitCPP and Adobe Alchemy.
 * 
 * Copyright (C)2009 Ryo Iizuka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For further information please contact.
 *	http://nyatla.jp/nyatoolkit/
 *	<airmail(at)ebony.plala.or.jp>
 * 
 */
#include <stdlib.h>
#include <stdio.h>

#include "AlchemyMaster.h"
#include "AS3.h"

#include "NyAR_types.h"
#include "NyStdLib.h"
using namespace NyARToolkitCPP;
#define AL_ENDIAN_LITTLE 1
#define AL_ENDIAN_BIG    2
#define AL_ENDIAN       AL_ENDIAN_LITTLE

#include "S_NyARIntSize.h"
#include "T_NyARDoubleMatrix34.h"
#include "S_NyARDoubleMatrix34.h"
#include "S_NyARPerspectiveProjectionMatrix.h"
#include "S_NyARParam.h"
#include "S_NyARCameraDistortionFactor.h"
#include "S_NyARCode.h"
#include "S_NyARRgbRaster_XRGB32.h"
#include "S_NyARRgbRaster_BGRA.h"
/*#include "S_NyARSingleDetectMarker.h"
#include "S_SingleNyIdMarkerProcessor.h"
#include "S_NyIdMarkerDataEncoder_RawBit.h"
*/
#include "S_NyARSquareStack.h"
#include "S_NyARSquare.h"
#include "S_NyARBinRaster.h"

#include "S_NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "S_NyARSquareDetector_Rle.h"
#include "S_NyARTransMat.h"
#include "S_NyARTransMatResult.h"
#include "S_NyARMatchPattDeviationColorData.h"
#include "S_NyARSquareStack.h"
#include "S_NyARColorPatt_Perspective_O2.h"
#include "S_NyARMatchPattResult.h"
#include "S_NyARRasterFilter_ARToolkitThreshold.h"
#include "S_NyARRasterThresholdAnalyzer_SlidePTile.h"
#include "S_NyIdMarkerPattern.h"
#include "S_NyIdMarkerParam.h"
#include "S_NyIdMarkerPickup.h"
#include "S_NyIdMarkerDataEncoder_RawBit.h"
#include "S_NyIdMarkerData_RawBit.h"
/********************************************************************************
 * 
 * Alchemy Entry point
 * --Alchemy???????????????????????????
 *
 ********************************************************************************/
static AS3_Val getVersion(void* self, AS3_Val args)
{
	return AS3_String("NyARToolkitCPP/2.4.0");
}


//entry point for code
int main()
{
	AS3_Val result = AS3_Object("");
	AS3ObjectBuilder builder(result);
	builder.addFunction("getVersion",getVersion);
	builder.addFunction("NyARParam_createInstance",S_NyARParam::createInstance);
	builder.addFunction("NyARDoubleMatrix34_createInstance",S_NyARDoubleMatrix34::createInstance);
	builder.addFunction("NyARPerspectiveProjectionMatrix_createInstance",S_NyARPerspectiveProjectionMatrix::createInstance);
	builder.addFunction("NyARCode_createInstance",S_NyARCode::createInstance);
	builder.addFunction("NyARRgbRaster_BGRA_createInstance",S_NyARRgbRaster_BGRA::createInstance);
/*	builder.addFunction("NyARSingleDetectMarker_createInstance",S_NyARSingleDetectMarker::createInstance);
*/	builder.addFunction("NyARTransMatResult_createInstance",S_NyARTransMatResult::createInstance);
	builder.addFunction("NyARTransMat_createInstance",S_NyARTransMat::createInstance);
	builder.addFunction("NyARRgbRaster_XRGB32_createInstance",S_NyARRgbRaster_XRGB32::createInstance);
	builder.addFunction("NyARIntSize_createInstance",S_NyARIntSize::createInstance);
/*	builder.addFunction("SingleNyIdMarkerProcesser_createInstance",S_SingleNyIdMarkerProcesser::createInstance);
*/	builder.addFunction("NyARSquareStack_createInstance",S_NyARSquareStack::createInstance);
	builder.addFunction("NyARSquareDetector_Rle_createInstance",S_NyARSquareDetector_Rle::createInstance);
	builder.addFunction("NyARBinRaster_createInstance",S_NyARBinRaster::createInstance);
	builder.addFunction("NyARMatchPatt_Color_WITHOUT_PCA_createInstance",S_NyARMatchPatt_Color_WITHOUT_PCA::createInstance);
	builder.addFunction("NyARMatchPattDeviationColorData_createInstance",S_NyARMatchPattDeviationColorData::createInstance);
	builder.addFunction("NyARColorPatt_Perspective_O2_createInstance",S_NyARColorPatt_Perspective_O2::createInstance);
	builder.addFunction("NyARMatchPattResult_createInstance",S_NyARMatchPattResult::createInstance);
	builder.addFunction("NyARRasterFilter_ARToolkitThreshold_createInstance",S_NyARRasterFilter_ARToolkitThreshold::createInstance);

	builder.addFunction("NyIdMarkerPattern_createInstance",S_NyIdMarkerPattern::createInstance);
	builder.addFunction("NyIdMarkerParam_createInstance",S_NyIdMarkerParam::createInstance);
	builder.addFunction("NyIdMarkerPickup_createInstance",S_NyIdMarkerPickup::createInstance);
	builder.addFunction("NyIdMarkerDataEncoder_RawBit_createInstance",S_NyIdMarkerDataEncoder_RawBit::createInstance);
	builder.addFunction("NyIdMarkerData_RawBit_createInstance",S_NyIdMarkerData_RawBit::createInstance);


	builder.addFunction("NyARRasterThresholdAnalyzer_SlidePTile_createInstance",S_NyARRasterThresholdAnalyzer_SlidePTile::createInstance);

	// notify that we initialized -- THIS DOES NOT RETURN!
	AS3_LibInit( result );

	// should never get here!
	return 0;
}


