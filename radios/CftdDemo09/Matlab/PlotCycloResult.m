close all
clear all

Data = read_float('../Output_Data/result', inf);
plot(Data);title('SCF at cyclic frequency');
