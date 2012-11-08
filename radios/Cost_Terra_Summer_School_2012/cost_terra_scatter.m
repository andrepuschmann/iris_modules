function matlab_output = cost_terra_scatter(matlab_input)
% This function plots the magnitude, angle, real and imaginary parts of an
% array

matlab_output = [];
sfigure(1);plot(real(matlab_input),imag(matlab_input),'kx');
grid on
xlim([-2 2]);xlabel('Real');
ylim([-2 2]);ylabel('Imag');
