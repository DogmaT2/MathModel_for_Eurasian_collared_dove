%This programme is written for agypsy%


function xdot=amodel(t,x)

global a q b u alpha lambda mu v


xdot=[a*(1-q)*x(1)-b*x(1)-u*x(1)*x(3) ...
		a*q*x(1)+u*x(1)*x(3)-(b+alpha)*x(2) ...
		lambda*x(2)-mu*x(3)-v*(x(1)+x(2))*x(3)]';


%Programme ends%



