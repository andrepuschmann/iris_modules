% Matlab Script describing the evalstring command

function PlotSignaturesIris2(matlab_input, size)

Data = matlab_input(1:size);
Data = fftshift(Data);
X = 1:size;
plot(X, Data);
grid on
xlim([1, size]);
ylim([0, 1]);
%xlabel('Frequency (Hz)');
ylabel('Normalized power');


