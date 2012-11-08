close all
clear all

filename = '../Output_Data/TX_Symbol_';

for k=0:19
   currentSymName = [filename num2str(k)];
   currentSym = read_complex_float(currentSymName, inf);
   
   figure(1);
   subplot(3,1,1);plot(abs(fftshift(fft(currentSym))));title('Power');
   subplot(3,1,2);plot(real(fft(currentSym)));title('Real');
   subplot(3,1,3);plot(imag(fft(currentSym)));title('Imag');
end

