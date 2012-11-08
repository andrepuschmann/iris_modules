close all
clear all

filename = '../Output_Data/TX_Symbol_';

for k=0:20
   currentSymName = [filename num2str(k)];
   currentSym = read_complex_float_single_file(currentSymName, inf);
   
   subplot(3,1,1);plot(real(currentSym));title('Real');
   subplot(3,1,2);plot(imag(currentSym));title('Imag');  
   subplot(3,1,3);plot(abs(fftshift(fft(currentSym))));title('FFT');
end