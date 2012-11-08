file = 'Times';

Times = read_double(strcat('../', file), inf);

figure;
plot(Times);
grid on
% ylim([0,0.01]);
ylabel('Latency (sec)');
xlabel('DataSet index');

aveTime = mean(Times)
