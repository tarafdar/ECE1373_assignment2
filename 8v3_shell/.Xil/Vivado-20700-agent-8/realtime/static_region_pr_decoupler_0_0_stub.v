// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "prd_static_region_pr_decoupler_0_0,Vivado 2017.2" *)
module static_region_pr_decoupler_0_0(s_gmem_ARVALID, rp_gmem_ARVALID, 
  s_gmem_ARREADY, rp_gmem_ARREADY, s_gmem_AWVALID, rp_gmem_AWVALID, s_gmem_AWREADY, 
  rp_gmem_AWREADY, s_gmem_BVALID, rp_gmem_BVALID, s_gmem_BREADY, rp_gmem_BREADY, 
  s_gmem_RVALID, rp_gmem_RVALID, s_gmem_RREADY, rp_gmem_RREADY, s_gmem_WVALID, 
  rp_gmem_WVALID, s_gmem_WREADY, rp_gmem_WREADY, s_gmem_AWID, rp_gmem_AWID, s_gmem_AWADDR, 
  rp_gmem_AWADDR, s_gmem_AWLEN, rp_gmem_AWLEN, s_gmem_AWSIZE, rp_gmem_AWSIZE, s_gmem_AWBURST, 
  rp_gmem_AWBURST, s_gmem_AWLOCK, rp_gmem_AWLOCK, s_gmem_AWCACHE, rp_gmem_AWCACHE, 
  s_gmem_AWPROT, rp_gmem_AWPROT, s_gmem_AWREGION, rp_gmem_AWREGION, s_gmem_AWQOS, 
  rp_gmem_AWQOS, s_gmem_WID, rp_gmem_WID, s_gmem_WDATA, rp_gmem_WDATA, s_gmem_WSTRB, 
  rp_gmem_WSTRB, s_gmem_WLAST, rp_gmem_WLAST, s_gmem_BID, rp_gmem_BID, s_gmem_BRESP, 
  rp_gmem_BRESP, s_gmem_ARID, rp_gmem_ARID, s_gmem_ARADDR, rp_gmem_ARADDR, s_gmem_ARLEN, 
  rp_gmem_ARLEN, s_gmem_ARSIZE, rp_gmem_ARSIZE, s_gmem_ARBURST, rp_gmem_ARBURST, 
  s_gmem_ARLOCK, rp_gmem_ARLOCK, s_gmem_ARCACHE, rp_gmem_ARCACHE, s_gmem_ARPROT, 
  rp_gmem_ARPROT, s_gmem_ARREGION, rp_gmem_ARREGION, s_gmem_ARQOS, rp_gmem_ARQOS, s_gmem_RID, 
  rp_gmem_RID, s_gmem_RDATA, rp_gmem_RDATA, s_gmem_RRESP, rp_gmem_RRESP, s_gmem_RLAST, 
  rp_gmem_RLAST, s_ctrl_ARVALID, rp_ctrl_ARVALID, s_ctrl_ARREADY, rp_ctrl_ARREADY, 
  s_ctrl_AWVALID, rp_ctrl_AWVALID, s_ctrl_AWREADY, rp_ctrl_AWREADY, s_ctrl_BVALID, 
  rp_ctrl_BVALID, s_ctrl_BREADY, rp_ctrl_BREADY, s_ctrl_RVALID, rp_ctrl_RVALID, 
  s_ctrl_RREADY, rp_ctrl_RREADY, s_ctrl_WVALID, rp_ctrl_WVALID, s_ctrl_WREADY, 
  rp_ctrl_WREADY, s_ctrl_AWADDR, rp_ctrl_AWADDR, s_ctrl_AWPROT, rp_ctrl_AWPROT, 
  s_ctrl_AWREGION, rp_ctrl_AWREGION, s_ctrl_AWQOS, rp_ctrl_AWQOS, s_ctrl_WDATA, 
  rp_ctrl_WDATA, s_ctrl_WSTRB, rp_ctrl_WSTRB, s_ctrl_BRESP, rp_ctrl_BRESP, s_ctrl_ARADDR, 
  rp_ctrl_ARADDR, s_ctrl_ARPROT, rp_ctrl_ARPROT, s_ctrl_ARREGION, rp_ctrl_ARREGION, 
  s_ctrl_ARQOS, rp_ctrl_ARQOS, s_ctrl_RDATA, rp_ctrl_RDATA, s_ctrl_RRESP, rp_ctrl_RRESP, aclk, 
  s_axi_reg_awaddr, s_axi_reg_awvalid, s_axi_reg_awready, s_axi_reg_wdata, 
  s_axi_reg_wvalid, s_axi_reg_wready, s_axi_reg_bresp, s_axi_reg_bvalid, s_axi_reg_bready, 
  s_axi_reg_araddr, s_axi_reg_arvalid, s_axi_reg_arready, s_axi_reg_rdata, s_axi_reg_rresp, 
  s_axi_reg_rvalid, s_axi_reg_rready, s_axi_reg_aresetn);
  output s_gmem_ARVALID;
  input rp_gmem_ARVALID;
  input s_gmem_ARREADY;
  output rp_gmem_ARREADY;
  output s_gmem_AWVALID;
  input rp_gmem_AWVALID;
  input s_gmem_AWREADY;
  output rp_gmem_AWREADY;
  input s_gmem_BVALID;
  output rp_gmem_BVALID;
  output s_gmem_BREADY;
  input rp_gmem_BREADY;
  input s_gmem_RVALID;
  output rp_gmem_RVALID;
  output s_gmem_RREADY;
  input rp_gmem_RREADY;
  output s_gmem_WVALID;
  input rp_gmem_WVALID;
  input s_gmem_WREADY;
  output rp_gmem_WREADY;
  output [4:0]s_gmem_AWID;
  input [4:0]rp_gmem_AWID;
  output [63:0]s_gmem_AWADDR;
  input [63:0]rp_gmem_AWADDR;
  output [7:0]s_gmem_AWLEN;
  input [7:0]rp_gmem_AWLEN;
  output [2:0]s_gmem_AWSIZE;
  input [2:0]rp_gmem_AWSIZE;
  output [1:0]s_gmem_AWBURST;
  input [1:0]rp_gmem_AWBURST;
  output [0:0]s_gmem_AWLOCK;
  input [0:0]rp_gmem_AWLOCK;
  output [3:0]s_gmem_AWCACHE;
  input [3:0]rp_gmem_AWCACHE;
  output [2:0]s_gmem_AWPROT;
  input [2:0]rp_gmem_AWPROT;
  output [3:0]s_gmem_AWREGION;
  input [3:0]rp_gmem_AWREGION;
  output [3:0]s_gmem_AWQOS;
  input [3:0]rp_gmem_AWQOS;
  output [4:0]s_gmem_WID;
  input [4:0]rp_gmem_WID;
  output [127:0]s_gmem_WDATA;
  input [127:0]rp_gmem_WDATA;
  output [15:0]s_gmem_WSTRB;
  input [15:0]rp_gmem_WSTRB;
  output s_gmem_WLAST;
  input rp_gmem_WLAST;
  input [4:0]s_gmem_BID;
  output [4:0]rp_gmem_BID;
  input [1:0]s_gmem_BRESP;
  output [1:0]rp_gmem_BRESP;
  output [4:0]s_gmem_ARID;
  input [4:0]rp_gmem_ARID;
  output [63:0]s_gmem_ARADDR;
  input [63:0]rp_gmem_ARADDR;
  output [7:0]s_gmem_ARLEN;
  input [7:0]rp_gmem_ARLEN;
  output [2:0]s_gmem_ARSIZE;
  input [2:0]rp_gmem_ARSIZE;
  output [1:0]s_gmem_ARBURST;
  input [1:0]rp_gmem_ARBURST;
  output [0:0]s_gmem_ARLOCK;
  input [0:0]rp_gmem_ARLOCK;
  output [3:0]s_gmem_ARCACHE;
  input [3:0]rp_gmem_ARCACHE;
  output [2:0]s_gmem_ARPROT;
  input [2:0]rp_gmem_ARPROT;
  output [3:0]s_gmem_ARREGION;
  input [3:0]rp_gmem_ARREGION;
  output [3:0]s_gmem_ARQOS;
  input [3:0]rp_gmem_ARQOS;
  input [4:0]s_gmem_RID;
  output [4:0]rp_gmem_RID;
  input [127:0]s_gmem_RDATA;
  output [127:0]rp_gmem_RDATA;
  input [1:0]s_gmem_RRESP;
  output [1:0]rp_gmem_RRESP;
  input s_gmem_RLAST;
  output rp_gmem_RLAST;
  input s_ctrl_ARVALID;
  output rp_ctrl_ARVALID;
  output s_ctrl_ARREADY;
  input rp_ctrl_ARREADY;
  input s_ctrl_AWVALID;
  output rp_ctrl_AWVALID;
  output s_ctrl_AWREADY;
  input rp_ctrl_AWREADY;
  output s_ctrl_BVALID;
  input rp_ctrl_BVALID;
  input s_ctrl_BREADY;
  output rp_ctrl_BREADY;
  output s_ctrl_RVALID;
  input rp_ctrl_RVALID;
  input s_ctrl_RREADY;
  output rp_ctrl_RREADY;
  input s_ctrl_WVALID;
  output rp_ctrl_WVALID;
  output s_ctrl_WREADY;
  input rp_ctrl_WREADY;
  input [31:0]s_ctrl_AWADDR;
  output [31:0]rp_ctrl_AWADDR;
  input [2:0]s_ctrl_AWPROT;
  output [2:0]rp_ctrl_AWPROT;
  input [3:0]s_ctrl_AWREGION;
  output [3:0]rp_ctrl_AWREGION;
  input [3:0]s_ctrl_AWQOS;
  output [3:0]rp_ctrl_AWQOS;
  input [31:0]s_ctrl_WDATA;
  output [31:0]rp_ctrl_WDATA;
  input [3:0]s_ctrl_WSTRB;
  output [3:0]rp_ctrl_WSTRB;
  output [1:0]s_ctrl_BRESP;
  input [1:0]rp_ctrl_BRESP;
  input [31:0]s_ctrl_ARADDR;
  output [31:0]rp_ctrl_ARADDR;
  input [2:0]s_ctrl_ARPROT;
  output [2:0]rp_ctrl_ARPROT;
  input [3:0]s_ctrl_ARREGION;
  output [3:0]rp_ctrl_ARREGION;
  input [3:0]s_ctrl_ARQOS;
  output [3:0]rp_ctrl_ARQOS;
  output [31:0]s_ctrl_RDATA;
  input [31:0]rp_ctrl_RDATA;
  output [1:0]s_ctrl_RRESP;
  input [1:0]rp_ctrl_RRESP;
  input aclk;
  input [0:0]s_axi_reg_awaddr;
  input s_axi_reg_awvalid;
  output s_axi_reg_awready;
  input [31:0]s_axi_reg_wdata;
  input s_axi_reg_wvalid;
  output s_axi_reg_wready;
  output [1:0]s_axi_reg_bresp;
  output s_axi_reg_bvalid;
  input s_axi_reg_bready;
  input [0:0]s_axi_reg_araddr;
  input s_axi_reg_arvalid;
  output s_axi_reg_arready;
  output [31:0]s_axi_reg_rdata;
  output [1:0]s_axi_reg_rresp;
  output s_axi_reg_rvalid;
  input s_axi_reg_rready;
  input s_axi_reg_aresetn;
endmodule
