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
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "TNyARMatchPattResult.h"
#include "INyARColorPatt.h"
#include "INyARMatchPatt.h"
#include "../utils/NyArray.h"

namespace NyARToolkitCPP
{

	class NyARMatchPatt_Color_WITHOUT_PCA :public INyARMatchPatt
	{
	protected:
		const NyARCode* _code_patt;
		int _optimize_for_mod;
		int _rgbpixels;
	public:
		NyARMatchPatt_Color_WITHOUT_PCA(const NyARCode* i_code_ref);
		NyARMatchPatt_Color_WITHOUT_PCA(int i_width, int i_height);
		void setARCode(const NyARCode* i_code_ref);
		bool evaluate(const NyARMatchPattDeviationColorData& i_patt,TNyARMatchPattResult& o_result)const;
	};

}
