%% START: Question-9
%clear old variables
clear;clc;

%read the image
img = imread('image/lena.png');

%take dimentions of the image
[m,n,p] = size(img);

%take the gray version of the image
img_gray = rgb2gray(img);

%calculate the mean of the matrix
matrix_mean = mean(img_gray(:));

%calculate the standard deviation of elements of the matrix
matrix_std  = std(double(img_gray(:)));

%find value and linear index of the maximum element of the gray image matrix
[matrix_max, max_idx]  = max(img_gray(:));
%convert linear index to row and column location
[max_row, max_col] = ind2sub(size(img_gray), max_idx);

%find value and linear index of the maximum element of the gray image matrix
[matrix_min, min_idx]  = min(img_gray(:));
%convert linear index to row and column location
[min_row, min_col] = ind2sub(size(img_gray), min_idx);

%shows initial and gray forms of the image
subplot(1,2,1), imshow(img);
subplot(1,2,2), imshow(img_gray);
saveas(gcf, "figures/9.png")
% END: Question-9