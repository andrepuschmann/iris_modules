close all
clear all

filename = '../Output_Data/RX_Symbol_';

h = scatterplot(0);

for k=1:20
   currentSymName = [filename num2str(k)];
   currentSym = read_complex_float(currentSymName, inf);
   
   scatterplot(currentSym, 1,0,'kx', h);
   
   figure(2);
   subplot(3,1,1);plot(abs(currentSym));title('Power');
   subplot(3,1,2);plot(real(currentSym));title('Real');
   subplot(3,1,3);plot(imag(currentSym));title('Imag');
end