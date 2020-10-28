SSI_readme.txt

        Stock Synthesis ver 3.30 GUI Interface ver 3.30.16.4
        -----------------------------------------------------

This GUI is created as a front-end to the Stock Synthesis application,
versions 3.30.14 through 3.30.16. It allows the user to change the data,
run the executable, and display some results.

Built on Windows 10 using MinGW 5.3 with Qt 5.12

This is a work of the U.S. Government and is not subject to copyright
protection in the United States. Foreign copyrights may apply.
See ss_gui_copyright.txt for more information.

History:
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
    3.30.16.2
    2020-09-10
1. Corrected age and length bins for selectivity curves.
============================================================================
    3.30.16.1
    2020-08-26
1. Added check for selex parameters that are not estimated.
============================================================================
    3.30.15.15
    2020-07-23
1. Added age error parameters for a single error definition.
2. Added size selectivity pattern 11.
============================================================================
    3.30.15.14
    2020-06-13
1. Added report detail chooser dialog.
2. Corrected allowed length to be greater than 100, now limit is 399.
3. Fixed WC Rebuilder year display.
============================================================================
    3.30.15.13
    2020-06-04
1. Fixed a problem in reading starter file.
2. Added r4ss comparison of two models.
3. Fixed a bug with natMort 3 and 4 reading age specific values.
============================================================================
    3.30.15.12
    2020-05-13
1. Added SSI_readme.txt to 'About' dialog
2. Changed output charts table to likelihood.
3. Added information windows for Age and Size selectivity.
4. Selectivity graph axes now change for scaled curves.
============================================================================
    3.30.15.11
    2020-04-17
1. Added "Info" button for selectivity to showcase commonly used equations.
============================================================================
    3.30.15.10
    2020-04-10
1. Implemented selectivity curve 43 - scaled linear segments, curve 44 -
   two sex random walk, and curve 45 - two sex logist values.
2. Changed reading function to allow breaking out of reading bad files. This
   leaves SSI in an unstable state and must be followed by reading a good
   model.
3. Fixed curve 16 - Coleraine - Gaussian: limit for selex = 1.
============================================================================
    3.30.15.09
    2020-04-03
1. Implemented selectivity curve 42 - cubic spline.
2. Changed defaults for cubic spline 27 (and 42) to be similar to auto-
   generated parameters in SS.
============================================================================
    3.30.15.08
    2020-03-24
1. Fixed selectivity curves
   5 - Two parameters are limited by bin number
   8 - Added bins to chart view
   14, 20, 24, 25, & 26 - React correctly to slider input - BUG #76456
   27 - Responds to change in special
   41 - Reworked
2. Added capability for 2DAR1 specification and parameters.
============================================================================
    3.30.15.07
    2020-03-20
1. Changed Options menu name to Links, moved default dir and model choices
   to File menu.
2. Deleted Data menu.
3. SS3.par label changed to SS.par
4. Choose file for ctrl and dat file now work.
5. Run window now is non-modal (can keep run window up while editing in
   other windows).
6. Added likelihood table to run summary charts.
============================================================================
    3.30.15.06
    2020-03-13
1. Corrected gui to display recr_dist correctly.
============================================================================
    3.30.15.05
    2020-03-02
1. Implemented tags.
2. Moved Lambdas to Configuration and added others besides fleet.
3. Corrected reading age data when no age data.
4. Corrected reading forecast annual control rule buffer.
5. Forecast loop control #3 is implemented as forecast recruitment that
   requires a value 0-3, which may be needed for some older models.
============================================================================
    3.30.15.04
    2020-02-19
1. Window management.
============================================================================
    3.30.15.03
    2020-02-07
1. Added default model capability: set and return to.
2. Searches for models start in the default directory and searches for
   executable files and manuals start in the application directory.
3. Fixed several help windows and links.
============================================================================
    3.30.15.02
    2020-02-06
1. Added capability to generate r4ss charts in Run window.
============================================================================
    3.30.14.05
    2020-01-28
1. Corrected problem reading Red Snapper.
2. Changed read_starter to accomodate additional values at the end.
3. Corrected reading of catch multiplier for multiple fleets.
4. Finished selectivity equation dialog box: separate parameters window.
5. Changed run summary charts to have same number of grid lines.
============================================================================
    3.30.14.04
    2020-01-09
1. Added equation dialog box for Selectivity.
2. Changed equation dialog box for S-R: moved parameters to separate
   window.
3. Changed results graph dialog box.
============================================================================
    3.30.14.03
    2019-08-16
1. Changed equation dialog box for Spawn-Recruit.
============================================================================
    3.30.14.02
    2019-07-31
1. Changed values allowed for recruitment distribution to 2-4.
============================================================================
    3.30.14.01
    2019-07-18
1. Read/write/display of these parameters is fixed:
    a. age Natural Mort values (last one was truncated)
    b. recruitment parameters (the last three were set to defaults)
    c. selectivity parameters (in some cases, age selex were set to defaults)
2. Fixed problem reading some models and changing between models.
3. Re-arranged growth parameters - split up female/male to separate tabs.
4. Version number changed to mimic SS version.
============================================================================

Neal Schindler
neal.schindler(at)noaa.gov

