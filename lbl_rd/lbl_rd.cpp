#include <stdio.h>

#ifndef __SYNTHESIS__
#include <iostream>
#include <iomanip>
#include <cstdlib>
#endif

#include "lbl_rd.h"

#define CTRL_MASK_FORMAT		(0xF << CTRL_FORMAT_BITSHIFT)
#define CTRL_MASK_TB_LBL		(0x1 << CTRL_TB_LBL_BITSHIFT)

typedef hls::stream<ap_uint<SRC_PTR_WIDTH> > data_stream_t;

void to_pixels_stream_nv12(
	ap_uint<SRC_PTR_WIDTH>* pSrcY,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV,
	data_stream_t& strmDstYUV,
	int nWidthPC,
	int nHeight,
	int nSkip) {
	int nHeight_2 = (nHeight >> 1);
	int nPosLuma = 0, nPosChroma = 0, nPosChroma0, i, j;
	ap_uint<SRC_PTR_WIDTH> chroma;

loop_height: for (i = 0; i < nHeight_2; i++) {
		nPosChroma0 = nPosChroma;
	loop_width_even: for (j = 0; j < nWidthPC; j++, nPosLuma++, nPosChroma0++) {
#pragma HLS PIPELINE II=2
			strmDstYUV << pSrcY[nPosLuma];
			strmDstYUV << pSrcUV[nPosChroma0];
		}
		nPosLuma += nSkip;

	loop_width_odd: for (j = 0; j < nWidthPC; j++, nPosLuma++, nPosChroma++) {
#pragma HLS PIPELINE II=2
			strmDstYUV << pSrcY[nPosLuma];
			strmDstYUV << pSrcUV[nPosChroma];
		}
		nPosLuma += nSkip;
		nPosChroma += nSkip;
	}
}

void to_pixels_stream_nv16(
	ap_uint<SRC_PTR_WIDTH>* pSrcY,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV,
	data_stream_t& strmDstYUV,
	int nWidthPC,
	int nHeight,
	int nSkip) {
	int nPos = 0, i, j;
loop_height: for (i = 0; i < nHeight; i++) {
#ifndef __SYNTHESIS__
		std::cout << i << ": ";
#endif

	loop_width: for (j = 0; j < nWidthPC; j++, nPos++) {
#pragma HLS PIPELINE II=2

#ifndef __SYNTHESIS__
			std::cout << std::setw(32) << std::hex << pSrcY[nPos] << '/' << std::setw(32) << std::hex << pSrcUV[nPos] << ' ';
#endif

			strmDstYUV << pSrcY[nPos];
			strmDstYUV << pSrcUV[nPos];
		}
		nPos += nSkip;

#ifndef __SYNTHESIS__
		std::cout << std::endl;
#endif
	}
}

void to_pixels_stream_nv16_2(
	ap_uint<SRC_PTR_WIDTH>* pSrcY0,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV0,
	ap_uint<SRC_PTR_WIDTH>* pSrcY1,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV1,
	data_stream_t& strmDstYUV,
	int nWidthPC,
	int nHeight,
	int nSkip) {
	int nHeight_2 = (nHeight >> 1);
	int nPos = 0, i, j;
loop_height: for (i = 0; i < nHeight_2; i++) {
	loop_width_event: for (j = 0; j < nWidthPC; j++, nPos++) {
#pragma HLS PIPELINE II=2
			strmDstYUV << pSrcY0[nPos];
			strmDstYUV << pSrcUV0[nPos];
		}
		nPos += nSkip;

	loop_width_odd: for (j = 0; j < nWidthPC; j++, nPos++) {
#pragma HLS PIPELINE II=2
			strmDstYUV << pSrcY1[nPos];
			strmDstYUV << pSrcUV1[nPos];
		}
		nPos += nSkip;
	}
}

void to_axis(
	data_stream_t& strmSrcYUV,
	axis_pixel_stream_t& out_Axis,
	int nWidthPC,
	int nHeight) {
	ap_uint<SRC_PTR_WIDTH> luma;
	ap_uint<SRC_PTR_WIDTH> chroma;
	axis_pixel_t axi[SRC_TO_DST_NPPC];

loop_height: for (int i = 0; i < nHeight; i++) {
	loop_width: for (int j = 0; j < nWidthPC; j++) {
#pragma HLS PIPELINE II=8
			strmSrcYUV.read(luma);
			strmSrcYUV.read(chroma);

			for(int t = 0;t < SRC_TO_DST_NPPC;t++) {
#pragma HLS UNROLL
				axi[t].keep = -1;
				axi[t].strb = -1;
				axi[t].id = 0;
				axi[t].dest = 0;
				axi[t].user = (t == 0 && i == 0 && j == 0);
				axi[t].last = (t == (SRC_TO_DST_NPPC - 1) && j == (nWidthPC - 1));

#if 1
				axi[t].data.range(8*1-1, 8*0) = luma.range(t*16 + 8*1-1, t*16 + 8*0); // Y0
#else
				axi[t].data.range(8*1-1, 8*0) = 0xFF; // Y0
#endif

#if 1
				axi[t].data.range(8*2-1, 8*1) = chroma.range(t*16 + 8*1-1, t*16 + 8*0); // U0
				axi[t].data.range(8*3-1, 8*2) = chroma.range(t*16 + 8*2-1, t*16 + 8*1); // V0
#else
				axi[t].data.range(8*2-1, 8*1) = 0xFF; // U0
				axi[t].data.range(8*3-1, 8*2) = 0xFF; // V0
#endif

#if 1
				axi[t].data.range(8*4-1, 8*3) = luma.range(t*16 + 8*2-1, t*16 + 8*1); // Y1
#else
				axi[t].data.range(8*4-1, 8*3) = 0xFF; // Y1
#endif

#if 1
				axi[t].data.range(8*5-1, 8*4) = chroma.range(t*16 + 8*1-1, t*16 + 8*0); // U1
				axi[t].data.range(8*6-1, 8*5) = chroma.range(t*16 + 8*2-1, t*16 + 8*1); // V1
#else
				axi[t].data.range(8*5-1, 8*4) = 0xFF; // U1
				axi[t].data.range(8*6-1, 8*5) = 0xFF; // V1
#endif
			}

			for(int t = 0;t < SRC_TO_DST_NPPC;t++) {
				out_Axis.write(axi[t]);
			}
		}
	}
}

void do_nv12(
	ap_uint<SRC_PTR_WIDTH>* pSrcY,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV,
	int nSrcStride,
	axis_pixel_stream_t& out_Axis,
	int nWidth,
	int nHeight) {
	int nWidthPC = nWidth >> SRC_BITSHIFT;
	int nSrcStridePC = nSrcStride >> SRC_BITSHIFT;
	int nSrcSkip = nSrcStridePC - nWidthPC;

	data_stream_t strmSrcYUV("strmSrcYUV");

#pragma HLS STREAM variable=strmSrcYUV depth=4

#ifndef __SYNTHESIS__
	std::cout << __FUNCTION__ << std::endl;
#endif

#pragma HLS DATAFLOW
	to_pixels_stream_nv12(pSrcY, pSrcUV, strmSrcYUV, nWidthPC, nHeight, nSrcSkip);
	to_axis(strmSrcYUV, out_Axis, nWidthPC, nHeight);
}

void do_nv16(
	ap_uint<SRC_PTR_WIDTH>* pSrcY,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV,
	int nSrcStride,
	axis_pixel_stream_t& out_Axis,
	int nWidth,
	int nHeight) {
	int nWidthPC = nWidth >> SRC_BITSHIFT;
	int nSrcStridePC = nSrcStride >> SRC_BITSHIFT;
	int nSrcSkip = nSrcStridePC - nWidthPC;

	data_stream_t strmSrcYUV("strmSrcYUV");

#pragma HLS STREAM variable=strmSrcYUV depth=4

#ifndef __SYNTHESIS__
	std::cout << __FUNCTION__ << std::endl;
#endif

#pragma HLS DATAFLOW
	to_pixels_stream_nv16(pSrcY, pSrcUV, strmSrcYUV, nWidthPC, nHeight, nSrcSkip);
	to_axis(strmSrcYUV, out_Axis, nWidthPC, nHeight);
}

void do_nv16_tb_lbl(
	ap_uint<SRC_PTR_WIDTH>* pSrcY0,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV0,
	ap_uint<SRC_PTR_WIDTH>* pSrcY1,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV1,
	int nSrcStride,
	axis_pixel_stream_t& out_Axis,
	int nWidth,
	int nHeight) {
	int nWidthPC = nWidth >> SRC_BITSHIFT;
	int nSrcStridePC = nSrcStride >> SRC_BITSHIFT;
	int nSrcSkip = nSrcStridePC - nWidthPC;

	data_stream_t strmSrcYUV("strmSrcYUV");

#pragma HLS STREAM variable=strmSrcYUV depth=4

#ifndef __SYNTHESIS__
	std::cout << __FUNCTION__ << std::endl;
#endif

#pragma HLS DATAFLOW
	to_pixels_stream_nv16_2(pSrcY0, pSrcUV0, pSrcY1, pSrcUV1,
		strmSrcYUV, nWidthPC, nHeight, nSrcSkip);
	to_axis(strmSrcYUV, out_Axis, nWidthPC, nHeight);
}

void lbl_rd(
	ap_uint<SRC_PTR_WIDTH>* pSrcY0,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV0,
	ap_uint<SRC_PTR_WIDTH>* pSrcY1,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV1,
	ap_uint<32> nSrcStride,
	axis_pixel_stream_t& s_axis,
	ap_uint<32> nWidth,
	ap_uint<32> nHeight,
	ap_uint<32> nControl) {
#pragma HLS INTERFACE m_axi port=pSrcY0 offset=slave bundle=mm_video
#pragma HLS INTERFACE m_axi port=pSrcUV0 offset=slave bundle=mm_video
#pragma HLS INTERFACE m_axi port=pSrcY1 offset=slave bundle=mm_video
#pragma HLS INTERFACE m_axi port=pSrcUV1 offset=slave bundle=mm_video
#pragma HLS INTERFACE axis port=s_axis register_mode=both
#pragma HLS INTERFACE s_axilite port=nSrcStride
#pragma HLS INTERFACE s_axilite port=nWidth
#pragma HLS INTERFACE s_axilite port=nHeight
#pragma HLS INTERFACE s_axilite port=nControl
#pragma HLS INTERFACE s_axilite port=return

#ifndef __SYNTHESIS__
	std::cout << __FUNCTION__ << std::endl;
#endif

	int nFormat = (nControl & CTRL_MASK_FORMAT) >> CTRL_FORMAT_BITSHIFT;

	switch(nFormat) {
	case FORMAT_NV12:
		do_nv12(pSrcY0, pSrcUV0, nSrcStride, s_axis, nWidth, nHeight);
		break;

	case FORMAT_NV16: {
		bool bTB_LBL = (nControl & CTRL_MASK_TB_LBL) >> CTRL_TB_LBL_BITSHIFT;

		if(bTB_LBL) {
			do_nv16_tb_lbl(pSrcY0, pSrcUV0, pSrcY1, pSrcUV1, nSrcStride,
				s_axis, nWidth, nHeight);
		} else {
			do_nv16(pSrcY0, pSrcUV0, nSrcStride, s_axis, nWidth, nHeight);
		}
	}
		break;
	}
}
