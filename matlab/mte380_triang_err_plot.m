%syms a b d
%syms a b theta
%d = sqrt(a^2 + b^2 - 2*a*b*cos(theta));
D = 300; %cm

%syms err_a err_b err_d
%syms err_a err_b err_theta
err_a = 0.5; %cm
err_b = 0.5; %cm
err_theta = 0*(pi/180);

%define a,b,theta as functions of x,y
a = @(x, y) sqrt(x.^2 + y.^2);
b = @(x, y) sqrt((x-D).^2 + y.^2);
theta = @(x, y) atan(x./y) + atan((D-x)./y);

%convert a,b,theta to plotable matrices
[X,Y] = meshgrid(0:1:300, 30:1:300);
a = a(X,Y);
b = b(X,Y);
theta = theta(X,Y);
d = sqrt(a.^2 + b.^2 - 2.*a.*b.*cos(theta));

%sanity check: plot xx, yy
%xx = (d.^2 + a.^2 - b.^2)/(2.*D);
%yy = sqrt(4*a.^2*D.^2 - (D.^2 + a.^2 - b.^2).^2)./(2*D);

err_x = (err_a.^2.*((4.*a - 2.*b.*cos(theta))./(2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2)) - ((2.*a.^2 - 2.*b.*cos(theta).*a).*(2.*a - 2.*b.*cos(theta)))./(4.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2))).^2 + err_b.^2.*(((2.*a.^2 - 2.*b.*cos(theta).*a).*(2.*b - 2.*a.*cos(theta)))./(4.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2)) + (a.*cos(theta))./(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2)).^2 + err_theta.^2.*((a.*b.*sin(theta))./(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2) - (a.*b.*sin(theta).*(2.*a.^2 - 2.*b.*cos(theta).*a))./(2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2))).^2).^(1./4);
err_y = (err_b.^2.*(((4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*b.*cos(theta).*a).^2).^(1./2).*(2.*b - 2.*a.*cos(theta)))./(4.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2)) - (4.*a.^2.*(2.*b - 2.*a.*cos(theta)) + 4.*a.*cos(theta).*(2.*a.^2 - 2.*b.*cos(theta).*a))./(4.*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*a.*b.*cos(theta)).^2).^(1./2).*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2))).^2 + err_a.^2.*(((4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*b.*cos(theta).*a).^2).^(1./2).*(2.*a - 2.*b.*cos(theta)))./(4.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2)) - (8.*a.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - 2.*(2.*a.^2 - 2.*b.*cos(theta).*a).*(4.*a - 2.*b.*cos(theta)) + 4.*a.^2.*(2.*a - 2.*b.*cos(theta)))./(4.*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*a.*b.*cos(theta)).^2).^(1./2).*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2))).^2 + err_theta.^2.*((8.*a.^3.*b.*sin(theta) - 4.*a.*b.*sin(theta).*(2.*a.^2 - 2.*b.*cos(theta).*a))./(4.*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*a.*b.*cos(theta)).^2).^(1./2).*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2)) - (a.*b.*sin(theta).*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*b.*cos(theta).*a).^2).^(1./2))./(2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2))).^2).^(1./2);

%mesh(X,Y,xx);
%mesh(X,Y,yy);
%mesh(X,Y,err_x);
mesh(X,Y,err_y);