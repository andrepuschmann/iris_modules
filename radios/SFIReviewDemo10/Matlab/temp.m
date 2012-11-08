close all
clear all

BinsData = read_complex_float('../Output_Data/preambleBinsSequence', 1024);
subplot(2,1,1);plot(abs(BinsData));title('Short Equalizer Bins');
subplot(2,1,2);plot(angle(BinsData));title('Short Equalizer Bins - Phase');