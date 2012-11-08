close all
clear all

Diffs = read_complex_float('../Output_Data/diffs', inf);

subplot(2,1,1);plot(abs(Diffs));title('Pilot diffs - Power');
subplot(2,1,2);plot(angle(Diffs));title('Pilot diffs - Phase');