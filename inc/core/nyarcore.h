/* 
 * PROJECT: NyARToolkitCPP
 * --------------------------------------------------------------------------------
 *
 * The NyARToolkitCS is C++ version NyARToolkit class library.
 * 
 * Copyright (C)2008 R.Iizuka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For further information please contact.
 *	http://nyatla.jp/nyatoolkit/
 *	<airmail(at)ebony.plala.or.jp>
 * 
 */


/*	コーディングルール

	戻り値
	・オブジェクト
	・配列

	引数
	・オブジェクト
	　内部で参照を残すものは、ポインタで。残さないものは参照。
	・配列

	





*/
#pragma
#include "NyARException.h"
#include <assert.h>
#include <stdlib.h>
#define NyAR_MALLOC(i_size) malloc(i_size)
#define NyAR_SAFE_FREE(i_buf) {free(i_buf);(i_buf)=NULL;}

#define NyAR_SAFE_DELETE(i_object) if((i_object)!=NULL){delete (i_object);(i_object)=NULL;}
#define NyAR_SAFE_ARRAY_DELETE(i_array) if((i_array)!=NULL){delete [] (i_array);(i_array)=NULL;}
#define NyAR_ASSERT(i_exp) assert(i_exp)
