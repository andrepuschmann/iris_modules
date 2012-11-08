close all
clear all

Data = read_float('../SampleFiles/tempmaMagnitudes', inf);
figure(1);plot(Data);title('SCF at cyclic frequency');

Data = read_float('../SampleFiles/tempmaPwrMagnitudes', inf);
figure(2);plot(Data);title('Power');
