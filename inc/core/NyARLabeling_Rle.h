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
#include "NyARContourPickup.h"
#include "NyAR_types.h"
#include "INyARPca2d.h"
#include "NyARLabelingImage.h"
#include "NyARBinRaster.h"
#include "NyARGrayscaleRaster.h"
#include "NyARRleLabelFragmentInfoStack.h"
namespace NyARToolkitCPP
{
	typedef struct TRleElement_t TRleElement;
	class RleInfoStack;


	// RleImageÇÉâÉxÉäÉìÉOÇ∑ÇÈÅB
	class NyARLabeling_Rle:public NyARBaseClass
	{
	private:
		RleInfoStack* _rlestack;
		TRleElement* _rle1;
		TRleElement* _rle2;
		int _max_area;
		int _min_area;
	public:
		NyARLabeling_Rle(int i_width,int i_height);
		~NyARLabeling_Rle();
	public:
		void setAreaRange(int i_max,int i_min);
	private:
		int toRel(int* i_bin_buf, int i_st, int i_len, TRleElement* i_out);
		void addFragment(TRleElement& i_rel_img, int i_nof, int i_row_index,RleInfoStack& o_stack);
	public:
		int labeling(const NyARBinRaster& i_bin_raster, int i_top, int i_bottom,NyARRleLabelFragmentInfoStack& o_stack);
		int labeling(const NyARGrayscaleRaster& i_gs_raster,int i_th, int i_top, int i_bottom,NyARRleLabelFragmentInfoStack& o_stack);
	private:
		int imple_labeling(const INyARRaster& i_raster,int i_th,int i_top, int i_bottom,NyARRleLabelFragmentInfoStack& o_stack);
};



}

