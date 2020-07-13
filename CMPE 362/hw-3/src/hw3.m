%% Clear Old Variables
clear; clc;   

%% Read Image
img = imread("img/jokerimage.png");
figure('Name','Original');
imshow(uint8(img));

%% Blur
%blur kernel
blur = [
    1 1 1;
    1 1 1;
    1 1 1]/9;

%apply blur filter
imgBlur = apply_effect(img, blur);
imgBlur = uint8(imgBlur);

%display result
figure('Name','Blurred');
imshow(imgBlur);

%% Sharpening
%sharpness kernel
sharp = [
    -1 -1 -1;
    -1 9 -1;
    -1 -1 -1];

%apply sharpness filter
imgSharp = apply_effect(imgBlur, sharp);
imgSharp = uint8(imgSharp);

%display result
figure('Name','Sharpened');
imshow(imgSharp);

%% Edge
%sobel kernels kernel
top_sobel = [1 2 1;0 0 0;-1 -2 -1];
bottom_sobel = [-1 -2 -1;0 0 0;1 2 1];
left_sobel = [1 0 -1;2 0 -2;1 0 -1];
right_sobel = [-1 0 1;-2 0 2;-1 0 1];

%apply bottom sobel filter
imgBottomSobel = apply_effect(img, bottom_sobel);
imgBottomSobel(imgBottomSobel<0) = 0;

%apply rignt sobel filter
imgRightSobel = apply_effect(img, right_sobel);
imgRightSobel(imgRightSobel<0) = 0;

%calculate final result
imgEdge = (imgBottomSobel.^2 + imgRightSobel.^2).^(1/2);
imgEdge = uint8(imgEdge);

%display result
figure('Name','Edge Detection');
imshow(imgEdge);

%% Emboss
%embossing kernel
emboss = [
    2 1 0;
    1 1 -1;
    0 -1 -2];

%apply embossing filter
imgEmboss = apply_effect(img, emboss);
imgEmboss = uint8(imgEmboss);

%display result
figure('Name','Embossed');
imshow(imgEmboss);
%% Functions
function img_final = apply_effect(img, kernel)
    %separate color channels
    imgR = img(:,:,1);
    imgG = img(:,:,2);
    imgB = img(:,:,3);

    %take kernel size
    K = size(kernel);
    
    %add padding to img
    imgR_p = add_padding(imgR,K);
    imgG_p = add_padding(imgG,K);
    imgB_p = add_padding(imgB,K);
    
    %apply convolution
    img_finalR = my_conv2(double(kernel),double(imgR_p));
    img_finalG = my_conv2(double(kernel),double(imgG_p));
    img_finalB = my_conv2(double(kernel),double(imgB_p));
    
    %merge color channels
    img_final = cat(3, img_finalR, img_finalG, img_finalB);
end

function result = my_conv2(kernel, img)
    %take size of the matrixes
    I = size(img);
    K = size(kernel);
    new_img = zeros(512,512);
    %iterate over image
    for i = 1:(I(1)-K(1)+1)
        for j = 1:(I(2)-K(2)+1)
            %calculate the pixel value for the current location
            pixel_value = 0;
            for a = 1:K(1)
                for b = 1:K(2)
                    pixel_value = pixel_value + kernel(a,b)*img(i+a-1,j+b-1);
                end
            end
            %set the pixel value
            new_img(i,j) = pixel_value;
        end
    end
    result = new_img;
end

function img_with_padding = add_padding(img, K)
    % take size of img
    img_size = size(img);
    
    %calculate padding gap
    gap = (K(1)-1)/2;
    
    %add zero padding
    img_with_padding = [zeros(gap,img_size(1)+(gap*2)); [zeros(img_size(2),gap), img, zeros(img_size(2),gap)]; zeros(gap,img_size(1)+(gap*2))];
end