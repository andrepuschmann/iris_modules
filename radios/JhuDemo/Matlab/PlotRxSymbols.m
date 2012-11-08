close all
clear all

filename = '../outputfile';

Data = read_complex_float_single_file(filename, inf);
length = length(Data);
numSyms = length/272;

Data = reshape(Data, 272, numSyms);
Data = Data';

for k=1:numSyms
    currentSym = Data(k,:);
    currentSym = currentSym(17:272);
   
    figure(1);
    subplot(3,1,1);plot(abs(fftshift(fft(currentSym))));title('Power');
    subplot(3,1,2);plot(real(currentSym));title('Real');
    subplot(3,1,3);plot(imag(currentSym));title('Imag');
end


%BinsData = read_complex_float('../Output_Data/PreambleBins', 256);
%PreambleData = read_complex_float('../Output_Data/Preamble', 256);

%figure(1);plot(abs(BinsData));
%figure(2);plot(abs(PreambleData));