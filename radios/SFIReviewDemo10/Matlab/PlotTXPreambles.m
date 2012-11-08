close all
clear all

BinsDataSequence = read_complex_float('../Output_Data/preambleBinsSequence', 1024);
BinsData = read_complex_float('../Output_Data/PreambleBins', 1024);
PreambleData = read_complex_float('../Output_Data/Preamble', 1024);

figure(1);plot(abs(BinsDataSequence));title('Bins Sequence');
figure(2);plot(abs(BinsData));title('Bins');
figure(3);plot(abs(PreambleData));title('Symbol');
