TOPLEVEL_LANG = verilog
SIM ?= icarus
WAVES ?= 1
RTL_SRC = ../test0/hls/impl/verilog

VERILOG_SOURCES = \
./top.v \
./mem.v \
$(wildcard ${RTL_SRC}/%.v)

TOPLEVEL = top
MODULE = tb

include $(shell cocotb-config --makefiles)/Makefile.sim
