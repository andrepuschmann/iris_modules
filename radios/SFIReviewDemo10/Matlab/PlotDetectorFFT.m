close all
clear all

Data = read_complex_float_single_file('../SampleFiles/DetectorFftInput', inf);
figure(1);
subplot(2,1,1);plot(real(Data));title('Detector FFT Input');
subplot(2,1,2);plot(imag(Data));

Data = read_complex_float_single_file('../SampleFiles/DetectorFftOutput', inf);
figure(2);plot(abs(Data));title('Detector FFT Output');
