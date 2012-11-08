close all
clear all

Sig = read_complex_float_single_file('../capture', inf);
plot(abs(Sig(16384:1638400)))