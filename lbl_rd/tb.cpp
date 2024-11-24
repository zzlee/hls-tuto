#include <stdio.h>
#include <vector>
#include <iostream>
#include "lbl_rd.h"

int main () {
	int nWidth = 64;
	int nHeight = 16;
	int nWidth_pc = (nWidth >> SRC_BITSHIFT);
	int nSrcStride = nWidth + 32;
	int nSrcStride_pc = (nSrcStride >> SRC_BITSHIFT);

	std::cout << nWidth << 'x' << nHeight << ',' << nSrcStride << std::endl;
	std::cout << nWidth_pc << ',' << nSrcStride_pc << std::endl;

	std::vector<ap_uint<SRC_PTR_WIDTH> > vSrcBuf(nSrcStride_pc * nHeight * 2); // NV16

	ap_uint<SRC_PTR_WIDTH> nVal = "0xAABBCCDDEEDDCCBBAABBCCDDEEDDCCBB";
	for(int i = 0;i < nHeight * 2;i++) {
		for(int j = 0;j < nWidth_pc;j++) {
			int nIndex = i * nSrcStride_pc + j;

			vSrcBuf[nIndex] = nVal;
			nVal++;
		}
	}

	std::cout << "input:" << std::endl;
	for(int i = 0;i < nHeight * 2;i++) {
		std::cout << i << ": ";
		for(int j = 0;j < nWidth_pc;j++) {
			int nIndex = i * nSrcStride_pc + j;

			std::cout << std::setw(32) << std::hex << vSrcBuf[nIndex] << ' ';
		}
		std::cout << std::endl;
	}

	ap_uint<SRC_PTR_WIDTH>* pSrcY0 = &vSrcBuf[0];
	ap_uint<SRC_PTR_WIDTH>* pSrcUV0 = &vSrcBuf[nSrcStride_pc * nHeight];
	ap_uint<SRC_PTR_WIDTH>* pSrcY1 = &vSrcBuf[nSrcStride_pc];
	ap_uint<SRC_PTR_WIDTH>* pSrcUV1 = &vSrcBuf[nSrcStride_pc * nHeight + nSrcStride_pc];
	axis_pixel_stream_t out_Axis;

	int nFormat = FORMAT_NV16;
	int bTB_LBL = 0;
	ap_uint<32> nControl = (nFormat << CTRL_FORMAT_BITSHIFT) | (bTB_LBL << CTRL_TB_LBL_BITSHIFT);

	std::cout << std::hex << nControl << std::endl;

#if 1
	lbl_rd(
		pSrcY0,
		pSrcUV0,
		pSrcY1,
		pSrcUV1,
		nSrcStride,
		out_Axis,
		nWidth,
		nHeight,
		nControl);
#endif

	int nWidth_dst_pc = nWidth >> DST_BITSHIFT;

	std::cout << "output:" << std::endl;
	for(int i = 0;i < nHeight;i++) {
		std::cout << i << ": ";
		for(int j = 0;j < nWidth_dst_pc;j++) {
			axis_pixel_t axi = out_Axis.read();

			std::cout << std::setw(12) << std::hex << axi.data << ' ';
		}
		std::cout << std::endl;
	}

	return 0;
}
