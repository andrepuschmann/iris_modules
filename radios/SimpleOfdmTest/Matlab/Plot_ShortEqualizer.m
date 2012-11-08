close all
clear all

rxPreambleShortBins = read_complex_float_single_file('../Output_Data/Equalizer_Short_Bins', inf);
plot_complex_array(rxPreambleShortBins, 'Rx Preamble Bins');
