close all
clear all

PreambleBins = read_complex_float_single_file('../Output_Data/PreambleBins', inf);
plot_complex_array(PreambleBins, 'Tx Preamble Bins');
