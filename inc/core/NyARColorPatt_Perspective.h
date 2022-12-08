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
#include "INyARColorPatt.h"
#include "NyAR_types.h"
#include "NyARPerspectiveParamGenerator_O1.h"
#include "NyARRgbPixelReader_INT1D_X8R8G8B8_32.h"
#include "NyARColorPatt_Perspective.h"
#include "NyARBufferType.h"


namespace NyARToolkitCPP
{
	/**
	* 遠近法を使ったパースペクティブ補正をかけて、ラスタ上の四角形から
	* 任意解像度の矩形パターンを作成します。
	*
	*/


	class NyARColorPatt_Perspective :public INyARColorPatt
	{
	protected:
		int* _patdata;
		TNyARIntPoint2d _pickup_lt;
		int _resolution;
		TNyARIntSize _size;
		NyARPerspectiveParamGenerator_O1* _perspective_gen;
	private:
		const static int BUFFER_FORMAT=NyARBufferType::INT1D_X8R8G8B8_32;
		NyARRgbPixelReader_INT1D_X8R8G8B8_32* _pixelreader;

	private:
		void initializeInstance(int i_width, int i_height,int i_point_per_pix);
		/**
		* 例えば、64
		* @param i_width
		* 取得画像の解像度幅
		* @param i_height
		* 取得画像の解像度高さ
		* @param i_point_per_pix
		* 1ピクセルあたりの縦横サンプリング数。2なら2x2=4ポイントをサンプリングする。
		*/
	public:
		NyARColorPatt_Perspective(int i_width, int i_height,int i_point_per_pix);
		~NyARColorPatt_Perspective();
		/**
		* 例えば、64
		* @param i_width
		* 取得画像の解像度幅
		* @param i_height
		* 取得画像の解像度高さ
		* @param i_edge_percentage
		* エッジ幅の割合(ARToolKit標準と同じなら、25)
		*/
		NyARColorPatt_Perspective(int i_width, int i_height,int i_resolution,int i_edge_percentage);
		/**
		* 矩形領域のエッジサイズを指定します。
		* エッジの計算方法は以下の通りです。
		* 1.マーカ全体を(i_x_edge*2+width)x(i_y_edge*2+height)の解像度でパラメタを計算します。
		* 2.ピクセルの取得開始位置を(i_x_edge/2,i_y_edge/2)へ移動します。
		* 3.開始位置から、width x height個のピクセルを取得します。
		* 
		* ARToolKit標準マーカの場合は、width/2,height/2を指定してください。
		* @param i_x_edge
		* @param i_y_edge
		*/
		void setEdgeSize(int i_x_edge,int i_y_edge,int i_resolution);
		void setEdgeSizeByPercent(int i_x_percent,int i_y_percent,int i_resolution);
		int getWidth()const;
		int getHeight()const;
		const TNyARIntSize& getSize()const;
		INyARRgbPixelReader& getRgbPixelReader()const;


		bool pickFromRaster(const INyARRgbRaster& image,const TNyARIntPoint2d i_vertexs[]);

		void* getBuffer()const;
		int getBufferType()const;
		bool isEqualBufferType(int i_type_value)const;
		bool hasBuffer()const;
		void wrapBuffer(void* i_ref_buf);





	};
}

