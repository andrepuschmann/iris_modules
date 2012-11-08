close all 
clear all
 
data = read_complex_float_single_file('../out', 4096);

figure;
subplot(3,1,1);plot(real(data));title('In-Phase');
subplot(3,1,2);plot(imag(data));title('Quadrature');
subplot(3,1,3);plot(10*log10(fftshift(abs(fft(data)).^2)));title('PSD');