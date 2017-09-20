% A program to compute the integrodifference equations of the Collared Dove
% Growth-Dispersal
% Nt curve is plotted after dispersal

%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% 
%%%%% Pre - Initialization:
%%%%% 
%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear
figure(2)
clf;
F_P = [170 140 800 600];
Fig = gcf;
set(Fig, 'position', F_P);

%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% 
%%%%% Initialization:
%%%%% 
%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%% System Constants:
Generation_Count = 30;

%%%%% Spatial Discretization:
% The physical domain is defined for -50 <= x <= 50.
% However, for the FFT, the computational domain is buffered with zeros for 
% -100 <= x < -50 and 50 <= x <= 100.
% (The number of grid points should be a power of 2 for the FFT solve.)

xl=35;
P      = 15;                   % 2^P data points for the Physical domain
Px     = 2^(P+1);              % Number of data points in the computational domain
x_step = 2*xl / (2^(P+1));     % 2^(P+1) data points for the computational domain
x      = [-xl : x_step : xl - 0.001*x_step];
x_plot = [0 : x_step : xl - 0.001*x_step];

%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% 
%%%%% Define the Initial Population:
%%%%% 
%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%% Define the initial population:
% This is a delta function IC:
N1                      = zeros(1,Px);
N1                      = 0.5*(abs(x-1)<=1);
% N1(Px/4+3:3*Px/4-1)   = 34;

%%%%% Plot the initial population:
% subplot(2,3,4);
% plot( x_plot, N1(2^P+1:2^(P+1)), 'b-')
% xlabel( 'x');
% ylabel( 'N_{t}');
% title(  'Initial Population Density');
% axis([0,0.1*xl,0,max(N1)]);
% pause( 0.2 ); 

%%%%% Wave Front:
% Population threshold for the wave speed calculation.
wave_front = 0.1;

% To suppress numerical error, a week Allee effect is introduced.
Allee_tol = 10^(-10) * ones( size( x ) );

%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% 
%%%%% Dispersal Kernel: x is the distance from the reference point in 100Km
%%%%% 
%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%Normal + Laplace distribution:
 a1 = 0.07532;
 a2 = 10.1;
 b2 = 0.08904;
 c1 = 3.906;
 c2 = 1.988;
 K1 = a1 * exp(-(abs(x)/c1).^2)+b2*a2/c2*exp(-a2*abs(x)/c2);
 K1 = K1/(x_step*sum(K1)); % normalize

%%%%% Plot the dispersal Kernel:
% subplot(2,3,5);
% plot( x_plot-0.5*xl, K1(2^(P-1)+1:2^(P+1)-2^(P-1)), 'k-'); 
% %plot( x(2^(P-1)+1:2^(P+1)-2^(P-1)), K1(2^(P-1)+1:2^(P+1)-2^(P-1)), 'k-');axis([-10,10,0,1])
% xlabel( 'x');
% ylabel( 'y');
% title(  'Dispersal Kernel');

%%%%% Compute the FFT of the Dispersal Kernel:
%       calculate the fft of K1, multiplying by x_step to account
%       for the additional factor of Px and converting from a
%       interval length of 0 to 2*xl.  The fftshift accounts for
%       using an interval of (-xl,xl) as opposed to (0,2*xl).
% 
K_w1  = fft( fftshift( K1 ) )*x_step;


%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% 
%%%%% Compute the solution to the IDE:
%%%%% 
%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% parameters for growth dynamics
s = 0.4845;  
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   
% Convert T(z) to T(x) where z is the latitude w.r.t Florida
% and x is the corresponding distance from Florida in 100Km.    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
sigma   = 18.6;
pp1     = 16.07;
q1      = 13.84;
T_x1    = ones(1,0.5*Px);
T_xfit  = pp1./(x_plot+q1*ones(1,0.5*Px));
tempT   = T_xfit-T_x1;
tempT2  = sign(tempT);
[YT,indn1]          = min(tempT2);
T_x1(indn1:0.5*Px)  = T_xfit(indn1:0.5*Px);
T_x     = ones(1,Px);
T_x(0.5*Px+1:Px)=T_x1;
    
%c = 0.558; % 2 eggs/breeding pair * survival rate of 27.9%
c       = 0.558;
delta   = 1.5;  % carrying capacity of  y = H(P) default: delta = 16.67
%%%%% Compute N_{t}(x) for t = 1 to Generation_Count:
Npl1    = N1;

for i = 1:1:Generation_Count
 
%%%%% Growth Stage:
% Growth equations (coupled)
  % partitions the vector Npl1 by the number of clutches per breeding
  % season
  
  % First, determine the indeces
  % 0<= T(x) < 1/(max_clutch) --> 0 clutches
  % 1/(max_clutch)<= T(x) < 2/(max_clutch) --> 1 clutches
  % 2/(max_clutch)<= T(x) < 3/(max_clutch) --> 2 clutches
  % 3/(max_clutch)<= T(x) < 4/(max_clutch) --> 3 clutches
  % 4/(max_clutch)<= T(x) < 5/(max_clutch) --> 4 clutches
  % :
  % (max_clutch-1)/(max_clutch)<= T(x) < 1 --> max_clutch-1 clutches
  % T(x) = 1 -->( max_clutch)clutches
   temp3 = T_x(0.5*Px+1:Px);
   tempNpl1 = Npl1(0.5*Px+1:Px);  % population density on the right half domain(physical domain)
   %determine the maximum/minimum numbers of clutches for given T(x). Note:
   %T(x) is a decreasing function.
   
   max_clutch =6; % maximum number of clutches possible per year
   Cmax = floor(max_clutch*temp3(1));
   Cmin = floor(max_clutch*temp3(0.5*Px));
   d = Cmax - Cmin;
   vec_ind = zeros(1,d);
   
   j=1;
   for k = Cmax:-1:Cmin+1
       temp4 = sign(temp3 -(k/max_clutch));
       [minval,indn2] = min(temp4);
       vec_ind(j) = indn2;  
       j=j+1;
   end 
   k = 1;
   C = max(Cmin,1);
  % empty_nest = 1;
   %stop = 0;
  %while empty_nest & ~stop
   for  l= Cmax:-1:C       
       if l == Cmax 
           M1=tempNpl1(1:vec_ind(k)-1);
       elseif l == C 
           M1=tempNpl1(vec_ind(k-1):0.5*Px);
           %stop = 1;
       else
           M1=tempNpl1(vec_ind(k-1):vec_ind(k)-1);
       end
       %initialize
       N_old = M1;
       P_old = zeros(1,length(M1));
       Q_new = zeros(1,length(M1));
       R_old = zeros(1,length(M1));
       delta_new = delta*ones(1,length(M1));
       Offsp_total = zeros(1,length(M1));
       
       for j = l:-1:1
       N_new = N_old-2*P_old;%unmated birds
       P_new = (N_new).^2./(4*max_clutch/sigma+2*N_new); % potential breeding pairs newly formed 
       Q_new = (P_new+R_old)./(1+P_new+R_old./delta_new); %pairs that found nest
       R_new = P_new + R_old - Q_new;%pairs that could not find nest but remain as pairs till breeding season ends
       Offsp_total =Offsp_total+c*j*Q_new; 

       %update
       N_old = N_new;
       P_old = P_new;
       R_old = R_new;
       delta_new = delta_new - Q_new;
%             if delta_new <= 0  % all nests are occupied and no more breeding takes place
%                empty_nest = 0;
%             end
       end 
       if l == Cmax
          temp5 = Offsp_total;
       else
          temp5 = [temp5 Offsp_total];   
       end
     k = k+1;
   end % first for loop
%end % while loop
       %tempoffsp = zeros(1,Px); 
       %if empty_nest ==1
          %tempoffsp(0.5*Px+1:Px) = temp5;
      % else
          tempoffsp(0.5*Px+1:0.5*Px+length(temp5)) = temp5;
       %end
  new_offsp = tempoffsp;
  temp2 = s*Npl1 + new_offsp;
  Npl1 = temp2; 

  
  Na   = Npl1;
  Nr   = Npl1(Px:-1:1);
    
 %%%%% FFT of the Na, Nr:
 % Na---actual population before dispersal 
 % Nr---reflected populuation before dispersal
  N_w1       = fft( Na );
  N_w2       = fft( Nr );
    
%%%%% Dispersal Stage:
% Convolve the dispersal kernel K with N_k-1:
  Npl_w1     = ( K_w1 .* N_w1 );
  Npl_w2     = ( K_w1 .* N_w2 );
  
% IFFT the population:
  temp1      =real(ifft( Npl_w1 + Npl_w2 ));
        % Zero any complex component of the FFT.  
        
% Implement reflexive boundary conditions:
  Npl1=temp1.*(x>=0);
  
% Week Allee effect:
% (Needed for any numerical noise at the leading edge of the population.)
  N_noise1 = le( Allee_tol, Npl1);
  Npl1     = Npl1 .* N_noise1;
  
  % update the population
  Np1_plot1  = Npl1;  
%%%%% Plot the new generation:
%if i/5 == floor(i/5)
figure(3)
  hold on
  plot( x_plot, Np1_plot1(2^P+1:2^(P+1)), 'k-')
  xlabel('Distance');
  ylabel('Density');
  hold off
%end
%%%%% Compute the location of the wave front:
  T_11 = Np1_plot1(2^P+1:2^(P+1)) - wave_front;
  T_21 = sign( T_11);
  [Y,ndx1(i)]  = min(T_21);

%%%%% Plot the location of the wave front from x = 0:
%   subplot(2,3,6);
figure(4)
  plot( [1:1:i], ndx1*x_step, 'ko-');
  axis( [0, Generation_Count, 0, 1.1*max(ndx1*x_step)] );
  xlabel( 'Generation');
  ylabel( 'x');
  %title(  'Wave front location of the dove');
      
%%%% Update the generation count:
  Gen_Num = i;
  pause( 0.2 );    % Pause to refresh the screen.
  
   end