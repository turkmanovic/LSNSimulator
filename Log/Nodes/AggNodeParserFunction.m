function [x y] = AggNodeParserFunction(filename1, filename2)

NODE_LOGFILE_POSITION_TIME        = 1;
NODE_LOGFILE_POSITION_LPFLAG      = 2;
NODE_LOGFILE_POSITION_LPINFO      = 3;
NODE_LOGFILE_POSITION_CONSUMPTION = 12;


readFormat = repmat('%s',1,12);
NODE1_LOGFILE  = fopen(filename1,'r');
NODE2_LOGFILE  = fopen(filename2,'r');

NODE1_SCANNED   = textscan(NODE1_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE1_LOGFILE);
NODE1_CONTENT = NODE1_SCANNED{1};

NODE2_SCANNED   = textscan(NODE2_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE2_LOGFILE);
NODE2_CONTENT = NODE2_SCANNED{1};

%%try to extract node without LP informations
NODE1_WithoutLPInfo = NODE1_CONTENT;
%try to find where node enter in LP
NODE1_LP_EnterPoint_Index = find(ismember({NODE1_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE1_LP_EnterPoint_Time = NODE1_WithoutLPInfo(NODE1_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE1_LP_EnterPoint_Time = cellfun(@str2num,NODE1_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE1_WithoutLPInfo(NODE1_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE1_LP_ExitPoint_Index = find(ismember({NODE1_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE1_LP_ExitPoint_Time = NODE1_WithoutLPInfo(NODE1_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE1_LP_ExitPoint_Time = cellfun(@str2double,NODE1_LP_ExitPoint_Time);
NODE1_WithoutLPInfo(NODE1_LP_ExitPoint_Index,:) = [];

NODE1_TimeCellOnly = {NODE1_WithoutLPInfo{:,1}};
NODE1_Timeset1 = cellfun(@str2double,NODE1_TimeCellOnly);

NODE1_ConsumptionCellOnly = {NODE1_WithoutLPInfo{:,12}};
NODE1_Consumption = cellfun(@str2double,NODE1_ConsumptionCellOnly);

NODE1_ACTIVE_Index = find(ismember({NODE1_CONTENT{:,2}},'A'));
NODE1_CONTENT(NODE1_ACTIVE_Index,2) = {'1'};

NODE1_LP_Index = find(ismember({NODE1_CONTENT{:,2}},'LP'));
NODE1_CONTENT(NODE1_LP_Index,2) = {'0'};

NODE1_TimeCellOnly = {NODE1_CONTENT{:,1}};
NODE1_Timeset = cellfun(@str2double,NODE1_TimeCellOnly);

NODE1_ActiveStatus = {NODE1_CONTENT{:,2}};
NODE1_ActiveStatus = cellfun(@str2double,NODE1_ActiveStatus);


NODE1_ConsumptionWithTime = [NODE1_Timeset1; NODE1_Consumption];


%%try to extract node without LP informations
NODE2_WithoutLPInfo = NODE2_CONTENT;
%try to find where node enter in LP
NODE2_LP_EnterPoint_Index = find(ismember({NODE2_CONTENT{:,3}},'ENTER'));
%get time when node enter in LP
NODE2_LP_EnterPoint_Time = NODE2_WithoutLPInfo(NODE2_LP_EnterPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE2_LP_EnterPoint_Time = cellfun(@str2num,NODE2_LP_EnterPoint_Time);
%delet content where node enter in LP mode
NODE2_WithoutLPInfo(NODE2_LP_EnterPoint_Index,:) = [];
%try to find where node exit from lp
NODE2_LP_ExitPoint_Index = find(ismember({NODE2_WithoutLPInfo{:,3}},'EXIT'));
%get time when node exit from LP
NODE2_LP_ExitPoint_Time = NODE2_WithoutLPInfo(NODE2_LP_ExitPoint_Index,NODE_LOGFILE_POSITION_TIME);
NODE2_LP_ExitPoint_Time = cellfun(@str2double,NODE2_LP_ExitPoint_Time);
NODE2_WithoutLPInfo(NODE2_LP_ExitPoint_Index,:) = [];

NODE2_TimeCellOnly = {NODE2_WithoutLPInfo{:,1}};
NODE2_Timeset1 = cellfun(@str2double,NODE2_TimeCellOnly);

NODE2_ConsumptionCellOnly = {NODE2_WithoutLPInfo{:,12}};
NODE2_Consumption = cellfun(@str2double,NODE2_ConsumptionCellOnly);

NODE2_ACTIVE_Index = find(ismember({NODE2_CONTENT{:,2}},'A'));
NODE2_CONTENT(NODE2_ACTIVE_Index,2) = {'1'};

NODE2_LP_Index = find(ismember({NODE2_CONTENT{:,2}},'LP'));
NODE2_CONTENT(NODE2_LP_Index,2) = {'0'};

NODE2_TimeCellOnly = {NODE2_CONTENT{:,1}};
NODE2_Timeset = cellfun(@str2double,NODE2_TimeCellOnly);

NODE2_ActiveStatus = {NODE2_CONTENT{:,2}};
NODE2_ActiveStatus = cellfun(@str2double,NODE2_ActiveStatus);


NODE2_ConsumptionWithTime = [NODE2_Timeset1; NODE2_Consumption];

h = [0 diff(NODE1_ConsumptionWithTime(2,:))];
[a c1] = intersect(NODE2_Timeset1,NODE1_Timeset1);c1';
[a c2] = intersect(NODE1_Timeset1,NODE2_Timeset1);c2';
h(c2) = h(c2) + NODE2_Consumption(c1);
x = cumsum(h);
y = NODE1_Timeset1;
end