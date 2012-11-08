close all
clear all

Data = read_int('../Output_Data/pilotIndices', inf);
Index = zeros(1,1024);
Index(Data) = 1;
plot(Index);

