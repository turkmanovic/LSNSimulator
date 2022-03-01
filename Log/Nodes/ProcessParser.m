clear all
close all

SIMULATION_DURATION = 10000;
SIMULATION_RESOLUTION = 10;
SIMULATION_NAME = 'CP/';
FOLDER_NAMEs={
    '100B_100ms_AggNo',
    '100B_100ms_Agg2',
    '100B_100ms_Agg5',
    '100B_100ms_Agg10',
    '100B_100ms_Agg20',
    '100B_100ms_Agg50',
    '100B_100ms_Agg100',};
NUMBER_OF_FILES = 7
a = [];
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
    [N3_N4_Consumption t2]= AggNodeParserFunction(NODE3_NAME, NODE4_NAME);

    N1_N2_Consumption(end)
    a = [a,N1_N2_Consumption(end)./(36000000)];
    plot(t2./1000, N3_N4_Consumption./(36000000))
    grid on
    grid minor
    hold on

end

b = [];
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

    b = [b, mean(DATA_PROCESSING_TIME)./1000];
    interval = 0:SIMULATION_RESOLUTION:SIMULATION_DURATION;

    freq = histc(DATA_PROCESSING_TIME,interval);
    
    subplot(NUMBER_OF_FILES,1,n)
    bar(interval,freq)
    grid on
    grid minor
    hold on    
end

figure
bar([1:NUMBER_OF_FILES],a)
set(gca,'xticklabel',{'50','100','250','500','1000','2500','5000'})
title('Zavisnost energije od kolicine podataka - period komunikacije fiksan 1s')
xlabel('Kolicina podataka [B]')
ylabel('[mAh]')

figure
bar(b)
set(gca,'xticklabel',{'50','100','250','500','1000','2500','5000'})
title('Prosecno vreme dostupnosti podataka - period komunikacije fiksan 1s')
xlabel('Kolicina podataka [B]')
ylabel('Prosecno vreme [s]')