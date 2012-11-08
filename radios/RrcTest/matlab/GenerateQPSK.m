close all 
clear all

L=1024;
M=4;
N=4;

info = randint(1,L,M-1);
data = qammod(info,M);

write_complex_float_single_file('../qpsk4samp', data);