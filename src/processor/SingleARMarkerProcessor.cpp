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
#include "SingleARMarkerProcessor.h"
#include "nyarcore.h"
#include <float.h>
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "NyARSquareContourDetector_Rle.h"
#include "NyARColorPatt_Perspective_O2.h"
namespace NyARToolkitCPP
{
	/**
	* detectMarkerのコールバック関数
	*/
	class DetectSquareCB : public NyARSquareContourDetector::IDetectMarkerCallback
	{
		//公開プロパティ
	public:
		NyARSquare square;
		double confidence;
		int code_index;
		double cf_threshold_new;
		double cf_threshold_exist;

		//参照
	private:
		const INyARRgbRaster* _ref_raster;
		//所有インスタンス
		INyARColorPatt* _inst_patt;
		NyARMatchPattDeviationColorData* _deviation_data;
		NyArray<NyARMatchPatt_Color_WITHOUT_PCA*>* _match_patt;
		NyARCoord2Linear* _coordline;

	public:
		DetectSquareCB(const NyARParam& i_param)
		{
			this->_match_patt    =NULL;
			this->_deviation_data=NULL;
			this->_inst_patt     =NULL;
			this->_coordline=new NyARCoord2Linear(i_param.getScreenSize(),i_param.getDistortionFactor());
			return;
		}
		~DetectSquareCB()
		{
			freeARCodeTable();
			NyAR_SAFE_DELETE(this->_coordline);
			return;
		}
		void setARCodeTable(NyARCode* const i_ref_code_table[],int i_number_of_code, int i_code_resolution)
		{
			freeARCodeTable();
			//unmanagedで実装するときは、ここでリソース解放をすること。
			this->_deviation_data=new NyARMatchPattDeviationColorData(i_code_resolution,i_code_resolution);
			this->_inst_patt=new NyARColorPatt_Perspective_O2(i_code_resolution,i_code_resolution,4,25);
			this->_match_patt = new NyArray<NyARMatchPatt_Color_WITHOUT_PCA*>(i_number_of_code);
			for(int i=0;i<i_number_of_code;i++){
				this->_match_patt->item[i]=new NyARMatchPatt_Color_WITHOUT_PCA(i_ref_code_table[i]);
			}
		}
	private:
		void freeARCodeTable()
		{
			//割当済のオブジェクトを削除
			NyAR_SAFE_DELETE(this->_deviation_data);
			NyAR_SAFE_DELETE(this->_inst_patt);
			if(this->_match_patt!=NULL){
				for(int i=0;i<this->_match_patt->length;i++){
					NyAR_SAFE_DELETE(this->_match_patt->item[i]);
				}
				NyAR_SAFE_DELETE(this->_match_patt);
			}
			return;
		}
	private:
		int _target_id;
	public:
		void init(const INyARRgbRaster* i_raster,int i_target_id)
		{
			this->cf_threshold_new = 0.50;
			this->cf_threshold_exist = 0.30;

			this->_ref_raster=i_raster;
			this->_target_id=i_target_id;
			this->code_index=-1;
			this->confidence=DBL_MIN;
		}

		/**
		* 矩形が見付かるたびに呼び出されます。
		* 発見した矩形のパターンを検査して、方位を考慮した頂点データを確保します。
		*/
	public:
		void onSquareDetect(const NyARSquareContourDetector& i_sender,int i_coordx[],int i_coordy[],int i_coor_num,int i_vertex_index[])
		{
			if (this->_match_patt==NULL) {
				return;
			}
			//輪郭座標から頂点リストに変換
			TNyARIntPoint2d vertex[4];
			vertex[0].x=i_coordx[i_vertex_index[0]];
			vertex[0].y=i_coordy[i_vertex_index[0]];
			vertex[1].x=i_coordx[i_vertex_index[1]];
			vertex[1].y=i_coordy[i_vertex_index[1]];
			vertex[2].x=i_coordx[i_vertex_index[2]];
			vertex[2].y=i_coordy[i_vertex_index[2]];
			vertex[3].x=i_coordx[i_vertex_index[3]];
			vertex[3].y=i_coordy[i_vertex_index[3]];

			//画像を取得
			if (!this->_inst_patt->pickFromRaster(*this->_ref_raster,vertex)){
				return;//取得失敗
			}
			//取得パターンをカラー差分データに変換して評価する。
			this->_deviation_data->setRaster(*this->_inst_patt);


			//code_index,dir,c1にデータを得る。
			TNyARMatchPattResult mr;
			int lcode_index = 0;
			int dir = 0;
			double c1 = 0;
			for (int i = 0; i < this->_match_patt->length; i++) {
				this->_match_patt->item[i]->evaluate(*this->_deviation_data,mr);
				double c2 = mr.confidence;
				if (c1 < c2) {
					lcode_index = i;
					c1 = c2;
					dir = mr.direction;
				}
			}

			//認識処理
			if (this->_target_id == -1) { // マーカ未認識
				//現在は未認識
				if (c1 < this->cf_threshold_new) {
					return;
				}
				if (this->confidence > c1) {
					// 一致度が低い。
					return;
				}
				//認識しているマーカIDを保存
				this->code_index=lcode_index;
			}else{
				//現在はマーカ認識中				
				// 現在のマーカを認識したか？
				if (lcode_index != this->_target_id) {
					// 認識中のマーカではないので無視
					return;
				}
				//認識中の閾値より大きいか？
				if (c1 < this->cf_threshold_exist) {
					return;
				}
				//現在の候補よりも一致度は大きいか？
				if (this->confidence>c1) {
					return;
				}
				this->code_index=this->_target_id;
			}
			//新しく認識、または継続認識中に更新があったときだけ、Square情報を更新する。
			//ココから先はこの条件でしか実行されない。

			//一致率の高い矩形があれば、方位を考慮して頂点情報を作成
			this->confidence=c1;
			NyARSquare& sq=this->square;
			//directionを考慮して、squareを更新する。
			for(int i=0;i<4;i++){
				int idx=(i+4 - dir) % 4;
				this->_coordline->coord2Line(i_vertex_index[idx],i_vertex_index[(idx+1)%4],i_coordx,i_coordy,i_coor_num,sq.line[i]);
			}
			for (int i = 0; i < 4; i++) {
				//直線同士の交点計算
				if(!TNyARLinear::crossPos(sq.line[i],sq.line[(i + 3) % 4],sq.sqvertex[i])){
					throw NyARException();//ここのエラー復帰するならダブルバッファにすればOK
				}
			}
		}
	};


	SingleARMarkerProcesser::SingleARMarkerProcesser()
	{
		this->_square_detect   =NULL;
		this->_transmat        =NULL;
		this->_tobin_filter    =NULL;
		this->_bin_raster      =NULL;
		this->_threshold_detect=NULL;
		//コールバックハンドラ
		this->_detectmarker_cb =NULL;
		this->_initialized=false;
	}
	SingleARMarkerProcesser::~SingleARMarkerProcesser()
	{
		NyAR_SAFE_DELETE(this->_square_detect);
		NyAR_SAFE_DELETE(this->_transmat);
		NyAR_SAFE_DELETE(this->_tobin_filter);

		// ２値画像バッファを作る
		NyAR_SAFE_DELETE(this->_bin_raster);
		NyAR_SAFE_DELETE(this->_threshold_detect);
		//コールバックハンドラ
		NyAR_SAFE_DELETE(this->_detectmarker_cb);
		return;

	}
	void SingleARMarkerProcesser::initInstance(const NyARParam* i_param,int i_raster_type)
	{
		//初期化済？
		NyAR_ASSERT(this->_initialized==false);

		this->_lost_delay_count=0;
		this->_lost_delay=5;
		this->_threshold=110;
		this->_current_arcode_index=-1;

		const TNyARIntSize& scr_size = i_param->getScreenSize();
		// 解析オブジェクトを作る
		this->_square_detect = new NyARSquareContourDetector_Rle(scr_size);
		this->_transmat = new NyARTransMat(i_param);
		this->_tobin_filter=new NyARRasterFilter_ARToolkitThreshold(110,i_raster_type);

		// ２値画像バッファを作る
		this->_bin_raster = new NyARBinRaster(scr_size.w, scr_size.h);
		this->_threshold_detect=new NyARRasterThresholdAnalyzer_SlidePTile(15,i_raster_type,4);
		this->_initialized=true;
		//コールバックハンドラ
		this->_detectmarker_cb=new DetectSquareCB(*i_param);
		return;
	}
	/**検出するマーカコードの配列を指定します。 検出状態でこの関数を実行すると、
	* オブジェクト状態に強制リセットがかかります。
	*/
	void SingleARMarkerProcesser::setARCodeTable(NyARCode* const i_ref_code_table[],int i_number_of_code, int i_code_resolution, double i_marker_width)
	{
		if (this->_current_arcode_index != -1) {
			// 強制リセット
			reset(true);
		}
		//検出するマーカセット、情報、検出器を作り直す。(1ピクセル4ポイントサンプリング,マーカのパターン領域は50%)
		this->_detectmarker_cb->setARCodeTable(i_ref_code_table,i_number_of_code,i_code_resolution);
		this->_offset.setSquare(i_marker_width);
		return;
	}

	void SingleARMarkerProcesser::reset(bool i_is_force)
	{
		if (this->_current_arcode_index != -1 && i_is_force == false) {
			// 強制書き換えでなければイベントコール
			this->onLeaveHandler();
		}
		// カレントマーカをリセット
		this->_current_arcode_index = -1;
		return;
	}
	void SingleARMarkerProcesser::detectMarker(const INyARRgbRaster& i_raster)
	{
		// サイズチェック
		NyAR_ASSERT(this->_bin_raster->getSize().isEqualSize(i_raster.getSize().w, i_raster.getSize().h));

		//BINイメージへの変換
		this->_tobin_filter->setThreshold(this->_threshold);
		this->_tobin_filter->doFilter(i_raster,*this->_bin_raster);

		// スクエアコードを探す(ここでi_rasterをポインタ化してるけど、この関数内で参照が閉じてるからOKとする。)
		this->_detectmarker_cb->init(&i_raster,this->_current_arcode_index);
		this->_square_detect->detectMarkerCB(*this->_bin_raster,*this->_detectmarker_cb);

		// 認識状態を更新
		bool is_id_found=this->updateStatus(this->_detectmarker_cb->square,this->_detectmarker_cb->code_index);
		//閾値フィードバック(detectExistMarkerにもあるよ)
		if(!is_id_found){
			//マーカがなければ、探索+DualPTailで基準輝度検索
			int th=this->_threshold_detect->analyzeRaster(i_raster);
			this->_threshold=(this->_threshold+th)/2;
		}
		return;
	}
	/**
	* 
	* @param i_new_detect_cf
	* @param i_exist_detect_cf
	*/
	void SingleARMarkerProcesser::setConfidenceThreshold(double i_new_cf,double i_exist_cf)
	{
		this->_detectmarker_cb->cf_threshold_exist=i_exist_cf;
		this->_detectmarker_cb->cf_threshold_new=i_new_cf;
	}
	/**	オブジェクトのステータスを更新し、必要に応じてハンドル関数を駆動します。
	* 	戻り値は、「実際にマーカを発見する事ができたか」です。クラスの状態とは異なります。
	*/
	bool SingleARMarkerProcesser::updateStatus(const NyARSquare& i_square, int i_code_index)
	{
		NyARTransMatResult& result=this->__NyARSquare_result;
		if (this->_current_arcode_index < 0) {// 未認識中
			if (i_code_index < 0) {// 未認識から未認識の遷移
				// なにもしないよーん。
				return false;
			} else {// 未認識から認識の遷移
				this->_current_arcode_index = i_code_index;
				// イベント生成
				// OnEnter
				this->onEnterHandler(i_code_index);
				// 変換行列を作成
				this->_transmat->transMat(i_square, this->_offset, result);
				// OnUpdate
				this->onUpdateHandler(i_square, result);
				this->_lost_delay_count = 0;
				return true;
			}
		} else {// 認識中
			if (i_code_index < 0) {// 認識から未認識の遷移
				this->_lost_delay_count++;
				if (this->_lost_delay < this->_lost_delay_count) {
					// OnLeave
					this->_current_arcode_index = -1;
					this->onLeaveHandler();
				}
				return false;
			} else if (i_code_index == this->_current_arcode_index) {// 同じARCodeの再認識
				// イベント生成
				// 変換行列を作成
				this->_transmat->transMatContinue(i_square, this->_offset, result);
				// OnUpdate
				this->onUpdateHandler(i_square, result);
				this->_lost_delay_count = 0;
				return true;
			} else {// 異なるコードの認識→今はサポートしない。
				throw NyARException();
			}
		}
	}

}

