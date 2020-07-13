%% START: Question-7
%clear old variables
clear; clc;

%load provided workspace
load('workspace/mysignal.mat');

Y = fft(x);                 %discrete fourier transform
n = length(x);              %number of samples
f = (0:n-1)*(fs/n);         %frequency range   
p = abs(Y).^2/n;            %power of the DFT

Y0 = fftshift(Y);           %shift Y values
f0 = (-n/2:n/2-1)*(fs/n);   %0-centered frequency range
p0 = abs(Y0).^2/n;          %0-centered power

%plot signal with specified labels
subplot(3,1,1), plot(t,x), xlabel('Time'), ylabel('Signal'), ylim([-5 6.5]);
subplot(3,1,2), plot(f,p), xlabel('Frequency'), ylabel('Power'), ylim([0 2500]);
subplot(3,1,3), plot(f0,p0), xlabel('Frequency'), ylabel('Power'), ylim([0 2500]);
% END: Question-7