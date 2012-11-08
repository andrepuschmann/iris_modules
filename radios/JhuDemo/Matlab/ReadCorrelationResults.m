close all
clear all

CorrData = read_float('../Output_Data/Integer_Freq_Offset_Correlation', inf);
figure(1);plot(CorrData);title('Correlation results');

