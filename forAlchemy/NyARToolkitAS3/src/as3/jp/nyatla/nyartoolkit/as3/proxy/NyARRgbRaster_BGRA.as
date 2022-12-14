/* 
 * PROJECT: NyARToolkitCPP Alchemy bind
 * --------------------------------------------------------------------------------
 * The NyARToolkitCPP Alchemy bind is stub/proxy classes for NyARToolkitCPP and Adobe Alchemy.
 * 
 * Copyright (C)2009 Ryo Iizuka
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
package jp.nyatla.nyartoolkit.as3.proxy
{
	import jp.nyatla.alchemymaster.*;
	import flash.utils.ByteArray;
	import jp.nyatla.as3utils.*;	
	
	public class NyARRgbRaster_BGRA extends INyARRgbRaster
	{
		public function NyARRgbRaster_BGRA(...args:Array)
		{
			super(NyARToolkitAS3.BASECLASS);			
			switch(args.length){
			case 1:
				if(args[0] is CONST_BASECLASS)
				{	//Base Class
					return;
				}
				break;				
			case 2:
				//function NyARRgbRaster_BGRA(i_width:int,i_height:int)
				this.attachAlchemyObject(
					NyARToolkitAS3.cmodule.NyARRgbRaster_BGRA_createInstance(int(args[0]),int(args[1]))
				);
				return;
			default:
				break;
			}
			throw new Error("NyARRgbRaster_BGRA");
		}		
		//現在の位置からBGRA(BYTE)形式データを読み込みます。
		public function setFromByteArray(i_source:ByteArray):void
		{
			var old:int=i_source.position;
			this._alchemy_stub.setData(this._alchemy_stub._native,i_source);
			i_source.position=old;
			return;
		}
		
	}
}

