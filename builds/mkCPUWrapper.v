module wrap( output reg Dout,
             input         Din,           
             input         clr,           
             input         shift,           
             input         ld,           
             input            CLK);

   parameter Nout = 623;
   parameter Nin  = 309;
   
   integer              i;

   reg [Nin-1:0]         shift_reg_i;
   always @(posedge CLK) begin
      if (clr) shift_reg_i <= 0;
      else if (shift) begin
         shift_reg_i[0] <= Din;
         for (i = 1; i < Nin ; i = i + 1) begin                        
            shift_reg_i[i] <= shift_reg_i[i-1];
         end
      end
   end // always @ (posedge CLK)

   // inputs 309
   wire           RST_N;
   wire           hart0_server_reset_request_put                ;
   wire           EN_hart0_server_reset_request_put             ;
   wire           EN_hart0_server_reset_response_get            ;
   wire           imem_master_awready                           ;
   wire           imem_master_wready                            ;
   wire           imem_master_bvalid                            ;
   wire  [3 : 0]  imem_master_bid                               ;
   wire  [1 : 0]  imem_master_bresp                             ;
   wire           imem_master_arready                           ;
   wire           imem_master_rvalid                            ;
   wire  [3 : 0]  imem_master_rid                               ;
   wire  [63 : 0] imem_master_rdata                             ;
   wire  [1 : 0]  imem_master_rresp                             ;
   wire           imem_master_rlast                             ;
   wire           mem_master_awready                            ;
   wire           mem_master_wready                             ;
   wire           mem_master_bvalid                             ;
   wire  [3 : 0]  mem_master_bid                                ;
   wire  [1 : 0]  mem_master_bresp                              ;
   wire           mem_master_arready                            ;
   wire           mem_master_rvalid                             ;
   wire  [3 : 0]  mem_master_rid                                ;
   wire  [63 : 0] mem_master_rdata                              ;
   wire  [1 : 0]  mem_master_rresp                              ;
   wire           mem_master_rlast                              ;
   wire           m_external_interrupt_req_set_not_clear        ;
   wire           s_external_interrupt_req_set_not_clear        ;
   wire           software_interrupt_req_set_not_clear          ;
   wire           timer_interrupt_req_set_not_clear             ;
   wire           nmi_req_set_not_clear                         ;
   wire  [3 : 0]  set_verbosity_verbosity                       ;
   wire  [63 : 0] set_verbosity_logdelay                        ;
   wire           EN_set_verbosity                              ;
   wire           set_watch_tohost_watch_tohost                 ;
   wire  [63 : 0] set_watch_tohost_tohost_addr                  ;
   wire           EN_set_watch_tohost                           ;
   wire           EN_ma_ddr4_ready                              ;
   
   assign {RST_N                                         ,
           hart0_server_reset_request_put                ,
           EN_hart0_server_reset_request_put             ,
           EN_hart0_server_reset_response_get            ,
           imem_master_awready                           ,
           imem_master_wready                            ,
           imem_master_bvalid                            ,
           imem_master_bid                               ,
           imem_master_bresp                             ,
           imem_master_arready                           ,
           imem_master_rvalid                            ,
           imem_master_rid                               ,
           imem_master_rdata                             ,
           imem_master_rresp                             ,
           imem_master_rlast                             ,
           mem_master_awready                            ,
           mem_master_wready                             ,
           mem_master_bvalid                             ,
           mem_master_bid                                ,
           mem_master_bresp                              ,
           mem_master_arready                            ,
           mem_master_rvalid                             ,
           mem_master_rid                                ,
           mem_master_rdata                              ,
           mem_master_rresp                              ,
           mem_master_rlast                              ,
           m_external_interrupt_req_set_not_clear        ,
           s_external_interrupt_req_set_not_clear        ,
           software_interrupt_req_set_not_clear          ,
           timer_interrupt_req_set_not_clear             ,
           nmi_req_set_not_clear                         ,
           set_verbosity_verbosity                       ,
           set_verbosity_logdelay                        ,
           EN_set_verbosity                              ,
           set_watch_tohost_watch_tohost                 ,
           set_watch_tohost_tohost_addr                  ,
           EN_set_watch_tohost                           ,
           EN_ma_ddr4_ready} = shift_reg_i;


   // outputs 623
   wire           RDY_hart0_server_reset_request_put  ;
   wire           hart0_server_reset_response_get     ;
   wire           RDY_hart0_server_reset_response_get ;
   wire           imem_master_awvalid                 ;
   wire [3 : 0]   imem_master_awid                    ;
   wire [63 : 0]  imem_master_awaddr                  ;
   wire [7 : 0]   imem_master_awlen                   ;
   wire [2 : 0]   imem_master_awsize                  ;
   wire [1 : 0]   imem_master_awburst                 ;
   wire           imem_master_awlock                  ;
   wire [3 : 0]   imem_master_awcache                 ;
   wire [2 : 0]   imem_master_awprot                  ;
   wire [3 : 0]   imem_master_awqos                   ;
   wire [3 : 0]   imem_master_awregion                ;
   wire           imem_master_wvalid                  ;
   wire [63 : 0]  imem_master_wdata                   ;
   wire [7 : 0]   imem_master_wstrb                   ;
   wire           imem_master_wlast                   ;
   wire           imem_master_bready                  ;
   wire           imem_master_arvalid                 ;
   wire [3 : 0]   imem_master_arid                    ;
   wire [63 : 0]  imem_master_araddr                  ;
   wire [7 : 0]   imem_master_arlen                   ;
   wire [2 : 0]   imem_master_arsize                  ;
   wire [1 : 0]   imem_master_arburst                 ;
   wire           imem_master_arlock                  ;
   wire [3 : 0]   imem_master_arcache                 ;
   wire [2 : 0]   imem_master_arprot                  ;
   wire [3 : 0]   imem_master_arqos                   ;
   wire [3 : 0]   imem_master_arregion                ;
   wire           imem_master_rready                  ;
   wire           mem_master_awvalid                  ;
   wire [3 : 0]   mem_master_awid                     ;
   wire [63 : 0]  mem_master_awaddr                   ;
   wire [7 : 0]   mem_master_awlen                    ;
   wire [2 : 0]   mem_master_awsize                   ;
   wire [1 : 0]   mem_master_awburst                  ;
   wire           mem_master_awlock                   ;
   wire [3 : 0]   mem_master_awcache                  ;
   wire [2 : 0]   mem_master_awprot                   ;
   wire [3 : 0]   mem_master_awqos                    ;
   wire [3 : 0]   mem_master_awregion                 ;
   wire           mem_master_wvalid                   ;
   wire [63 : 0]  mem_master_wdata                    ;
   wire [7 : 0]   mem_master_wstrb                    ;
   wire           mem_master_wlast                    ;
   wire           mem_master_bready                   ;
   wire           mem_master_arvalid                  ;
   wire [3 : 0]   mem_master_arid                     ;
   wire [63 : 0]  mem_master_araddr                   ;
   wire [7 : 0]   mem_master_arlen                    ;
   wire [2 : 0]   mem_master_arsize                   ;
   wire [1 : 0]   mem_master_arburst                  ;
   wire           mem_master_arlock                   ;
   wire [3 : 0]   mem_master_arcache                  ;
   wire [2 : 0]   mem_master_arprot                   ;
   wire [3 : 0]   mem_master_arqos                    ;
   wire [3 : 0]   mem_master_arregion                 ;
   wire           mem_master_rready                   ;
   wire           RDY_set_verbosity                   ;
   wire           RDY_set_watch_tohost                ;
   wire [63 : 0]  mv_tohost_value                     ;
   wire           RDY_mv_tohost_value                 ;
   wire           RDY_ma_ddr4_ready                   ;
   wire [7 : 0]   mv_status                           ;
   reg [Nout-1:0] shift_reg_o                         ;
   always @(posedge CLK) begin
      if (clr) shift_reg_o <= 0;
      else if (ld) shift_reg_o <= { RDY_hart0_server_reset_request_put  ,
                                    hart0_server_reset_response_get     ,
                                    RDY_hart0_server_reset_response_get ,
                                    imem_master_awvalid                 ,
                                    imem_master_awid                    ,
                                    imem_master_awaddr                  ,
                                    imem_master_awlen                   ,
                                    imem_master_awsize                  ,
                                    imem_master_awburst                 ,
                                    imem_master_awlock                  ,
                                    imem_master_awcache                 ,
                                    imem_master_awprot                  ,
                                    imem_master_awqos                   ,
                                    imem_master_awregion                ,
                                    imem_master_wvalid                  ,
                                    imem_master_wdata                   ,
                                    imem_master_wstrb                   ,
                                    imem_master_wlast                   ,
                                    imem_master_bready                  ,
                                    imem_master_arvalid                 ,
                                    imem_master_arid                    ,
                                    imem_master_araddr                  ,
                                    imem_master_arlen                   ,
                                    imem_master_arsize                  ,
                                    imem_master_arburst                 ,
                                    imem_master_arlock                  ,
                                    imem_master_arcache                 ,
                                    imem_master_arprot                  ,
                                    imem_master_arqos                   ,
                                    imem_master_arregion                ,
                                    imem_master_rready                  ,
                                    mem_master_awvalid                  ,
                                    mem_master_awid                     ,
                                    mem_master_awaddr                   ,
                                    mem_master_awlen                    ,
                                    mem_master_awsize                   ,
                                    mem_master_awburst                  ,
                                    mem_master_awlock                   ,
                                    mem_master_awcache                  ,
                                    mem_master_awprot                   ,
                                    mem_master_awqos                    ,
                                    mem_master_awregion                 ,
                                    mem_master_wvalid                   ,
                                    mem_master_wdata                    ,
                                    mem_master_wstrb                    ,
                                    mem_master_wlast                    ,
                                    mem_master_bready                   ,
                                    mem_master_arvalid                  ,
                                    mem_master_arid                     ,
                                    mem_master_araddr                   ,
                                    mem_master_arlen                    ,
                                    mem_master_arsize                   ,
                                    mem_master_arburst                  ,
                                    mem_master_arlock                   ,
                                    mem_master_arcache                  ,
                                    mem_master_arprot                   ,
                                    mem_master_arqos                    ,
                                    mem_master_arregion                 ,
                                    mem_master_rready                   ,
                                    RDY_set_verbosity                   ,
                                    RDY_set_watch_tohost                ,
                                    mv_tohost_value                     ,
                                    RDY_mv_tohost_value                 ,
                                    RDY_ma_ddr4_ready                   ,
                                    mv_status                           };

      else if (shift) begin
         Dout <= shift_reg_o[Nout-1];
         for (i = Nout-1; i > 0; i = i - 1) begin                        
            shift_reg_o[i] <= shift_reg_o[i-1];
         end
      end
   end // always @ (posedge CLK)
   

  mkCPU dut(.CLK                                      (CLK                                                   ),
            .RST_N                                    (RST_N                                                 ),
            .hart0_server_reset_request_put           (hart0_server_reset_request_put ),
            .imem_master_arready                      (imem_master_arready ),
            .imem_master_awready                      (imem_master_awready ),
            .imem_master_bid                          (imem_master_bid ),
            .imem_master_bresp                        (imem_master_bresp ),
            .imem_master_bvalid                       (imem_master_bvalid ),
            .imem_master_rdata                        (imem_master_rdata ),
            .imem_master_rid                          (imem_master_rid ),
            .imem_master_rlast                        (imem_master_rlast ),
            .imem_master_rresp                        (imem_master_rresp ),
            .imem_master_rvalid                       (imem_master_rvalid ),
            .imem_master_wready                       (imem_master_wready ),
            .m_external_interrupt_req_set_not_clear   (m_external_interrupt_req_set_not_clear ),
            .mem_master_arready                       (mem_master_arready ),
            .mem_master_awready                       (mem_master_awready ),
            .mem_master_bid                           (mem_master_bid ),
            .mem_master_bresp                         (mem_master_bresp ),
            .mem_master_bvalid                        (mem_master_bvalid ),
            .mem_master_rdata                         (mem_master_rdata ),
            .mem_master_rid                           (mem_master_rid ),
            .mem_master_rlast                         (mem_master_rlast ),
            .mem_master_rresp                         (mem_master_rresp ),
            .mem_master_rvalid                        (mem_master_rvalid ),
            .mem_master_wready                        (mem_master_wready ),
            .nmi_req_set_not_clear                    (nmi_req_set_not_clear ),
            .s_external_interrupt_req_set_not_clear   (s_external_interrupt_req_set_not_clear ),
            .set_verbosity_logdelay                   (set_verbosity_logdelay ),
            .set_verbosity_verbosity                  (set_verbosity_verbosity ),
            .set_watch_tohost_tohost_addr             (set_watch_tohost_tohost_addr ),
            .set_watch_tohost_watch_tohost            (set_watch_tohost_watch_tohost ),
            .software_interrupt_req_set_not_clear     (software_interrupt_req_set_not_clear ),
            .timer_interrupt_req_set_not_clear        (timer_interrupt_req_set_not_clear ),
            .EN_hart0_server_reset_request_put        (EN_hart0_server_reset_request_put ),
            .EN_hart0_server_reset_response_get       (EN_hart0_server_reset_response_get ),
            .EN_set_verbosity                         (EN_set_verbosity ),
            .EN_set_watch_tohost                      (EN_set_watch_tohost ),
            .EN_ma_ddr4_ready                         (EN_ma_ddr4_ready ),
            .RDY_hart0_server_reset_request_put       (RDY_hart0_server_reset_request_put ),
            .hart0_server_reset_response_get          (hart0_server_reset_response_get ),
            .RDY_hart0_server_reset_response_get      (RDY_hart0_server_reset_response_get ),
            .imem_master_awvalid                      (imem_master_awvalid ),
            .imem_master_awid                         (imem_master_awid ),
            .imem_master_awaddr                       (imem_master_awaddr ),
            .imem_master_awlen                        (imem_master_awlen ),
            .imem_master_awsize                       (imem_master_awsize ),
            .imem_master_awburst                      (imem_master_awburst ),
            .imem_master_awlock                       (imem_master_awlock ),
            .imem_master_awcache                      (imem_master_awcache ),
            .imem_master_awprot                       (imem_master_awprot ),
            .imem_master_awqos                        (imem_master_awqos ),
            .imem_master_awregion                     (imem_master_awregion ),
            .imem_master_wvalid                       (imem_master_wvalid ),
            .imem_master_wdata                        (imem_master_wdata ),
            .imem_master_wstrb                        (imem_master_wstrb ),
            .imem_master_wlast                        (imem_master_wlast ),
            .imem_master_bready                       (imem_master_bready ),
            .imem_master_arvalid                      (imem_master_arvalid ),
            .imem_master_arid                         (imem_master_arid ),
            .imem_master_araddr                       (imem_master_araddr ),
            .imem_master_arlen                        (imem_master_arlen ),
            .imem_master_arsize                       (imem_master_arsize ),
            .imem_master_arburst                      (imem_master_arburst ),
            .imem_master_arlock                       (imem_master_arlock ),
            .imem_master_arcache                      (imem_master_arcache ),
            .imem_master_arprot                       (imem_master_arprot ),
            .imem_master_arqos                        (imem_master_arqos ),
            .imem_master_arregion                     (imem_master_arregion ),
            .imem_master_rready                       (imem_master_rready ),
            .mem_master_awvalid                       (mem_master_awvalid ),
            .mem_master_awid                          (mem_master_awid ),
            .mem_master_awaddr                        (mem_master_awaddr ),
            .mem_master_awlen                         (mem_master_awlen ),
            .mem_master_awsize                        (mem_master_awsize ),
            .mem_master_awburst                       (mem_master_awburst ),
            .mem_master_awlock                        (mem_master_awlock ),
            .mem_master_awcache                       (mem_master_awcache ),
            .mem_master_awprot                        (mem_master_awprot ),
            .mem_master_awqos                         (mem_master_awqos ),
            .mem_master_awregion                      (mem_master_awregion ),
            .mem_master_wvalid                        (mem_master_wvalid ),
            .mem_master_wdata                         (mem_master_wdata ),
            .mem_master_wstrb                         (mem_master_wstrb ),
            .mem_master_wlast                         (mem_master_wlast ),
            .mem_master_bready                        (mem_master_bready ),
            .mem_master_arvalid                       (mem_master_arvalid ),
            .mem_master_arid                          (mem_master_arid ),
            .mem_master_araddr                        (mem_master_araddr ),
            .mem_master_arlen                         (mem_master_arlen ),
            .mem_master_arsize                        (mem_master_arsize ),
            .mem_master_arburst                       (mem_master_arburst ),
            .mem_master_arlock                        (mem_master_arlock ),
            .mem_master_arcache                       (mem_master_arcache ),
            .mem_master_arprot                        (mem_master_arprot ),
            .mem_master_arqos                         (mem_master_arqos ),
            .mem_master_arregion                      (mem_master_arregion ),
            .mem_master_rready                        (mem_master_rready ),
            .RDY_set_verbosity                        (RDY_set_verbosity ),
            .RDY_set_watch_tohost                     (RDY_set_watch_tohost ),
            .mv_tohost_value                          (mv_tohost_value ),
            .RDY_mv_tohost_value                      (RDY_mv_tohost_value ),
            .RDY_ma_ddr4_ready                        (RDY_ma_ddr4_ready ),
            .mv_status                                (mv_status ));

endmodule // wrap
