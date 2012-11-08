function write_complex_float_single_file(File, Data);
% This function outputs complex values to file in the form:
% real imag real imag

precision = 'float32';

fid1 = fopen([File '.bin'],'w+');

%fwrite only works with integral types - need to split complex up into real
%and imag
splitData = [real(Data); imag(Data)];

%fwrite will write values to file, one column at a time
fwrite(fid1, splitData, precision);

fclose(fid1);
