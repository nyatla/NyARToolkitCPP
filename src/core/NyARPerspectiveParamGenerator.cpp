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
#include "NyARPerspectiveParamGenerator.h"

namespace NyARToolkitCPP
{


	/**
	* 遠近法を用いたPerspectiveパラメータを計算するクラスです。
	*
	*/
	NyARPerspectiveParamGenerator::NyARPerspectiveParamGenerator(int i_local_x,int i_local_y,int i_width, int i_height)
	{
		this->_height=i_height;
		this->_width=i_width;
		this->_local_x=i_local_x;
		this->_local_y=i_local_y;
		return;
	}
	NyARPerspectiveParamGenerator::~NyARPerspectiveParamGenerator()
	{
	}

}
