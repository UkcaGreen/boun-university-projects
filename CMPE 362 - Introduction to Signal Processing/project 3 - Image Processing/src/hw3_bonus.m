%% Clear Old Variables
clear; clc;   

%% Read Image
img = imread("img/jokerimage.png");
cigarette = imread("img/cigarette.png");
flower = imread("img/flower.png");

%% Detect Feature Points
img = rgb2gray(img);
cigarette = rgb2gray(cigarette);

cigarettePoints = detectSURFFeatures(cigarette);
imgPoints = detectSURFFeatures(img);

%display results
figure;
imshow(cigarette);
title('Strongest Feature Points from Cigarette');
hold on;
plot(cigarettePoints);

figure;
imshow(img);
title('Strongest Feature Points from Joker Image');
hold on;
plot(imgPoints);

%% Extract Feature Descriptors
[cigaretteFeatures, cigarettePoints] = extractFeatures(cigarette, cigarettePoints);
[imgFeatures, imgPoints] = extractFeatures(img, imgPoints);

%% Find Putative Point Matches
cigarettePairs = matchFeatures(cigaretteFeatures, imgFeatures);

matchedCigarettePoints = cigarettePoints(cigarettePairs(:, 1), :);
matchedImgPoints = imgPoints(cigarettePairs(:, 2), :);

%display results
figure;
showMatchedFeatures(cigarette, img, matchedCigarettePoints, ...
    matchedImgPoints, 'montage');
title('Putatively Matched Points (Including Outliers)');

%% Locate the Object in the Scene Using Putative Matches
[tform, inlierCigarettePoints, inlierImgPoints] = ...
    estimateGeometricTransform(matchedCigarettePoints, matchedImgPoints, 'affine');

%display results
figure;
showMatchedFeatures(cigarette, img, inlierCigarettePoints, ...
    inlierImgPoints, 'montage');
title('Matched Points (Inliers Only)');

%% Put the Cigarette in a Rectangle
cigarettePolygon = [1, 1;...                           % top-left
        size(cigarette, 2), 1;...                 % top-right
        size(cigarette, 2), size(cigarette, 1);... % bottom-right
        1, size(cigarette, 1);...                 % bottom-left
        1, 1];                   % top-left again to close the polygon
    
newCigarettePolygon = transformPointsForward(tform, cigarettePolygon);

%display results
figure;
imshow(img);
hold on;
line(newCigarettePolygon(:, 1), newCigarettePolygon(:, 2), 'Color', 'y');
title('Detected Box');

%% Place The Flower Censor
img = imread("img/jokerimage.png");

%display results
figure;
imshow(img);
hold on;
mask = double( any(flower, 3) );
image([min(newCigarettePolygon(:,1)) max(newCigarettePolygon(:,1))], [min(newCigarettePolygon(:,2)) max(newCigarettePolygon(:,2))],flower,'AlphaData', mask);   
title('Flower Added');

