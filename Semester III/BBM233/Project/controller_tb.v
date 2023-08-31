`timescale 1s / 100ms
module controller_tb;
    reg clk, cong;
    // Outputs
    wire [1:0] side_state, main_state;
    initial begin
       cong = 0;
       clk = 1;
        forever begin
            #0.5;
            clk = ~clk; // change every 500000000ns = 0.5s
        end
    end
    // Instantiate the Unit Under Test (UUT)
    controller uut(.clk(clk), .cong(cong), .side_state(side_state), .main_state(main_state));
    always @ (posedge clk) begin
            //cong=1; set this if you want to test congestion at the beginning but other timing won't be right
            #40
            cong=1;// congestion at side road GO lasting 60 seconds.
            #60
            cong=0;
            #10
            cong=1;// congestion at the and of a side road GO lasting 10 seconds.
            #10
            cong=0;
            #13
            cong=1;// congestion at 20 second of main road GO lasting 5 seconds.
            #5
            cong=0;
            #15
            cong=1;// congestion at 40 second of main road GO lasting 10 seconds.
            #10
            cong=0;
            #24
            cong=1;// congestion at main road STOP-ATTENTION lasting 50 seconds.
            #50
            cong=0;
            #1
            cong=1;// congestion at main road GO-ATTENTION lasting 20 seconds.
            #20
            cong=0;
            #20
            cong=1;// congestion at main road GO-ATTENTION ending within.
            #2
            cong=0;
            #20;// at this point we are at a 20 second of a main road GO.
    end
endmodule
