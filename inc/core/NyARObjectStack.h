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
#include "../utils/NyArray.h"
#include "NyARException.h"
#include <cstdlib>
using namespace std;
namespace NyARToolkitCPP
{
	template <class T> class NyARObjectStack
	{
	private:
		const static int ARRAY_APPEND_STEP=64;
	protected:
		NyArray<T*>* _items;
		int _length;
	private:
		int _allocated_size;
	private:
		//コピーコンストラクタ削除
		NyARObjectStack(const NyARObjectStack& );
		NyARObjectStack& operator=(const NyARObjectStack&);
	public:
		NyARObjectStack(int i_length)
		{
			//要素を予め確保
			this->_items =new NyArray<T*>(i_length);
			for (int i =0; i < i_length; i++){
				this->_items->item[i] =createElement();
			}
			// 使用中個数をリセット
			this->_length = 0;
			return;
		}
		virtual ~NyARObjectStack(void)
		{
			//要素削除
			T** a=this->_items->item;
			int l=this->_items->length;
			for(int i=0;i<l;i++){
				delete *(a+i);
			}
			//メンバ削除
			delete this->_items;
			return;
		}
		T* prePush()
		{
			// 必要に応じてアロケート
			if (this->_length >= this->_items->length){
				return NULL;
			}
			// 使用領域を+1して、予約した領域を返す。
			T* ret = this->_items->item[this->_length];
			this->_length++;
			return ret;
		}
		void init(int i_reserv_length)
		{
			// 必要に応じてアロケート
			if (i_reserv_length >= this->_items->length){
				throw NyARException();
			}
			this->_length=i_reserv_length;
		}
		T* pop()
		{
			assert(this->_length>=1);
			this->_length--;
			return this->_items->item[this->_length];
		}
		void pops(int i_count)
		{
			assert(this->_length>=i_count);
			this->_length-=i_count;
			return;
		}
		const NyArray<T*>& getArray() const
		{
			return *(this->_items);
		}
		T* getItem(int i_index) const
		{
			return *(this->_items->item+i_index);
		}
		int getLength() const
		{
			return this->_length;
		}
		void clear()
		{
			this->_length = 0;
		}
	protected:
		virtual T* createElement()
		{
			return new T();
		}

	};
}
