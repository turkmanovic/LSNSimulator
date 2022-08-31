clear all
close all

S = [1 10 25 50 100 250 500 1000 2500 5000 10000];
Results = [4.0602 4.0754 4.5589 6.1448 9.4572 19.3832 36.275 69.881 170.733 338.934 800.945]

figure
plot(S, Results)
grid on
grid minor
ylabel('[mS]','interpreter','latex','FontSize', 22)
xlabel('Scalling Factor','interpreter','latex','FontSize', 22)
set(gca,'fontsize',25)