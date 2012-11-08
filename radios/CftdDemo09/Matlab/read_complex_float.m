function Data = read_complex_float(File, numSamples);

precision = 'float32';

fid1 = fopen([File, '_real.bin'],'r');
fid2 = fopen([File, '_imag.bin'],'r');

[realData,count] = fread(fid1, numSamples, precision);
[imagData,count] = fread(fid2, numSamples, precision);

realData = realData';
imagData = imagData';

fclose(fid1);
fclose(fid2);

Data = complex(realData,imagData);