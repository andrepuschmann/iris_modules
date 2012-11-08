close all
clear all

filename = '../Output_Data/Symbol';

for k=0:159
   currentSymName = [filename num2str(k)];
   currentSym = read_complex_float(currentSymName, 256);
   
   figure(1);
   subplot(3,1,1);plot(abs(fftshift(fft(currentSym))));title('Power');
   subplot(3,1,2);plot(real(currentSym));title('Real');
   subplot(3,1,3);plot(imag(currentSym));title('Imag');
end

%BinsData = read_complex_float('../Output_Data/PreambleBins', 256);
%PreambleData = read_complex_float('../Output_Data/Preamble', 256);

%figure(1);plot(abs(BinsData));
%figure(2);plot(abs(PreambleData));