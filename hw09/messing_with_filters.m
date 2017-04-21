figure(1)
w = [0:0.1:50]';
s = 1i * w;
G1 = 10 ./ (s+10);
G2 = 10^2 ./ ((s+10*exp(1i*pi()/4)).*(s+10*exp(-1i*pi()/4)));
G3 = 10^3 ./ ((s+10).*(s+10*exp(1i*pi()/3)).*(s+10*exp(-1i*pi()/3)));
plot(w, abs([G1,G2,G3]));
xlabel('rad/sec');
ylabel('gain');

figure(2)
angle = 65;
angle2 = 60;
G4 = 10^3 ./ ((s+10).*(s+10*exp(1i*pi()/180*angle)).*(s+10*exp(-1i*pi()/180*angle)));
G5 = 10^3 ./ ((s+10).*(s+10*exp(1i*pi()/180*angle2)).*(s+10*exp(-1i*pi()/180*angle2)));
G6 = 10^3 ./ ((s+10).*(s+10*exp(1i*pi()/180*60)).*(s+10*exp(-1i*pi()/180*60)));

plot(w, abs([G3,G4,G5,G6]));
xlabel('rad/sec');
ylabel('gain');
