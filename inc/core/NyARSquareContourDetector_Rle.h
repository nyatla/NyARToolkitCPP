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
#include "NyARRleLabelFragmentInfoStack.h"
#include "NyARCoord2SquareVertexIndexes.h"
#include "NyARLabelOverlapChecker.h"
#include "NyARLabeling_Rle.h"
#include "NyARSquareContourDetector.h"
#include "NyARPca2d_MatrixPCA_O2.h"
#include "../utils/NyArray.h"
#include "nyarcore.h"
#include <cstring>
namespace NyARToolkitCPP
{
	class NyARSquareContourDetector_Rle : public NyARSquareContourDetector
	{
	private:
		int _width;
		int _height;
		NyARLabeling_Rle* _labeling;
		NyARRleLabelFragmentInfoStack* _stack;
		/**/
		NyARContourPickup _cpickup;
		NyARCoord2SquareVertexIndexes _coord2vertex;
		NyARLabelOverlapChecker<NyARRleLabelFragmentInfo>* _overlap_checker;

	private:
		int _max_coord;
		int* _xcoord;
		int* _ycoord;
		/**
		* 最大i_squre_max個のマーカーを検出するクラスを作成する。
		* 
		* @param i_param
		*/
	public:
		NyARSquareContourDetector_Rle(const TNyARIntSize& i_size);
		virtual ~NyARSquareContourDetector_Rle();
	public:
		void detectMarkerCB(const NyARBinRaster& i_raster,NyARSquareContourDetector::IDetectMarkerCallback& i_callback);
	public:
		NyARRleLabelFragmentInfoStack* _getFragmentStack();

	};


}
