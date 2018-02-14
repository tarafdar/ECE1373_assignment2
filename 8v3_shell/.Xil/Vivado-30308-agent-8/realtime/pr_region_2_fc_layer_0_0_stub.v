// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "fc_layer,Vivado 2017.2" *)
module pr_region_2_fc_layer_0_0(s_axi_CTRL_BUS_AWADDR, 
  s_axi_CTRL_BUS_AWVALID, s_axi_CTRL_BUS_AWREADY, s_axi_CTRL_BUS_WDATA, 
  s_axi_CTRL_BUS_WSTRB, s_axi_CTRL_BUS_WVALID, s_axi_CTRL_BUS_WREADY, 
  s_axi_CTRL_BUS_BRESP, s_axi_CTRL_BUS_BVALID, s_axi_CTRL_BUS_BREADY, 
  s_axi_CTRL_BUS_ARADDR, s_axi_CTRL_BUS_ARVALID, s_axi_CTRL_BUS_ARREADY, 
  s_axi_CTRL_BUS_RDATA, s_axi_CTRL_BUS_RRESP, s_axi_CTRL_BUS_RVALID, 
  s_axi_CTRL_BUS_RREADY, ap_clk, ap_rst_n, interrupt, m_axi_mem_AWADDR, m_axi_mem_AWLEN, 
  m_axi_mem_AWSIZE, m_axi_mem_AWBURST, m_axi_mem_AWLOCK, m_axi_mem_AWREGION, 
  m_axi_mem_AWCACHE, m_axi_mem_AWPROT, m_axi_mem_AWQOS, m_axi_mem_AWVALID, 
  m_axi_mem_AWREADY, m_axi_mem_WDATA, m_axi_mem_WSTRB, m_axi_mem_WLAST, m_axi_mem_WVALID, 
  m_axi_mem_WREADY, m_axi_mem_BRESP, m_axi_mem_BVALID, m_axi_mem_BREADY, m_axi_mem_ARADDR, 
  m_axi_mem_ARLEN, m_axi_mem_ARSIZE, m_axi_mem_ARBURST, m_axi_mem_ARLOCK, 
  m_axi_mem_ARREGION, m_axi_mem_ARCACHE, m_axi_mem_ARPROT, m_axi_mem_ARQOS, 
  m_axi_mem_ARVALID, m_axi_mem_ARREADY, m_axi_mem_RDATA, m_axi_mem_RRESP, m_axi_mem_RLAST, 
  m_axi_mem_RVALID, m_axi_mem_RREADY);
  input [5:0]s_axi_CTRL_BUS_AWADDR;
  input s_axi_CTRL_BUS_AWVALID;
  output s_axi_CTRL_BUS_AWREADY;
  input [31:0]s_axi_CTRL_BUS_WDATA;
  input [3:0]s_axi_CTRL_BUS_WSTRB;
  input s_axi_CTRL_BUS_WVALID;
  output s_axi_CTRL_BUS_WREADY;
  output [1:0]s_axi_CTRL_BUS_BRESP;
  output s_axi_CTRL_BUS_BVALID;
  input s_axi_CTRL_BUS_BREADY;
  input [5:0]s_axi_CTRL_BUS_ARADDR;
  input s_axi_CTRL_BUS_ARVALID;
  output s_axi_CTRL_BUS_ARREADY;
  output [31:0]s_axi_CTRL_BUS_RDATA;
  output [1:0]s_axi_CTRL_BUS_RRESP;
  output s_axi_CTRL_BUS_RVALID;
  input s_axi_CTRL_BUS_RREADY;
  input ap_clk;
  input ap_rst_n;
  output interrupt;
  output [63:0]m_axi_mem_AWADDR;
  output [7:0]m_axi_mem_AWLEN;
  output [2:0]m_axi_mem_AWSIZE;
  output [1:0]m_axi_mem_AWBURST;
  output [1:0]m_axi_mem_AWLOCK;
  output [3:0]m_axi_mem_AWREGION;
  output [3:0]m_axi_mem_AWCACHE;
  output [2:0]m_axi_mem_AWPROT;
  output [3:0]m_axi_mem_AWQOS;
  output m_axi_mem_AWVALID;
  input m_axi_mem_AWREADY;
  output [31:0]m_axi_mem_WDATA;
  output [3:0]m_axi_mem_WSTRB;
  output m_axi_mem_WLAST;
  output m_axi_mem_WVALID;
  input m_axi_mem_WREADY;
  input [1:0]m_axi_mem_BRESP;
  input m_axi_mem_BVALID;
  output m_axi_mem_BREADY;
  output [63:0]m_axi_mem_ARADDR;
  output [7:0]m_axi_mem_ARLEN;
  output [2:0]m_axi_mem_ARSIZE;
  output [1:0]m_axi_mem_ARBURST;
  output [1:0]m_axi_mem_ARLOCK;
  output [3:0]m_axi_mem_ARREGION;
  output [3:0]m_axi_mem_ARCACHE;
  output [2:0]m_axi_mem_ARPROT;
  output [3:0]m_axi_mem_ARQOS;
  output m_axi_mem_ARVALID;
  input m_axi_mem_ARREADY;
  input [31:0]m_axi_mem_RDATA;
  input [1:0]m_axi_mem_RRESP;
  input m_axi_mem_RLAST;
  input m_axi_mem_RVALID;
  output m_axi_mem_RREADY;
endmodule
