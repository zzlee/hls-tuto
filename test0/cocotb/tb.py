import random

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, Timer, ClockCycles
from cocotb.types import LogicArray

@cocotb.test()
async def testbench0(dut):
    clock = Clock(dut.ap_clk, 10, units="ns")
    cocotb.start_soon(clock.start(start_high=False))

    await ClockCycles(dut.ap_clk, 1);
    dut.ap_rst.value = 1;
    await ClockCycles(dut.ap_clk, 1);
    dut.ap_rst.value = 0;
    await ClockCycles(dut.ap_clk, 1);

    dut.a.value = 5;
    dut.b.value = 9;
    dut.c.value = 12;
    dut.ap_start = 1;
    await RisingEdge(dut.ap_done);
    dut.ap_start = 0;

    await Timer(300, 'ns');
