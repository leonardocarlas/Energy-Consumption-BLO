/*********************************************
 * OPL 20.1.0.0 Model
 * Author: LEONARDO CARLASSARE
 * Creation Date: 3 nov 2021 at 11:34:55
 *********************************************/


/* Datas */

{string} IntervalPrices = ...; 
{string} J = ...;

int T = ...;
range Time = 1..T;
int L = ...;
range PowerLevels = 1..L;
int IPstart_interval[IntervalPrices] = ...;
int IPend_interval[IntervalPrices] = ...;
float Prices[IntervalPrices] = ...;

float instantPrices[Time] = ...;

int T1_J[J] = ...;
int T2_J[J] = ...;
int d_J[J] = ...;

float pricePerPowerLevel[PowerLevels] = ...; 
float maxPowerPerPowerLevel[PowerLevels] = ...; 



/* Decision vaiables */

// P(j,t)
dvar float+ p[J][Time];

// f(j,r) Power required at stage r
tuple DoubleIndex{ string j;  int r; } 
{DoubleIndex} DIndex = {<j,r> | j in J, r in 1..d_J[j]};
int f_J[s in DIndex] = ...;

// w(j,r,t)
tuple TripleIndex{ string j; int r; int t; } 
{TripleIndex} TIndex = {<j,r,t> | j in J, r in 1..d_J[j], t in T1_J[j]..T2_J[j]};
dvar boolean w[s in TIndex];

// P(t)
dvar float+ P[Time];

// u(l)
dvar boolean u[PowerLevels];




/* Objective function */

//dexpr float cost = (1/1000)*(sum(i in IntervalPrices, t in IPstart_interval[i]..IPend_interval[i]) Prices[i]*(sum(j in J) p[j][t]));
dexpr float cost = sum(t in 1..T) P[t] * instantPrices[t] + sum(l in 1..L) pricePerPowerLevel[l] * u[l]  ;
minimize cost;


/* Constraints */

subject to {
  
// Constraint C1
 forall (t in 1..T)
	P[t] == sum(j in J) p[j][t]; 

// Constraint C2
forall (t in Time)
    P[t] <= (sum(l in PowerLevels) maxPowerPerPowerLevel[l] * u[l]);
 
// Constraint C3
 sum(l in PowerLevels) u[l] == 1;

// Constraint  C4
 forall (j in J, t in T1_J[j]..T2_J[j])
  p[j][t] == sum(r in 1..d_J[j]) f_J[<j,r>]*w[<j,r,t>];
  
// Constraint C5
 forall (j in J, t in 1..T1_J[j]-1)
  p[j][t] == 0;
 forall (j in J, t in T2_J[j]+1..T)
  p[j][t] == 0;

// Constraint C6
 forall (j in J, t in T1_J[j]..T2_J[j])
  sum(r in 1..d_J[j]) w[<j,r,t>] <= 1;

// Constraint C7
 forall (j in J, r in 1..d_J[j]-1,t in T1_J[j]..T2_J[j]-1)
  w[<j,r+1,t+1>] >= w[<j,r,t>];

// Constraint C8
 forall (j in J, r in 1..d_J[j])
  sum(t in T1_J[j]..T2_J[j]) w[<j,r,t>] == 1;

// Constraint C9
 forall (j in J)
  sum(t in T1_J[j]..T2_J[j]-d_J[j]+1) w[<j,1,t>] >= 1;
  


}




