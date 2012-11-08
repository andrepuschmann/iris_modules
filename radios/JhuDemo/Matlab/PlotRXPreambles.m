close all
clear all

BinsData = read_complex_float('../Output_Data/UnRotated_RX_Preamble_Bins', inf);
figure(1);subplot(4,1,1);plot(abs(BinsData));title('RX Short Preamble Bins');
figure(2);subplot(4,1,1);plot(angle(BinsData));title('RX Short Preamble Bins');

BinsData = read_complex_float('../Output_Data/Rotated_RX_Preamble_Bins', inf);
figure(1);subplot(4,1,2);plot(abs(BinsData));title('Rotated RX Short Preamble Bins');
figure(2);subplot(4,1,2);plot(angle(BinsData));title('Rotated RX Short Preamble Bins');

BinsData = read_complex_float('../Output_Data/Interp_Short_Equalizer_Bins', inf);
figure(1);subplot(4,1,3);plot(abs(BinsData));title('Interpolated Short Equalizer Bins');
figure(2);subplot(4,1,3);plot(angle(BinsData));title('Interpolated Short Equalizer Bins');

BinsData = read_complex_float('../Output_Data/Full_Equalizer_Bins', inf);
figure(1);subplot(4,1,4);plot(abs(BinsData));title('Full Equalizer Bins');
figure(2);subplot(4,1,4);plot(angle(BinsData));title('Full Equalizer Bins');