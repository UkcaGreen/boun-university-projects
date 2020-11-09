%% START: Question-8
%clear old variables
clear;clc;

%read the audio file
[y,fs] = audioread('audio/nyan-cat.m4a');

%get the dimentions of the audio
[m,n] = size(y);

%there are two channels
y1 = y(:,1);
y2 = y(:,2);

%find unit time
dt = 1/fs;
%create time vector
t = dt*(0:m-1);

Y = fft(y);                 %discrete fourier transform
f = (0:m-1)*(fs/m);         %frequency range   
p = abs(Y).^2/m;            %power of the DFT

Y0 = fftshift(Y);           %shift Y values
f0 = (-m/2:m/2-1)*(fs/m);   %0-centered frequency range
p0 = abs(Y0).^2/m;          %0-centered power

subplot(3,1,1), plot(t,y), xlabel('Time'), ylabel('Signal');
subplot(3,1,2), plot(f,p), xlabel('Frequency'), ylabel('Power'), ylim([0 130]);
subplot(3,1,3), plot(f0,p0), xlabel('Frequency'), ylabel('Power'), ylim([0 130]);
% END: Question-8