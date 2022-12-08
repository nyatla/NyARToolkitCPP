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

#include "NyARLabelOverlapChecker.h"
#include "NyARLabeling_ARToolKit.h"
#include "NyARLabelingImage.h"
#include "NyARCoord2SquareVertexIndexes.h"
#include "NyARSquareContourDetector.h"
#include "NyARContourPickup.h"
#include "nyarcore.h"
#include <cstring>
namespace NyARToolkitCPP
{
	class NyARSquareContourDetector_ARToolKit : public NyARSquareContourDetector
	{
	private:
		int _width;
		int _height;
		NyARLabeling_ARToolKit* _labeling;
		NyARLabelingImage* _limage;

		NyARLabelOverlapChecker<NyARLabelingLabel>* _overlap_checker;
		NyARSquareContourDetector* _sqconvertor;
		NyARContourPickup* _cpickup;
		NyARCoord2SquareVertexIndexes* _coord2vertex;

		int _max_coord;
		int* _xcoord;
		int* _ycoord;
		/**
		* 最大i_squre_max個のマーカーを検出するクラスを作成する。
		* 
		* @param i_param
		*/
	public:
		NyARSquareContourDetector_ARToolKit(const TNyARIntSize& i_size);
		virtual ~NyARSquareContourDetector_ARToolKit();
	private:
		void detectMarker(const NyARBinRaster& i_raster,IDetectMarkerCallback& i_callback);
	};
}
