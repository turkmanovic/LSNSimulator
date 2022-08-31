function [data] = GetNodeConsumptionWithTime(filename1)

NODE_LOGFILE_POSITION_TIME        = 1;
NODE_LOGFILE_POSITION_LPFLAG      = 2;
NODE_LOGFILE_POSITION_LPINFO      = 3;
NODE_LOGFILE_POSITION_CONSUMPTION = 12;


readFormat = repmat('%s',1,12);
NODE_LOGFILE  = fopen(filename1,'r');

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
NODE_Timeset1 = cellfun(@str2double,NODE_TimeCellOnly);

NODE_ConsumptionCellOnly = {NODE_WithoutLPInfo{:,12}};
NODE_Consumption = cellfun(@str2double,NODE_ConsumptionCellOnly);

NODE_ACTIVE_Index = find(ismember({NODE_CONTENT{:,2}},'A'));
NODE_CONTENT(NODE_ACTIVE_Index,2) = {'1'};

NODE_LP_Index = find(ismember({NODE_CONTENT{:,2}},'LP'));
NODE_CONTENT(NODE_LP_Index,2) = {'0'};

NODE_TimeCellOnly = {NODE_CONTENT{:,1}};
NODE_Timeset = cellfun(@str2double,NODE_TimeCellOnly);

NODE_ActiveStatus = {NODE_CONTENT{:,2}};
NODE_ActiveStatus = cellfun(@str2double,NODE_ActiveStatus);


NODE_ConsumptionWithTime = [NODE_Timeset1; NODE_Consumption];

data = NODE_ConsumptionWithTime;

end