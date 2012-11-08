close all
clear all

h = scatterplot(0);
hold on
RXData = read_complex_float('../Output_Data/RX_Pilots', inf);
%TXData = read_complex_float('../Output_Data/TX_Pilots', inf);
for k=1:8
    scatterplot(RXData(k),1,0,'kx',h);
%    scatterplot(TXData(k),1,0, 'rx',h);
end

