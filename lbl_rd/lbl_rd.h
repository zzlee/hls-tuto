#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

#define MAX_WIDTH					4096
#define MAX_HEIGHT					2160

#define SRC_PIXEL_TYPE              XF_8UC1
#define SRC_NPPC                    XF_NPPC16

#define DST_PIXEL_TYPE              XF_8UC3
#define DST_NPPC                    XF_NPPC2

#define SRC_PTR_WIDTH  				XF_WORDDEPTH(XF_WORDWIDTH(SRC_PIXEL_TYPE, SRC_NPPC))
#define DST_WORD_WIDTH		    	XF_WORDDEPTH(XF_WORDWIDTH(DST_PIXEL_TYPE, DST_NPPC))
#define SRC_BITSHIFT                XF_BITSHIFT(SRC_NPPC)
#define DST_BITSHIFT                XF_BITSHIFT(DST_NPPC)

#define SRC_TO_DST_NPPC             (XF_NPIXPERCYCLE(SRC_NPPC) / XF_NPIXPERCYCLE(DST_NPPC))

#define CTRL_FORMAT_BITSHIFT	(0)
#define CTRL_TB_LBL_BITSHIFT	(0x10)

#define FORMAT_NV12 0
#define FORMAT_NV16 1

typedef ap_axiu<DST_WORD_WIDTH, 1, 1, 1> axis_pixel_t;
typedef hls::stream<axis_pixel_t> axis_pixel_stream_t;

extern void lbl_rd(
	ap_uint<SRC_PTR_WIDTH>* pSrcY0,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV0,
	ap_uint<SRC_PTR_WIDTH>* pSrcY1,
	ap_uint<SRC_PTR_WIDTH>* pSrcUV1,
	ap_uint<32> nSrcStride,
	axis_pixel_stream_t& out_Axis,
	ap_uint<32> nWidth,
	ap_uint<32> nHeight,
	ap_uint<32> nControl);
