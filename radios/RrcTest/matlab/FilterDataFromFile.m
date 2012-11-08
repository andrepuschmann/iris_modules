close all 
clear all

N = 4;

data = read_complex_float_single_file('../qpsk4samp', inf);
% upsample data
data_upsamp = data(ones(1,N),:);
data_upsamp = data_upsamp(:).';

figure;
subplot(3,1,1);plot(real(data));ylim([-2,2]);title('In-Phase');
subplot(3,1,2);plot(imag(data));ylim([-2,2]);title('Quadrature');
subplot(3,1,3);
plot(10*log10(fftshift(abs(fft(data_upsamp)).^2)));title('PSD');ylim([-20,60]);

[num,den] = rcosine(1,N,'sqrt',0.5);
figure;
plot(num);

% filter data (including upsampling)
filtData = rcosflt(data, 1, N, 'fir/sqrt', .5, 3);


figure;
subplot(3,1,1);plot(real(filtData));title('In-Phase');
subplot(3,1,2);plot(imag(filtData));title('Quadrature');
subplot(3,1,3);plot(10*log10(fftshift(abs(fft(filtData)).^2)));title('PSD');ylim([-20,60]);

write_complex_float_single_file('../matlabout', filtData(1:length(data)*4));