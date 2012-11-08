close all
clear all

Data = read_int('../Output_Data/dataCarrierIndices', inf);
Index = zeros(1,1024);
Index(Data) = 1;
plot(Index);

