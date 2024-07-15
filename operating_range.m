clc
clear all
close all

%%

load("R.mat")

V = linspace(0,25,100); % for different Voc, we need to select different value

Vt = 0.0259;

n =75; % FOR Voc = 5 V, n=3.4 for 5V n=33 for 5, 38 for 9 n=50 for 16 n=75 for 25V
% n = 80 ; % FOR Voc = 25 V

Io = 1.7e-8;

I_ph = 0.8;

I = Io*exp(V/(n*Vt));

I_ph = max(I)

I = -I + I_ph;

figure(1)
plot(V,I,'LineWidth',1.5)
xlabel('Output Voltage V (V)')
ylabel('Current I (A)')
grid on


R_eqn = V./I;

R_near = [];

indx = [];
for i = 1:16
    [temp,idx] = min(abs(R_eqn-R(i)));
    R_near = [R_near,temp];
    indx = [indx idx];
end

% V(indx);
R_cell = num2cell(R);

figure(1)
xline([V(indx)],'--',R_cell,'linewidth',1)

str = ['I_{ph} = ',num2str(I_ph,'%0.2f'),' A, V_{oc} = ',num2str(V(end)),' V']
title(str)
set(gca,'FontSize',12,'fontweight','b')
saveas(gcf,'f14.jpg','jpeg');

clc;
clear;
close all;

Voc = [0.5, 5, 9, 16, 25];
Imax = [0.12, 1.74, 3.75, 6.97, 13.6];
Imin = [1.2e-4, 1.2e-3, 2.8e-3, 4.6e-3, 1e-2];

figure;
semilogy(Voc, Imax, '--o', 'LineWidth', 2); % Plotting maximum values
hold on;
semilogy(Voc, Imin, '--o', 'LineWidth', 2); % Plotting minimum values

% Fill the space between the two curves with gray color
patch([Voc fliplr(Voc)], [Imax fliplr(Imin)], 'k', 'FaceColor', [0.7, 0.7, 0.7], 'EdgeColor', 'none');

hold off;

xlabel('Open circuit Voltage (V)');
ylabel('Short Circuit Current (A)');
title('Range of Voc and Isc');
legend('Maximum Isc', 'Minimum Isc', 'Operating Region', 'Location', 'Best');
set(gca,'FontSize',12,'fontweight','b')
grid on;
