close all
clear all

detectDatabuffer = read_complex_float('../Output_Data/detectDataBuffer', inf);

figure(1);
plot(abs(detectDatabuffer));title('detectDatabuffer');