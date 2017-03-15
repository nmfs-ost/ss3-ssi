readme.txt

        Stock Synthesis ver 3.30 GUI Interface ver 3.3.7
        ------------------------------------------------

This GUI is created to display input to the Stock Synthesis application, 
allow for changes to the data, run the executable, and display results.

Currently, the only results displayed are the warnings.sso and echoinput.sso.

Built on Windows 7 using MinGW 5.3 with Qt 5.7

History:
============================================================================
      3.3.13b
      2017-03-15
1. Revamped the reading of time varying parameters.
2. Corrected Spawn-Recruit parameter reading.
3. Made all parameters visible in gui.
============================================================================
      3.3.10a
      2017-01-06
1. Created a new time varying parameter model into gui. The parameters
   are now linked with the full parameters in a single object.
2. Implemented setup-parameter model for seasonal effects in population.
============================================================================
      3.3.9
      2016-12-13
1. Months per season is now a floating point number rather than an integer.
2. Added the ability to hide the toolbar.
3. Added the ability to view wtatage.ss if it is used.
4. Population reset now resets all components.
5. Changed some table headers for fleet observations.
6. Fixed some crashes when changing models.
============================================================================
      3.3.8
      2016-12-07
1. Corrected copying fleet observations. The number in the observation
   itself changes correctly.
2. Changed the layout of Files and Forecast a little.
3. Got table popup menu to work for some capabilities - copy, paste, append
   a row, and delete selected rows.
============================================================================
      3.3.7
      2016-11-01
1. Corrected reading/writing of time-varying params for all but tagging.
2. Corrected reading/writing of general size composition params.
3. Added error message when EOF encountered prematurely.
4. Fixed Mainwindow File and Data menu access.
============================================================================
      3.3.6
      2016-11-04
1. Changed max number of bins to 200.
2. Changed max number of environment variables to 100,
    and max number of environment observations to 300.
3. In writing data file:
     Added number of population size bins,
     Changed output of environment observations to current format, and
     Corrected output of benchmark and forecast years.

============================================================================
      3.3.5
      2016-11-01
1. Added fleet number to all observations to allow for negative fleet number.
============================================================================
      3.3.4
      2016-10-28
1. Able to convert old 3.24 files and read in new 3.30 files and
   run ss.exe to get results.
============================================================================
      3.3.3
      2016-10-26
1. Changed the major version number to prevent user's confusion.
2. Many fixes to read/write routines and displays.
3. Model/Views are used for most parameters.
4. Dropped support for 3.24 files. Now, the user is given the opportunity
   to either convert the files using ss_trans.exe, picking a different
   model, or exiting the GUI. If converting, a new dialog opens to run
   ss_trans (the same as the run command brings up but with different
   executable), and when done, prompts the user for a new directory.
============================================================================
      2.3.1
      2016-07-25
1. Data displays and is interactive.
2. Four main data files read/write correctly. 

============================================================================

Neal Schindler
neal.schindler(at)noaa.gov

