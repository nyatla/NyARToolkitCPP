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



#include "NyARSquareContourDetector_Rle.h"
#include "NyARLabelingImage.h"

#include "INyARPca2d.h"
#include "NyARLabelingLabelStack.h"
#include "NyARSquareContourDetector.h"
#include "../utils/NyArray.h"
#include "nyarcore.h"
#include <cstring>
#include <cstdio>
namespace NyARToolkitCPP
{

#define VERTEX_FACTOR 1.0// 線検出のファクタ
#define AR_AREA_MAX 100000// #define AR_AREA_MAX 100000
#define AR_AREA_MIN 70// #define AR_AREA_MIN 70


	NyARSquareContourDetector_Rle::NyARSquareContourDetector_Rle(const TNyARIntSize& i_size)
	{
		this->_width = i_size.w;
		this->_height = i_size.h;
		this->_labeling = new NyARLabeling_Rle(this->_width,this->_height);
		this->_labeling->setAreaRange(AR_AREA_MAX, AR_AREA_MIN);
		this->_stack=new NyARRleLabelFragmentInfoStack(i_size.w*i_size.h*2048/(320*240)+32);//検出可能な最大ラベル数
		this->_overlap_checker=new NyARLabelOverlapChecker<NyARRleLabelFragmentInfo>(32);




		// 輪郭の最大長は画面に映りうる最大の長方形サイズ。
		int number_of_coord = (this->_width + this->_height) * 2;
		// 輪郭バッファは頂点変換をするので、輪郭バッファの２倍取る。
		this->_max_coord = number_of_coord;
		this->_xcoord = new int[number_of_coord * 2];
		this->_ycoord = new int[number_of_coord * 2];
		return;
	}
	NyARSquareContourDetector_Rle::~NyARSquareContourDetector_Rle()
	{
		NyAR_SAFE_DELETE(this->_overlap_checker);
		NyAR_SAFE_DELETE(this->_labeling);
		NyAR_SAFE_DELETE(this->_stack);
		NyAR_SAFE_ARRAY_DELETE(this->_xcoord);
		NyAR_SAFE_ARRAY_DELETE(this->_ycoord);
		return;
	}
	void NyARSquareContourDetector_Rle::detectMarkerCB(const NyARBinRaster& i_raster,NyARSquareContourDetector::IDetectMarkerCallback& i_callback)
	{
		NyARRleLabelFragmentInfoStack& flagment = *(this->_stack);
		NyARLabelOverlapChecker<NyARRleLabelFragmentInfo>& overlap = *(this->_overlap_checker);

		// ラベル数が0ならここまで
		int label_num=this->_labeling->labeling(i_raster, 0, i_raster.getHeight(), flagment);
		if (label_num < 1) {
			return;
		}
		//ラベルをソートしておく
		flagment.sortByArea();
		//ラベルリストを取得
		NyARRleLabelFragmentInfo** labels=flagment.getArray().item;

		int xsize = this->_width;
		int ysize = this->_height;
		int* xcoord = this->_xcoord;
		int* ycoord = this->_ycoord;
		int coord_max = this->_max_coord;
		int mkvertex[4];


		//重なりチェッカの最大数を設定
		overlap.setMaxLabels(label_num);

		for (int i=0; i < label_num; i++) {
			NyARRleLabelFragmentInfo& label_pt=*labels[i];
			int label_area = label_pt.area;
		
			// クリップ領域が画面の枠に接していれば除外
			if (label_pt.clip_l == 0 || label_pt.clip_r == xsize-1){
				continue;
			}
			if (label_pt.clip_t == 0 || label_pt.clip_b == ysize-1){
				continue;
			}
			// 既に検出された矩形との重なりを確認
			if (!overlap.check(label_pt)) {
				// 重なっているようだ。
				continue;
			}
			
			//輪郭を取得
			int coord_num = _cpickup.getContour(i_raster,label_pt.entry_x,label_pt.clip_t, coord_max, xcoord, ycoord);
			if (coord_num == coord_max) {
				// 輪郭が大きすぎる。
				continue;
			}
			//輪郭線をチェックして、矩形かどうかを判定。矩形ならばmkvertexに取得
			if (!this->_coord2vertex.getVertexIndexes(xcoord, ycoord,coord_num,label_area, mkvertex)) {
				// 頂点の取得が出来なかった
				continue;
			}
			//矩形を発見したことをコールバック関数で通知
			i_callback.onSquareDetect(*this,xcoord,ycoord,coord_num,mkvertex);

			// 検出済の矩形の属したラベルを重なりチェックに追加する。
			overlap.push(&label_pt);
		
		}
		return;
	}
	/**
	 * デバック用API
	 * @return
	 */
	NyARRleLabelFragmentInfoStack* NyARSquareContourDetector_Rle::_getFragmentStack()
	{
		return this->_stack;
	}

}
