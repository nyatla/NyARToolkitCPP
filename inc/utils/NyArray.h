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
namespace NyARToolkitCPP
{
	template <class T> class NyArray
	{
	public:
		int length;
		T* item;
	private:
		//コピーコンストラクタ削除
		NyArray(const NyArray& );
		NyArray& operator=(const NyArray&);

	public:
		NyArray()
		{
			this->item=new T[32];
			this->length=32;
			return;
		}
		NyArray(int i_length)
		{
			this->item=new T[i_length];
			this->length=i_length;
			return;
		}
		virtual ~NyArray()
		{
			delete [] this->item;
			return;
		}
		virtual void renew(int i_new_length)
		{
			T* new_item=new T[i_new_length];
			delete [] this->item;
			this->item=new_item;
			this->length=i_new_length;
			return;
		}
	};
}
