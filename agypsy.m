%This programme is written to solve the 3 coupled equations given in the paper by Foster, Schultz and Hunter 1992.%

%The programme also uses another programme, amodel%


%Gypsy programme, agypsy%


global a q b u alpha lambda mu v

a=input('Birth rate of GM a=');
q=input('Proportion of births infected by vertical transmission q=');
b=input('Natural death rate of GM b=');
u=input('Horizontal transmision coefficient v=');
alpha=input('Disease induced mortallity alpha=');
lambda=input('PIBS enterance rate to the atmosphere landa=');
mu=input('PIBS departure rate from the atmosphere, excluding GM ingestion mu=');
v=input('PIBS ingestion rate by GM nu=');


%Initial conditions%


tS=input('Start time tS=');
tE=input('End time tE=');
xS=input('Initial population levels, in row vector (X0 Y0 W0)=');

Tspan=[tS tE];

[t,x]=ode45('amodel',Tspan,xS);



subplot(3,3,2), plot (t,x);

xlabel('Years')
ylabel('Population')

     s=size(x);
     subplot(3,3,4), plot(t,x(1:s(1),1));
xlabel('Years')
ylabel('Population')
     subplot(3,3,5), plot(t,x(1:s(1),2));
xlabel('Years')
ylabel('Population')
     subplot(3,3,6), plot(t,x(1:s(1),3));
xlabel('Years')
ylabel('Population')
     subplot(3,3,7), plot(x(1:s(1),1),x(1:s(1),2));
xlabel('Population')
ylabel('Population')
     subplot(3,3,8), plot(x(1:s(1),1),x(1:s(1),3));
xlabel('Population')
ylabel('Population')
     subplot(3,3,9), plot(x(1:s(1),2),x(1:s(1),3));
xlabel('Population')
ylabel('Population')

%Programme ends%