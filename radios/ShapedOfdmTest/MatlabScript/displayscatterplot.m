function matlab_output = displayscatterplot(matlab_input)

scatter(real(matlab_input),imag(matlab_input),'b.');grid on;
axis([-2 2 -2 2]);
matlab_output = matlab_input;
