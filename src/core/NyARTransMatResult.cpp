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
#include "NyARTransMatResult.h"
#include <cmath>
namespace NyARToolkitCPP
{
	NyARTransMatResult::NyARTransMatResult()
	{
		this->has_value=false;
		return;
	}
    /**
     * ‚±‚ÌŠÖ”‚ÍA0-PI‚ÌŠÔ‚Å’l‚ð•Ô‚µ‚Ü‚·B
     * @param o_out
     */
	void NyARTransMatResult::getZXYAngle(TNyARDoublePoint3d& o_out)
    {
        double sina = this->m21;
        if (sina >= 1.0)
        {
            o_out.x = NyAR_PI / 2;
            o_out.y = 0;
            o_out.z = atan2(-this->m10, this->m00);
        }
        else if (sina <= -1.0)
        {
            o_out.x = -NyAR_PI / 2;
            o_out.y = 0;
            o_out.z = atan2(-this->m10, this->m00);
        }
        else
        {
            o_out.x = asin(sina);
            o_out.z = atan2(-this->m01, this->m11);
            o_out.y = atan2(-this->m20, this->m22);
        }
    }
	void NyARTransMatResult::transformVertex(double i_x,double i_y,double i_z,TNyARDoublePoint3d& o_out)
	{
		o_out.x=this->m00*i_x+this->m01*i_y+this->m02*i_z+this->m03;
		o_out.y=this->m10*i_x+this->m11*i_y+this->m12*i_z+this->m13;
		o_out.z=this->m20*i_x+this->m21*i_y+this->m22*i_z+this->m23;
		return;
	}
	void NyARTransMatResult::transformVertex(const TNyARDoublePoint3d& i_in,TNyARDoublePoint3d& o_out)
	{
		transformVertex(i_in.x,i_in.y,i_in.z,o_out);
	}

}
