/* 
* PROJECT: NyARToolkitCPP (Extension)
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
#include "NyARBaseClass.h"
#include "NyARBufferType.h"
#include "NyARHistogram.h"
#include "INyARRaster.h"
namespace NyARToolkitCPP
{
	/**
	* 画像のヒストグラムを計算します。
	* RGBの場合、(R+G+B)/3のヒストグラムを計算します。
	* 
	* 
	*/
	class ICreateHistogramImpl;
	class NyARRasterAnalyzer_Histogram : public NyARBaseClass
	{
	protected:
		ICreateHistogramImpl* _histImpl;
		/**
		* ヒストグラム解析の縦方向スキップ数。継承クラスはこのライン数づつ
		* スキップしながらヒストグラム計算を行うこと。
		*/
		int _vertical_skip;


	public:
		NyARRasterAnalyzer_Histogram(int i_raster_format,int i_vertical_interval);
		~NyARRasterAnalyzer_Histogram();
	protected:
		bool initInstance(int i_raster_format,int i_vertical_interval);
	public:
		void setVerticalInterval(int i_step);
		int analyzeRaster(const INyARRaster& i_input,NyARHistogram& o_histogram);
	};
}

