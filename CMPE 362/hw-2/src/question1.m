%clear old variables
clear; clc;  

fs = 4096;                              %frequency
dt = 1/fs;                              %unit time
t = 0:dt:0.2;                           %time array
x = sawtooth(2*pi*25*t,1/2)/2 + 1/2;    %triangle signal

Y = fft(x);                             %discrete fourier transform
n = length(x);                          %number of samples
Y0 = fftshift(Y);                       %shift transformed values
f0 = (-n/2:n/2-1)*(fs/n);               %frequency range
Y0 = Y0/n;                              %normalize

H3 = calculate_harmonic(Y0*n,f0,3);     %strips the needed part of harmonics
H3 = ifftshift(H3);                     %inverse shift
H3 = ifft(H3);                          %inverse fft

H11 = calculate_harmonic(Y0*n,f0,11);   %strips the needed part of harmonics
H11 = ifftshift(H11);                   %inverse shift
H11 = ifft(H11);                        %inverse fft

%plot frequency spectrum
figure(1);
plot(f0,Y0),title("Frequency Spectrum"), xlabel('Frequency'), ylabel('Value'), ylim([-0.3 0.6]), xlim([-300 300]);

%plot harmonics
figure(2);
subplot(2,1,1);
plot(t,H3), title("Up to 3rd Harmonic"),xlabel('Time'), ylabel('Value');
subplot(2,1,2);
plot(t,H11), title("Up to 11th Harmonic"),xlabel('Time'), ylabel('Value');

%strips the harmonics
function y = calculate_harmonic(x, f0, h)
    H = h*25;
    y = x;
    [~,n] = size(x);
    for i = 1:n
        if abs(f0(i)) > H
            y(i) = 0;
        end
    end
end