close all
clear all

BinsData = read_complex_float('../Output_Data/PreambleBins', 256);
PreambleData = read_complex_float('../Output_Data/Preamble', 256);

figure(1);plot(abs(BinsData));title('Bins');
figure(2);plot(abs(PreambleData));title('Symbol');