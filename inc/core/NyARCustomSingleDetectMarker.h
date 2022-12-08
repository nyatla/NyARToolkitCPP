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
#include "INyARRasterFilter_Rgb2Bin.h" 
#include "NyARCustomSingleDetectMarker.h"
#include "TNyARMatchPattResult.h"
#include "NyARSquareContourDetector.h"
#include "NyARCoord2Linear.h"
#include "INyARColorPatt.h"
#include "INyARTransMat.h"
#include "NyARParam.h"

#include "NyARCode.h"
namespace NyARToolkitCPP
{
	class DetectSquareCB;
	class NyARCustomSingleDetectMarker: public NyARBaseClass
	{
	private:
		bool _is_continue;
		NyARSquareContourDetector* _square_detect;
		INyARTransMat* _transmat;
		//画処理用
		NyARBinRaster* _bin_raster;
	protected:
		const INyARRasterFilter_Rgb2Bin* _tobin_filter;
	private:
		DetectSquareCB* _detect_cb;
		NyARRectOffset _offset; 

	protected:
		NyARCustomSingleDetectMarker();
		void initInstance(
			INyARColorPatt* i_patt_inst,
			NyARSquareContourDetector* i_sqdetect_inst,
			INyARTransMat* i_transmat_inst,
			const INyARRasterFilter_Rgb2Bin* i_filter,
			const NyARParam& i_ref_param,
			const NyARCode*	i_ref_code,
			double		i_marker_width);
	public:
		~NyARCustomSingleDetectMarker();
		/**
		 * i_imageにマーカー検出処理を実行し、結果を記録します。
		 * 
		 * @param i_raster
		 * マーカーを検出するイメージを指定します。イメージサイズは、カメラパラメータ
		 * と一致していなければなりません。
		 * @return マーカーが検出できたかを真偽値で返します。
		 * @throws NyARException
		 */
		bool detectMarkerLite(const INyARRgbRaster& i_raster);
		void getTransmationMatrix(NyARTransMatResult &o_result)const;

		/**
		* 画面上のマーカ頂点情報を配列へのリファレンスを返します。
		* 返されたオブジェクトはクラスに所有し続けられています。クラスのメンバ関数を実行すると内容が書き変わります。
		* 外部でデータをストックする場合は、getSquarePositionで複製して下さい。
		* @return
		*/
		const NyARSquare& refSquare()const;
		/**
		* 検出したマーカーの一致度を返します。
		* 
		* @return マーカーの一致度を返します。0～1までの値をとります。 一致度が低い場合には、誤認識の可能性が高くなります。
		* @throws NyARException
		*/
		double getConfidence()const;
		/**
		* getTransmationMatrixの計算モードを設定します。 初期値はTRUEです。
		* 
		* @param i_is_continue
		* TRUEなら、transMatCont互換の計算をします。 FALSEなら、transMat互換の計算をします。
		*/
		void setContinueMode(bool i_is_continue);
	};


}

