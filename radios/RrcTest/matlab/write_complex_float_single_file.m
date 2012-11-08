function write_complex_float_single_file(File, Data)
% This function outputs complex values to file in the form:
% real imag real imag

precision = 'float32';

fid1 = fopen([File '.bin'],'w+');

%fwrite only works with integral types - need to split complex up into real
%and imag
Data = Data(:);

wrdat = zeros(length(Data)*2,1);

wrdat(1:2:end) = real(Data);

wrdat(2:2:end) = imag(Data);

fwrite(fid1, wrdat, precision);

fclose(fid1);
