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
#include "NyARCoord2Linear.h"
#include "NyARSquareContourDetector.h"
#include "NyARMatchPattDeviationColorData.h"
#include "NyARTransMat.h"
#include "NyARRectOffset.h"
#include "NyARRasterFilter_ARToolkitThreshold.h"
#include "NyARRasterThresholdAnalyzer_SlidePTile.h"
#include "NyARCode.h"
namespace NyARToolkitCPP
{
	class DetectSquareCB;
	/**
	* このクラスは、同時に１個のマーカを処理することのできる、アプリケーションプロセッサです。
	* マーカの出現・移動・消滅を、イベントで通知することができます。
	* クラスには複数のマーカを登録できます。一つのマーカが見つかると、プロセッサは継続して同じマーカを
	* １つだけ認識し続け、見失うまでの間は他のマーカを認識しません。
	* 
	* イベントは、 OnEnter→OnUpdate[n]→OnLeaveの順で発生します。
	* マーカが見つかるとまずOnEnterが１度発生して、何番のマーカが発見されたかがわかります。
	* 次にOnUpdateにより、現在の変換行列が連続して渡されます。最後にマーカを見失うと、OnLeave
	* イベントが発生します。
	* 
	*/
	class SingleARMarkerProcesser:public NyARBaseClass
	{
	public:
		/**オーナーが自由に使えるタグ変数です。
		*/
		void* tag;
	private:
		int _lost_delay_count;
		int _lost_delay;
		NyARSquareContourDetector* _square_detect;
	protected:
		NyARTransMat* _transmat;
	private:
		NyARRectOffset _offset; 
		int _threshold;
		NyARBinRaster* _bin_raster;
		NyARRasterFilter_ARToolkitThreshold* _tobin_filter;
	protected:
		int _current_arcode_index;
	private:
		NyARRasterThresholdAnalyzer_SlidePTile* _threshold_detect;
	private:
		bool _initialized;
		DetectSquareCB* _detectmarker_cb;
	protected:
		SingleARMarkerProcesser();
		~SingleARMarkerProcesser();
		virtual void initInstance(const NyARParam* i_param,int i_raster_type);
	public:
		/**検出するマーカコードの配列を指定します。 検出状態でこの関数を実行すると、
		 * オブジェクト状態に強制リセットがかかります。
		 */
		void setARCodeTable(NyARCode* const i_ref_code_table[],int i_number_of_code, int i_code_resolution, double i_marker_width);
		void reset(bool i_is_force);
		void detectMarker(const INyARRgbRaster& i_raster);
		void setConfidenceThreshold(double i_new_cf,double i_exist_cf);
	private:
		NyARTransMatResult __NyARSquare_result;
	/**	オブジェクトのステータスを更新し、必要に応じてハンドル関数を駆動します。
	 * 	戻り値は、「実際にマーカを発見する事ができたか」です。クラスの状態とは異なります。
	 */
	private:
		bool updateStatus(const NyARSquare& i_square, int i_code_index);
	protected:
		virtual void onEnterHandler(int i_code)=0;
		virtual void onLeaveHandler()=0;
		virtual void onUpdateHandler(const NyARSquare& i_square, const NyARTransMatResult& result)=0;
	};
}
