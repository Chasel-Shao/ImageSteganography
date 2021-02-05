function [A, H, V, D] = IntHaar2(Im)
% Transforms the Image I(256 color) using the 2D Haar transform

[m,n] = size(Im);
DIm = double(Im);
%S = zeros(m, floor(n/2));
%DD = zeros(m, floor(n/2));

for i = 1:m
    for j = 1:2:n-1
        ii = i;
        jj = floor((j+1)/2);
        
        S(ii, jj) = floor((DIm(i,j) + DIm(i,j+1))/2);
        DD(ii, jj)= DIm(i,j) - DIm(i,j+1);
    end
end

for j = 1:floor(n/2)
    for i = 1:2:m-1
        ii =floor((i+1)/2) ;
        jj = j;
        
        A( ii, jj ) = floor((S(i,j) + S(i+1,j))/2) ;
        H( ii, jj ) = floor((DD(i,j) + DD(i+1,j))/2) ;
        V( ii, jj ) = S(i,j) - S(i+1,j) ;
        D( ii, jj ) = DD(i,j) - DD(i+1,j);
    end
end

