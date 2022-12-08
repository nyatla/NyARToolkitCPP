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
#include "INyARRgbRaster.h"
#include "NyARSquare.h"
#include "NyARColorPatt_Perspective.h"

namespace NyARToolkitCPP
{
	class IpickFromRaster_Impl;
	/**
	* 遠近法を使ったパースペクティブ補正をかけて、ラスタ上の四角形から
	* 任意解像度の矩形パターンを作成します。
	*
	*/
	class NyARColorPatt_Perspective_O2 :public NyARColorPatt_Perspective
	{
	private:
		IpickFromRaster_Impl* _pickup;
	public:
		NyARColorPatt_Perspective_O2(int i_width, int i_height,int i_resolution,int i_edge_percentage);
		virtual ~NyARColorPatt_Perspective_O2();
	public:
		bool pickFromRaster(const INyARRgbRaster& image,const TNyARIntPoint2d i_vertexs[]);
	};
}

