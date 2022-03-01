function [x,y] = DataProccessTimeDistribution(filename)

PROCESS_LOGFILE  = fopen(filename,'r');
readFormat = repmat('%s',1,6);

PROCESS_SCANNED   = textscan(PROCESS_LOGFILE,readFormat, Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(PROCESS_LOGFILE);
PROCESS_CONTENT = PROCESS_SCANNED{1};

%%try to extract node without LP informations
PROCESS_WithoutAggregation = PROCESS_CONTENT;
%try to find where node enter in LP
PROCESS_AggDataIndex_Index = find(ismember({PROCESS_WithoutAggregation{:,1}},'A'));
%delet content where node enter in LP mode
PROCESS_WithoutAggregation(PROCESS_AggDataIndex_Index,:) = [];

x = {PROCESS_WithoutAggregation{:,2}};
y = {PROCESS_WithoutAggregation{:,6}};

x = cellfun(@str2double,x);
y = cellfun(@str2double,y);


end