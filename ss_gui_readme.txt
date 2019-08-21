ss_gui_readme.txt

        Stock Synthesis ver 3.30 GUI Interface ver 3.30.14.02
        ----------------------------------------------------

This GUI is created to display input to the Stock Synthesis application,
versions 3.30.13 and 3.30.14. It allows the user to change the data,
run the executable, and display results.

Currently, the only results displayed are the TIME-SERIES charts from
Report.sso, files warnings.sso, and echoinput.sso.

Built on Windows 10 using MinGW 5.3 with Qt 5.12

This is a work of the U.S. Government and is not subject to copyright
protection in the United States. Foreign copyrights may apply.
See ss_gui_copyright.txt for more information.

History:
============================================================================
    3.30.14.03
    2019-08-16
1. Exploring separate parameter dialog for equations.
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

Neal Schindler
neal.schindler(at)noaa.gov

