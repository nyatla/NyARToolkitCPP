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
#include "nyarcore.h"
#include "NyARLabelingImage.h"
#include <cstdlib>
#define MAX_LABELS 1024*32

namespace NyARToolkitCPP
{
	NyARLabelingImage::NyARLabelingImage(int i_width, int i_height):NyARRaster_BasicClass(i_width,i_height,NyARBufferType::INT1D)
	{
		this->_ref_buf =new int[i_height*i_width];
		this->_label_list = new NyARLabelingLabelStack(MAX_LABELS);
		this->_index_table=new NyArray<int>(MAX_LABELS);
		this->_is_index_table_enable=false;
		//生成時に枠を書きます。
		drawFrameEdge();
		return;
	}
	NyARLabelingImage::~NyARLabelingImage()
	{
		NyAR_SAFE_DELETE(this->_index_table);
		NyAR_SAFE_DELETE(this->_label_list);
		NyAR_SAFE_ARRAY_DELETE(this->_ref_buf);
		return;
	}
	/**
	 * エッジを書きます。
	 */
	void NyARLabelingImage::drawFrameEdge()
	{
		int w=this->_size.w;
		int h=this->_size.h;
		// NyLabelingImageのイメージ初期化(枠書き)
		int* img = (int*) this->_ref_buf;
		int bottom_ptr = (h - 1) * w;
		for (int i = 0; i < w; i++) {
			img[i] = 0;
			img[bottom_ptr + i] = 0;
		}
		for (int i = 0; i < h; i++) {
			img[i * w] = 0;
			img[(i + 1) * w - 1] = 0;
		}
		return;
	}
	void* NyARLabelingImage::getBuffer()const
	{
		return this->_ref_buf;
	}
	bool NyARLabelingImage::hasBuffer()const
	{
		return this->_ref_buf!=NULL;
	}
	void NyARLabelingImage::wrapBuffer(void* i_ref_buf)
	{
		NyARException::notImplement();
	}

	const NyArray<int>* NyARLabelingImage::getIndexArray()const
	{
		return this->_is_index_table_enable?this->_index_table:NULL;
	}

	NyARLabelingLabelStack& NyARLabelingImage::getLabelStack()const
	{
		return *(this->_label_list);
	}
	void NyARLabelingImage::reset(bool i_label_index_enable)
	{
		NyAR_ASSERT(i_label_index_enable==true);//非ラベルモードは未実装
		this->_label_list->clear();
		this->_is_index_table_enable=i_label_index_enable;
		return;
	}
	int NyARLabelingImage::getTopClipTangentX(const NyARLabelingLabel& i_label)const
	{
		int pix;
		int i_label_id=i_label.id;
		const int* index_table=this->_index_table->item;
		const int* limage=this->_ref_buf;
		int limage_ptr=i_label.clip_t*this->_size.w;
		int clip1 = i_label.clip_r;
		// p1=ShortPointer.wrap(limage,j*xsize+clip.get());//p1 =&(limage[j*xsize+clip[0]]);
		for (int i = i_label.clip_l; i <= clip1; i++) {// for( i = clip[0]; i <=clip[1]; i++, p1++ ) {
			pix = limage[limage_ptr+i];
			if (pix > 0 && index_table[pix-1] == i_label_id){
				return i;
			}
		}
		//あれ？見つからないよ？
		throw NyARException();
	}
}
