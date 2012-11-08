function plot_complex_array(data, name)
% This function plots the magnitude, angle, real and imaginary parts of an
% array

figure;
subplot(4,1,1);plot(abs(data));title(strcat(name,' Magnitude'));
subplot(4,1,2);plot(angle(data));title(strcat(name,' Angle'));
subplot(4,1,3);plot(real(data));title(strcat(name,' Real'));
subplot(4,1,4);plot(imag(data));title(strcat(name,' Imag'));
scatterplot(data);
