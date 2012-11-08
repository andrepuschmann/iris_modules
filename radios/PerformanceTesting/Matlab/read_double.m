function Data = read_double(File, numSamples);

precision = 'double';

fid1 = fopen([File, '.bin'],'r');

[Data,count] = fread(fid1, numSamples, precision);

fclose(fid1);
