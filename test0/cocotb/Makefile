TOPLEVEL_LANG = verilog
SIM ?= icarus
WAVES ?= 1
RTL_SRC = ../test0/hls/impl/verilog

VERILOG_SOURCES = \
./top.v \
./mem.v \
${RTL_SRC}/foo2.v \
${RTL_SRC}/foo2_flow_control_loop_pipe.v \
${RTL_SRC}/foo2_mul_32s_8s_32_1_1.v

TOPLEVEL = top
MODULE = tb

include $(shell cocotb-config --makefiles)/Makefile.sim
