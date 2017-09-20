clear
a1 = 1.15;
a2 = 2;
a3 = 6;
k =10;
b = 0.2;
length = 20;
% x = zeros();
% y = zeros(20);
% z = zeros(20);
x(1) = 1;
y(1) = 1;
z(1) = 1;
t = [0:length];
for i=1:length
    x(i+1) = a1*x(i);
    y(i+1) = a2*y(i)/(1+y(i)*(a2-1)/k);
    z(i+1) = a3*z(i)*exp(-b*z(i));
end
plot(t,x,t,y,t,z)