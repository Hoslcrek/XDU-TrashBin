`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/16 13:26:05
// Design Name: 
// Module Name: PWM
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module PWM(
	clk,
	rst_n,
	mode,
	pwm_out,
	);

input			clk;				//输入50Mhz时钟信号
input			rst_n;				//复位信号,低电平复位
input 			[2:0]mode;			    //角度选择
output wire		pwm_out;				//pwm输出信号

reg 			pwm = 0;
reg			[12:0] i = 0;
reg			[12:0] cnt = 0;	//分频计数器
reg         div_50khz = 0;

assign cnt0 = cnt;
assign pwm_out = pwm;

always@(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
		i <= 0;
		div_50khz <= 0;
	end
	else if (i == 12'd499) begin
		div_50khz <= ~div_50khz;
		i <= 0;
	end 
	else
		i <= i+1;
end
//cnt
always@(posedge div_50khz or negedge rst_n)begin
	if(!rst_n)
		cnt <= 12'd0;
	else if(cnt == 12'd 999)
		cnt <= 12'd0;
	else
		cnt <= cnt + 1'd1;
end

//pwm_out
always@(posedge div_50khz or negedge rst_n) begin
	if(!rst_n)
		pwm <= 1'd0;
	else begin
		case(mode) //0.02*100=2  0.02*75=1.5 
			4'd0:pwm <= (cnt < 25  ) ? 1'd1 : 1'd0; //0
			4'd1:pwm <= (cnt < 50  ) ? 1'd1 : 1'd0; //CCW45
			4'd2:pwm <= (cnt < 75  ) ? 1'd1 : 1'd0; //CCW90
			4'd3:pwm <= (cnt < 100 ) ? 1'd1 : 1'd0; //CCW135
			4'd4:pwm <= (cnt < 125 ) ? 1'd1 : 1'd0; //CCW180			
			default:pwm <= 1'd0;
		endcase
	end
end

endmodule 
