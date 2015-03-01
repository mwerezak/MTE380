%syms a b d
syms a b theta
d = sqrt(a^2 + b^2 - 2*a*b*cos(theta));
x = (d^2 + a^2 - b^2)/(2*d);
y = sqrt(4*a^2*d^2 - (d^2 + a^2 - b^2)^2)/(2*d);

%syms err_a err_b err_d
syms err_a err_b err_theta
err_x = sqrt(  sqrt((diff(x,1,a)*err_a)^2 + (diff(x,1,b)*err_b)^2 + (diff(x,1,theta)*err_theta)^2));
err_y = subs(  sqrt((diff(y,1,a)*err_a)^2 + (diff(y,1,b)*err_b)^2 + (diff(y,1,theta)*err_theta)^2), sqrt(a^2 + b^2 - 2*a*b*cos(theta)), d);

%err_y = (err_b.^2.*(((4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*b.*cos(theta).*a).^2).^(1./2).*(2.*b - 2.*a.*cos(theta)))./(4.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2)) - (4.*a.^2.*(2.*b - 2.*a.*cos(theta)) + 4.*a.*cos(theta).*(2.*a.^2 - 2.*b.*cos(theta).*a))./(4.*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*a.*b.*cos(theta)).^2).^(1./2).*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2))).^2 + err_a.^2.*(((4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*b.*cos(theta).*a).^2).^(1./2).*(2.*a - 2.*b.*cos(theta)))./(4.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2)) - (8.*a.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - 2.*(2.*a.^2 - 2.*b.*cos(theta).*a).*(4.*a - 2.*b.*cos(theta)) + 4.*a.^2.*(2.*a - 2.*b.*cos(theta)))./(4.*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*a.*b.*cos(theta)).^2).^(1./2).*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2))).^2 + err_theta.^2.*((8.*a.^3.*b.*sin(theta) - 4.*a.*b.*sin(theta).*(2.*a.^2 - 2.*b.*cos(theta).*a))./(4.*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*a.*b.*cos(theta)).^2).^(1./2).*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(1./2)) - (a.*b.*sin(theta).*(4.*a.^2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2) - (2.*a.^2 - 2.*b.*cos(theta).*a).^2).^(1./2))./(2.*(a.^2 - 2.*cos(theta).*a.*b + b.^2).^(3./2))).^2).^(1./2)