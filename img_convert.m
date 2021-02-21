clear; clc; close all;

width = 64;
height = 32;
depth = 5;
gamma = 2.4;

%% Image process

IMG = imread('sankarea.bmp');
IMG = imresize(IMG, [height, width]);

for i = 1:1:3
    for j = 1:1:height
        for k = 1:1:width
            fprintf('%d, ', IMG(j,k,i));
        end
        fprintf('\n');
    end
end

imshow(IMG);

%% Gamma correction LUT
% [0 .. 255] -> [0 .. 2^d-1]

garray = 0:1:255;
rarray = 255*(garray/255).^gamma;
rarray = bitshift(uint8(rarray), -(8-depth));