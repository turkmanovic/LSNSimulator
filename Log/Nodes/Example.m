clear all
close all

NodeProducerConsumptionOverTime_100ms_mAms = GetNodeConsumptionWithTime('1_100ms.txt');
NodeProducerConsumptionOverTime_1000ms_mAms = GetNodeConsumptionWithTime('1_1000ms.txt');
NodeProducerConsumptionOverTime_10000ms_mAms = GetNodeConsumptionWithTime('1_10000ms.txt');
NodeProducerConsumptionOverTime_100ms_mAH = NodeProducerConsumptionOverTime_100ms_mAms./(1000*3600);
NodeProducerConsumptionOverTime_1000ms_mAH = NodeProducerConsumptionOverTime_1000ms_mAms./(1000*3600);
NodeProducerConsumptionOverTime_10000ms_mAH = NodeProducerConsumptionOverTime_10000ms_mAms./(1000*3600);

figure
plot(NodeProducerConsumptionOverTime_100ms_mAH(1,:),NodeProducerConsumptionOverTime_100ms_mAH(2,:))
hold on
plot(NodeProducerConsumptionOverTime_1000ms_mAms(1,:),NodeProducerConsumptionOverTime_1000ms_mAms(2,:))
hold on
plot(NodeProducerConsumptionOverTime_10000ms_mAms(1,:),NodeProducerConsumptionOverTime_10000ms_mAms(2,:))

E_100ms = NodeProducerConsumptionOverTime_100ms_mAH(2,end);
E_1000ms = NodeProducerConsumptionOverTime_1000ms_mAH(2,end);
E_10000ms = NodeProducerConsumptionOverTime_10000ms_mAH(2,end);

