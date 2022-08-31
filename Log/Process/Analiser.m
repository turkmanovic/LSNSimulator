close all
clear all


NODE_LOGFILE_Response  = fopen('ElapsedTime.txt','r');

NODE_Data_Response   = textscan(NODE_LOGFILE_Response,'%c %d %s %s %s %s', Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
fclose(NODE_LOGFILE_Response);

Available_Reponse = NODE_Data_Response{3};
Available_Reponse = str2double(Available_Reponse(:,4));
c_Reponse = min(Available_Reponse):0.0001:max(Available_Reponse);
figure
%plot(Available_Reponse);
freq_Reponse = histc(sort(Available_Reponse),c_Reponse)./length(Available_Reponse).*100;
mean(Available_Reponse)
% figure
% subplot(211)
% bar(c_Reponse, freq_Reponse);
% grid on
% grid minor
% xlim([min(c_Reponse) max(c_Reponse)])
% ylim([0 max(freq_Reponse)+1])
% ylabel('\%','interpreter','latex','FontSize', 22)
% title('Distribution of data availability','interpreter','latex','FontSize', 22)
% set(gca,'fontsize',25)
% 
% subplot(212)
% h_Reponse = cumsum(freq_Reponse);
% plot(c_Reponse,h_Reponse)
% grid on
% grid minor
% xlim([min(c_Reponse) max(c_Reponse)])
% ylim([0 max(h_Reponse)+1])
% ylabel('\%','interpreter','latex','FontSize', 22)
% xlabel('\textbf{[$$\mu$$s]}','interpreter','latex','FontSize', 22)
% set(gca,'fontsize',25)


% NODE_LOGFILE_NoResponse  = fopen('ElapsedTime_NoResponse.txt','r');
% 
% NODE_Data_NoResponse   = textscan(NODE_LOGFILE_NoResponse,'%c %d %s %s %s %s', Inf, 'delimiter',',','EndOfLine','\r\n','CollectOutput',true);
% fclose(NODE_LOGFILE_NoResponse);
% 
% Available_NoReponse = NODE_Data_NoResponse{3};
% Available_NoReponse = str2double(Available_NoReponse(:,4));
% c_NoReponse = min(Available_NoReponse):0.0001:max(Available_NoReponse);
% figure
% plot(Available_NoReponse);
% freq_NoReponse = histc(sort(Available_NoReponse),c_NoReponse)./length(Available_NoReponse).*100;
% figure
% subplot(211)
% bar(c_NoReponse, freq_NoReponse);
% grid on
% grid minor
% xlim([min(c_NoReponse) max(c_NoReponse)])
% ylim([0 max(freq_NoReponse)+1])
% ylabel('\%','interpreter','latex','FontSize', 22)
% %title('Distribution of data availability','interpreter','latex','FontSize', 22)
% set(gca,'fontsize',25)
% 
% subplot(212)
% h_NoReponse = cumsum(freq_NoReponse);
% plot(c_NoReponse,h_NoReponse)
% grid on
% grid minor
% xlim([min(c_NoReponse) max(c_NoReponse)])
% ylim([0 max(h_NoReponse)+1])
% ylabel('\%','interpreter','latex','FontSize', 22)
% xlabel('\textbf{[$$\mu$$s]}','interpreter','latex','FontSize', 22)
% set(gca,'fontsize',25)
% % 
% figure
% plot(h_Reponse, c_Reponse)
% hold on
% plot(h_NoReponse,c_NoReponse)
% hold on
% grid on
% grid minor
% slim([0 105])
% 
% figure
% bar(c_Reponse, freq_Reponse);
% hold on
% 
% bar(c_NoReponse, freq_NoReponse);
% grid on
% grid minor

