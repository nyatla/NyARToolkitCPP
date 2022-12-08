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
#include "NyARCustomSingleDetectMarker.h"
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "nyarcore.h"

namespace NyARToolkitCPP
{

	/**
	 * detectMarkerのコールバック関数
	 */
	class DetectSquareCB : public NyARSquareContourDetector::IDetectMarkerCallback
	{
		//公開プロパティ
	public:
		double confidence;
		NyARSquare square;
	private:
		//参照インスタンス
		const INyARRgbRaster* _ref_raster;
		INyARColorPatt* _inst_patt;
	private:
		//所有インスタンス
		NyARMatchPattDeviationColorData* _deviation_data;
		NyARCoord2Linear* _coordline;
		NyARMatchPatt_Color_WITHOUT_PCA* _match_patt;
		
	public:
		DetectSquareCB(INyARColorPatt* i_inst_patt,const NyARCode* i_ref_code,const NyARParam& i_param)
		{
			this->_inst_patt=i_inst_patt;
			this->_deviation_data=new NyARMatchPattDeviationColorData(i_ref_code->getWidth(),i_ref_code->getHeight());
			this->_coordline=new NyARCoord2Linear(i_param.getScreenSize(),i_param.getDistortionFactor());
			this->_match_patt=new NyARMatchPatt_Color_WITHOUT_PCA(i_ref_code);
			return;
		}
		~DetectSquareCB()
		{
			NyAR_SAFE_DELETE(this->_deviation_data);
			NyAR_SAFE_DELETE(this->_coordline);
			NyAR_SAFE_DELETE(this->_match_patt);
			NyAR_SAFE_DELETE(this->_inst_patt);
		}

		/**
		 * 矩形が見付かるたびに呼び出されます。
		 * 発見した矩形のパターンを検査して、方位を考慮した頂点データを確保します。
		 */
	public:
		void onSquareDetect(const NyARSquareContourDetector& i_sender,int i_coordx[],int i_coordy[],int i_coor_num,int i_vertex_index[])
		{
			TNyARMatchPattResult mr;
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
			if (!this->_inst_patt->pickFromRaster(*(this->_ref_raster),vertex)){
				return;
			}
			//取得パターンをカラー差分データに変換して評価する。
			this->_deviation_data->setRaster(*this->_inst_patt);
			if(!this->_match_patt->evaluate(*this->_deviation_data,mr)){
				return;
			}
			//現在の一致率より低ければ終了
			if (this->confidence > mr.confidence){
				return;
			}
			//一致率の高い矩形があれば、方位を考慮して頂点情報を作成
			NyARSquare& sq=this->square;
			this->confidence = mr.confidence;
			//directionを考慮して、squareを更新する。
			for(int i=0;i<4;i++){
				int idx=(i+4 - mr.direction) % 4;
				this->_coordline->coord2Line(i_vertex_index[idx],i_vertex_index[(idx+1)%4],i_coordx,i_coordy,i_coor_num,sq.line[i]);
			}
			for (int i = 0; i < 4; i++) {
				//直線同士の交点計算
				if(!TNyARLinear::crossPos(sq.line[i],sq.line[(i + 3) % 4],sq.sqvertex[i])){
					throw NyARException();//ここのエラー復帰するならダブルバッファにすればOK
				}
			}
		}
		void init(const INyARRgbRaster* i_raster)
		{
			this->confidence=0;
			this->_ref_raster=i_raster;
		}
	};


	NyARCustomSingleDetectMarker::~NyARCustomSingleDetectMarker()
	{
		NyAR_SAFE_DELETE(this->_square_detect);
		NyAR_SAFE_DELETE(this->_transmat);

		NyAR_SAFE_DELETE(this->_bin_raster);
		NyAR_SAFE_DELETE(this->_detect_cb);
		return;
	}


	NyARCustomSingleDetectMarker::NyARCustomSingleDetectMarker()
	{
		return;
	}
	void NyARCustomSingleDetectMarker::initInstance(
		INyARColorPatt* i_patt_inst,
		NyARSquareContourDetector* i_sqdetect_inst,
		INyARTransMat* i_transmat_inst,
		const INyARRasterFilter_Rgb2Bin* i_filter,
		const NyARParam&	i_ref_param,
		const NyARCode*	i_ref_code,
		double		i_marker_width)
	{
		this->_is_continue = false;

		const TNyARIntSize& scr_size=i_ref_param.getScreenSize();		
		//オブジェクト所有権の移譲
		this->_square_detect = i_sqdetect_inst;
		this->_transmat = i_transmat_inst;
		this->_tobin_filter=i_filter;
		//２値画像バッファを作る
		this->_bin_raster=new NyARBinRaster(scr_size.w,scr_size.h);
		//_detect_cb
		this->_detect_cb=new DetectSquareCB(i_patt_inst,i_ref_code,i_ref_param);
		//オフセットをセット
		this->_offset.setSquare(i_marker_width);
		return;
	}

	

	
	/**
	 * i_imageにマーカー検出処理を実行し、結果を記録します。
	 * 
	 * @param i_raster
	 * マーカーを検出するイメージを指定します。イメージサイズは、カメラパラメータ
	 * と一致していなければなりません。
	 * @return マーカーが検出できたかを真偽値で返します。
	 * @throws NyARException
	 */
	bool NyARCustomSingleDetectMarker::detectMarkerLite(const INyARRgbRaster& i_raster)
	{
		//サイズチェック
		if(!this->_bin_raster->getSize().isEqualSize(i_raster.getSize())){
			throw NyARException();
		}

		//ラスタを２値イメージに変換する.
		this->_tobin_filter->doFilter(i_raster,*this->_bin_raster);

		//コールバックハンドラの準備
		this->_detect_cb->init(&i_raster);
		//矩形を探す(戻り値はコールバック関数で受け取る。)
		this->_square_detect->detectMarkerCB(*this->_bin_raster,*this->_detect_cb);
		if(this->_detect_cb->confidence==0){
			return false;
		}
		return true;
	}


	void NyARCustomSingleDetectMarker::getTransmationMatrix(NyARTransMatResult &o_result)const
	{
		// 一番一致したマーカーの位置とかその辺を計算
		if (this->_is_continue) {
			this->_transmat->transMatContinue(this->_detect_cb->square,this->_offset,o_result);
		} else {
			this->_transmat->transMat(this->_detect_cb->square,this->_offset,o_result);
		}
		return;
	}

	/**
	* 画面上のマーカ頂点情報を配列へのリファレンスを返します。
	* 返されたオブジェクトはクラスに所有し続けられています。クラスのメンバ関数を実行すると内容が書き変わります。
	* 外部でデータをストックする場合は、getSquarePositionで複製して下さい。
	* @return
	*/
	const NyARSquare& NyARCustomSingleDetectMarker::refSquare()const
	{
		return this->_detect_cb->square;
	}


	/**
	* 検出したマーカーの一致度を返します。
	* 
	* @return マーカーの一致度を返します。0～1までの値をとります。 一致度が低い場合には、誤認識の可能性が高くなります。
	* @throws NyARException
	*/
	double NyARCustomSingleDetectMarker::getConfidence()const
	{
		return this->_detect_cb->confidence;
	}

	/**
	* getTransmationMatrixの計算モードを設定します。 初期値はTRUEです。
	* 
	* @param i_is_continue
	* TRUEなら、transMatCont互換の計算をします。 FALSEなら、transMat互換の計算をします。
	*/
	void NyARCustomSingleDetectMarker::setContinueMode(bool i_is_continue)
	{
		this->_is_continue = i_is_continue;
	}


}
