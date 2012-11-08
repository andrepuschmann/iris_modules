close all
clear all

Preamble = read_complex_float_single_file('../Output_Data/Preamble', inf);
plot_complex_array(Preamble, 'Tx Preamble');
