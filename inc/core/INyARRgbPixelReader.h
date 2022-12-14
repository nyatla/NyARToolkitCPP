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
#include "NyARBaseClass.h"
namespace NyARToolkitCPP
{
	class INyARRgbPixelReader:public NyARBaseClass
	{
	public:
		/**
		* 1ピクセルをint配列にして返します。
		* 
		* @param i_x
		* @param i_y
		* @param i_rgb
		*/
		virtual void getPixel(int i_x, int i_y, int* i_rgb) const=0;
		/**
		* 複数のピクセル値をint配列に返します。
		* 配列には、[R1][G1][B1][R2][G2][B2]の順でピクセル値が格納されます。
		* 
		* @param i_x
		* xのインデックス配列
		* @param i_y
		* yのインデックス配列
		*/
		virtual void getPixelSet(const int* i_x, const int* i_y, int i_num, int* i_intrgb)const =0;
	
		/**
		 * 1ピクセルを設定します。
		 * @param i_x
		 * @param i_y
		 * @param i_rgb
		 * @throws NyARException
		 */
		virtual void setPixel(int i_x, int i_y, const int* i_rgb) =0;
		/**
		 * 複数のピクセル値をint配列から設定します。
		 * @param i_x
		 * @param i_y
		 * @param i_num
		 * @param i_intrgb
		 * @throws NyARException
		 */
		virtual void setPixels(const int* i_x, const int* i_y, int i_num, const int* i_intrgb) =0;
		virtual void switchBuffer(void* i_ref_buffer)=0;

	};
}
