function Data = read_float(File, numSamples);

precision = 'float32';

fid1 = fopen([File, '.bin'],'r');

[Data,count] = fread(fid1, numSamples, precision);

fclose(fid1);
