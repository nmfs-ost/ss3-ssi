SSI_readme.txt

        Stock Synthesis ver 3.30 GUI Interface ver 3.30.18.04
        -----------------------------------------------------

This GUI is created as a front-end to the Stock Synthesis application,
version 3.30.18. It allows the user to change the data,
run the executable, and display some results.

Built on Windows 10 using MinGW 5.3 with Qt 5.12

This is a work of the U.S. Government and is not subject to copyright
protection in the United States. Foreign copyrights may apply.
See ss_gui_copyright.txt for more information.

History:
============================================================================
    3.30.18.05
    2022-01-13
Bug Fixes
1. Fixed the display of output and new file directories on Output Files tab.
============================================================================
   3.30.18.04
   2021-12-23
Bug Fixes
1. Fixed handling of F Method = 2. An extra value was being added causing
  the model to fail to execute.
Minor Improvements
1. Added capability for Do_Msy=5 (MEY).
2. Added switches for sso and ss_new files for saving to separate folders.
============================================================================
    3.30.18.03
    2021-11-17
Bug Fixes
1. The negative limit for early rec dev phase was set to -5, now set to -99.
   The positive limit remains at 99.
2. Fixed age selectivity retain time varying param display.
   The age selex retain tv params display was reading the number of parameters
   from size retain tv params. The data files were unaffected, but the display
   was incorrect.
============================================================================
    3.30.18.02
    2021-11-08
Minor Improvements
1. Added Cessation parameter for Growth model 8.
Bug Fixes
1. A default is set for BLimit and it only shows for Benchmarks = 3.
2. Number of platoons is forced to 1, 3, or 5 and a default is set
   for the between/within ratio.
============================================================================
    3.30.18.01
    2021-10-13
Bug Fixes
1. Fixed resetting selectivity time-varying parameters between models.
2. Increased minimum population size to 50.
3. Increased decimal places to 3 in population widget.
4. Added terminator to annual control rule buffer in writing forecast.
============================================================================
    3.30.18.0
    2021-09-28
Major Improvements
1. Added Fleet specific F mort.
2. Allowed Fleet type 4 for predators.
3. Added Benchmark option 3 and Blimit value.
4. Removed support for ss_trans.
Minor Improvements
1. Added version info to summary charts.
2. Improved look of Likelihoods dialog.
Bug Fixes
1. Fixed treatment of male/female offset selectivity parameters.
2. Fixed reading/writing of Q time-varying parameters.
============================================================================
    3.30.17.6
    2021-08-11
1. Fixed recruitment advanced option forecast recruitment phase read/write.
2. Added a default model name to readme.txt.
3. Got code in place for fleet specific F mort (population).
============================================================================
    3.30.17.5
    2021-07-23
1. Fixed writing data file for option length composition use.
============================================================================
    3.30.17.4
    2021-07-07
1. Added view for input variable adjustments.
2. Added views for age selectivity discard and retention parameters.
3. Fixed display of number of parameters for size selectivity.
============================================================================
    3.30.17.3
    2021-07-01
1. Added discard and retention parameters for age selex.
2. Added option 7 for input value variance.
3. If catch observation season exceeds number of seasons, will now
   a. change value to last season, and
   b. display dialog with this information.
============================================================================
    3.30.17.2
    2021-06-30
1. Corrected treatment of unused option in control file.
2. Reordered Population/Recruitment a little.
============================================================================
    3.30.17.1
    2021-06-11
1. Added -1 as option for forecast method.
2. Changed control rule method from 1-2 to 0-5.
3. Fixed bug with Lambdas read twice.
4. Fixed bug with autogenerating age selex spline values.
5. Fixed bug with losing age cubic spline data.
6. Fixed age random walk where special is non-zero.
7. Fixed a bug that changed first stochastic loop.
8. Fixed bug in forecast that changed benchmark relf.
9. Changed references to vlab.ncep.noaa.gov to vlab.noaa.gov
============================================================================
    3.30.16.7
    2021-03-18
1. Added create/view readme.txt file for model info (optional).
============================================================================
    3.30.16.6
    2020-11-27
1. Restructured reading of tag data.
2. Added check for ordered x values of spline curves. If not ordered, spline
   does not build.
3. Internal change to reading files hopefully works in error conditions.
============================================================================
    3.30.16.5
    2020-11-09
1. Set F Mort number of tuning iterations from 1 to 15 for method 3 (hybrid).
2. Corrected reading of do morph composition.
3. Corrected reading of time-varying deviation parameters.
============================================================================
    3.30.16.4
    2020-10-28
1. Corrected reading recapture events.
2. Added line number to file reading error messages.
3. Changed column headers from "gender" to "sex".
============================================================================
    3.30.16.3
    2020-10-05
1. Corrected reading tag groups.
2. Cleaned up some unused files and commented out code.
============================================================================

Neal Schindler
neal.schindler(at)noaa.gov

