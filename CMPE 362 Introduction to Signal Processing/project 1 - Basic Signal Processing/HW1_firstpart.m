%% START: Question-1
%clear old variables
clear; clc;

%create time vector
t = -2:0.01:2;

%create vectors for function values
y1 = sin(2*pi*t);
y2 = sin(2*pi*10*t);
y3 = 10*sin(2*pi*t);
y4 = sin(2*pi*t)+10;
y5 = sin(2*pi*(t-0.5));
y6 = 10*sin(2*pi*10*t);
y7 = t.*sin(2*pi*t);
y8 = sin(2*pi*t)./t;
y9 = y1 + y2 + y3 + y4 + y5 + y6 + y7 + y8;

%create a figure with Name "Question-1"
figure('Name', 'Question-1', 'NumberTitle', 'off');
%plot the functions as subplots with specified titles and labels.
subplot(5,2,1), plot(t,y1), title('y1'), xlabel('time'), ylabel('signal');
subplot(5,2,2), plot(t,y2), title('y2'), xlabel('time'), ylabel('signal');
subplot(5,2,3), plot(t,y3), title('y3'), xlabel('time'), ylabel('signal');
subplot(5,2,4), plot(t,y4), title('y4'), xlabel('time'), ylabel('signal');
subplot(5,2,5), plot(t,y5), title('y5'), xlabel('time'), ylabel('signal');
subplot(5,2,6), plot(t,y6), title('y6'), xlabel('time'), ylabel('signal');
subplot(5,2,7), plot(t,y7), title('y7'), xlabel('time'), ylabel('signal');
subplot(5,2,8), plot(t,y8), title('y8'), xlabel('time'), ylabel('signal');
subplot(5,2,9), plot(t,y9), title('y9'), xlabel('time'), ylabel('signal');
saveas(gcf, 'figures/1.png');
% END: Question-1

%% START: Question-2
%clear old variables
clear; clc;

%create time vector
t = -2:0.01:2;

%create gaussian random numbers
z = randn(1,401);
z = z*0.1;

%create vectors for function values
%these two functions are from question-1
y1 = sin(2*pi*t);
y2 = sin(2*pi*10*t);

%create vectors for function values
y10 = z;
y11 = z+t;
y12 = z + y1;
y13 = z .* y1;
y14 = t.*sin(2*pi*z);
y15 = sin(2*pi*(t+z));
y16 = z .* y2;
y17 = sin(2*pi*(t+10*z));
y18 = y1 ./ z;
y19 = y10 + y11 + y12 + y13 + y14 + y15 + y16 + y17 + y18;

%create a figure with Name "Question-2"
figure('Name', 'Question-2', 'NumberTitle', 'off');
%plot the functions as subplots with specified titles and labels.
subplot(5,2,1), plot(t,y10), title('y10'), xlabel('time'), ylabel('signal');
subplot(5,2,2), plot(t,y11), title('y11'), xlabel('time'), ylabel('signal');
subplot(5,2,3), plot(t,y12), title('y12'), xlabel('time'), ylabel('signal');
subplot(5,2,4), plot(t,y13), title('y13'), xlabel('time'), ylabel('signal');
subplot(5,2,5), plot(t,y14), title('y14'), xlabel('time'), ylabel('signal');
subplot(5,2,6), plot(t,y15), title('y15'), xlabel('time'), ylabel('signal');
subplot(5,2,7), plot(t,y16), title('y16'), xlabel('time'), ylabel('signal');
subplot(5,2,8), plot(t,y17), title('y17'), xlabel('time'), ylabel('signal');
subplot(5,2,9), plot(t,y18), title('y18'), xlabel('time'), ylabel('signal');
subplot(5,2,10), plot(t,y19), title('y19'), xlabel('time'), ylabel('signal');
saveas(gcf, 'figures/2.png');
% END: Question-2

%% START: Question-3
%clear old variables
clear; clc;

%create time vector
t = -2:0.01:2;

%create random numbers
z = rand(1,401);
z = z*0.1;

%create vectors for function values
%these two functions are from question-1
y1 = sin(2*pi*t);
y2 = sin(2*pi*10*t);

%create vectors for function values
y20 = z;
y21 = z+t;
y22 = z+y1;
y23 = z.*y1;
y24 = t.*sin(2*pi*z);
y25 = sin(2*pi*(t+z));
y26 = z.*y2;
y27 = sin(2*pi*(t+10*z));
y28 = y1./z;
y29 = y21 + y22 + y23 + y24 + y25 + y26 + y27 + y28;

%create a figure with Name "Question-1"
figure('Name', 'Question-3', 'NumberTitle', 'off');
%plot the functions as subplots with specified titles and labels.
subplot(5,2,1), plot(t,y20), title('y20'), xlabel('time'), ylabel('signal');
subplot(5,2,2), plot(t,y21), title('y21'), xlabel('time'), ylabel('signal');
subplot(5,2,3), plot(t,y22), title('y22'), xlabel('time'), ylabel('signal');
subplot(5,2,4), plot(t,y23), title('y23'), xlabel('time'), ylabel('signal');
subplot(5,2,5), plot(t,y24), title('y24'), xlabel('time'), ylabel('signal');
subplot(5,2,6), plot(t,y25), title('y25'), xlabel('time'), ylabel('signal');
subplot(5,2,7), plot(t,y26), title('y26'), xlabel('time'), ylabel('signal');
subplot(5,2,8), plot(t,y27), title('y27'), xlabel('time'), ylabel('signal');
subplot(5,2,9), plot(t,y28), title('y28'), xlabel('time'), ylabel('signal');
subplot(5,2,10), plot(t,y29), title('y29'), xlabel('time'), ylabel('signal');
saveas(gcf, 'figures/3.png');
% END: Question-3

%% START: Question-4
%clear old variables
clear; clc;

%create gaussian random numbers
z = randn(1,5000);

%create a distribution with 0 mean and 1 variance
r1 = 0 + sqrt(1)*z;
%create a distribution with 0 mean and 8 variance
r2 = 0 + sqrt(8)*z;
%create a distribution with 0 mean and 64 variance
r3 = 0 + sqrt(64)*z;
%create a distribution with 0 mean and 256 variance
r4 = 0 + sqrt(256)*z;

%create a figure with Name "Question-4"
figure('Name', 'Question-4', 'NumberTitle', 'off');
%plot the histograms of random numbers as subplots 
%with specified titles and labels.
subplot(2,2,1), hist(r1,60), title('r1');
subplot(2,2,2), hist(r2,60), title('r2');
subplot(2,2,3), hist(r3,60), title('r3');
subplot(2,2,4), hist(r4,60), title('r4');
saveas(gcf, 'figures/4.png');
% END: Question-4


%% START: Question-5
%clear old variables
clear; clc;

%create gaussian random numbers
z = randn(1,5000);

%create a distribution with 10 mean and 1 variance
r5 = 10 + sqrt(1)*z;
%create a distribution with 20 mean and 4 variance
r6 = 20 + sqrt(4)*z;
%create a distribution with -10 mean and 1 variance
r7 = -10 + sqrt(1)*z;
%create a distribution with -20 mean and 4 variance
r8 = -20 + sqrt(4)*z;

%create a figure with Name "Question-5"
figure('Name', 'Question-5', 'NumberTitle', 'off');
%plot the histograms of random numbers as subplots 
%with specified titles and labels.
subplot(2,2,1), hist(r5,60), title('r5');
subplot(2,2,2), hist(r6,60), title('r6');
subplot(2,2,3), hist(r7,60), title('r7');
subplot(2,2,4), hist(r8,60), title('r8');
saveas(gcf, 'figures/5.png');
% END: Question-5

%% START: Question-6
%clear old variables
clear; clc;

%create random numbers
z = rand(1,5000);

%create random numbers between 4 and -4
r11 = (z*8)-4;
%create random numbers between 20 and -20
r21 = (z*40)-20;

%create a figure with Name "Question-6"
figure('Name', 'Question-6', 'NumberTitle', 'off');
%plot the histograms of random numbers as subplots 
%with specified titles and labels.
subplot(2,1,1), hist(r11,120), title('r11');
subplot(2,1,2), hist(r21,120), title('r21');
saveas(gcf, 'figures/6.png');
% END: Question-6


