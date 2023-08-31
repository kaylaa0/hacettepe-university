`timescale 1s / 100ms
module controller(
    input clk, //1s = 1 cycle
    input cong, //congestion
    output side_state, //side road state, 0 - 1 - 2 - 3 for GO - GO-ATTENTION - STOP - STOP-ATTENTION
    output main_state //main road state, 0 - 1 - 2 - 3 for STOP - STOP-ATTENTION - GO - GO-ATTENTION
    );
    integer counter; // max counter value is 20, can be replaced with a [4:0] register.
    reg [2:0] state; //first bit represents congestion
    reg [1:0] side_state;
    reg [1:0] main_state;
    initial begin
        state=0; // Beginning state is side road GO main road STOP
        side_state = state[1:0];
        main_state = state[1:0];
        counter=11;
    end
    always@(posedge clk) begin
        if(state == 0) begin
            if (cong == 1) begin
                counter=3;
                state=1;
            end
            else if (counter == 1) begin
                counter=3;
                state=1;
            end
            else begin
                counter=counter-1;
            end
        end
        else if(state == 1) begin
            if (counter == 1) begin
                counter=20;
                state=2;
            end
            else begin
                counter=counter-1;
            end
        end
        else if(state == 2) begin
            if (counter == 1) begin
                if (cong == 1) begin
                    counter=20;
                    state=6;
                end
                else begin
                    counter=3;
                    state=3;
                end
            end
            else begin
                counter=counter-1;
            end
        end
        else if(state == 3) begin
            if (counter == 1) begin
                counter=10;
                state=0;
            end
            else begin
                counter=counter-1;
            end
        end
        //state 4, there being a congestion and side road being GO is imposible.
        else if(state == 5) begin
            if (counter == 1) begin
                counter=20;
                state=6;
            end
            else begin
                counter=counter-1;
            end
        end
        else if(state == 6) begin
            if (counter == 1) begin
                if (cong == 1) begin
                    counter=1;
                end
                else begin
                    counter=3;
                    state=3;
                end
            end
            else begin
                counter=counter-1;
            end
        end
        //state 7 is same with state 3.
        side_state = state[1:0];
        main_state = state[1:0];
        //Displaying signals
        if(main_state == 0) begin
            $display("Current time: %d    Counter: %d    Main Road: STOP              Side Roads: GO",$time,counter);
        end
        else if(main_state == 1) begin
            $display("Current time: %d    Counter: %d    Main Road: STOP-ATTENTION    Side Roads: GO-ATTENTION",$time,counter);
        end
        else if(main_state == 2) begin
            $display("Current time: %d    Counter: %d    Main Road: GO                Side Roads: STOP",$time,counter);
        end
        else if(main_state == 3) begin
            $display("Current time: %d    Counter: %d    Main Road: GO-ATTENTION      Side Roads: STOP-ATTENTION",$time,counter);
        end
    end
endmodule
