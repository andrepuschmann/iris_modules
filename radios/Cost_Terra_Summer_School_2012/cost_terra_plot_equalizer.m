function Eqplot(data, size)
% This function plots the magnitude, angle, real and imaginary parts of an
% array

data = data(1:size);
sfigure(1);subplot(4,1,1);plot(fftshift(abs(data)));title('Magnitude');
sfigure(1);subplot(4,1,2);plot(fftshift(angle(data)));title('Phase');
sfigure(1);subplot(4,1,3);plot(fftshift(real(data)));title('Real');
sfigure(1);subplot(4,1,4);plot(fftshift(imag(data)));title('Imag');

