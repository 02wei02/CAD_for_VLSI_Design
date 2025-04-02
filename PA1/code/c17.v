`timescale 1ns/1ps
module c17 (1gat, 2gat, 3gat, 6gat, 7gat, 22gat_out, 23gat_out);
input 1gat, 2gat, 3gat, 6gat, 7gat;
output 22gat_out, 23gat_out;
wire 10gat_out, 11gat_out, 16gat_out, 19gat_out;

nand 10gat ( 10gat_out, 1gat, 3gat );
nand 11gat ( 11gat_out, 3gat, 6gat );
nand 16gat ( 16gat_out, 2gat, 11gat_out );
nand 19gat ( 19gat_out, 11gat_out, 7gat );
nand 22gat ( 22gat_out, 10gat_out, 16gat_out );
nand 23gat ( 23gat_out, 16gat_out, 19gat_out );

endmodule
