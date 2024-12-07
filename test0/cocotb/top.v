`resetall
`timescale 10ns / 1ps
`default_nettype none

module top #
(
    parameter ADDR_BITS = 2
)
(
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    a,
    b,
    c,
);

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
input  [7:0] a;
input  [7:0] b;
input  [7:0] c;

wire  [(ADDR_BITS-1):0] in_r_address0;
wire   in_r_ce0;
wire  [31:0] in_r_q0;
wire  [(ADDR_BITS-1):0] out_r_address0;
wire   out_r_ce0;
wire   out_r_we0;
wire  [31:0] out_r_d0;

mem #(
    .ADDR_BITS(ADDR_BITS)
)
mem_inst (
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .in_r_address0(in_r_address0),
    .in_r_ce0(in_r_ce0),
    .in_r_q0(in_r_q0),
    .out_r_address0(out_r_address0),
    .out_r_ce0(out_r_ce0),
    .out_r_we0(out_r_we0),
    .out_r_d0(out_r_d0)
);

foo2 #(
)
foo2_inst (
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .ap_start(ap_start),
    .ap_done(ap_done),
    .ap_idle(ap_idle),
    .ap_ready(ap_ready),
    .in_r_address0(in_r_address0),
    .in_r_ce0(in_r_ce0),
    .in_r_q0(in_r_q0),
    .a(a),
    .b(b),
    .c(c),
    .out_r_address0(out_r_address0),
    .out_r_ce0(out_r_ce0),
    .out_r_we0(out_r_we0),
    .out_r_d0(out_r_d0)
);

endmodule
