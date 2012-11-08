function Data = read_complex_float_single_file(File, count);
% This function reads complex values from file in the form:
% real imag real imag

precision = 'float32';

filename = [File '.bin'];
fid1 = fopen(filename,'r');

%fread will read values from file
[splitData n] = fread(fid1, count*2, precision);

real = splitData(1:2:n-1);
imag = splitData(2:2:n);

Data = real + i*imag;
Data = Data.';

fclose(fid1);
