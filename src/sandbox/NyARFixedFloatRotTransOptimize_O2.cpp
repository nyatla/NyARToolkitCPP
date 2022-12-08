#include "NyARPerspectiveProjectionMatrix.h"
#include "NyARDoubleMatrix33.h"
#include "NyAR_types.h"
#include "NyARRotMatrix.h"
#include "NyAR_types.h"
#include "NyMath.h"
namespace NyARToolkitCPP{

class NyARI64Matrix33
{
public:
	NyAR_INT64_t m00;
	NyAR_INT64_t m01;
	NyAR_INT64_t m02;
	NyAR_INT64_t m10;
	NyAR_INT64_t m11;
	NyAR_INT64_t m12;
	NyAR_INT64_t m20;
	NyAR_INT64_t m21;
	NyAR_INT64_t m22;
};


	class NyARFixedFloatRotTransOptimize_O2
	{
	private:
		const static int AR_GET_TRANS_MAT_MAX_LOOP_COUNT = 1;// #define AR_GET_TRANS_MAT_MAX_LOOP_COUNT 5
		const NyARPerspectiveProjectionMatrix* _projection_mat_ref;
	public:
		NyARFixedFloatRotTransOptimize_O2(const NyARPerspectiveProjectionMatrix* i_projection_mat_ref)
		{
			this->_projection_mat_ref=i_projection_mat_ref;
			return;
		}
		
		double optimize(NyARRotMatrix& io_rotmat,TNyARDoublePoint3d& io_transvec,const NyARFitVecCalculator& i_calculator)
		{
			const TNyARDoublePoint2d* fit_vertex=i_calculator.getFitSquare();
			const TNyARDoublePoint3d* offset_square=i_calculator.getOffsetVertex()->vertex;
			
			double err = -1;
			err = modifyMatrix(io_rotmat,io_transvec,offset_square,fit_vertex);
			/*ループを抜けるタイミングをARToolKitと合わせるために変なことしてます。*/
			for (int i = 0; ; i++)
			{
				// <arGetTransMat3>
				err = modifyMatrix(io_rotmat, io_transvec, offset_square, fit_vertex);
				i_calculator.calculateTransfer(io_rotmat, io_transvec);
				err = modifyMatrix(io_rotmat, io_transvec, offset_square, fit_vertex);
				// //</arGetTransMat3>
				if (err < 1.0 || i == AR_GET_TRANS_MAT_MAX_LOOP_COUNT - 1)
				{
					break;
				}
				i_calculator.calculateTransfer(io_rotmat, io_transvec);
			}
			return err;
		}
		
//		private final long[][] __modifyMatrix_double1D = new long[8][3];
	private:
		const static NyAR_INT64_t INITIAL_FACTOR=(NyAR_INT64_t)(0x10000*5.0 * 3.1415926535897932384626433832795 / 180.0);
		/**
		 * arGetRot計算を階層化したModifyMatrix 896
		 * 
		 * @param nyrot
		 * @param trans
		 * @param i_vertex3d
		 * [m][3]
		 * @param i_vertex2d
		 * [n][2]
		 * @return
		 * @throws NyARException
		 */
		double modifyMatrix(NyARRotMatrix& io_rot,TNyARDoublePoint3d& trans,const TNyARDoublePoint3d* i_vertex3d, const TNyARDoublePoint2d* i_vertex2d)
		{
			NyAR_INT64_t a2, b2, c2;
			NyAR_INT64_t h, x, y;
			NyAR_INT64_t err, minerr = 0;
			int t1, t2, t3;
			int best_idx=0;

			NyAR_INT64_t factor = INITIAL_FACTOR;
			NyAR_INT64_t rot0, rot1, rot2;
			NyAR_INT64_t combo00, combo01, combo02, combo03, combo10, combo11, combo12, combo13, combo20, combo21, combo22, combo23;
			NyAR_INT64_t combo02_2, combo02_5, combo02_8, combo02_11;
			NyAR_INT64_t combo22_2, combo22_5, combo22_8, combo22_11;
			NyAR_INT64_t combo12_2, combo12_5, combo12_8, combo12_11;
			// vertex展開
			NyAR_INT64_t VX00, VX01, VX02, VX10, VX11, VX12, VX20, VX21, VX22, VX30, VX31, VX32;
			VX00 = (NyAR_INT64_t)(i_vertex3d[0].x*0x10000);
			VX01 = (NyAR_INT64_t)(i_vertex3d[0].y*0x10000);
			VX02 = (NyAR_INT64_t)(i_vertex3d[0].z*0x10000);
			VX10 = (NyAR_INT64_t)(i_vertex3d[1].x*0x10000);
			VX11 = (NyAR_INT64_t)(i_vertex3d[1].y*0x10000);
			VX12 = (NyAR_INT64_t)(i_vertex3d[1].z*0x10000);
			VX20 = (NyAR_INT64_t)(i_vertex3d[2].x*0x10000);
			VX21 = (NyAR_INT64_t)(i_vertex3d[2].y*0x10000);
			VX22 = (NyAR_INT64_t)(i_vertex3d[2].z*0x10000);
			VX30 = (NyAR_INT64_t)(i_vertex3d[3].x*0x10000);
			VX31 = (NyAR_INT64_t)(i_vertex3d[3].y*0x10000);
			VX32 = (NyAR_INT64_t)(i_vertex3d[3].z*0x10000);
			NyAR_INT64_t P2D00, P2D01, P2D10, P2D11, P2D20, P2D21, P2D30, P2D31;
			P2D00 = (NyAR_INT64_t)(i_vertex2d[0].x*0x10000);
			P2D01 = (NyAR_INT64_t)(i_vertex2d[0].y*0x10000);
			P2D10 = (NyAR_INT64_t)(i_vertex2d[1].x*0x10000);
			P2D11 = (NyAR_INT64_t)(i_vertex2d[1].y*0x10000);
			P2D20 = (NyAR_INT64_t)(i_vertex2d[2].x*0x10000);
			P2D21 = (NyAR_INT64_t)(i_vertex2d[2].y*0x10000);
			P2D30 = (NyAR_INT64_t)(i_vertex2d[3].x*0x10000);
			P2D31 = (NyAR_INT64_t)(i_vertex2d[3].y*0x10000);
			const NyARPerspectiveProjectionMatrix& prjmat = *this->_projection_mat_ref;
			NyAR_INT64_t CP0,CP1,CP2,CP3,CP4,CP5,CP6,CP7,CP8,CP9,CP10;
			CP0 = (NyAR_INT64_t)(prjmat.m00*0x10000L);
			CP1 = (NyAR_INT64_t)(prjmat.m01*0x10000L);
			CP2 = (NyAR_INT64_t)(prjmat.m02*0x10000L);
			CP4 = (NyAR_INT64_t)(prjmat.m10*0x10000L);
			CP5 = (NyAR_INT64_t)(prjmat.m11*0x10000L);
			CP6 = (NyAR_INT64_t)(prjmat.m12*0x10000L);
			CP8 = (NyAR_INT64_t)(prjmat.m20*0x10000L);
			CP9 = (NyAR_INT64_t)(prjmat.m21*0x10000L);
			CP10 =(NyAR_INT64_t)(prjmat.m22*0x10000L);

			NyAR_INT64_t trans_x=(NyAR_INT64_t)(trans.x*0x10000L);
			NyAR_INT64_t trans_y=(NyAR_INT64_t)(trans.y*0x10000L);
			NyAR_INT64_t trans_z=(NyAR_INT64_t)(trans.z*0x10000L);

			combo03 = ((CP0 * trans_x + CP1 * trans_y + CP2 * trans_z)>>16) + (NyAR_INT64_t)(prjmat.m03*0x10000L);
			combo13 = ((CP4 * trans_x + CP5 * trans_y + CP6 * trans_z)>>16) + (NyAR_INT64_t)(prjmat.m13*0x10000L);
			combo23 = ((CP8 * trans_x + CP9 * trans_y + CP10 * trans_z)>>16) + (NyAR_INT64_t)(prjmat.m23*0x10000L);
			NyAR_INT64_t CACA, SASA, SACA, CA, SA;
			NyAR_INT64_t CACACB, SACACB, SASACB, CASB, SASB;
			NyAR_INT64_t SACASC, SACACBSC, SACACBCC, SACACC;

			NyAR_INT64_t a_factor[3];
			NyAR_INT64_t sinb[3];
			NyAR_INT64_t cosb[3];
			NyAR_INT64_t b_factor[3];
			NyAR_INT64_t sinc[3];
			NyAR_INT64_t cosc[3];
			NyAR_INT64_t c_factor[3];
			NyAR_INT64_t w, w2;
			NyAR_INT64_t wsin, wcos;
			
			const TNyARDoublePoint3d& angle=io_rot.refAngle();// arGetAngle( rot, &a, &b, &c );
			a2 =(NyAR_INT64_t)(angle.x*0x1000000);
			b2 =(NyAR_INT64_t)(angle.y*0x1000000);
			c2 =(NyAR_INT64_t)(angle.z*0x1000000);

			// comboの3行目を先に計算
			for (int i = 0; i < 10; i++) {
				minerr = 0x4000000000000000L;
				// sin-cosテーブルを計算(これが外に出せるとは…。)
				for (int j = 0; j < 3; j++) {
					w2 = factor * (j - 1);//S16
					w = a2 + w2;//S16
					a_factor[j] = w;//S16
					w = b2 + w2;//S16
					b_factor[j] = w;//S16
					sinb[j] = NyMath::sinFixedFloat24((int)w);
					cosb[j] = NyMath::cosFixedFloat24((int)w);
					w = c2 + w2;//S16
					c_factor[j] = w;//S16
					sinc[j] = NyMath::sinFixedFloat24((int)w);
					cosc[j] = NyMath::cosFixedFloat24((int)w);
				}
				//
				for (t1 = 0; t1 < 3; t1++) {
					SA = NyMath::sinFixedFloat24((int)a_factor[t1]);
					CA = NyMath::cosFixedFloat24((int)a_factor[t1]);
					// Optimize
					CACA = (CA * CA)>>24;//S24
					SASA = (SA * SA)>>24;//S24
					SACA = (SA * CA)>>24;//S24
					for (t2 = 0; t2 < 3; t2++) {
						wsin = sinb[t2];//S24
						wcos = cosb[t2];//S24
						CACACB = (CACA * wcos)>>24;//S24
						SACACB = (SACA * wcos)>>24;//S24
						SASACB = (SASA * wcos)>>24;//S24
						CASB = (CA * wsin)>>24;//S24
						SASB = (SA * wsin)>>24;//S24

						// comboの計算1
						combo02 = (CP0 * CASB + CP1 * SASB + CP2 * wcos)>>24;//S24*S16>>24=S16
						combo12 = (CP4 * CASB + CP5 * SASB + CP6 * wcos)>>24;//S24*S16>>24=S16
						combo22 = (CP8 * CASB + CP9 * SASB + CP10 * wcos)>>24;//S24*S16>>24=S16

						combo02_2 = ((combo02 * VX02)>>16) + combo03;//S16
						combo02_5 = ((combo02 * VX12)>>16) + combo03;//S16
						combo02_8 = ((combo02 * VX22)>>16) + combo03;//S16
						combo02_11 = ((combo02 * VX32)>>16) + combo03;//S16
						combo12_2 = ((combo12 * VX02)>>16) + combo13;//S16
						combo12_5 = ((combo12 * VX12)>>16) + combo13;//S16
						combo12_8 = ((combo12 * VX22)>>16) + combo13;//S16
						combo12_11 = ((combo12 * VX32)>>16) + combo13;//S16
						combo22_2 = ((combo22 * VX02)>>16) + combo23;//S16
						combo22_5 = ((combo22 * VX12)>>16) + combo23;//S16
						combo22_8 = ((combo22 * VX22)>>16) + combo23;//S16
						combo22_11 = ((combo22 * VX32)>>16) + combo23;//S16
						for (t3 = 0; t3 < 3; t3++) {
							wsin = sinc[t3];//S24
							wcos = cosc[t3];//S24
							SACASC = (SACA * wsin)>>24;//S24
							SACACC = (SACA * wcos)>>24;//S24
							SACACBSC =(SACACB * wsin)>>24;//S24;
							SACACBCC = (SACACB * wcos)>>24;//S24;

							rot0 = ((CACACB * wcos + SASA * wcos)>>24) + SACACBSC - SACASC;//S24;
							rot1 = SACACBCC - SACACC + ((SASACB * wsin + CACA * wsin)>>24);//S24;
							rot2 = (-CASB * wcos - SASB * wsin)>>24;//S24;
							combo00 = (CP0 * rot0 + CP1 * rot1 + CP2 * rot2)>>24;//S16
							combo10 = (CP4 * rot0 + CP5 * rot1 + CP6 * rot2)>>24;//S16
							combo20 = (CP8 * rot0 + CP9 * rot1 + CP10 * rot2)>>24;//S16

							rot0 = ((-CACACB * wsin - SASA * wsin)>>24) + SACACBCC - SACACC;//S24
							rot1 = -SACACBSC + SACASC + ((SASACB * wcos + CACA * wcos)>>24);//S24
							rot2 = (CASB * wsin - SASB * wcos)>>24;//S24

							combo01 =(CP0 * rot0 + CP1 * rot1 + CP2 * rot2)>>24;//S16
							combo11 =(CP4 * rot0 + CP5 * rot1 + CP6 * rot2)>>24;//S16
							combo21 =(CP8 * rot0 + CP9 * rot1 + CP10 * rot2)>>24;//S16
							//
							err =0;
							h = ((combo20 * VX00 + combo21 * VX01)>>16) + combo22_2;//S16
							x = P2D00 - ((((combo00 * VX00 + combo01 * VX01)>>16) + combo02_2)<<16) / h;//S16
							y = P2D01 - ((((combo10 * VX00 + combo11 * VX01)>>16) + combo12_2)<<16) / h;//S16
							err += ((x * x + y * y)>>16);
							h = ((combo20 * VX10 + combo21 * VX11)>>16) + combo22_5;
							x = P2D10 - ((((combo00 * VX10 + combo01 * VX11)>>16) + combo02_5)<<16) / h;//S16
							y = P2D11 - ((((combo10 * VX10 + combo11 * VX11)>>16) + combo12_5)<<16) / h;//S16
							err += ((x * x + y * y)>>16);
							h = ((combo20 * VX20 + combo21 * VX21)>>16) + combo22_8;
							x = P2D20 - ((((combo00 * VX20 + combo01 * VX21)>>16) + combo02_8)<<16) / h;//S16
							y = P2D21 - ((((combo10 * VX20 + combo11 * VX21)>>16) + combo12_8)<<16) / h;//S16
							err += ((x * x + y * y)>>16);
							h = ((combo20 * VX30 + combo21 * VX31)>>16) + combo22_11;
							x = P2D30 - ((((combo00 * VX30 + combo01 * VX31)>>16) + combo02_11)<<16) / h;//S16
							y = P2D31 - ((((combo10 * VX30 + combo11 * VX31)>>16) + combo12_11)<<16) / h;//S16
							err += ((x * x + y * y)>>16);
							if (err < minerr) {
								minerr = err;
								a2 = a_factor[t1];
								b2 = b_factor[t2];
								c2 = c_factor[t3];
								best_idx=t1+t2*3+t3*9;
							}
						}
					}
				}
				if (best_idx==(1+3+9)) {
					factor=factor>>1;
				}
			}
			io_rot.setAngle((double)a2/NyMath::FIXEDFLOAT24_1,(double)b2/NyMath::FIXEDFLOAT24_1,(double)c2/NyMath::FIXEDFLOAT24_1);
			/* printf("factor = %10.5f\n", factor*180.0/MD_PI); */
			return (double)(minerr /4);//この設定値おかしくね？16bitfixedfloatなら16で割らないと。
		}	
		
		
	};

}