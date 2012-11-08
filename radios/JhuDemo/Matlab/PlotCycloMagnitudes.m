close all
clear all

Data = read_float('../SampleFiles/maMagnitudes', inf);
figure(1);plot(Data);title('SCF at cyclic frequency');

Data = read_float('../SampleFiles/maPwrMagnitudes', inf);
figure(2);plot(Data);title('Power');
