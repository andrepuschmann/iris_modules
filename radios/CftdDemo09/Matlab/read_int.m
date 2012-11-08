function Data = read_int(File, numSamples);

precision = 'int32';

fid1 = fopen([File, '.bin'],'r');

[Data,count] = fread(fid1, numSamples, precision);

fclose(fid1);