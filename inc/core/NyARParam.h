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
#include "NyARPerspectiveProjectionMatrix.h"

#include "NyARBaseClass.h"
#include "NyARCameraDistortionFactor.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	typedef struct NyARParamFileStruct NyARParamFileStruct_t;
	struct NyARParamFileStruct{
		int x;
		int y;
		double projection[12];
		double distortion[4];
	};


	class NyARParam : public NyARBaseClass
	{
	protected:
		TNyARIntSize _screen_size;
		TNyAREndian _endian;
	private:
		static const int SIZE_OF_PARAM_SET = 4 + 4 + (3 * 4 * 8) + (4 * 8);
		NyARCameraDistortionFactor _dist;
		NyARPerspectiveProjectionMatrix _projection_matrix;
	public:
		NyARParam();
	public:
		void setEndian(TNyAREndian i_new_endian);
	public:
		const TNyARIntSize& getScreenSize()const;
		const NyARPerspectiveProjectionMatrix& getPerspectiveProjectionMatrix()const;
		const NyARCameraDistortionFactor& getDistortionFactor()const;
		void loadARParamFromFile(const char* i_filename);
		void changeScreenSize(int i_xsize, int i_ysize);
		void loadARParam(const NyARParamFileStruct_t& i_stream);
		void saveARParam();
		//
		//@param i_factor
		//	NyARCameraDistortionFactorにセットする配列を指定する。要素数は4であること。
		//@param i_projection
		//	NyARPerspectiveProjectionMatrixセットする配列を指定する。要素数は12であること。
		void setValue(const double i_factor[],const double i_projection[]);
	};
}
