readme.txt

        Stock Synthesis ver 3.30 GUI Interface ver 3.3.5
        ------------------------------------------------

This GUI is created to display input to the Stock Synthesis application, 
allow for changes to the data, run the executable, and display results.

Currently, the only results displayed are the warnings.sso and echoinput.sso.

Built on Windows 7 using MinGW 5.3 with Qt 5.7

History:
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

