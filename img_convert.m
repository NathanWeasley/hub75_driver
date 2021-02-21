clear; clc; close all;

width = 64;
height = 32;
depth = 5;

IMG = double(imread('rainbow.bmp')) / 255;
IMG = imresize(IMG, [height, width]);

% Gamma correction
gamma = 0.45;
IMG = uint8(255*IMG.^(1/gamma));

IMG = bitshift(IMG, -(8-depth));

for i = 1:1:3
    for j = 1:1:height
        for k = 1:1:width
            fprintf('%d, ', IMG(j,k,i));
        end
        fprintf('\n');
    end
end

imshow(IMG*(2^(8-depth)));
