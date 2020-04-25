//*****************************************************************************
// qwi09_webled.v
//
// This module is the top wrapper of qwi09_webled project.
//
// Change History:
//  VER.   Author         DATE              Change Description
//  1.0    Qiwei Wu       Apr. 21, 2019     Initial Release
//*****************************************************************************

module qwi09_webled
(
   // DDR interface
   inout     [14:0]   DDR_addr         ,
	inout     [2:0]    DDR_ba           ,
	inout              DDR_cas_n        ,
	inout              DDR_ck_n         ,
	inout              DDR_ck_p         ,
	inout              DDR_cke          ,
	inout              DDR_cs_n         ,
	inout     [3:0]    DDR_dm           ,
	inout     [31:0]   DDR_dq           ,
	inout     [3:0]    DDR_dqs_n        ,
	inout     [3:0]    DDR_dqs_p        ,
	inout              DDR_odt          ,
	inout              DDR_ras_n        ,
	inout              DDR_reset_n      ,
	inout              DDR_we_n         ,
   // FIXED_IO interface
	inout              FIXED_IO_ddr_vrn ,
	inout              FIXED_IO_ddr_vrp ,
	inout     [53:0]   FIXED_IO_mio     ,
	inout              FIXED_IO_ps_clk  ,
	inout              FIXED_IO_ps_porb ,
	inout              FIXED_IO_ps_srstb,
   // LED
   output             LED
);

//*****************************************************************************
// Includes
//*****************************************************************************
`include "Define.vh"

//*****************************************************************************
// Signals
//*****************************************************************************
wire [12:0] reg_ctrl_addr;
wire        reg_ctrl_clk;
wire [31:0] reg_ctrl_din;
wire [31:0] reg_ctrl_dout;
wire        reg_ctrl_en;
wire        reg_ctrl_rst;
wire [3:0]  reg_ctrl_we;

wire [31:0] reg_fw_ver;
wire [31:0] reg_led_ctrl;

//*****************************************************************************
// Maps
//*****************************************************************************
assign LED = reg_led_ctrl[0];

system u_system
(
   // DDR interface
   .DDR_addr               ( DDR_addr ),
   .DDR_ba                 ( DDR_ba ),
   .DDR_cas_n              ( DDR_cas_n ),
   .DDR_ck_n               ( DDR_ck_n ),
   .DDR_ck_p               ( DDR_ck_p ),
   .DDR_cke                ( DDR_cke ),
   .DDR_cs_n               ( DDR_cs_n ),
   .DDR_dm                 ( DDR_dm ),
   .DDR_dq                 ( DDR_dq ),
   .DDR_dqs_n              ( DDR_dqs_n ),
   .DDR_dqs_p              ( DDR_dqs_p ),
   .DDR_odt                ( DDR_odt ),
   .DDR_ras_n              ( DDR_ras_n ),
   .DDR_reset_n            ( DDR_reset_n ),
   .DDR_we_n               ( DDR_we_n ),
   // FIXED_IO interface
   .FIXED_IO_ddr_vrn       ( FIXED_IO_ddr_vrn ),
   .FIXED_IO_ddr_vrp       ( FIXED_IO_ddr_vrp ),
   .FIXED_IO_mio           ( FIXED_IO_mio ),
   .FIXED_IO_ps_clk        ( FIXED_IO_ps_clk ),
   .FIXED_IO_ps_porb       ( FIXED_IO_ps_porb ),
   .FIXED_IO_ps_srstb      ( FIXED_IO_ps_srstb ),
   //Reg control
   .REG_CTRL_addr          ( reg_ctrl_addr ),
   .REG_CTRL_clk           ( reg_ctrl_clk ),
   .REG_CTRL_din           ( reg_ctrl_din ),
   .REG_CTRL_dout          ( reg_ctrl_dout ),
   .REG_CTRL_en            ( reg_ctrl_en ),
   .REG_CTRL_rst           ( reg_ctrl_rst ),
   .REG_CTRL_we            ( reg_ctrl_we )
);

//*****************************************************************************
// Reg control
//*****************************************************************************
qwiregctrl
#(
   .REGCNT                 ( REG_CNT ),
   .AWID                   ( 9 ),
   .DWID                   ( 32 )
)
u_qwiregctrl
(  
   .sys_rst                ( 1'b0 ),
   .reg_ce                 ( reg_ctrl_en ),
   .reg_rst                ( reg_ctrl_rst ),
   .reg_clk                ( reg_ctrl_clk ),
   .reg_we                 ( reg_ctrl_we ),
   .reg_addr               ( reg_ctrl_addr[12:4] ),
   .reg_wrd                ( reg_ctrl_din ),
   .reg_rdd                ( reg_ctrl_dout ),

   .reg_out                ( {{reg_led_ctrl,
                               reg_fw_ver }} ),
   .reg_in                 ( {{reg_led_ctrl,
                               reg_fw_ver }} )
);

endmodule
