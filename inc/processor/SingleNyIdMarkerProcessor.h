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
#pragma
#include "NyARBaseClass.h"
#include "NyARTransMat.h"
#include "NyARSquareStack.h"
#include "NyARParam.h"
#include "NyARBinRaster.h"
#include "INyIdMarkerDataEncoder.h"
#include "NyARRasterFilter_ARToolkitThreshold.h"
#include "NyIdMarkerPickup.h"
#include "NyIdMarkerPattern.h"
#include "NyIdMarkerParam.h"
#include "NyARRasterThresholdAnalyzer_SlidePTile.h"
#include "NyARSquareContourDetector_Rle.h"
#include "INyARColorPatt.h"
#include "NyARMatchPattDeviationColorData.h"
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "NyARCoord2Linear.h"

namespace NyARToolkitCPP
{

	class DetectSquareCB;
	class SingleNyIdMarkerProcesser:public NyARBaseClass
	{

		/**
		* オーナーが自由に使えるタグ変数です。
		*/
	public:
		void *tag;

		/**
		* ロスト遅延の管理
		*/
	private:
		int _lost_delay_count;
		int _lost_delay;
	private:
		NyARSquareContourDetector_Rle* _square_detect;
	protected:
		INyARTransMat* _transmat;
	private:
		NyARRectOffset _offset; 
		bool _is_active;
		int _current_threshold;
		// [AR]検出結果の保存用
		NyARBinRaster* _bin_raster;
		NyARRasterFilter_ARToolkitThreshold* _tobin_filter;
		DetectSquareCB* _callback;
		INyIdMarkerData* _data_current;


	protected:
		SingleNyIdMarkerProcesser();
		~SingleNyIdMarkerProcesser();
	private:
		bool _initialized;
	protected:
		virtual void initInstance(const NyARParam* i_param,const INyIdMarkerDataEncoder* i_encoder,double i_marker_width,int i_raster_format);
	public:
		void setMarkerWidth(int i_width);
		void reset(bool i_is_force);
		void detectMarker(const INyARRgbRaster& i_raster);
	private:
		NyARRasterThresholdAnalyzer_SlidePTile* _threshold_detect;
		NyARTransMatResult __NyARSquare_result;

		/**オブジェクトのステータスを更新し、必要に応じてハンドル関数を駆動します。
		*/
		bool updateStatus(const NyARSquare& i_square,const INyIdMarkerData* i_marker_data);
	protected:
		//通知ハンドラ
		virtual void onEnterHandler(const INyIdMarkerData& i_code)=0;
		virtual void onLeaveHandler()=0;
		virtual void onUpdateHandler(const NyARSquare& i_square,const NyARTransMatResult& result)=0;
	};
}
