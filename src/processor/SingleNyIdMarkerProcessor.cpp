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



#include "SingleNyIdMarkerProcessor.h"
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
		const INyIdMarkerData* marker_data;
		int threshold;


		//参照
	private:
		const INyARRgbRaster* _ref_raster;
		//所有インスタンス
		INyIdMarkerData* _current_data;
		INyIdMarkerData* _data_temp;
		NyIdMarkerPickup _id_pickup;
		NyARCoord2Linear* _coordline;
		const INyIdMarkerDataEncoder* _encoder;


		const INyIdMarkerData* _prev_data;
	public:
		DetectSquareCB(const NyARParam& i_param,const INyIdMarkerDataEncoder* i_encoder)
		{
			this->_coordline=new NyARCoord2Linear(i_param.getScreenSize(),i_param.getDistortionFactor());
			this->_data_temp=i_encoder->createDataInstance();
			this->_current_data=i_encoder->createDataInstance();
			this->_encoder=i_encoder;
			return;
		}
		~DetectSquareCB()
		{
			NyAR_SAFE_DELETE(this->_coordline);
			NyAR_SAFE_DELETE(this->_data_temp);
			NyAR_SAFE_DELETE(this->_current_data);
		}

	private:
		//		NyARIntPoint2d[] __tmp_vertex=NyARIntPoint2d.createArray(4);
		/**
		* Initialize call back handler.
		*/
	public:
		void init(const INyARRgbRaster* i_raster,const INyIdMarkerData* i_prev_data)
		{
			this->marker_data=NULL;
			this->_prev_data=i_prev_data;
			this->_ref_raster=i_raster;
		}
		/**
		* 矩形が見付かるたびに呼び出されます。
		* 発見した矩形のパターンを検査して、方位を考慮した頂点データを確保します。
		*/
	public:
		void onSquareDetect(const NyARSquareContourDetector& i_sender,int i_coordx[],int i_coordy[],int i_coor_num,int i_vertex_index[])
		{
			//既に発見済なら終了
			if(this->marker_data!=NULL){
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

			TNyIdMarkerParam param;
			TNyIdMarkerPattern patt_data;
			// 評価基準になるパターンをイメージから切り出す
			if (!this->_id_pickup.pickFromRaster(*this->_ref_raster,vertex, patt_data, param)){
				return;
			}
			//エンコード
			if(!this->_encoder->encode(patt_data,*this->_data_temp)){
				return;
			}

			//継続認識要求されている？
			if (this->_prev_data==NULL){
				//継続認識要求なし
				this->_current_data->copyFrom(*this->_data_temp);
			}else{
				//継続認識要求あり
				if(!this->_prev_data->isEqual((*this->_data_temp))){
					return;//認識請求のあったIDと違う。
				}
			}
			//新しく認識、または継続認識中に更新があったときだけ、Square情報を更新する。
			//ココから先はこの条件でしか実行されない。
			NyARSquare& sq=this->square;
			//directionを考慮して、squareを更新する。
			for(int i=0;i<4;i++){
				int idx=(i+4 - param.direction) % 4;
				this->_coordline->coord2Line(i_vertex_index[idx],i_vertex_index[(idx+1)%4],i_coordx,i_coordy,i_coor_num,sq.line[i]);
			}
			for (int i = 0; i < 4; i++) {
				//直線同士の交点計算
				if(!TNyARLinear::crossPos(sq.line[i],sq.line[(i + 3) % 4],sq.sqvertex[i])){
					throw NyARException();//ここのエラー復帰するならダブルバッファにすればOK
				}
			}
			this->threshold=param.threshold;
			this->marker_data=this->_current_data;//みつかった。
		}
	};


	SingleNyIdMarkerProcesser::SingleNyIdMarkerProcesser()
	{
		this->_lost_delay_count = 0;
		this->_lost_delay = 5;
		this->_current_threshold=110;

		this->_square_detect =NULL;
		this->_transmat      =NULL;
		this->_callback      =NULL;
		this->_bin_raster      =NULL;
		this->_data_current    =NULL;
		this->_tobin_filter    =NULL;
		this->_threshold_detect=NULL;
		this->_initialized=false;
		return;
	}
	SingleNyIdMarkerProcesser::~SingleNyIdMarkerProcesser()
	{
		NyAR_SAFE_DELETE(this->_square_detect);
		NyAR_SAFE_DELETE(this->_transmat);
		NyAR_SAFE_DELETE(this->_callback);


		// ２値画像バッファを作る
		NyAR_SAFE_DELETE(this->_bin_raster);
		//ワーク用のデータオブジェクトを２個作る
		NyAR_SAFE_DELETE(this->_data_current);
		NyAR_SAFE_DELETE(this->_tobin_filter);
		NyAR_SAFE_DELETE(this->_threshold_detect);
	}

	void SingleNyIdMarkerProcesser::initInstance(const NyARParam* i_param,const INyIdMarkerDataEncoder* i_encoder,double i_marker_width,int i_raster_format)
	{
		//初期化済？
		NyAR_ASSERT(this->_initialized==false);

		const TNyARIntSize& scr_size = i_param->getScreenSize();
		// 解析オブジェクトを作る
		this->_square_detect = new NyARSquareContourDetector_Rle(scr_size);
		this->_transmat = new NyARTransMat(i_param);
		this->_callback=new DetectSquareCB(*i_param,i_encoder);

		// ２値画像バッファを作る
		this->_bin_raster = new NyARBinRaster(scr_size.w, scr_size.h);
		//ワーク用のデータオブジェクトを２個作る
		this->_data_current=i_encoder->createDataInstance();
		this->_tobin_filter =new NyARRasterFilter_ARToolkitThreshold(110,i_raster_format);
		this->_threshold_detect=new NyARRasterThresholdAnalyzer_SlidePTile(15,i_raster_format,4);
		this->_initialized=true;
		this->_is_active=false;
		this->_offset.setSquare(i_marker_width);
		return;

	}


	void SingleNyIdMarkerProcesser::setMarkerWidth(int i_width)
	{
		this->_offset.setSquare(i_width);
		return;
	}

	void SingleNyIdMarkerProcesser::reset(bool i_is_force)
	{
		if (i_is_force == false && this->_is_active){
			// 強制書き換えでなければイベントコール
			this->onLeaveHandler();
		}
		//マーカ無効
		this->_is_active=false;
		return;
	}

	void SingleNyIdMarkerProcesser::detectMarker(const INyARRgbRaster& i_raster)
	{
		// サイズチェック
		if (!this->_bin_raster->getSize().isEqualSize(i_raster.getSize().w, i_raster.getSize().h)) {
			throw NyARException();
		}
		// ラスタを２値イメージに変換する.
		this->_tobin_filter->setThreshold(this->_current_threshold);
		this->_tobin_filter->doFilter(i_raster,*this->_bin_raster);

		// スクエアコードを探す(第二引数に指定したマーカ、もしくは新しいマーカを探す。)
		//ここでポインタを参照に切り替えてるけど、実質この２関数でしか使わないので参照でOK・・・APIかえるかもねえ。
		this->_callback->init(&i_raster,this->_is_active?this->_data_current:NULL);
		this->_square_detect->detectMarkerCB(*this->_bin_raster,*this->_callback);

		// 認識状態を更新(マーカを発見したなら、current_dataを渡すかんじ)
		bool is_id_found=updateStatus(this->_callback->square,this->_callback->marker_data);

		//閾値フィードバック(detectExistMarkerにもあるよ)
		if(is_id_found){
			//マーカがあれば、マーカの周辺閾値を反映
			this->_current_threshold=(this->_current_threshold+this->_callback->threshold)/2;
		}else{
			//マーカがなければ、探索+DualPTailで基準輝度検索
			int th=this->_threshold_detect->analyzeRaster(i_raster);
			this->_current_threshold=(this->_current_threshold+th)/2;
		}		
		return;
	}

	/**オブジェクトのステータスを更新し、必要に応じてハンドル関数を駆動します。
	*/
	bool SingleNyIdMarkerProcesser::updateStatus(const NyARSquare& i_square,const INyIdMarkerData* i_marker_data)
	{
		bool is_id_found=false;
		NyARTransMatResult& result = this->__NyARSquare_result;
		if (!this->_is_active) {// 未認識中
			if (i_marker_data==NULL) {// 未認識から未認識の遷移
				// なにもしないよーん。
				this->_is_active=false;
			} else {// 未認識から認識の遷移
				this->_data_current->copyFrom(*i_marker_data);
				// イベント生成
				// OnEnter
				this->onEnterHandler(*this->_data_current);
				// 変換行列を作成
				this->_transmat->transMat(i_square, this->_offset, result);
				// OnUpdate
				this->onUpdateHandler(i_square, result);
				this->_lost_delay_count = 0;
				this->_is_active=true;
				is_id_found=true;
			}
		} else {// 認識中
			if (i_marker_data==NULL) {
				// 認識から未認識の遷移
				this->_lost_delay_count++;
				if (this->_lost_delay < this->_lost_delay_count) {
					// OnLeave
					this->onLeaveHandler();
					this->_is_active=false;
				}
			} else if(this->_data_current->isEqual(*i_marker_data)) {
				//同じidの再認識
				this->_transmat->transMatContinue(i_square, this->_offset, result);
				// OnUpdate
				this->onUpdateHandler(i_square, result);
				this->_lost_delay_count = 0;
				is_id_found=true;
			} else {// 異なるコードの認識→今はサポートしない。
				throw NyARException();
			}
		}
		return is_id_found;
	}

}

