`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


module c17_tb;

reg N1,N2,N3,N6,N7;
reg [2:0]score;
wire N22,N23;

c17 uut(.gat1(N1),.gat2(N2),.gat3(N3),.gat6(N6),.gat7(N7),.gat_out22(N22),.gat_out23(N23));


initial begin


score <= 3'b000;

//00000//
N1 <= 0 ; N2 <= 0;  N3 <= 0 ; N6 <= 0 ; N7 <= 0 ;
#10
$display(" input pattern = ",N1,N2,N3,N6,N7, "--> golden value = 00 "); 
$display(" your answer = ", N22, N23); 
if (N22 == 1'b0 && N23 == 1'b0)
begin
	score = score + 3'b001;
end

//10101//
N1 <= 1 ; N2 <= 0;  N3 <= 1 ; N6 <= 0 ; N7 <= 1 ;
#10
$display(" input pattern = ",N1,N2,N3,N6,N7, "--> golden value = 11 "); 
$display(" your answer = ", N22, N23); 
if (N22 == 1'b1 && N23 == 1'b1)
begin
	score = score + 3'b001;
end

//01010//
N1 <= 0 ; N2 <= 1;  N3 <= 0 ; N6 <= 1 ; N7 <= 0 ;
#10
$display(" input pattern = ",N1,N2,N3,N6,N7, "--> golden value = 11 "); 
$display(" your answer = ", N22, N23); 
if (N22 == 1'b1 && N23 == 1'b1)
begin
	score = score + 3'b001;
end

//11011//
N1 <= 1 ; N2 <= 1;  N3 <= 0 ; N6 <= 1 ; N7 <= 1 ;
#10
$display(" input pattern = ",N1,N2,N3,N6,N7, "--> golden value = 11 "); 
$display(" your answer = ", N22, N23); 
if (N22 == 1'b1 && N23 == 1'b1)
begin
	score = score + 3'b001;
end

//11111//
N1 <= 1 ; N2 <= 1;  N3 <= 1 ; N6 <= 1 ; N7 <= 1 ;
#10
$display(" input pattern = ",N1,N2,N3,N6,N7, "--> golden value = 10 "); 
$display(" your answer = ", N22, N23); 
if (N22 == 1'b1 && N23 == 1'b0)
begin
	score = score + 3'b001;
end

if (score == 3'b101)
begin
$display("CORRECT!!!");
$display("                                                       ");
$display("                                                       ");
$display("                  ...XXXXXXXXXXXX..                    ");
$display("                .XXXXXXX.......XXXXXX.                 ");
$display("              XXXXX....     .......XXXX.               ");
$display("            .XXX.......     ....... ..XX.              ");
$display("           .XX.  ..             ...   .XX              ");
$display("          .XX..   .XXX.     .XXX.      .XX             ");
$display("          XXX.....XXXXX.    XXXXX     ..XX.            ");
$display("       ..XXX.......XXX.......XXX........XXX            ");
$display("    XXXXXXXX..XXXXXXXXXXXXXXXXXXXXXX.....XXXX.         ");
$display("  XXX.    XXXX..XXXXX........XXXXXX.. ...XXXXXXXXX.    ");
$display(" XX.      XXX.......XXXXXXXXXXX.....XXXXXX      .XXX.  ");
$display(" .XXX.    .XX.. ..  ...................XXX         XX. ");
$display("   .XXXXX. .XX.                     ..XXX        .XXX. ");
$display("     ...XXXXXXXX.                 ...XXX  ..XXXXXXX.   ");
$display("          ..XXXXX.             ....XXXXXXXXXX...       ");
$display("               .XXXX...........XXXXXX.                 ");
$display("                 .XXXXXXXXXXXXXXXXXXX.                 ");
$display("                  XXXXXXXXXXXXXXXXXXXX                 ");
$display("                  XXXXXXXXXXXXXXXXXXXX                 ");
$display("                  XXXXXXXXXXXXXXXXXXXX                 ");
$display("                                                       ");
end
else
begin
$display("WRONG!!!!!");
$display("                                                       ");
$display("                .XXXXXXXXXX..                          ");
$display("           ..XXXX.....  ....XXX..                      ");
$display("      ..XXXXXX...XX.   .XXX....XX                      ");
$display("     .XXXXXXX  XX...     ...X. .XX                     ");
$display("    XXX....XX. .  XX     XX      XXX                   ");
$display("   XX......XX.                    XX                   ");
$display("   XX....XXXX      ..XXXXX..      .XX                  ");
$display("   XXXX.X..      .XX.    ..XX.     XX                  ");
$display("  .X..XXX.    .....         ..     XX                  ");
$display(" .XX  XXXXX..XXXXX     .           XX                  ");
$display(" .X   .XXXXXX...XX..XXXXX.         X.                  ");
$display(" ..    .XX.....XXXXXXXXXXXX.                           ");
$display("  .....XXXXXXXXXX.   .. .XXXXX..                       ");
$display("XXXXXXXXX...         XX.   .XXXXX.                     ");
$display("X.....               XX.     ..XXXX.                   ");
$display("              ...X   XX.         XXXXX.                ");
$display("     .....XXXXXXXX   XX.           .XXXX..             ");
$display(".XXXXXXXXXXX....XX   XX.             ..XXXXX.          ");
$display("XXXX.....      .XX   XX.                .XXXXX.        ");
$display("              ..XX   XX.                   .XXXX..     ");
$display("       .....XXXXXX   XX.                     .XXXXX..  ");
$display("                                                       ");
end
end
  
endmodule
