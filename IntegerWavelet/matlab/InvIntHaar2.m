function [Im] = InvIntHaar2(A, H, V, D)
% Reconstructs the Image(256 color) I from Approximation
% and detail coeffecients using Inverse 2D Haar transform


[m,n] = size(A);

for j = 1:n
    for i = 1:m
        ii = 2 * i - 1;
        jj = j;
        
        S(ii, jj) = A(i, j) + floor((V(i, j)+1)/2);
        S(ii+1, jj) = S(ii, jj) - V(i, j);
        DD(ii, jj) = H(i, j) + floor((D(i, j)+1)/2);
        DD(ii+1, jj) = DD(ii, jj) - D(i, j);
    end
end

for  i = 1:m*2
    for j = 1:n
        ii = i ;
        jj = 2 * j - 1;
        
        Im(ii, jj) = S(i, j) + floor((DD(i, j)+1)/2);
        Im(ii, jj+1) = Im(ii, jj) - DD(i, j);
    end
end

Im = uint8(Im);
        
     