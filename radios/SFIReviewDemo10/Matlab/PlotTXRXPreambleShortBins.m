close all
clear all

BinsData = read_complex_float('../Output_Data/RX_Preamble_Short_Bins', inf);
subplot(2,1,1);plot(abs(BinsData));title('RX Preamble Short Bins');

BinsData = read_complex_float('../Output_Data/TX_Preamble_Short_Bins', inf);
subplot(2,1,2);plot(abs(BinsData));title('TX Preamble Short Bins');
