#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T08:40:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts printsupport


TARGET = SSI

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialoginputfiles.cpp \
    fileselector.cpp \
    dialog_yearlyvalues.cpp \
    form_yearvalue.cpp \
    data_widget.cpp \
    control_widget.cpp \
    input_file.cpp \
    model.cpp \
    area.cpp \
    file_info_widget.cpp \
    file_info_dialog.cpp \
    file_widget.cpp \
    population.cpp \
    selectivity.cpp \
    catchability.cpp \
    sizecomp.cpp \
    agecomp.cpp \
    dialog_about.cpp \
    forecast_widget.cpp \
    dialog_about_admb.cpp \
    fleet.cpp \
    composition.cpp \
    error_vector.cpp \
    ss_forecast.cpp \
    fleet_widget.cpp \
    observation_widget.cpp \
    growth.cpp \
    growth_morph.cpp \
    ss_observation.cpp \
    ss_recruitment.cpp \
    ss_movement.cpp \
    ss_fecundity.cpp \
    ss_mortality.cpp \
    long_parameter.cpp \
    short_parameter.cpp \
    method_setup.cpp \
    block_pattern.cpp \
    sd_reporting.cpp \
    variance_adjustment.cpp \
    tag.cpp \
    selex_equation.cpp \
    growth_season_effects.cpp \
    growth_pattern.cpp \
    dialog_about_nft.cpp \
    population_widget.cpp \
    yearindexmeasure.cpp \
    composition_widget.cpp \
    dialog_float_list.cpp \
    observation_dialog.cpp \
    fleetlambda.cpp \
    errorfloatdialog.cpp \
    dialog_run.cpp \
    dialog_view.cpp \
    spinboxdelegate.cpp \
    doublespinboxdelegate.cpp \
    dialog_fileview.cpp \
    console_redir.cpp \
    fleet_catch.cpp \
    lineeditdelegate.cpp \
    tablemodel.cpp \
    tableview.cpp \
    lambdadelegate.cpp \
    catchdelegate.cpp \
    abundancedelegate.cpp \
    mbweightdelegate.cpp \
    newfleetdialog.cpp \
    fileIO33.cpp \
    dialog_about_gui.cpp \
    metadata.cpp \
    fileIOgeneral.cpp \
    fleet_composition.cpp \
    documentdialog.cpp \
    message.cpp \
    fileconversiondialog.cpp \
    parametermodelTV.cpp \
    q_ratio.cpp \
    setupmodel.cpp \
    seasonaleffectsmodel.cpp \
    parametermodel.cpp \
    dialog_runoptions.cpp \
    conversions.cpp \
    chartdialog.cpp \
    equationdialog.cpp \
    ttk_spline.cpp \
    dialoginfo.cpp \
    dialoginputerror.cpp \
    srequationdialog.cpp \
    parameterview.cpp \
    dialogequationview.cpp \
    dialogparameterview.cpp \
    slxequationdialog.cpp \
    dialogspwnrcrequationview.cpp \
    doublelimitspinbox.cpp \
    dialogselectivityequationview.cpp \
    ss_math.cpp \
    dialogsummaryoutput.cpp \
    dialog_copyright.cpp \
    dialogtable.cpp

HEADERS  += mainwindow.h \
    dialoginputfiles.h \
    fileselector.h \
    dialog_yearlyvalues.h \
    form_yearvalue.h \
    data_widget.h \
    control_widget.h \
    metadata.h \
    input_file.h \
    model.h \
    area.h \
    file_info_widget.h \
    file_info_dialog.h \
    file_widget.h \
    population.h \
    selectivity.h \
    catchability.h \
    sizecomp.h \
    agecomp.h \
    dialog_about.h \
    forecast_widget.h \
    dialog_about_admb.h \
    fleet.h \
    composition.h \
    error_vector.h \
    ss_forecast.h \
    fleet_widget.h \
    observation_widget.h \
    growth.h \
    growth_morph.h \
    ss_observation.h \
    ss_recruitment.h \
    ss_movement.h \
    ss_fecundity.h \
    ss_mortality.h \
    long_parameter.h \
    short_parameter.h \
    method_setup.h \
    block_pattern.h \
    sd_reporting.h \
    variance_adjustment.h \
    tag.h \
    selex_equation.h \
    growth_season_effects.h \
    growth_pattern.h \
    dialog_about_nft.h \
    population_widget.h \
    yearindexmeasure.h \
    composition_widget.h \
    dialog_float_list.h \
    observation_dialog.h \
    fleetlambda.h \
    errorfloatdialog.h \
    dialog_run.h \
    dialog_view.h \
    spinboxdelegate.h \
    doublespinboxdelegate.h \
    dialog_fileview.h \
    console_redir.h \
    fleet_catch.h \
    lineeditdelegate.h \
    tablemodel.h \
    tableview.h \
    lambdadelegate.h \
    catchdelegate.h \
    abundancedelegate.h \
    mbweightdelegate.h \
    newfleetdialog.h \
    fileIO33.h \
    fileIOJSON.h \
    fileIOXML.h \
    dialog_about_gui.h \
    fileIOgeneral.h \
    fleet_composition.h \
    documentdialog.h \
    ss_math.h \
    message.h \
    fileconversiondialog.h \
    parametermodelTV.h \
    q_ratio.h \
    setupmodel.h \
    seasonaleffectsmodel.h \
    parametermodel.h \
    dialog_runoptions.h \
    conversions.h \
    chartdialog.h \
    equationdialog.h \
    ttk_spline.h \
    dialoginfo.h \
    dialoginputerror.h \
    srequationdialog.h \
    parameterview.h \
    dialogequationview.h \
    dialogparameterview.h \
    slxequationdialog.h \
    dialogspwnrcrequationview.h \
    doublelimitspinbox.h \
    dialogselectivityequationview.h \
    dialogsummaryoutput.h \
    dialog_copyright.h \
    dialogtable.h

FORMS    += mainwindow.ui \
    dialoginputfiles.ui \
    fileselector.ui \
    dialog_yearlyvalues.ui \
    form_yearvalue.ui \
    data_widget.ui \
    control_widget.ui \
    length_bin_data.ui \
    length_bin_boundaries.ui \
    file_info_widget.ui \
    file_info_dialog.ui \
    file_widget.ui \
    dialog_about.ui \
    forecast_widget.ui \
    dialog_about_admb.ui \
    fleet_widget.ui \
    observation_widget.ui \
    dialog_about_nft.ui \
    population_widget.ui \
    composition_widget.ui \
    dialog_float_list.ui \
    observation_dialog.ui \
    errorfloatdialog.ui \
    dialog_run.ui \
    dialog_view.ui \
    dialog_fileview.ui \
    newfleetdialog.ui \
    dialog_about_gui.ui \
    documentdialog.ui \
    fileconversiondialog.ui \
    dialog_runoptions.ui \
    chartdialog.ui \
    equationdialog.ui \
    dialoginfo.ui \
    dialoginputerror.ui \
    srequationdialog.ui \
    parameterview.ui \
    dialogequationview.ui \
    dialogparameterview.ui \
    doublelimitspinbox.ui \
    dialog_copyright.ui \
    dialogtable.ui

OTHER_FILES +=

RESOURCES += \
    stock_synth.qrc

DISTFILES += \
    ../SSI_readme.txt \
    ../SSI_readme_archive.txt \
    ../SSI_copyright.txt \
    SSI_icon.ico
