/* ****************************************************************************
  SPDX-License-Identifier: CERN-OHL-W-2.0

  Description: Store buffer
  Currently a simple single clock FIFO, but with the ambition to
  have combining and reordering capabilities in the future.

  Copyright (C) 2013 Stefan Kristiansson <stefan.kristiansson@saunalahti.fi>

 ******************************************************************************/
//`include "mor1kx-defines.v"

module mor1kx_store_buffer
  #(
    parameter DEPTH_WIDTH = 4,
    parameter OPTION_OPERAND_WIDTH = 32
    )
   (
    input 				clk,
    input 				rst,

    input [OPTION_OPERAND_WIDTH-1:0] 	pc_i,
    input [OPTION_OPERAND_WIDTH-1:0] 	adr_i,
    input [OPTION_OPERAND_WIDTH-1:0] 	dat_i,
    input [OPTION_OPERAND_WIDTH/8-1:0] 	bsel_i,
    input 				atomic_i,
    input 				write_i,

    output [OPTION_OPERAND_WIDTH-1:0] 	pc_o,
    output [OPTION_OPERAND_WIDTH-1:0] 	adr_o,
    output [OPTION_OPERAND_WIDTH-1:0] 	dat_o,
    output [OPTION_OPERAND_WIDTH/8-1:0] bsel_o,
    output 				atomic_o,
    input 				read_i,

    output 				full_o,
    output 				empty_o
    );

   // The fifo stores address + data + byte sel + pc + atomic
   localparam FIFO_DATA_WIDTH = OPTION_OPERAND_WIDTH*3 +
				OPTION_OPERAND_WIDTH/8 + 1;

   wire [FIFO_DATA_WIDTH-1:0] 		fifo_dout;
   wire [FIFO_DATA_WIDTH-1:0] 		fifo_din;

   reg [DEPTH_WIDTH:0]                  write_pointer;
   reg [DEPTH_WIDTH:0]                  read_pointer;

   assign fifo_din = {adr_i, dat_i, bsel_i, pc_i, atomic_i};
   assign {adr_o, dat_o, bsel_o, pc_o, atomic_o} = fifo_dout;

   assign full_o = (write_pointer[DEPTH_WIDTH] != read_pointer[DEPTH_WIDTH]) &&
                   (write_pointer[DEPTH_WIDTH-1:0] == read_pointer[DEPTH_WIDTH-1:0]);
   assign empty_o = write_pointer == read_pointer;

   always @(posedge clk `OR_ASYNC_RST)
     if (rst)
       write_pointer <= 0;
     else if (write_i)
       write_pointer <= write_pointer + 1'd1;

   always @(posedge clk `OR_ASYNC_RST)
     if (rst)
       read_pointer <= 0;
     else if (read_i)
       read_pointer <= read_pointer + 1'd1;

   mor1kx_simple_dpram_sclk
     #(
       .ADDR_WIDTH(DEPTH_WIDTH),
       .DATA_WIDTH(FIFO_DATA_WIDTH),
       .ENABLE_BYPASS(1)
       )
   fifo_ram
     (
      .clk			(clk),
      .dout			(fifo_dout),
      .raddr			(read_pointer[DEPTH_WIDTH-1:0]),
      .re			(read_i),
      .waddr			(write_pointer[DEPTH_WIDTH-1:0]),
      .we			(write_i),
      .din			(fifo_din)
      );

endmodule
