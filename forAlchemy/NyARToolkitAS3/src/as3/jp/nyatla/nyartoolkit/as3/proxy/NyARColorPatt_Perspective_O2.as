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
	
	public class NyARColorPatt_Perspective_O2 extends INyARColorPatt
	{	
		public function NyARColorPatt_Perspective_O2(...args:Array)
		{
			super(NyARToolkitAS3.BASECLASS);
			switch(args.length){
			case 1:
				if(args[0] is CONST_BASECLASS)
				{
					//Base Class
					return;
				}else if(args[0] is CONST_WRAPCLASS){
					//function NyARColorPatt_Perspective_O2(arg:CONST_WRAPCLASS)
					//Empty Wrapper
					return;
				}
				break;
			case 4:
				//function NyARColorPatt_Perspective_O2(int,int,int,int)
				this.attachAlchemyObject(
					NyARToolkitAS3.cmodule.NyARColorPatt_Perspective_O2_createInstance(int(args[0]),int(args[1]),int(args[2]),int(args[3]))
				);
				return;
			default:
			}
			throw new Error("NyARColorPatt_Perspective_O2");
		}
		/**
		 * This function write all pixel data to o_dest. Pixel format is INT-XRGB by little endian.
		 * The function does not change current position of BytreArray.
		 * --
		 * ????????????????????????????????????????????????????????????Int??????????????????o_dest????????????????????????
		 * ?????????????????????????????????Little endian???XRGB????????????o_dest?????????????????????
		 * w*h??????int??????????????????????????????
		 * 
		 * Marshaling format
		 * [GET] int[0],int[1]....int[w*h-1]
		 * 		 */
		public function getData(o_dest:ByteArray):void
		{
			var old:int=o_dest.position;
			this._alchemy_stub.getData(this._alchemy_stub._native,o_dest);
			o_dest.position=old;
			return;
		}
	}
}