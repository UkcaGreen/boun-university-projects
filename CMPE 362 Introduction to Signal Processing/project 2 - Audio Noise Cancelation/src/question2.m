%clear old variables
clear; clc;                                                                 

%create time vector and actual signal
[Y, t] = sample(1024);                                                      

fs = 0.1;                                                                  
err = Inf;
%increase sampling frequency until error falls under desired value
while err > 10e-3
    %take samples
    [Ys, ts] = sample(fs);                                                  
    if length(ts) > 1
        %reconstruct the signal
        Rs = sinc_interpolation(Ys, ts, t);
        %calculate error
        err = meanAbsoluteError(Y,Rs);                                         
    end
    fs = fs + 0.1;
end

under_fs = fs/2;
%take samples
[under_s, under_ts] = sample(under_fs);
%reconstruct the under sampled signal
under_recons = sinc_interpolation(under_s, under_ts, t);

exact_fs = fs;
%take samples
[exact_s, exact_ts] = sample(exact_fs);
%reconstruct the exact sampled signal
exact_recons = sinc_interpolation(exact_s, exact_ts, t);

over_fs = fs*4;
%take samples
[over_s, over_ts] = sample(over_fs);
%reconstruct the over sampled signal
over_recons = sinc_interpolation(over_s, over_ts, t);

%plot the under sampled signal
subplot(3, 1, 1);
plot(t,Y);
hold on;
stem(under_ts,under_s,'.');
hold on;
plot(t,under_recons);
title('Under Sampling Rate'), xlabel('time'), ylabel('value');
hold off;

%plot the exact sampled signal
subplot(3, 1, 2);
plot(t,Y);
hold on;
stem(exact_ts,exact_s,'.');
hold on;
plot(t,exact_recons);
title('Empirically Calculated Nyquist Sampling Rate'), xlabel('time'), ylabel('value');
hold off;

%plot the over sampled signal
subplot(3, 1, 3);
plot(t,Y);
hold on;
stem(over_ts,over_s,'.');
hold on;
plot(t,over_recons);
title('Over Sampling Rate'), xlabel('time'), ylabel('value');
hold off;

%performs sinc interpolation
function y = sinc_interpolation(sample, ts, t)
dts = ts(1) - ts(2);
[Ts,T] = ndgrid(ts,t);
y = sample*sinc((Ts - T)/dts);
end

%performs sampling
function [Ys,ts] = sample(fs)
dts = 1/fs;
ts = 0:dts:20;
Ys = cos(2*pi*0.2*ts) + cos(2*pi*0.7*ts);
end

%calculates mean absoulute error
function y = meanAbsoluteError(A, B)
y = mean(abs(A-B));
end



