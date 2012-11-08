close all
clear all

N = 4;

data1 = read_complex_float_single_file('../out', 4096);
data2 = read_complex_float_single_file('../matlabout', 4096);

figure;
subplot(2,1,1);hold on;
plot(real(data1), 'r-x');
plot(real(data2), 'b-x');
subplot(2,1,2);hold on;
plot(imag(data1), 'r-x');
plot(imag(data2), 'b-x');


