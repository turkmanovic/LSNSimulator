clear all
close all

SIMULATION_DURATION = 1000000;
SIMULATION_RESOLUTION = 10;
SIMULATION_NAME = 'AR/';
FOLDER_NAMEs={
    '1000ms_100B_Agg10_C1',
    '1000ms_100B_Agg10_C2',
    '1000ms_100B_Agg10_C5',
    '1000ms_100B_Agg10_C8',
    '1000ms_100B_Agg10_C10',
    '1000ms_100B_Agg10_C20',
    '1000ms_100B_Agg10_C50',
    '1000ms_100B_Agg10_C80',
    '1000ms_100B_Agg10_C100',};
NUMBER_OF_FILES = 9
agg_energy = [];
figure
for n=1:NUMBER_OF_FILES
    FOLDER_NAME = '';
    NODE1_NAME='';
    NODE2_NAME='';
    NODE3_NAME='';
    NODE4_NAME='';
    FOLDER_NAME=char(FOLDER_NAMEs{n});
    NODE1_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/1.txt');
    NODE2_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/2.txt');
    NODE3_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/3.txt');
    NODE4_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/4.txt');
    ELAPSED_TIME = strcat(SIMULATION_NAME, FOLDER_NAME,'/ElapsedTime.txt');

    %NodeParserFunction(NODE1_NAME);
    %NodeParserFunction(NODE2_NAME);
    %NodeParserFunction(NODE3_NAME);
    %NodeParserFunction(NODE4_NAME);

    [N1_N2_Consumption t1]= AggNodeParserFunction(NODE1_NAME, NODE2_NAME);
    %[N3_N4_Consumption t2]= AggNodeParserFunction(NODE3_NAME, NODE4_NAME);

    N1_N2_Consumption(end)
    agg_energy = [agg_energy, N1_N2_Consumption(end)./(36000000)];
    plot(t1./1000, N1_N2_Consumption./(36000000))
    grid on
    grid minor
    hold on

end

agg_time = [];
figure
for n=1:NUMBER_OF_FILES
    FOLDER_NAME = '';
    NODE1_NAME='';
    NODE2_NAME='';
    NODE3_NAME='';
    NODE4_NAME='';
    FOLDER_NAME=char(FOLDER_NAMEs{n});
    NODE1_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/1.txt');
    NODE2_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/2.txt');
    NODE3_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/3.txt');
    NODE4_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/4.txt');
    ELAPSED_TIME = strcat(SIMULATION_NAME, FOLDER_NAME,'/ElapsedTime.txt');
    [NODE_IDS DATA_PROCESSING_TIME] = DataProccessTimeDistribution(ELAPSED_TIME);
    mean(DATA_PROCESSING_TIME);

    agg_time = [agg_time, mean(DATA_PROCESSING_TIME)./1000];
    interval = 0:SIMULATION_RESOLUTION:SIMULATION_DURATION;

    freq = histc(DATA_PROCESSING_TIME,interval);
    
    subplot(NUMBER_OF_FILES,1,n)
    bar(interval,freq)
    grid on
    grid minor
    hold on    
end

figure
loglog([1, 2, 5, 8, 10, 20, 50, 80, 100]./10, agg_energy./agg_energy(5),'LineWidth',3)
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
set(gca,'fontsize',16)
xlabel('\textbf{CR/10}','interpreter','latex','FontSize', 22)
title('\textbf{[I]}','interpreter','latex','FontSize', 24)
grid on
grid minor

figure
loglog([1, 2, 5, 8, 10, 20, 50, 80, 100]./10, agg_time./agg_time(5),'LineWidth',3)
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
set(gca,'fontsize',16)
xlabel('\textbf{CR/10}','interpreter','latex','FontSize', 22)
title('\textbf{[I]}','interpreter','latex','FontSize', 24)
grid on
grid minor

figure
[hA y1 y2] = plotyy([1, 2, 5, 8, 10, 20, 50, 80, 100]./10, agg_energy./agg_energy(5),[1, 2, 5, 8, 10, 20, 50, 80, 100]./10, agg_time./agg_time(5),'loglog')
set(hA(1),'yticklabel',num2str(get(hA(1),'ytick').','%.2f'))
set(hA(1),'fontsize',24)
ylabel(hA(1),'\textbf{E, O}','interpreter','latex','FontSize', 28)
set(hA(2),'yticklabel',num2str(get(hA(2),'ytick').','%.2f'))
set(hA(2),'fontsize',24)
ylabel(hA(2),'\textbf{ADT}','interpreter','latex','FontSize', 28)

axis(hA,'tight');

set(y1,'linestyle','-','linewidth',6);
set(y2,'linestyle','-','linewidth',6);
grid on
grid minor
hold on

agg_egg_nom = agg_energy./agg_energy(5);
agg_time_nom = agg_time./agg_time(5);
save('agg_egg_nom.mat','agg_egg_nom');
save('agg_time_nom.mat','agg_time_nom');
O = 0.8.*agg_energy./agg_energy(5) + 0.2.*agg_time./agg_time(5);

loglog([1, 2, 5, 8, 10, 20, 50, 80, 100]./10, O);


SIMULATION_NAME = 'CP/';
FOLDER_NAMEs={
    '1000ms_100B_AggNo_C10',
    '1000ms_100B_Agg2_C10',
    '1000ms_100B_Agg5_C10',
    '1000ms_100B_Agg8_C10',
    '1000ms_100B_Agg10_C10',
    '1000ms_100B_Agg20_C10',
    '1000ms_100B_Agg50_C10',
    '1000ms_100B_Agg80_C10',
    '1000ms_100B_Agg100_C10'};
NUMBER_OF_FILES = 9
cp_energy = [];
figure
for n=1:NUMBER_OF_FILES
    FOLDER_NAME = '';
    NODE1_NAME='';
    NODE2_NAME='';
    NODE3_NAME='';
    NODE4_NAME='';
    FOLDER_NAME=char(FOLDER_NAMEs{n});
    NODE1_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/1.txt');
    NODE2_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/2.txt');
    NODE3_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/3.txt');
    NODE4_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/4.txt');
    ELAPSED_TIME = strcat(SIMULATION_NAME, FOLDER_NAME,'/ElapsedTime.txt');

    %NodeParserFunction(NODE1_NAME);
    %NodeParserFunction(NODE2_NAME);
    %NodeParserFunction(NODE3_NAME);
    %NodeParserFunction(NODE4_NAME);

    [N1_N2_Consumption t1]= AggNodeParserFunction(NODE1_NAME, NODE2_NAME);
    %[N3_N4_Consumption t2]= AggNodeParserFunction(NODE3_NAME, NODE4_NAME);

    N1_N2_Consumption(end)
    cp_energy = [cp_energy, N1_N2_Consumption(end)./(36000000)];
    plot(t1./1000, N1_N2_Consumption./(36000000))
    grid on
    grid minor
    hold on

end

cp_time = [];
figure
for n=1:NUMBER_OF_FILES
    FOLDER_NAME = '';
    NODE1_NAME='';
    NODE2_NAME='';
    NODE3_NAME='';
    NODE4_NAME='';
    FOLDER_NAME=char(FOLDER_NAMEs{n});
    NODE1_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/1.txt');
    NODE2_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/2.txt');
    NODE3_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/3.txt');
    NODE4_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/4.txt');
    ELAPSED_TIME = strcat(SIMULATION_NAME, FOLDER_NAME,'/ElapsedTime.txt');
    [NODE_IDS DATA_PROCESSING_TIME] = DataProccessTimeDistribution(ELAPSED_TIME);
    mean(DATA_PROCESSING_TIME);

    cp_time = [cp_time, mean(DATA_PROCESSING_TIME)./1000];
    interval = 0:SIMULATION_RESOLUTION:SIMULATION_DURATION;

    freq = histc(DATA_PROCESSING_TIME,interval);
    
    subplot(NUMBER_OF_FILES,1,n)
    bar(interval,freq)
    grid on
    grid minor
    hold on    
end

figure
loglog([1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000,cp_energy./cp_energy(5),'LineWidth',3)
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
set(gca,'fontsize',25)
grid on
grid minor

figure
loglog([1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000,cp_time./cp_time(5),'LineWidth',3)
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
set(gca,'fontsize',16)
xlabel('\textbf{TP[s]/10s}','interpreter','latex','FontSize', 22)
title('\textbf{[II]}','interpreter','latex','FontSize', 24)
grid on
grid minor

figure
[hA y1 y2] = plotyy([1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000,cp_energy./cp_energy(5),[1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000,cp_time./cp_time(5),'loglog')
%%set(hA(1),'yticklabel',num2str(get(hA(1),'ytick').','%.2f'))
set(hA(1),'fontsize',24)
ylabel(hA(1),'\textbf{E, O}','interpreter','latex','FontSize', 28)
%set(hA(2),'yticklabel',num2str(get(hA(2),'ytick').','%.2f'))
set(hA(2),'fontsize',24)
ylabel(hA(2),'\textbf{ADT}','interpreter','latex','FontSize', 28)
%{
set(hA(1),'YLim',[0.96 1.3])
set(hA(2),'YLim',[0.995 1.04])
%}
set(y1,'linestyle','-','linewidth',6);
set(y2,'linestyle','-','linewidth',6);
grid on
grid minor

hold on

cp_energy_nom = cp_energy./cp_energy(5);
cp_time_norm = cp_time./cp_time(5);

save('cp_energy_nom.mat','cp_energy_nom');
save('cp_time_norm.mat','cp_time_norm');

O = 0.8.*cp_energy./cp_energy(5) + 0.2.*cp_time./cp_time(5);




loglog([1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000, O);


SIMULATION_NAME = 'ST/';
FOLDER_NAMEs={
    '100ms_100B_Agg100_C10',
    '200ms_100B_Agg50_C10',
    '500ms_100B_Agg20_C10',
    '625ms_100B_Agg16_C10',
    '1000ms_100B_Agg10_C10',
    '2000ms_100B_Agg5_C10',
    '2500ms_100B_Agg4_C10',
    '5000ms_100B_Agg2_C10',
    '10000ms_100B_AggNo_C10'};
NUMBER_OF_FILES = 9
st_energy = [];
figure
for n=1:NUMBER_OF_FILES
    FOLDER_NAME = '';
    NODE1_NAME='';
    NODE2_NAME='';
    NODE3_NAME='';
    NODE4_NAME='';
    FOLDER_NAME=char(FOLDER_NAMEs{n});
    NODE1_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/1.txt');
    NODE2_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/2.txt');
    NODE3_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/3.txt');
    NODE4_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/4.txt');
    ELAPSED_TIME = strcat(SIMULATION_NAME, FOLDER_NAME,'/ElapsedTime.txt');

    %NodeParserFunction(NODE1_NAME);
    %NodeParserFunction(NODE2_NAME);
    %NodeParserFunction(NODE3_NAME);
    %NodeParserFunction(NODE4_NAME);

    [N1_N2_Consumption t1]= AggNodeParserFunction(NODE1_NAME, NODE2_NAME);
    %[N3_N4_Consumption t2]= AggNodeParserFunction(NODE3_NAME, NODE4_NAME);

    N1_N2_Consumption(end)
    st_energy = [st_energy, N1_N2_Consumption(end)./(36000000)];
    plot(t1./1000, N1_N2_Consumption./(36000000))
    grid on
    grid minor
    hold on

end

st_time = [];
figure
for n=1:NUMBER_OF_FILES
    FOLDER_NAME = '';
    NODE1_NAME='';
    NODE2_NAME='';
    NODE3_NAME='';
    NODE4_NAME='';
    FOLDER_NAME=char(FOLDER_NAMEs{n});
    NODE1_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/1.txt');
    NODE2_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/2.txt');
    NODE3_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/3.txt');
    NODE4_NAME = strcat(SIMULATION_NAME, FOLDER_NAME,'/4.txt');
    ELAPSED_TIME = strcat(SIMULATION_NAME, FOLDER_NAME,'/ElapsedTime.txt');
    [NODE_IDS DATA_PROCESSING_TIME] = DataProccessTimeDistribution(ELAPSED_TIME);
    mean(DATA_PROCESSING_TIME);

    st_time = [st_time, mean(DATA_PROCESSING_TIME)./1000];
    interval = 0:SIMULATION_RESOLUTION:SIMULATION_DURATION;

    freq = histc(DATA_PROCESSING_TIME,interval);
    
    subplot(NUMBER_OF_FILES,1,n)
    bar(interval,freq)
    grid on
    grid minor
    hold on    
end

figure
loglog([100, 200, 500, 625, 1000, 2000, 2500, 5000, 10000]./1000,st_energy./st_energy(5),'LineWidth',3)
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
set(gca,'fontsize',16)

xlabel('\textbf{SR[ms]/1000ms}','interpreter','latex','FontSize', 22)
title('\textbf{[III]}','interpreter','latex','FontSize', 24)
grid on
grid minor

figure
loglog([100, 200, 500, 625, 1000, 2000, 2500, 5000, 10000]./1000,st_time./st_time(5),'LineWidth',3)
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
set(gca,'fontsize',16)
xlabel('\textbf{SR[ms]/1000ms}','interpreter','latex','FontSize', 22)
title('\textbf{[III]}','interpreter','latex','FontSize', 24)
grid on
grid minor



st_time = 10 - st_time; 
figure
[hA y1 y2] = plotyy([100, 200, 500, 625, 1000, 2000, 2500, 5000, 10000]./1000,st_energy./st_energy(5),[100, 200, 500, 625, 1000, 2000, 2500, 5000, 10000]./1000,st_time./st_time(5),'loglog')
set(hA(1),'yticklabel',num2str(get(hA(1),'ytick').','%.2f'))
set(hA(1),'fontsize',24)
ylabel(hA(1),'\textbf{E, O}','interpreter','latex','FontSize', 28)
set(hA(2),'yticklabel',num2str(get(hA(2),'ytick').','%.2f'))
set(hA(2),'fontsize',24)
ylabel(hA(2),'\textbf{ADT}','interpreter','latex','FontSize', 28)
%{
set(hA(1),'YLim',[0.96 1.3])
set(hA(2),'YLim',[0.995 1.04])
%}
set(y1,'linestyle','-','linewidth',6);
set(y2,'linestyle','-','linewidth',6);
grid on
grid minor


hold on

st_energy_norm = st_energy./st_energy(5);
st_time_norm = st_time./st_time(5);

save('st_energy_norm.mat','st_energy_norm');
save('st_time_norm.mat','st_time_norm');
O = 0.8.*st_energy./st_energy(5) + 0.2.*st_time./st_time(5);

loglog([100, 200, 500, 625, 1000, 2000, 2500, 5000 10000]./1000, O);





figure
loglog([1, 2, 5, 8, 10, 20, 50, 80, 100]./10, agg_energy./agg_energy(5),'LineWidth',3,'LineStyle','-')
hold on
loglog([1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000,cp_energy./cp_energy(5),'LineWidth',3,'LineStyle','--')
hold on
loglog([100, 200, 500, 625, 1000, 2000, 2500, 5000, 10000]./1000,st_energy./st_energy(5),'LineWidth',3,'LineStyle',':')
grid on
grid minor
set(gca,'xticklabel',arrayfun(@(x) num2str(x),get(gca,'xtick'),'un',0))
set(gca,'yticklabel',arrayfun(@(x) num2str(x),get(gca,'ytick'),'un',0))
title('Energija')

% figure
% plot(log10([1, 5, 10, 50, 100]./10),log10(agg_time./agg_time(3)))
% hold on
% plot(log10([1000, 5000, 10000, 50000, 100000]./10000),log10(cp_time./cp_time(3)))
% hold on
% plot(log10([100, 500, 1000, 5000, 10000]./1000),log10(st_time./st_time(3)))
% legend('AggLvl','ComPer','SemPer')
% grid on
% grid minor
% title('Prosecno vreme dostupnosti podataka')
%}
