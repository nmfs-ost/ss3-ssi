ss_gui_readme_archive.txt

               Stock Synthesis Archived Changes
        ----------------------------------------------------

See ss_gui_copyright.txt for copyright information.

History:
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
    3.3.13.06
    2019-06-28
1. Read/write/display of all parameters is fixed.
============================================================================
    3.3.13.04
    2019-04-26
1. Added grid to graph for Spawn-Recruit graphs.
2. All Spawn-Recruit equations (except Shepherd Re-parm) are working.
============================================================================
    3.3.13.03
    2019-03-21
1. Rebuilt with correct toolkit.
============================================================================
    3.3.13.02
    2019-02-28
1. Added graph dialog for recruitment curves.
============================================================================
    3.3.13.01
    2019-02-13
1. Allows gender to be -1 to invoke Female SSB=SSB*fracfem.
2. Q option 4, Mirror with Offset is included.
============================================================================
    3.3.12.06
    2018-12-21
1. Spline selectivity scaling working differently now.
============================================================================
    3.3.12.05
    2018-10-31
1. Moved to Windows 10 and Qt 5.11.
============================================================================
    3.3.12.04
    2018-10-05
1. Removed confusing button from graph dialog.
2. No longer changes equation parameters if incorrect pattern.
============================================================================
    3.3.12.03
    2018-09-27
1. Fixed reading of selectivity model 42.
============================================================================
    3.3.12.02
    2018-09-24
1. Fixed reading of certain model files.
2. Corrected writing multiple morphs/platoons.
============================================================================
    3.3.12.01
    2018-09-17
1. Fixed allowed input for forecast MSY Method and Control Rule.
============================================================================
    3.3.11.08
    2018-08-24
1. Default parameters for cubic spline now work.
2. Fixed seg-fault when scrolling through selectivity equation numbers.
============================================================================
    3.3.11.07
    2018-07-19
1. Added depletion basis 4=X*SPB_endyr.
2. Added 'Type' field to mean body size input/output.
3. Added age selex patterns 44 and 45.
============================================================================
      3.3.11.06
      2018-06-19
1. Fleet Lambdas now respond to interface changes.
2. Moved around some items in Configuration Settings.
============================================================================
      3.3.11.05
      2018-06-05
1. Changing Selectivity Male and Discard options no longer overwrites
   method parameters.
2. The default directory is now used in every file search instead of the
   location of the executable.
3. Added option to add Dirichlet parameters.
4. Removed option for the technical document.
5. Added option for the Interface User Guide.
============================================================================
      3.3.11.04
      2018-06-04
1. Added option 3 for MCMC output.
2. Increased allowable MC Eval burn interval.
3. Set size of Dirichlet parameter table view.
4. Fixed changing tab viewed when changing fleets.
============================================================================
      3.3.11.03
      2018-05-31
1. Reads and writes dirichlet multinomial parameters.
2. Changed selex retain, discard, and male params to full params.
3. Added time varying parameters for each of above.
4. Minor changes to run dialog.
5. Added check for presence of ss.par file and wtatage.ss when copying.
6. increased allowable input line length.
============================================================================
      3.3.11.02
      2018-04-25
1. Implemented error checking in data and control files.
2. Fixed early recruitment dev start date viewer.
============================================================================
      3.3.11.01
      2018-04-19
1. Fixed Forecast benchmarks/reference pts to be a list instead of on/off.
2. Added error checking routines.
3. Implemented in reading input files starter.ss and forecast.ss
============================================================================
      3.3.10.03
      2018-02-07
1. Corrected reading alt bin max.
2. Fixed setup code for selectivities.
3. Changed some file comments to match ss_new files.
4. Changed the output graphs to summary.sso, so titles changed.
5. Added copyright notice to all header files and ss_gui_copyright.txt
============================================================================
      3.3.10.02
      2018-02-01
1. Improved selectivity graph repainting.
2. Supporting curves showing.
3. Added graphs for selectivity patterns 8 and 18.
============================================================================
      3.3.10.01
      2018-01-10
1. Added graph view for selectivity patterns 9 and 19.
2. Incorporated changes to support bycatch data.
   Compatible with SS ver 3.30.10.
============================================================================
      3.3.10.00
      2017-12-26
1. Added graph view for selectivity patterns 5, 10, 11, 12, 14, 15, 16, 17,
   22, 25, 26, 41, and 43.
============================================================================
      3.3.09.00
      2017-11-21
1. Enabled forecast recruitment adjustment control and value.
   Compatible with SS ver 3.30.09.
============================================================================
      3.3.08.04
      2017-11-09
1. Fixed Bug #40146, crash when changing selectivity model.
   Selectivity graph will change smoothly, but may need to hit
   Reset button occasionally.
============================================================================
      3.3.08.03
      2017-11-01
1. Fixed problem with recruitment distribution timings. GUI updated.
============================================================================
      3.3.08.02
      2017-10-27
1. Added graph view for Length Selectivity for constant (0), logistic (1),
   and linear segment (6).
2. Made compliant with SS version 3.30.08.
============================================================================
      3.3.07.01
      2017-09-25
1. Made compliant with SS version 3.30.07.
============================================================================
      3.3.05.06
      2017-08-04
1. Fixed problem with converting wtatage.ss - copying correct file.
2. Fixed setting read wtatage.ss checkbox with maturity option 5.
============================================================================
      3.3.05.05
      2017-07-27
1. Fixed problem with negative phase numbers in advanced recruitment.
============================================================================
      3.3.05.03
      2017-07-26
1. Changed over to reading settlement timing parameters instead of seasonal
   Recruitment Distribution parameters.
============================================================================
      3.3.05.2
      2017-07-24
1. Added charts view button to the run dialog. It reads the Report.sso file
   and creates charts dynamically.
============================================================================
      3.3.05.1
      2017-07-10
1. Added the minimum sample size to both the length and age comp header tables;
   the default value is 1 to match SS 3.24.
2. Added selectivity to use for forecasts (not yet implemented).
============================================================================
      3.3.20
      2017-06-07
1. Fixed major problem with converting old files. Copying of converted files
   created files with incorrect names.
============================================================================
      3.3.19
      2017-06-06
1. SS.PAR file handling fixed. It is now selectable and viewable.
============================================================================
      3.3.18
      2017-06-05
1. Reading/writing recruitment dist interaction fixed.
============================================================================
      3.3.17
      2017-05-30
1. Fleet Q setup/parameters/timevarying parameters fixed.
2. Disabled ss_error.txt in release version.
============================================================================
      3.3.16b
      2017-05-24
1. Critical file messages are now Warnings so that the program does not
   crash. It will still crash if bad data is read.
2. Ignores recruitment distribution interaction (only used for method 1).
============================================================================
      3.3.15b
      2017-05-17
1. Now reads/writes SS 3.30.03 data files.
2. Reworked Recruitment so that all parameters show and changes in the Gui
   are reflected attendant tables and in the data files.
============================================================================
      3.3.14b
      2017-03-21
1. Blocks in data widget are now functional.
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

