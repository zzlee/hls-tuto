#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"

#define INTR_BITS 6

enum STATE {
    STATE_IDLE,
    STATE_STEP1,
    STATE_STEP2,
};
static int state = STATE_IDLE;

static ap_uint<INTR_BITS> cur_intr = 0;
static ap_uint<INTR_BITS> last_intr = 0;

void pcie_intr(ap_uint<INTR_BITS> intr, ap_uint<1>* intr_vec_req, ap_uint<32>* intr_num, int* tick) {
// #pragma HLS INTERFACE mode=ap_none port=intr_vec_req
// #pragma HLS INTERFACE mode=ap_none port=intr_num

    switch(state) {
    case STATE_IDLE:
        if(intr == 0 || last_intr == intr)
            break;

        cur_intr = intr;
        last_intr = intr;
        state = STATE_STEP1;
        break;

    case STATE_STEP1: {
        if(cur_intr == 0) {
            state = STATE_STEP1;
            break;
        }

        int ctz = __builtin_ctz(cur_intr);
        *intr_num = ctz;
        *intr_vec_req = 1;

        // delay
        for(int i = 0;i < 5;i++)
            *tick++;

        *intr_vec_req = 0;
        cur_intr = cur_intr & (~(1 << ctz));
    }

        break;
    }
}