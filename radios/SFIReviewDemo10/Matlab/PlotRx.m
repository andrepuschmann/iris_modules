Frame = read_complex_float_single_file('../out', inf);

figure(1);
plot(abs(Frame));

figure(2);
subplot(3,1,1);plot(abs(Frame));
subplot(3,1,2);plot(real(Frame));
subplot(3,1,3);plot(imag(Frame));

figure(3);
plot(abs(fftshift(fft(Frame))));

