`resetall
`timescale 10ns / 1ps
`default_nettype none

module mem #(
    parameter ADDR_BITS = 6
)
(
    ap_clk,
    ap_rst,
    in_r_address0,
    in_r_ce0,
    in_r_q0,
    out_r_address0,
    out_r_ce0,
    out_r_we0,
    out_r_d0
);

input   ap_clk;
input   ap_rst;
input  [(ADDR_BITS-1):0] in_r_address0;
input   in_r_ce0;
output reg [31:0] in_r_q0;
input  [(ADDR_BITS-1):0] out_r_address0;
input   out_r_ce0;
input   out_r_we0;
input  [31:0] out_r_d0;

reg [(1 << ADDR_BITS)][31:0] i_buf;

// in_r
always @(posedge ap_clk) begin
    if(ap_rst) begin
        in_r_q0 <= 32'h0;
    end else begin
        if(in_r_ce0) begin
            in_r_q0[ADDR_BITS*1-1:ADDR_BITS*0] <= in_r_address0;
        end
    end
end

// out_r
always @(posedge ap_clk) begin
    if(ap_rst) begin
        i_buf <= 0;
    end else if(out_r_ce0 && out_r_we0) begin
        integer i;
        for (i = 0; i < (1 << ADDR_BITS);i++) begin
            if (out_r_address0 == i) begin
                i_buf[i] <= out_r_d0;
            end
        end
    end
end

endmodule
