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
#include "NyARLabeling_Rle.h"
#include "nyarcore.h"



#define AR_AREA_MAX 100000// #define AR_AREA_MAX 100000
#define AR_AREA_MIN 70// #define AR_AREA_MIN 70



namespace NyARToolkitCPP
{
	typedef struct TRleInfo_t
	{
		//継承メンバ
		int entry_x; // フラグメントラベルの位置
		int area;
		int clip_r;
		int clip_l;
		int clip_b;
		int clip_t;
		NyAR_INT64_t pos_x;
		NyAR_INT64_t pos_y;		
	}TRleInfo;

	class RleInfoStack : public NyARObjectStack<TRleInfo>
	{
	public:

	public:
		RleInfoStack(int i_length):NyARObjectStack<TRleInfo>(i_length)
		{
			return;
		}
	};
	/**
	* [strage class]
	*/

	struct TRleElement_t
	{
		int l;
		int r;
		int fid;
	};

	NyARLabeling_Rle::NyARLabeling_Rle(int i_width,int i_height)
	{
		this->_rlestack=new RleInfoStack(i_width*i_height*2048/(320*240)+32);
		this->_rle1 = new TRleElement[i_width/2+1];
		this->_rle2 = new TRleElement[i_width/2+1];
		setAreaRange(AR_AREA_MAX,AR_AREA_MIN);
		return;
	}
	NyARLabeling_Rle::~NyARLabeling_Rle()
	{
		NyAR_SAFE_DELETE(this->_rlestack);
		NyAR_SAFE_ARRAY_DELETE(this->_rle1);
		NyAR_SAFE_ARRAY_DELETE(this->_rle2);
		return;
	}


	void NyARLabeling_Rle::setAreaRange(int i_max,int i_min)
	{
		this->_max_area=i_max;
		this->_min_area=i_min;
		return;
	}


	int NyARLabeling_Rle::toRel(int* i_bin_buf, int i_st, int i_len, TRleElement* i_out)
	{
		int current = 0;
		int r = -1;
		// 行確定開始
		int x = i_st;
		const int right_edge = i_st + i_len - 1;
		while (x < right_edge) {
			// 暗点(0)スキャン
			if (i_bin_buf[x] != 0) {
				x++;
				continue;
			}
			// 暗点発見→暗点長を調べる
			r = (x - i_st);
			i_out[current].l = r;
			r++;// 暗点+1
			x++;
			while (x < right_edge) {
				if (i_bin_buf[x] != 0) {
					// 明点(1)→暗点(0)配列終了>登録
					i_out[current].r = r;
					current++;
					x++;// 次点の確認。
					r = -1;// 右端の位置を0に。
					break;
				} else {
					// 暗点(0)長追加
					r++;
					x++;
				}
			}
		}
		// 最後の1点だけ判定方法が少し違うの。
		if (i_bin_buf[x] != 0) {
			// 明点→rカウント中なら暗点配列終了>登録
			if (r >= 0) {
				i_out[current].r = r;
				current++;
			}
		} else {
			// 暗点→カウント中でなければl1で追加
			if (r >= 0) {
				i_out[current].r = (r + 1);
			} else {
				// 最後の1点の場合
				i_out[current].l = (i_len - 1);
				i_out[current].r = (i_len);
			}
			current++;
		}
		// 行確定
		return current;
	}

	void NyARLabeling_Rle::addFragment(TRleElement& i_rel_img, int i_nof, int i_row_index,RleInfoStack& o_stack)
	{
		int l=i_rel_img.l;
		const int len=i_rel_img.r - l;
		i_rel_img.fid = i_nof;// REL毎の固有ID
		TRleInfo* v = o_stack.prePush();
		v->entry_x = l;
		v->area =len;
		v->clip_l=l;
		v->clip_r=i_rel_img.r-1;
		v->clip_t=i_row_index;
		v->clip_b=i_row_index;
		v->pos_x=(len*(2*l+(len-1)))/2;
		v->pos_y=i_row_index*len;

		return;
	}
	int NyARLabeling_Rle::labeling(const NyARBinRaster& i_bin_raster, int i_top, int i_bottom,NyARRleLabelFragmentInfoStack& o_stack)
	{
		NyAR_ASSERT(i_bin_raster.isEqualBufferType(NyARBufferType::INT1D_BIN_8));
		return this->imple_labeling(i_bin_raster,0,i_top,i_bottom,o_stack);
	}
	int NyARLabeling_Rle::labeling(const NyARGrayscaleRaster& i_gs_raster,int i_th, int i_top, int i_bottom,NyARRleLabelFragmentInfoStack& o_stack)
	{
		assert(i_gs_raster.isEqualBufferType(NyARBufferType::INT1D_GRAY_8));
		return this->imple_labeling(i_gs_raster,i_th,i_top,i_bottom,o_stack);
	}
	//
	int NyARLabeling_Rle::imple_labeling(const INyARRaster& i_raster,int i_th,int i_top, int i_bottom,NyARRleLabelFragmentInfoStack& o_stack)
	{
		// リセット処理
		RleInfoStack& rlestack=*this->_rlestack;
		rlestack.clear();

		//
		TRleElement* rle_prev = this->_rle1;
		TRleElement* rle_current = this->_rle2;
		int len_prev = 0;
		int len_current = 0;
		const int width = i_raster.getWidth();
		int* in_buf = (int*) i_raster.getBuffer();

		int id_max = 0;
		int label_count=0;
		// 初段登録

		len_prev = toRel(in_buf, i_top, width, rle_prev);
		for (int i = 0; i < len_prev; i++) {
			// フラグメントID=フラグメント初期値、POS=Y値、RELインデクス=行
			addFragment(rle_prev[i], id_max, i_top, rlestack);
			id_max++;
			// nofの最大値チェック
			label_count++;
		}
		TRleInfo** const f_array = rlestack.getArray().item;
		// 次段結合
		for (int y = i_top + 1; y < i_bottom; y++) {
			// カレント行の読込
			len_current = toRel(in_buf, y * width, width, rle_current);
			int index_prev = 0;

			for (int i = 0; i < len_current; i++) {
				// index_prev,len_prevの位置を調整する
				int id = -1;
				// チェックすべきprevがあれば確認
				while (index_prev < len_prev) {
					if (rle_current[i].l - rle_prev[index_prev].r > 0) {// 0なら8方位ラベリング
						// prevがcurの左方にある→次のフラグメントを探索
						index_prev++;
						continue;
					} else if (rle_prev[index_prev].l - rle_current[i].r > 0) {// 0なら8方位ラベリングになる
						// prevがcur右方にある→独立フラグメント
						addFragment(rle_current[i], id_max, y,rlestack);
						id_max++;
						label_count++;
						// 次のindexをしらべる
						goto SCAN_CUR;
					}
					id=rle_prev[index_prev].fid;//ルートフラグメントid
					TRleInfo& id_ptr = *f_array[id];
					//結合対象(初回)->prevのIDをコピーして、ルートフラグメントの情報を更新
					rle_current[i].fid = id;//フラグメントIDを保存
					//
					const int l= rle_current[i].l;
					const int r= rle_current[i].r;
					const int len=r-l;
					//結合先フラグメントの情報を更新する。
					id_ptr.area += len;
					//tとentry_xは、結合先のを使うので更新しない。
					id_ptr.clip_l=l<id_ptr.clip_l?l:id_ptr.clip_l;
					id_ptr.clip_r=r>id_ptr.clip_r?r-1:id_ptr.clip_r;
					id_ptr.clip_b=y;
					id_ptr.pos_x+=(len*(2*l+(len-1)))/2;
					id_ptr.pos_y+=y*len;
					//多重結合の確認（２個目以降）
					index_prev++;
					while (index_prev < len_prev) {
						if (rle_current[i].l - rle_prev[index_prev].r > 0) {// 0なら8方位ラベリング
							// prevがcurの左方にある→prevはcurに連結していない。
							goto SCAN_PREV;
						} else if (rle_prev[index_prev].l - rle_current[i].r > 0) {// 0なら8方位ラベリングになる
							// prevがcurの右方にある→prevはcurに連結していない。
							index_prev--;
							goto SCAN_CUR;
						}
						// prevとcurは連結している→ルートフラグメントの統合

						//結合するルートフラグメントを取得
						const int prev_id =rle_prev[index_prev].fid;
						TRleInfo& prev_ptr = *f_array[prev_id];
						if (id != prev_id){
							label_count--;
							//prevとcurrentのフラグメントidを書き換える。
							for(int i2=index_prev;i2<len_prev;i2++){
								//prevは現在のidから最後まで
								if(rle_prev[i2].fid==prev_id){
									rle_prev[i2].fid=id;
								}
							}
							for(int i2=0;i2<i;i2++){
								//currentは0から現在-1まで
								if(rle_current[i2].fid==prev_id){
									rle_current[i2].fid=id;
								}
							}

							//現在のルートフラグメントに情報を集約
							id_ptr.area +=prev_ptr.area;
							id_ptr.pos_x+=prev_ptr.pos_x;
							id_ptr.pos_y+=prev_ptr.pos_y;
							//tとentry_xの決定
							if (id_ptr.clip_t > prev_ptr.clip_t) {
								// 現在の方が下にある。
								id_ptr.clip_t = prev_ptr.clip_t;
								id_ptr.entry_x = prev_ptr.entry_x;
							}else if (id_ptr.clip_t < prev_ptr.clip_t) {
								// 現在の方が上にある。prevにフィードバック
							} else {
								// 水平方向で小さい方がエントリポイント。
								if (id_ptr.entry_x > prev_ptr.entry_x) {
									id_ptr.entry_x = prev_ptr.entry_x;
								}else{
								}
							}
							//lの決定
							if (id_ptr.clip_l > prev_ptr.clip_l) {
								id_ptr.clip_l=prev_ptr.clip_l;
							}else{
							}
							//rの決定
							if (id_ptr.clip_r < prev_ptr.clip_r) {
								id_ptr.clip_r=prev_ptr.clip_r;
							}else{
							}
							//bの決定

							//結合済のルートフラグメントを無効化する。
							prev_ptr.area=0;
						}


						index_prev++;
					}
					index_prev--;
					break;
					SCAN_PREV:;
				}
				// curにidが割り当てられたかを確認
				// 右端独立フラグメントを追加
				if (id < 0){
					addFragment(rle_current[i], id_max, y,rlestack);
					id_max++;
					label_count++;
				}
			SCAN_CUR:;
			}
			// prevとrelの交換
			TRleElement* tmp = rle_prev;
			rle_prev = rle_current;
			len_prev = len_current;
			rle_current = tmp;
		}
		//対象のラベルだけ転写
		o_stack.init(label_count);
		NyARRleLabelFragmentInfo** o_dest_array=o_stack.getArray().item;
		const int max=this->_max_area;
		const int min=this->_min_area;
		int active_labels=0;
		for(int i=id_max-1;i>=0;i--){
			const int area=f_array[i]->area;
			if(area<min || area>max){//対象外のエリア0のもminではじく
				continue;
			}
			//
			const TRleInfo src_info=*f_array[i];
			NyARRleLabelFragmentInfo& dest_info=*o_dest_array[active_labels];
			dest_info.area=area;
			dest_info.clip_b=src_info.clip_b;
			dest_info.clip_r=src_info.clip_r;
			dest_info.clip_t=src_info.clip_t;
			dest_info.clip_l=src_info.clip_l;
			dest_info.entry_x=src_info.entry_x;
			dest_info.pos_x=(double)(src_info.pos_x/src_info.area);
			dest_info.pos_y=(double)(src_info.pos_y/src_info.area);
			active_labels++;
		}
		//ラベル数を再設定
		o_stack.pops(label_count-active_labels);
		//ラベル数を返却
		return active_labels;
	}	


}

