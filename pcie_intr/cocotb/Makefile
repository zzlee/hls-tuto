TOPLEVEL_LANG = verilog
SIM ?= icarus
WAVES ?= 1
RTL_SRC = ../pcie_intr/hls/impl/verilog

VERILOG_SOURCES = \
$(wildcard ${RTL_SRC}/%.v)

TOPLEVEL = pcie_intr
MODULE = tb

include $(shell cocotb-config --makefiles)/Makefile.sim
