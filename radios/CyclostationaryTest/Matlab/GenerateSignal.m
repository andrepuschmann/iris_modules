%% Generate OFDM signals with embedded signatures

clear all;
close all;
format long

%%%%%%%%%%%%%%%%%%%%%%%%%%%Parameters%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

prefixLength = 16;  %length of cyclic prefix before OFDM symbols
Fo = 0;          %Frequency offset in hertz
B = 1024000;        %Bandwidth of signal
carrierSpacing = B/256;     %Bandwidth between individual carriers
sampleTime = 1/B;           %Sec/sample
qamnumber = 4;

% Signature generation parameters
sigLocation = 16;
numSigCarriers = 3;

numWindows = 100;

% Channel parameters
SNR = 10;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Signal = [];

%Add some more WiMax symbols to the signal (with signature)
for k=1:numWindows
    data = randint(1, 200, [0,qamnumber-1]);
    modSymbols = qammod(data,qamnumber);
    ofdmSymbol = gen_wimax_symbol_single_signature(modSymbols, prefixLength, numSigCarriers, sigLocation);
    Signal = [Signal, ofdmSymbol];
end

% Muliply by complex sinusoid to simulate a frequency offset
t = (1:length(Signal)).*sampleTime;
offset = exp(2*pi*Fo*t*i);
Signal = Signal.*offset;

% Add noise
Signal = awgn(Signal, SNR, 'measured');

% Offset the data by a random number of samples uniformly
% distributed on 1:272
timeoffset = ceil(272*rand);
Signal = Signal(timeoffset:length(Signal)); 

figure;plot(abs(Signal));
figure:plot(fftshift(abs(fft(Signal))));

write_complex_float_single_file('../signal', Signal);

detect_normalized_single_sig(Signal, 272, 272, 34, 3);
