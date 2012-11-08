%% Perform cyclostationary signature detection using a time-smoothed cyclic
%% periodogram.

%% Signature row is MA filtered with order = numSigCarriers
%% Detection statistic is max(Row)

% Author:   Paul Sutton
% Date:     18/1/2007
% Company:  CTVR

function detectStat = detect_normalized_single_sig(Data, windowSize, fftSize, sigRow, numSigCarriers)

        
%Set up the rows 
smoothWindow = zeros(1, fftSize);            % The time-smoothed window
smoothPwrWindow = zeros(1, fftSize);         % The time-smoothed window

% Split data into sets of length windowSize
numSets = floor(length(Data)/windowSize);
counter = 1;
for a = 1:numSets
    for b = 1:windowSize
        DataSets(a,b) = Data(counter);
        counter = counter+1;
    end
end
        
for M=1:numSets
    currentSet = DataSets(M,:);

    FFTData = fft(currentSet, fftSize);   %Calculate the FFT
    conjFFTData = conj(FFTData);
    
    %Calculate the power
    currentSigPwr = FFTData.*conjFFTData;
    
    %Add current pwr to smoothPwrWindow
    smoothPwrWindow = smoothPwrWindow + currentSigPwr;
    
    %Calculate the correlation
    currentSigRow = circshift(FFTData, [0, -sigRow]);
    currentSigRow = (currentSigRow.*conjFFTData);    
  
    %Add current correlation to smoothWindow
    smoothWindow = smoothWindow + currentSigRow;
end

% Average
smoothPwrWindow = smoothPwrWindow/numSets;
smoothWindow = smoothWindow/numSets;

% Calculate the normalizing array 
smoothPwrWindow = sqrt(smoothPwrWindow.*(circshift(smoothPwrWindow, [0, -sigRow])));

% Get abs
absSmoothPwrWindow = abs(smoothPwrWindow);
absSmoothWindow = abs(smoothWindow);

if numSigCarriers > 1
    %Moving average filter
    b = ones(1,numSigCarriers);
    a = numSigCarriers;
    absSmoothWindow = filter(b,a,absSmoothWindow);
    absSmoothPwrWindow = filter(b,a, absSmoothPwrWindow);
end

%Normalize the SCD using the individual power bins
absSmoothWindow = absSmoothWindow./absSmoothPwrWindow;

figure;plot(absSmoothWindow);title('Smoothed Window');
axis([0 fftSize 0 1])

rowMax = max(absSmoothWindow);
rowMean = mean(absSmoothWindow);

% detectStat = rowMax/rowMean;
detectStat = rowMax;
