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
#include <windows.h>
#include <gl/GL.h>
#include <glut.h>

#include "NyAR_core.h"

namespace NyARUtils
{
	class NyARGLUtil
	{
	public:
		NyARGLUtil();
	private:
		/**
		 * ラスタのGLタイプを取得する。
		 * @param i_buffer_type
		 * @return
		 * @throws NyARException
		 */
		int getGlPixelFormat(int i_buffer_type);
	public:
		/**
		 * GLNyARRaster_RGBをバックグラウンドに書き出す。
		 * @param image
		 * @param zoom
		 */
		void drawBackGround(const NyARToolkitCPP::INyARRgbRaster& i_raster, double i_zoom);
	private:
		double view_scale_factor;
		double view_distance_min;
		double view_distance_max;

		/**
		 * arglDispImageStateful関数モドキ
		 * @param image
		 * @param zoom
		 */
	private:
		void arglDispImageStateful(const NyARToolkitCPP::INyARRgbRaster& i_raster, double zoom);
	public:
		void setScaleFactor(double i_new_value);
		void setViewDistanceMin(double i_new_value);
		void setViewDistanceMax(double i_new_value);
		/**
		 * void arglCameraFrustumRH(const ARParam *cparam, const double focalmin, const double focalmax, GLdouble m_projection[16])
		 * 関数の置き換え
		 * NyARParamからOpenGLのProjectionを作成します。
		 * @param i_arparam
		 * @param o_gl_projection
		 * double[16]を指定して下さい。
		 */
		void toCameraFrustumRH(const NyARToolkitCPP::NyARParam& i_arparam,GLdouble* o_gl_projection);
		/**
		 * NyARTransMatResultをOpenGLの行列へ変換します。
		 * @param i_ny_result
		 * @param o_gl_result
		 * @throws NyARException
		 */
		void toCameraViewRH(const NyARToolkitCPP::NyARTransMatResult& i_ny_result, GLdouble* o_gl_result);
	};
}