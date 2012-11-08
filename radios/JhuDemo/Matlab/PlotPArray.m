close all
clear all

P_Array = read_complex_float('../Output_Data/P_Array', 5712);

figure(1);plot(abs(P_Array));title('Bins');
