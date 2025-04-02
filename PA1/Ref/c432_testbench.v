`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


module c432_tb;

reg N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,
      N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,
      N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,
      N99,N102,N105,N108,N112,N115;
reg [2:0]score;
wire N223,N329,N370,N421,N430,N431,N432;

c432 uut(.gat1(N1),.gat4(N4),.gat8(N8),.gat11(N11),.gat14(N14),.gat17(N17),.gat21(N21),.gat24(N24),.gat27(N27),.gat30(N30),.gat34(N34),.gat37(N37),.gat40(N40),.gat43(N43),.gat47(N47),.gat50(N50),.gat53(N53),.gat56(N56),.gat60(N60),.gat63(N63),.gat66(N66),.gat69(N69),.gat73(N73),.gat76(N76),.gat79(N79),.gat82(N82),.gat86(N86),.gat89(N89),.gat92(N92),.gat95(N95),.gat99(N99),.gat102(N102),.gat105(N105),.gat108(N108),.gat112(N112),.gat115(N115),.gat_out223(N223),.gat_out329(N329),.gat_out370(N370),.gat_out421(N421),.gat_out430(N430),.gat_out431(N431),.gat_out432(N432));


initial begin


score <= 3'b000;

//111110000011111000001111100000111110//
N1 <= 1 ; N4 <= 1;  N8 <= 1 ; N11 <= 1 ; N14 <= 1 ; N17 <= 0 ; N21 <= 0;  N24 <= 0 ; N27 <= 0 ; N30 <= 0 ; N34 <= 1 ; N37 <= 1 ;  N40 <=  1; N43 <= 1 ; N47 <= 1 ; N50 <= 0 ; N53 <= 0;  N56 <= 0 ; N60 <= 0 ; N63 <= 0 ; N66 <= 1 ; N69 <= 1;  N73 <= 1 ; N76 <= 1 ; N79 <= 1 ; N82 <= 0 ; N86 <= 0 ; N89 <= 0 ; N92 <= 0 ; N95 <= 0 ; N99 <= 1 ; N102 <= 1 ; N105 <= 1 ; N108 <= 1 ; N112 <= 1 ; N115 <= 0 ;
#10
$display(" input pattern = ",N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,N99,N102,N105,N108,N112,N115, "--> golden value = 1001011 "); 
$display(" your answer = " , N223,N329,N370,N421,N430,N431,N432); 
if (N223 == 1'b1 && N329 == 1'b0 && N370 == 1'b0 && N421 == 1'b1 && N430 == 1'b0&& N431 == 1'b1 && N432 == 1'b1 )
begin
	score = score + 3'b001;
end

//110011111111111111110000000000001100//
N1 <= 1 ; N4 <= 1;  N8 <= 0 ; N11 <= 0 ; N14 <= 1 ; N17 <= 1 ; N21 <= 1;  N24 <= 1 ; N27 <= 1 ; N30 <= 1 ; N34 <= 1 ; N37 <= 1 ;  N40 <=  1; N43 <= 1 ; N47 <= 1 ; N50 <= 1 ; N53 <= 1;  N56 <= 1 ; N60 <= 1 ; N63 <= 1 ; N66 <=  0; N69 <= 0;  N73 <= 0 ; N76 <= 0; N79 <= 0 ; N82 <= 0 ; N86 <= 0 ; N89 <= 0 ; N92 <= 0 ; N95 <= 0 ; N99 <= 0 ; N102 <= 0 ; N105 <= 1 ; N108 <= 1 ; N112 <= 0 ; N115 <= 0 ;
#10
$display(" input pattern = ",N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,N99,N102,N105,N108,N112,N115," --> golden value = 1111000 "); 
$display(" your answer = ", N223,N329,N370,N421,N430,N431,N432); 
if (N223 == 1'b1 && N329 == 1'b1 && N370 == 1'b1 && N421 == 1'b1 && N430 == 1'b0&& N431 == 1'b0 && N432 == 1'b0 )
begin
	score = score + 3'b001;
end

//000001111111111111110000000000000000//
N1 <= 0 ; N4 <= 0;  N8 <= 0 ; N11 <= 0 ; N14 <= 0 ; N17 <= 1 ; N21 <= 1;  N24 <= 1 ; N27 <= 1 ; N30 <= 1 ; N34 <= 1 ; N37 <= 1 ;  N40 <=  1; N43 <= 1 ; N47 <= 1 ; N50 <= 1 ; N53 <= 1;  N56 <= 1 ; N60 <= 1 ; N63 <= 1 ; N66 <=  0; N69 <= 0;  N73 <= 0 ; N76 <= 0; N79 <= 0 ; N82 <= 0 ; N86 <= 0 ; N89 <= 0 ; N92 <= 0 ; N95 <= 0 ; N99 <= 0 ; N102 <= 0 ; N105 <= 0 ; N108 <= 0 ; N112 <= 0 ; N115 <= 0 ;
#10
$display(" input pattern = ",N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,N99,N102,N105,N108,N112,N115," --> golden value = 1001111 "); 
$display(" your answer = ", N223,N329,N370,N421,N430,N431,N432); 
if (N223 == 1'b1 && N329 == 1'b0 && N370 == 1'b0 && N421 == 1'b1 && N430 == 1'b1&& N431 == 1'b1 && N432 == 1'b1 )
begin
	score = score + 3'b001;
end

//000001111111111111110000000000011111//
N1 <= 0 ; N4 <= 0;  N8 <= 0 ; N11 <= 0 ; N14 <= 0 ; N17 <= 1 ; N21 <= 1;  N24 <= 1 ; N27 <= 1 ; N30 <= 1 ; N34 <= 1 ; N37 <= 1 ;  N40 <=  1; N43 <= 1 ; N47 <= 1 ; N50 <= 1 ; N53 <= 1;  N56 <= 1 ; N60 <= 1 ; N63 <= 1 ; N66 <=  0; N69 <= 0;  N73 <= 0 ; N76 <= 0; N79 <= 0 ; N82 <= 0 ; N86 <= 0 ; N89 <= 0 ; N92 <= 0 ; N95 <= 0 ; N99 <= 0 ; N102 <= 1 ; N105 <= 1 ; N108 <= 1 ; N112 <= 1 ; N115 <= 1 ;
#10
$display(" input pattern = ",N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,N99,N102,N105,N108,N112,N115," --> golden value = 1001111 "); 
$display(" your answer = ", N223,N329,N370,N421,N430,N431,N432); 
if (N223 == 1'b1 && N329 == 1'b0 && N370 == 1'b0 && N421 == 1'b1 && N430 == 1'b1&& N431 == 1'b1 && N432 == 1'b1 )
begin
	score = score + 3'b001;
end

//111111111111111111001100000000000000//
N1 <= 1 ; N4 <= 1;  N8 <= 1 ; N11 <= 1 ; N14 <= 1 ; N17 <= 1 ; N21 <= 1;  N24 <= 1 ; N27 <= 1 ; N30 <= 1 ; N34 <= 1 ; N37 <= 1 ;  N40 <=  1; N43 <= 1 ; N47 <= 1 ; N50 <= 1 ; N53 <= 1;  N56 <= 1 ; N60 <= 0 ; N63 <= 0 ; N66 <=  1; N69 <= 1;  N73 <= 0 ; N76 <= 0; N79 <= 0 ; N82 <= 0 ; N86 <= 0 ; N89 <= 0 ; N92 <= 0 ; N95 <= 0 ; N99 <= 0 ; N102 <= 0 ; N105 <= 0 ; N108 <= 0 ; N112 <= 0 ; N115 <= 0 ;
#10
$display(" input pattern = ",N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,N99,N102,N105,N108,N112,N115," --> golden value = 1111011 "); 
$display(" your answer = ", N223,N329,N370,N421,N430,N431,N432); 
if (N223 == 1'b1 && N329 == 1'b1 && N370 == 1'b1 && N421 == 1'b1 && N430 == 1'b0&& N431 == 1'b1 && N432 == 1'b1 )
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