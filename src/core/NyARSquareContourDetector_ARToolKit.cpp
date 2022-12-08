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
#include "NyARSquareContourDetector_ARToolKit.h"
#include "NyARLabeling_ARToolKit.h"
#include "NyARLabelingImage.h"
#include "NyARObserv2IdealMap.h"
#include "INyARPca2d.h"
#include "NyARPca2d_MatrixPCA_O2.h"
#include "../utils/NyArray.h"
#include "nyarcore.h"
#include <cstring>
#include <cstdio>
namespace NyARToolkitCPP
{
#define AR_AREA_MAX 100000// #define AR_AREA_MAX 100000
#define AR_AREA_MIN 70// #define AR_AREA_MIN 70



#define VERTEX_FACTOR 1.0// 線検出のファクタ
#define AR_AREA_MAX 100000// #define AR_AREA_MAX 100000
#define AR_AREA_MIN 70// #define AR_AREA_MIN 70


	NyARSquareContourDetector_ARToolKit::NyARSquareContourDetector_ARToolKit(const TNyARIntSize& i_size)
	{
		this->_width = i_size.w;
		this->_height = i_size.h;

		this->_labeling = new NyARLabeling_ARToolKit();
		this->_limage = new NyARLabelingImage(this->_width, this->_height);
		this->_overlap_checker = new NyARLabelOverlapChecker<NyARLabelingLabel>(32);
		this->_coord2vertex=new NyARCoord2SquareVertexIndexes();


		// 輪郭の最大長は画面に映りうる最大の長方形サイズ。
		int number_of_coord = (this->_width + this->_height) * 2;
		// 輪郭バッファは頂点変換をするので、輪郭バッファの２倍取る。
		this->_max_coord = number_of_coord;
		this->_xcoord = new int[number_of_coord * 2];
		this->_ycoord = new int[number_of_coord * 2];
		return;
	}
	NyARSquareContourDetector_ARToolKit::~NyARSquareContourDetector_ARToolKit()
	{
		NyAR_SAFE_DELETE(this->_coord2vertex);
		NyAR_SAFE_DELETE(this->_overlap_checker);
		NyAR_SAFE_DELETE(this->_labeling);
		NyAR_SAFE_DELETE(this->_limage);
		NyAR_SAFE_ARRAY_DELETE(this->_xcoord);
		NyAR_SAFE_ARRAY_DELETE(this->_ycoord);
		return;
	}
	void NyARSquareContourDetector_ARToolKit::detectMarker(const NyARBinRaster& i_raster,IDetectMarkerCallback& i_callback)
	{
		NyARLabelingImage& limage = *this->_limage;

		// ラベリング
		int label_num =this->_labeling->labeling(i_raster,limage);

		// ラベル数が0ならここまで
		if (label_num < 1){
			return;
		}

		NyARLabelingLabelStack& stack = limage.getLabelStack();
		// ラベルを大きい順に整列
		stack.sortByArea();

		const NyArray<NyARLabelingLabel*>& labels = stack.getArray();



		// デカいラベルを読み飛ばし
		int i;
		for (i = 0; i < label_num; i++) {
			// 検査対象内のラベルサイズになるまで無視
			if (labels.item[i]->area <= AR_AREA_MAX) {
				break;
			}
		}

		const int xsize = this->_width;
		const int ysize = this->_height;
		int* xcoord = this->_xcoord;
		int* ycoord = this->_ycoord;
		const int coord_max = this->_max_coord;
		NyARLabelOverlapChecker<NyARLabelingLabel>* overlap = this->_overlap_checker;

		//重なりチェッカの最大数を設定
		overlap->setMaxLabels(label_num);

		for (; i < label_num; i++) {
			const NyARLabelingLabel& label_pt = *labels.item[i];
			const int label_area = label_pt.area;
			// 検査対象サイズよりも小さくなったら終了
			if (label_area < AR_AREA_MIN) {
				break;
			}
			// クリップ領域が画面の枠に接していれば除外
			if (label_pt.clip_l == 1 || label_pt.clip_r == xsize - 2) {// if(wclip[i*4+0] == 1 || wclip[i*4+1] ==xsize-2){
				continue;
			}
			if (label_pt.clip_t == 1 || label_pt.clip_b == ysize - 2) {// if( wclip[i*4+2] == 1 || wclip[i*4+3] ==ysize-2){
				continue;
			}
			// 既に検出された矩形との重なりを確認
			if (!overlap->check(label_pt)) {
				// 重なっているようだ。
				continue;
			}

			// 輪郭を取得
			const int coord_num = _cpickup->getContour(limage,limage.getTopClipTangentX(label_pt),label_pt.clip_t, coord_max, xcoord, ycoord);
			if (coord_num == coord_max) {
				// 輪郭が大きすぎる。
				continue;
			}
			int mkvertex[4];
			//輪郭線をチェックして、矩形かどうかを判定。矩形ならばmkvertexに取得
			if (!this->_coord2vertex->getVertexIndexes(xcoord, ycoord,coord_num,label_area, mkvertex)) {
				// 頂点の取得が出来なかった
				continue;
			}
			//矩形を発見したことをコールバック関数で通知
			i_callback.onSquareDetect(*this,xcoord,ycoord,coord_num,mkvertex);

			// 検出済の矩形の属したラベルを重なりチェックに追加する。
			overlap->push(&label_pt);

		}	
		return;
	}
}
