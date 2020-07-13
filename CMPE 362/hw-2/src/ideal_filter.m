%clear old variables
clear; clc;  

%read the audio files
[noisy,fs] = audioread('noisy/p232_001.wav');
[clean,~] = audioread('clean/p232_001.wav');

[m,~] = size(noisy);                                %get size of the signal
dt = 1/fs;                                          %calculate unit time
t = dt*(0:m-1);                                     %initialize time vector
freq_response = fft(clean)./fft(noisy);             %calculate frequency response
shifted_freq_response = fftshift(freq_response);    %shift the values
time_response = ifft(freq_response);                %calculate time response
cleaned = cconv(noisy, time_response, m);           %clean noisy signal

[f_noisy, freq_range]   = to_freq_dom(noisy, fs);   %switch to freq. domain
[f_clean, ~]            = to_freq_dom(clean, fs);   %switch to freq. domain
[f_cleaned, ~]          = to_freq_dom(clean, fs);   %switch to freq. domain

%plot figures
figure(1);
plot(freq_range,shifted_freq_response), title("frequency response"), xlabel('Frequency'), ylabel('Value');
customPlot("noisy", t, noisy, freq_range, f_noisy, 2);
customPlot("clean", t, clean, freq_range, f_clean, 3);
customPlot("cleaned", t, cleaned, freq_range, f_cleaned, 4);

function [shift_freq_dom, freq_range] = to_freq_dom(x, fs)
    [m,~] = size(x);                        %get size of the signal
    freq_dom = fft(x);                      %switch to freq. domain
    shift_freq_dom = fftshift(freq_dom);    %shift the values
    freq_range = ((-m/2):(m/2-1))*(fs/m);   %calculate frequency range
end

function customPlot(name, t, time, f, freq, fig)
figure(fig);
subplot(2,1,1);
plot(t,time), title(strcat(name, " time domain")), xlabel('Time'), ylabel('Value');
subplot(2,1,2);
plot(f,freq), title(strcat(name, " freq. domain")), xlabel('Frequency'), ylabel('Value');
end