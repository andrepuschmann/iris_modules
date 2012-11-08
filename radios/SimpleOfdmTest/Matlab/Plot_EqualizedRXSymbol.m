close all
clear all

filename = '../Output_Data/Equalized_RX_Symbol';

currentSym = read_complex_float_single_file(filename, inf);

figure(1);
subplot(4,1,1);plot(abs(currentSym));title('Power');
subplot(4,1,2);plot(angle(currentSym));title('Angle');
subplot(4,1,3);plot(real(currentSym));title('Real');
subplot(4,1,4);plot(imag(currentSym));title('Imag');

scatterplot(currentSym);