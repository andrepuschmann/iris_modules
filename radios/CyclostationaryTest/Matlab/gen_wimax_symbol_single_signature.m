% Generate a WiMax symbol with cyclic prefix using a 256 bin fft
% This version uses carrier mapping to generate a cyclostationary signature
% ---Signature generated is not centred on the carrier frequency---

% Frequency domain array used for the ifft:
% +--------------------------------------------------------------------+
% | DC | 100 data carriers  | 55 guard carriers | 100 data carriers    |
% +--------------------------------------------------------------------+

% Author:   Paul Sutton
% Date:     18/1/2007
% Company:  CTVR

function symbol = gen_wimax_symbol_single_signature(Data, cplength, numSigCarriers, sigLocation)

dataLen = length(Data);

% Data must be of length 200
if dataLen ~= 200
    error('gen_wimax_frame:invalidDataLength','Data length must be 200.');
end

numDataCarriers = 200;
Nfft = 256;
numguards = 55;

%Get the carriers to be repeated
cycloCarriers = Data(sigLocation:sigLocation + numSigCarriers - 1);

%Create the bins 
bins = [0, Data(1:numDataCarriers/2), zeros(1, numguards), Data((numDataCarriers/2)+1:numDataCarriers)];

%Copy in the cyclocarriers  
bins(Nfft - sigLocation+1:Nfft - sigLocation + numSigCarriers) = cycloCarriers;

%Create the signal from the bins
symbol = ifft(bins);

% Add the cyclic prefix
symbol = [symbol(Nfft-cplength+1:Nfft), symbol];





