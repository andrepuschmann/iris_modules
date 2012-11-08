close all
clear all

V_Array = read_float('../Output_Data/V_Array', inf);
P_Array = read_complex_float('../Output_Data/P_Array', inf);
E_Array = read_complex_float('../Output_Data/E_Array', inf);
Incoming_Array = read_complex_float('../Output_Data/Incoming_Array', inf);

figure(1);
subplot(4,1,1);plot(V_Array);title('V_Array');
subplot(4,1,2);plot(abs(P_Array));title('P_Array power');
subplot(4,1,3);plot(abs(E_Array));title('E_Array power');
subplot(4,1,4);plot(abs(Incoming_Array));title('Incoming_Array power');
