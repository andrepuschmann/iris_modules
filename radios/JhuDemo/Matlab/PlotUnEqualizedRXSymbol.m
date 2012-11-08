close all
clear all

filename = '../Output_Data/UnEqualized_RX_Data_Symbol';

currentSym = read_complex_float(filename, inf);

figure(1);
subplot(3,1,1);plot(abs(currentSym));title('Power');
subplot(3,1,2);plot(real(currentSym));title('Real');
subplot(3,1,3);plot(imag(currentSym));title('Imag');

scatterplot(currentSym);

filename = '../Output_Data/Equalized_RX_Symbol';

currentSym = read_complex_float(filename, inf);

figure(2);
subplot(3,1,1);plot(abs(currentSym));title('Power');
subplot(3,1,2);plot(real(currentSym));title('Real');
subplot(3,1,3);plot(imag(currentSym));title('Imag');

scatterplot(currentSym);

filename = '../Output_Data/UnRotated_RX_Symbol';

currentSym = read_complex_float(filename, inf);

figure(2);
subplot(3,1,1);plot(abs(currentSym));title('Power');
subplot(3,1,2);plot(real(currentSym));title('Real');
subplot(3,1,3);plot(imag(currentSym));title('Imag');

scatterplot(currentSym);