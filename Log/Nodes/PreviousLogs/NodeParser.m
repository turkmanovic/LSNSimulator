clear all
close all

NODE_LOGFILE_POSITION_TIME        = 1;
NODE_LOGFILE_POSITION_LPFLAG      = 2;
NODE_LOGFILE_POSITION_LPINFO      = 3;
NODE_LOGFILE_POSITION_CONSUMPTION = 12;


NODE_LOGFILE  = fopen('100B_NoAgg/3.txt','r');
readFormat = repmat('%s',1,12);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
NODE_WithoutLPInfo = NODE_CONTENT;
%try to find where node enter in LP
NODE_LP_EnterPoint_Index = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE_LP_EnterPoint_Time = NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_EnterPoint_Time = cellfun(@str2num,NODE_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint_Index = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE_LP_ExitPoint_Time = NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_ExitPoint_Time = cellfun(@str2double,NODE_LP_ExitPoint_Time);
NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption1 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly1_1 = {NODE_CONTENT{:,1}};
NODE_Timeset1_1 = cellfun(@str2num,NODE_TimeCellOnly1_1);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus = cellfun(@str2num,NODE_ActiveStatus);

figure
plotyy(NODE_Timeset1_1,NODE_ActiveStatus,NODE_Timeset1,NODE_Consumption1)
grid on
grid minor



NODE_LOGFILE  = fopen('100B_Agg10/3.txt','r');
readFormat = repmat('%s',1,12);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
NODE_WithoutLPInfo = NODE_CONTENT;
%try to find where node enter in LP
NODE_LP_EnterPoint_Index = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE_LP_EnterPoint_Time = NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_EnterPoint_Time = cellfun(@str2num,NODE_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint_Index = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE_LP_ExitPoint_Time = NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_ExitPoint_Time = cellfun(@str2double,NODE_LP_ExitPoint_Time);
NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1_10 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption1_10 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly1_1 = {NODE_CONTENT{:,1}};
NODE_Timeset1_1_2 = cellfun(@str2num,NODE_TimeCellOnly1_1);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus_2 = cellfun(@str2num,NODE_ActiveStatus);

figure
plotyy(NODE_Timeset1_1_2,NODE_ActiveStatus_2,NODE_Timeset1_10,NODE_Consumption1_10)
grid on
grid minor






NODE_LOGFILE  = fopen('100B_Agg20/3.txt','r');
readFormat = repmat('%s',1,12);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
NODE_WithoutLPInfo = NODE_CONTENT;
%try to find where node enter in LP
NODE_LP_EnterPoint_Index = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE_LP_EnterPoint_Time = NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_EnterPoint_Time = cellfun(@str2num,NODE_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint_Index = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE_LP_ExitPoint_Time = NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_ExitPoint_Time = cellfun(@str2double,NODE_LP_ExitPoint_Time);
NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1_2 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption1_2 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly1_1 = {NODE_CONTENT{:,1}};
NODE_Timeset1_1_2 = cellfun(@str2num,NODE_TimeCellOnly1_1);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus_2 = cellfun(@str2num,NODE_ActiveStatus);

figure
plotyy(NODE_Timeset1_1_2,NODE_ActiveStatus_2,NODE_Timeset1_2,NODE_Consumption1_2)
grid on
grid minor









NODE_LOGFILE  = fopen('100B_Agg30/3.txt','r');
readFormat = repmat('%s',1,12);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
NODE_WithoutLPInfo = NODE_CONTENT;
%try to find where node enter in LP
NODE_LP_EnterPoint_Index = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE_LP_EnterPoint_Time = NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_EnterPoint_Time = cellfun(@str2num,NODE_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint_Index = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE_LP_ExitPoint_Time = NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_ExitPoint_Time = cellfun(@str2double,NODE_LP_ExitPoint_Time);
NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1_30 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption1_30 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly1_1 = {NODE_CONTENT{:,1}};
NODE_Timeset1_1_2 = cellfun(@str2num,NODE_TimeCellOnly1_1);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus_2 = cellfun(@str2num,NODE_ActiveStatus);

figure
plotyy(NODE_Timeset1_1_2,NODE_ActiveStatus_2,NODE_Timeset1_2,NODE_Consumption1_2)
grid on
grid minor







NODE_LOGFILE  = fopen('100B_Agg40/3.txt','r');
readFormat = repmat('%s',1,12);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
NODE_WithoutLPInfo = NODE_CONTENT;
%try to find where node enter in LP
NODE_LP_EnterPoint_Index = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE_LP_EnterPoint_Time = NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_EnterPoint_Time = cellfun(@str2num,NODE_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint_Index = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE_LP_ExitPoint_Time = NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_ExitPoint_Time = cellfun(@str2double,NODE_LP_ExitPoint_Time);
NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1_40 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption1_40 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly1_1 = {NODE_CONTENT{:,1}};
NODE_Timeset1_1_2 = cellfun(@str2num,NODE_TimeCellOnly1_1);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus_2 = cellfun(@str2num,NODE_ActiveStatus);

figure
plotyy(NODE_Timeset1_1_2,NODE_ActiveStatus_2,NODE_Timeset1_2,NODE_Consumption1_2)
grid on
grid minor









NODE_LOGFILE  = fopen('100B_Agg50/3.txt','r');
readFormat = repmat('%s',1,12);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
NODE_WithoutLPInfo = NODE_CONTENT;
%try to find where node enter in LP
NODE_LP_EnterPoint_Index = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE_LP_EnterPoint_Time = NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_EnterPoint_Time = cellfun(@str2num,NODE_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE_WithoutLPInfo(NODE_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint_Index = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE_LP_ExitPoint_Time = NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE_LP_ExitPoint_Time = cellfun(@str2double,NODE_LP_ExitPoint_Time);
NODE_WithoutLPInfo(NODE_LP_ExitPoint_Index,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1_50 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption1_50 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly1_1 = {NODE_CONTENT{:,1}};
NODE_Timeset1_1_2 = cellfun(@str2num,NODE_TimeCellOnly1_1);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus_2 = cellfun(@str2num,NODE_ActiveStatus);

figure
plotyy(NODE_Timeset1_1_2,NODE_ActiveStatus_2,NODE_Timeset1_2,NODE_Consumption1_2)
grid on
grid minor







figure
plot(NODE_Timeset1, NODE_Consumption1)
hold on
plot(NODE_Timeset1_10, NODE_Consumption1_10)
hold on
plot(NODE_Timeset1_2, NODE_Consumption1_2)
hold on
plot(NODE_Timeset1_30, NODE_Consumption1_30)
hold on
plot(NODE_Timeset1_40, NODE_Consumption1_40)
hold on
plot(NODE_Timeset1_50, NODE_Consumption1_50)
hold on
legend('No Agg','Agg lvl = 10','Agg lvl = 20','Agg lvl = 30','Agg lvl = 40','Agg lvl = 50')
grid on
grid minor
title('Node 3 - Different Agg level ')
xlabel('Vreme')
ylabel('Potrosnja')




%{
NODE_LOGFILE  = fopen('NoAgg/2.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset2 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption2 = cellfun(@str2num,NODE_ConsumptionCellOnly);




NODE_LOGFILE  = fopen('NoAgg/3.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset3 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption3 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_LOGFILE  = fopen('NoAgg/4.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset4 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption4 = cellfun(@str2num,NODE_ConsumptionCellOnly);

figure
plot(NODE_Timeset1, NODE_Consumption1);
hold on
plot(NODE_Timeset2, NODE_Consumption2);
hold on
plot(NODE_Timeset3, NODE_Consumption3);
hold on
plot(NODE_Timeset4, NODE_Consumption4);
legend('1','2','3','4')



NODE_LOGFILE  = fopen('Agg/1.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1agg = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption1agg = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_LOGFILE  = fopen('Agg/2.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset2agg = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption2agg = cellfun(@str2num,NODE_ConsumptionCellOnly);




NODE_LOGFILE  = fopen('Agg/3.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset3agg = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption3agg = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_LOGFILE  = fopen('Agg/4.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset4agg = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption4agg = cellfun(@str2num,NODE_ConsumptionCellOnly);

figure
plot(NODE_Timeset1agg, NODE_Consumption1agg);
hold on
plot(NODE_Timeset2agg, NODE_Consumption2agg);
hold on
plot(NODE_Timeset3agg, NODE_Consumption3agg);
hold on
plot(NODE_Timeset4agg, NODE_Consumption4agg);
legend('1','2','3','4')




NODE_LOGFILE  = fopen('1.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset1Agg4 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption1Agg4 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_LOGFILE  = fopen('2.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset2Agg4 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption2Agg4 = cellfun(@str2num,NODE_ConsumptionCellOnly);




NODE_LOGFILE  = fopen('3.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset3Agg4 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption3Agg4 = cellfun(@str2num,NODE_ConsumptionCellOnly);



NODE_LOGFILE  = fopen('4.txt','r');
readFormat = repmat('%s',1,11);

NODE_SCANNED   = textscan(NODE_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
NODE_CONTENT = NODE_SCANNED{1};

%%try to extract node without LP informations
%try to find where node enter in LP
NODE_LP_EnterPoint = find(ismember({NODE_CONTENT{:,3}},'ENTER'));
NODE_WithoutLPInfo = NODE_CONTENT;
NODE_WithoutLPInfo(NODE_LP_EnterPoint,:) = [];
%try to find where node exit from lp
NODE_LP_ExitPoint = find(ismember({NODE_WithoutLPInfo{:,3}},'EXIT'));
NODE_WithoutLPInfo(NODE_LP_ExitPoint,:) = [];

NODE_TimeCellOnly = {NODE_WithoutLPInfo{:,1}};
NODE_Timeset4Agg4 = cellfun(@str2num,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,11}}
NODE_Consumption4Agg4 = cellfun(@str2num,NODE_ConsumptionCellOnly);

figure
plot(NODE_Timeset1Agg4, NODE_Consumption1Agg4);
hold on
plot(NODE_Timeset2Agg4, NODE_Consumption2Agg4);
hold on
plot(NODE_Timeset3Agg4, NODE_Consumption3Agg4);
hold on
plot(NODE_Timeset4Agg4, NODE_Consumption4Agg4);
legend('1','2','3','4')


figure
plot(NODE_Timeset3agg, NODE_Consumption3agg);
hold on
plot(NODE_Timeset3, NODE_Consumption3);
hold on
plot(NODE_Timeset3Agg4, NODE_Consumption3Agg4);
legend('Agg 2','NoAgg', 'Agg 4')
%}
