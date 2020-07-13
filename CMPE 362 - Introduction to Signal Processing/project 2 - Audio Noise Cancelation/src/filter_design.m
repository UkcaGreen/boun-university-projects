%clear old variables
clear; clc;                                                                 

%input and output files
input_file = 'noisy/p232_001.wav';
output_file = 'out.wav';

%read input file aka. noisy signal
[noisy,fs] = audioread(input_file);
[m,~] = size(noisy);
dt = 1/fs;
t = dt*(0:m-1);

[f_noisy, freq_range] = to_freq_dom(noisy, fs);

%prepare high pass filter
hp_filter = zeros(size(freq_range));
for i = 1:m
    if abs(freq_range(i)) < 125
        hp_filter(i) = 0;
    else
        hp_filter(i) = 1;
    end
end

%prepare high pass filter
lp_filter = zeros(size(freq_range));
for i = 1:m
    if abs(freq_range(i)) > 9250
        lp_filter(i) = 0;
    else
        lp_filter(i) = 1;
    end
end

%generate a band pass filter 
%with combining low pass and high pass filters
bp_filter = lp_filter.*hp_filter;

%move band pass filter to time domain
unshifted_bp_filter = ifftshift(bp_filter);
t_bp_filter = ifft(unshifted_bp_filter);

%apply band pass filter
cleaned = cconv(noisy,t_bp_filter,m);

%apply average filter to smoothen the signal
for i = 1:m
    if i < m-10
        cleaned(i) = mean(cleaned(i:i+10));
    else
        cleaned(i) = mean(cleaned(i:m));
    end
end

audiowrite(output_file,cleaned,fs);

function [shift_freq_dom, freq_range] = to_freq_dom(x, fs)
    [m,~] = size(x);                        %get size of the signal
    freq_dom = fft(x);                      %switch to freq. domain
    shift_freq_dom = fftshift(freq_dom);    %shift the values
    freq_range = ((-m/2):(m/2-1))*(fs/m);   %calculate frequency range
end
