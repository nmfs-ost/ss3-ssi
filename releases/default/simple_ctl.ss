#V3.30.12;_Stock_Synthesis_by_Richard_Methot_(NOAA)
#_File written by GUI version 3.3.12.4 
#C growth parameters are estimated
#C spawner-recruitment bias adjustment Not tuned For optimality
#_data_and_control_files: simple_dat.ss // simple_ctl.ss
0 #_0 means do not read wtatage.ss; 1 means read and use wtatage.ss and also read and use growth parameters
1 #_N_Growth_Patterns
1 #_N_platoons_Within_GrowthPattern 
#_Cond 1 #_Morph_between/within_stdev_ratio (no read if N_morphs=1)
#_Cond 1 # vector_Morphdist_(-1_in_first_val_gives_normal_approx)
#
2 # recr_dist_method for parameters:  2=main effects for GP, Settle timing, Area; 3=each Settle entity; 4=none when N_GP*Nsettle*pop==1
1 # not yet implemented; Future usage: Spawner-Recruitment: 1=global; 2=by area
1 # number of recruitment settlement assignments 
0 # unused option
#GPattern month area age (for each settlement assignment)
 1  1  1  0 
#
#_Cond 0 #_N_movement_definitions goes here if N_areas > 1
#_Cond 1.0 # first age that moves (real age at begin of season, not integer) also cond on do_migration>0
#_Cond 1 1 1 2 4 10 # example move definition for seas=1, morph=1, source=1 dest=2, age1=4, age2=10
#
1 #_Nblock_Patterns
1 #_blocks_per_pattern 
# begin and end years of blocks
 1970 1970 # pattern 1
#
# controls for all timevary parameters 
1 #_env/block/dev_adjust_method for all time-vary parms (1=warn relative to base parm bounds; 3=no bound check)
# autogen
0 0 0 0 0 # autogen 
# where: 0 = autogen all time-varying parms; 1 = read each time-varying parm line; 2 = read then autogen if parm min==-12345
# 1st element for biology, 2nd for SR, 3rd for Q, 4th reserved, 5th for selex
#
# setup for M, growth, maturity, fecundity, recruitment distibution, movement 
#
0 #_natM_type:_0=1Parm; 1=N_breakpoints;_2=Lorenzen;_3=agespecific;_4=agespec_withseasinterpolate
  #_no additional input for selected M option; read 1P per morph
1 # GrowthModel: 1=vonBert with L1&L2; 2=Richards with L1&L2; 3=age_specific_K; 4=not implemented
0 #_Age(post-settlement)_for_L1;linear growth below this
25 #_Growth_Age_for_L2 (999 to use as Linf)
-999 #_exponential decay for growth above maxage (fixed at 0.2 in 3.24; value should approx initial Z; -999 replicates 3.24)
0 #_placeholder for future growth feature
0 #_SD_add_to_LAA (set to 0.1 for SS2 V1.x compatibility)
0 #_CV_Growth_Pattern:  0 CV=f(LAA); 1 CV=F(A); 2 SD=F(LAA); 3 SD=F(A); 4 logSD=F(A)
1 #_maturity_option:  1=length logistic; 2=age logistic; 3=read age-maturity matrix by growth_pattern; 4=read age-fecundity; 5=disabled; 6=read length-maturity
1 #_First_Mature_Age
1 #_fecundity option:(1)eggs=Wt*(a+b*Wt);(2)eggs=a*L^b;(3)eggs=a*Wt^b; (4)eggs=a+b*L; (5)eggs=a+b*W
0 #_hermaphroditism option:  0=none; 1=female-to-male age-specific fxn; -1=male-to-female age-specific fxn
1 #_parameter_offset_approach (1=none, 2= M, G, CV_G as offset from female-GP1, 3=like SS2 V1.x)
#
#_growth_parms
#_ LO HI INIT PRIOR PR_SD PR_type PHASE env_var&link dev_link dev_minyr dev_maxyr dev_PH Block Block_Fxn
 0.05 0.15 0.1 0.1 0.8 0 -3 0 0 0 0 0 0 0 # NatM_p_1_Fem_GP_1
 -10 45 21.6535 36 10 6 2 0 0 0 0 0 0 0 # L_at_Amin_Fem_GP_1
 40 90 71.6493 70 10 6 4 0 0 0 0 0 0 0 # L_at_Amax_Fem_GP_1
 0.05 0.25 0.147297 0.15 0.8 6 4 0 0 0 0 0 0 0 # VonBert_K_Fem_GP_1
 0.05 0.25 0.1 0.1 0.8 0 -3 0 0 0 0 0 0 0 # CV_young_Fem_GP_1
 0.05 0.25 0.1 0.1 0.8 0 -3 0 0 0 0 0 0 0 # CV_old_Fem_GP_1
 -3 3 2.44e-06 2.44e-06 0.8 0 -3 0 0 0 0 0 0 0 # Wtlen_1_Fem
 -3 4 3.34694 3.34694 0.8 0 -3 0 0 0 0 0 0 0 # Wtlen_2_Fem
 50 60 55 55 0.8 0 -3 0 0 0 0 0 0 0 # Mat50%_Fem
 -3 3 -0.25 -0.25 0.8 0 -3 0 0 0 0 0 0 0 # Mat_slope_Fem
 -3 3 1 1 0.8 0 -3 0 0 0 0 0 0 0 # Eggs/kg_inter_Fem
 -3 3 0 0 0.8 0 -3 0 0 0 0 0 0 0 # Eggs/kg_slope_wt_Fem
 0.05 0.15 0.1 0.1 0.8 0 -3 0 0 0 0 0 0 0 # NatM_p_1_Mal_GP_1
 1 45 0 36 10 0 -3 0 0 0 0 0 0 0 # L_at_Amin_Mal_GP_1
 40 90 69.5362 70 10 6 4 0 0 0 0 0 0 0 # L_at_Amax_Mal_GP_1
 0.05 0.25 0.163533 0.15 0.8 6 4 0 0 0 0 0 0 0 # VonBert_K_Mal_GP_1
 0.05 0.25 0.1 0.1 0.8 0 -3 0 0 0 0 0 0 0 # CV_young_Mal_GP_1
 0.05 0.25 0.1 0.1 0.8 0 -3 0 0 0 0 0 0 0 # CV_old_Mal_GP_1
 -3 3 2.44e-06 2.44e-06 0.8 0 -3 0 0 0 0 0 0 0 # Wtlen_1_Mal
 -3 4 3.34694 3.34694 0.8 0 -3 0 0 0 0 0 0 0 # Wtlen_2_Mal
 0  0  0  0  0  0 -4  0  0  0  0  0  0  0 # RecrDist_GP_1
 0  0  0  0  0  0 -4  0  0  0  0  0  0  0 # RecrDist_Area_1
 0  0  0  0  0  0 -4  0  0  0  0  0  0  0 # RecrDist_Timing_1
 0.1 10 1 1 1 0 -1 0 0 0 0 0 0 0 # CohortGrowDev
 1e-06 0.999999 0.5 0.5 0.5 0 -99 0 0 0 0 0 0 0 # FracFemale_GP_1
#
# timevary MG parameters 
#_ LO HI INIT PRIOR PR_SD PR_type PHASE
#_Cond -2 2 0 0 -1 99 -2 #_placeholder when no time-vary parameters
# info on dev vectors created for MGparms are reported with other devs after tag parameter section 
#
#_seasonal_effects_on_biology_parms
  0  0  0  0  0  0  0  0  0  0 # femwtlen1,femwtlen2,mat1,mat2,fec1,fec2,Malewtlen1,malewtlen2,L1,K
#_ LO HI INIT PRIOR PR_SD PR_type PHASE
#_Cond -2 2 0 0 -1 99 -2 #_placeholder when no seasonal MG parameters
#
3 #_Spawner-Recruitment; Options: 2=Ricker; 3=std_B-H; 4=SCAA; 5=Hockey; 6=B-H_flattop; 7=survival_3Parm; 8=Shepard_3Parm; 9=Shepard_3Parm_transformed; 10=RickerPower_3parm_transformed
0 # 0/1 to use steepness in initial equ recruitment calculation
0 # future feature:  0/1 to make realized sigmaR a function of SR curvature
#_ LO HI INIT PRIOR PR_SD PR_type PHASE env-var use_dev dev_mnyr dev_mxyr dev_PH Block Blk_Fxn  #  parm_name
   3   31 8.81505 10.3   10    0    1    0    0    0    0    0    0    0 # 1
 0.2    1 0.614248  0.7 0.05    1    4    0    0    0    0    0    0    0 # 2
   0    2  0.6  0.8  0.8    0   -4    0    0    0    0    0    0    0 # 8
  -5    5    0    0    1    0   -4    0    0    0    0    0    0    0 # 9
   0    0    0    0    0    0  -99    0    0    0    0    0    0    0 # 10
1 #do_recdev:  0=none; 1=devvector; 2=simple deviations
1971 # first year of main recr_devs; early devs can preceed this era
2001 # last year of main recr_devs; forecast devs start in following year
2 #_recdev phase 
1 # (0/1) to read 13 advanced options
 0 #_recdev_early_start (0=none; neg value makes relative to recdev_start)
 -4 #_recdev_early_phase
 0 #_forecast_recruitment phase (incl. late recr) (0 value resets to maxphase+1)
 1 #_lambda for Fcast_recr_like occurring before endyr+1
 1900 #_last_yr_nobias_adj_in_MPD; begin of ramp
 1900 #_first_yr_fullbias_adj_in_MPD; begin of plateau
 2001 #_last_yr_fullbias_adj_in_MPD
 2002 #_end_yr_for_ramp_in_MPD (can be in forecast to shape ramp, but SS sets bias_adj to 0.0 for fcast yrs)
 1 #_max_bias_adj_in_MPD (-1 to override ramp and set biasadj=1.0 for all estimated recdevs)
 0 #_period of cycles in recruitment (N parms read below)
 -5 #_min rec_dev
 5 #_max rec_dev
 0 #_read recdevs
#_end of advanced SR options
#
#_placeholder for full parameter lines for recruitment cycles
# read specified recr devs
#_Yr Input_value
#

#Fishing Mortality info 
0.3 # F ballpark
-2001 # F ballpark year (neg value to disable)
3 # F_Method: 1=Pope; 2=instan. F; 3=hybrid (hybrid is recommended)
2.95 # max F or harvest rate, depends on F_Method
# no additional F input needed for Fmethod 1
# if Fmethod=2; read overall start F value; overall phase; N detailed inputs to read
# if Fmethod=3; read N iterations for tuning for Fmethod 3
4 # N iterations for tuning F in hybrid method (recommend 3 to 7)
# Fleet Year Seas F_value se phase (for detailed setup of F_Method=2)
#
#_initial_F_parms; count = 0
#_ LO HI INIT PRIOR PR_SD PR_type PHASE
# 

#_Q_setup for fleets with cpue or survey data
#_1:  link type: (1=simple q, 1 parm; 2=mirror simple q, 1 mirrored parm; 3=q and power, 2 parm)
#_2:  extra input for link, i.e. mirror fleet
#_3:  0/1 to select extra sd parameter
#_4:  0/1 for biasadj or not
#_5:  0/1 to float
#_fleet   link link_info extra_se biasadj float # fleetname
      2      1      0      1      0      0  #  SURVEY1
      3      1      0      0      0      0  #  SURVEY2
  -9999      0      0      0      0      0  #  terminator
#
#_Q_parms(if_any);Qunits_are_ln(q)
# LO HI INIT PRIOR PR_SD PR_type PHASE env-var use_dev dev_mnyr dev_mxyr dev_PH Block Blk_Fxn # parm_name
  -7   5   0.516018   0   1   0   1   0   0   0   0   0   0   0  #  LnQ_base_SURVEY1(2)
  0   0.5   0   0.05   1   0   -4   0   0   0   0   0   0   0   #  Q_extraSD_SURVEY1(2)
  -7   5   -6.6281   0   1   0   1   0   0   0   0   0   0   0  #  LnQ_base_SURVEY2(3)
# timevary Q parameters
#_      LO        HI      INIT     PRIOR   PR_SD  PR_type      PHASE
# info on dev vectors created for Q parms are reported with other devs after tag parameter section 
#
#_size_selex_patterns
#Pattern:_0; parm=0; selex=1.0 for all sizes
#Pattern:_1; parm=2; logistic; with 95% width specification
#Pattern:_5; parm=2; mirror another size selex; PARMS pick the min-max bin to mirror
#Pattern:_15; parm=0; mirror another age or length selex
#Pattern:_6; parm=2+special; non-parm len selex
#Pattern:_43; parm=2+special+2;  like 6, with 2 additional param for scaling (average over bin range)
#Pattern:_8; parm=8; New doublelogistic with smooth transitions and constant above Linf option
#Pattern:_9; parm=6; simple 4-parm double logistic with starting length; parm 5 is first length; parm 6=1 does desc as offset
#Pattern:_21; parm=2+special; non-parm len selex, read as pairs of size, then selex
#Pattern:_22; parm=4; double_normal as in CASAL
#Pattern:_23; parm=6; double_normal where final value is directly equal to sp(6) so can be >1.0
#Pattern:_24; parm=6; double_normal with sel(minL) and sel(maxL), using joiners
#Pattern:_25; parm=3; exponential-logistic in size
#Pattern:_27; parm=3+special; cubic spline 
#Pattern:_42; parm=2+special+3; // like 27, with 2 additional param for scaling (average over bin range)
#__options:_0=none;_1=define_retention;_2=retention&mortality;_3=all_discarded_dead;_4=define_dome-shaped_retention
#_Pattern Discard Male Special
  1   0   0   0 # 1 FISHERY
  1   0   0   0 # 2 SURVEY1
  0   0   0   0 # 3 SURVEY2
#
#_age_selex_types
#Pattern:_0; parm=0; selex=1.0 for ages 0 to maxage
#Pattern:_10; parm=0; selex=1.0 for ages 1 to maxage
#Pattern:_11; parm=2; selex=1.0  for specified min-max age
#Pattern:_12; parm=2; age logistic
#Pattern:_13; parm=8; age double logistic
#Pattern:_14; parm=nages+1; age empirical
#Pattern:_15; parm=0; mirror another age or length selex
#Pattern:_16; parm=2; Coleraine - Gaussian
#Pattern:_17; parm=nages+1; empirical as random walk  N parameters to read can be overridden by setting special to non-zero
#Pattern:_41; parm=2+nages+1; // like 17, with 2 additional param for scaling (average over bin range)
#Pattern:_18; parm=8; double logistic - smooth transition
#Pattern:_19; parm=6; simple 4-parm double logistic with starting age
#Pattern:_20; parm=6; double_normal,using joiners
#Pattern:_26; parm=3; exponential-logistic in age
#Pattern:_27; parm=3+special; cubic spline in age
#Pattern:_42; parm=2+nages+1; // cubic spline; with 2 additional param for scaling (average over bin range)
#_Pattern Discard Male Special
 11   0   0   0 # 1 FISHERY
 11   0   0   0 # 2 SURVEY1
 11   0   0   0 # 3 SURVEY2
#
#_ LO   HI   INIT  PRIOR PR_SD PR_type PHASE env-var use_dev dev_mnyr dev_mxyr dev_PH Block Blk_Fxn # parm_name
    19     80 53.6411     50   0.01      1      2      0      0      0      0      0      0      0 # SizeSel_P1_FISHERY(1)
  0.01     60 18.9232     15   0.01      1      3      0      0      0      0      0      0      0 # SizeSel_P2_FISHERY(1)
    19     70 36.653     30   0.01      1      2      0      0      0      0      0      0      0 # SizeSel_P1_SURVEY1(2)
  0.01     60 6.59179     10   0.01      1      3      0      0      0      0      0      0      0 # SizeSel_P2_SURVEY1(2)
     0     40      0      5     99      0     -1      0      0      0      0      0      0      0 # AgeSel_P1_FISHERY(1)
     0     40     40      6     99      0     -1      0      0      0      0      0      0      0 # AgeSel_P2_FISHERY(1)
     0     40      0      5     99      0     -1      0      0      0      0      0      0      0 # AgeSel_P1_SURVEY1(2)
     0     40     40      6     99      0     -1      0      0      0      0      0      0      0 # AgeSel_P2_SURVEY1(2)
     0     40      0      5     99      0     -1      0      0      0      0      0      0      0 # AgeSel_P1_SURVEY2(3)
     0     40      0      6     99      0     -1      0      0      0      0      0      0      0 # AgeSel_P2_SURVEY2(3)
# timevary selex parameters
#_   LO       HI     INIT    PRIOR    PR_SD    PR_type  PHASE
# info on dev vectors created for selex parms are reported with other devs after tag parameter section 
#
0 #_use 2D_AR1 selectivity(0/1):  experimental feature
#_no 2D_AR1 selex offset used
#
# Tag loss and Tag reporting parameters go next
0 # TG_custom:  0=no read; 1=read if tags exist
#_Cond -6 6 1 1 2 0.01 -4 0 0 0 0 0 0 0  #_placeholder if no parameters
#
# Input variance adjustments factors: 
 #_1=add_to_survey_CV
 #_2=add_to_discard_stddev
 #_3=add_to_bodywt_CV
 #_4=mult_by_lencomp_N
 #_5=mult_by_agecomp_N
 #_6=mult_by_size-at-age_N
 #_7=mult_by_generalized_sizecomp
#_Factor  Fleet  Value
 -9999    1     0  # terminator
#
4 #_maxlambdaphase
1 #__sd_offset; must be 1 if any growthCV, sigmaR, or survey extraSD is an estimated parameter
# read 3 changes to default Lambdas (default value is 1.0)
# Like_comp codes:  1=surv; 2=disc; 3=mnwt; 4=length; 5=age; 6=SizeFreq; 7=sizeage; 8=catch; 9=init_equ_catch; 
# 10=recrdev; 11=parm_prior; 12=parm_dev; 13=CrashPen; 14=Morphcomp; 15=Tag-comp; 16=Tag-negbin; 17=F_ballpark
#like_comp  fleet  phase  value  sizefreq_method
 1 2 2 1 1
 4 2 2 1 1
 4 2 3 1 1
 -9999  1  1  1  1  # terminator
1 # (0/1) read specs for more stddev reporting 
 1 1 -1 5 1 5 1 -1 5 # selex type, len/age, year, N selex bins, Growth pattern, N growth ages, NatAge_area(-1 for all), NatAge_yr, N Natages
 5 15 25 35 43 # vector with selex std bin picks (-1 in first bin to self-generate)
 1 2 14 26 40 # vector with growth std bin picks (-1 in first bin to self-generate)
 1 2 14 26 40 # vector with NatAge std bin picks (-1 in first bin to self-generate)
999

