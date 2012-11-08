close all
clear all

BinsData = read_complex_float('../Output_Data/Short_Equalizer_Bins', inf);
subplot(2,1,1);plot(abs(BinsData));title('Short Equalizer Bins');
subplot(2,1,2);plot(angle(BinsData));title('Short Equalizer Bins - Phase');

BinsData = read_complex_float('../Output_Data/Interp_Short_Equalizer_Bins', inf);
subplot(2,1,1);plot(abs(BinsData));title('Interpolated Short Equalizer Bins');
subplot(2,1,2);plot(angle(BinsData));title('Interpolated Short Equalizer Bins - Phase');

BinsData = read_complex_float('../Output_Data/Full_Equalizer_Bins', inf);
subplot(2,1,1);plot(abs(BinsData));title('Full Equalizer Bins');
subplot(2,1,2);plot(angle(BinsData));title('Full Equalizer Bins - Phase');
