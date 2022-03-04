#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T08:40:37
#
#-------------------------------------------------
CONFIG   += static

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts printsupport


TARGET = SSI

TEMPLATE = app


SOURCES += main.cpp\
    longparametermodel.cpp \
        mainwindow.cpp \
    fileselector.cpp \
    form_yearvalue.cpp \
    data_widget.cpp \
    input_file.cpp \
    model.cpp \
    file_info_widget.cpp \
    file_info_dialog.cpp \
    file_widget.cpp \
    population.cpp \
    selectivity.cpp \
    shortparametermodel.cpp \
    sizecomp.cpp \
    dialog_about.cpp \
    forecast_widget.cpp \
    dialog_about_admb.cpp \
    fleet.cpp \
    error_vector.cpp \
    ss_forecast.cpp \
    fleet_widget.cpp \
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
    tag.cpp \
    selex_equation.cpp \
    growth_season_effects.cpp \
    growth_pattern.cpp \
    dialog_about_nft.cpp \
    population_widget.cpp \
    composition_widget.cpp \
    fleetlambda.cpp \
    errorfloatdialog.cpp \
    dialog_run.cpp \
    spinboxdelegate.cpp \
    dialog_fileview.cpp \
    fleet_catch.cpp \
    tablemodel.cpp \
    tableview.cpp \
    mbweightdelegate.cpp \
    newfleetdialog.cpp \
    fileIO33.cpp \
    dialog_about_gui.cpp \
    metadata.cpp \
    fileIOgeneral.cpp \
    fleet_composition.cpp \
    documentdialog.cpp \
    message.cpp \
    parametermodelTV.cpp \
    q_ratio.cpp \
    setupmodel.cpp \
    seasonaleffectsmodel.cpp \
    parametermodel.cpp \
    dialog_runoptions.cpp \
    conversions.cpp \
    chartdialog.cpp \
    ttk_spline.cpp \
    dialoginfo.cpp \
    dialoginputerror.cpp \
    parameterview.cpp \
    dialogequationview.cpp \
    dialogparameterview.cpp \
    dialogspwnrcrequationview.cpp \
    doublelimitspinbox.cpp \
    dialogselectivityequationview.cpp \
    ss_math.cpp \
    dialogsummaryoutput.cpp \
    dialog_copyright.cpp \
    dialogtable.cpp \
    mainwindowrun.cpp \
    dar1.cpp \
    dialogdirectories.cpp \
    dialogchosereport.cpp \
    sscomposition.cpp \
    dialog_readme.cpp

HEADERS  += mainwindow.h \
    fileselector.h \
    form_yearvalue.h \
    data_widget.h \
    longparametermodel.h \
    metadata.h \
    input_file.h \
    model.h \
    file_info_widget.h \
    file_info_dialog.h \
    file_widget.h \
    population.h \
    selectivity.h \
    shortparametermodel.h \
    sizecomp.h \
    dialog_about.h \
    forecast_widget.h \
    dialog_about_admb.h \
    fleet.h \
    error_vector.h \
    ss_forecast.h \
    fleet_widget.h \
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
    tag.h \
    selex_equation.h \
    growth_season_effects.h \
    growth_pattern.h \
    dialog_about_nft.h \
    population_widget.h \
    composition_widget.h \
    fleetlambda.h \
    errorfloatdialog.h \
    dialog_run.h \
    spinboxdelegate.h \
    dialog_fileview.h \
    fleet_catch.h \
    tablemodel.h \
    tableview.h \
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
    parametermodelTV.h \
    q_ratio.h \
    setupmodel.h \
    seasonaleffectsmodel.h \
    parametermodel.h \
    dialog_runoptions.h \
    conversions.h \
    chartdialog.h \
    ttk_spline.h \
    dialoginfo.h \
    dialoginputerror.h \
    parameterview.h \
    dialogequationview.h \
    dialogparameterview.h \
    dialogspwnrcrequationview.h \
    doublelimitspinbox.h \
    dialogselectivityequationview.h \
    dialogsummaryoutput.h \
    dialog_copyright.h \
    dialogtable.h \
    mainwindowrun.h \
    dar1.h \
    dialogdirectories.h \
    dialogchosereport.h \
    sscomposition.h \
    dialog_readme.h

FORMS    += mainwindow.ui \
    fileselector.ui \
    form_yearvalue.ui \
    data_widget.ui \
    length_bin_data.ui \
    length_bin_boundaries.ui \
    file_info_widget.ui \
    file_info_dialog.ui \
    file_widget.ui \
    dialog_about.ui \
    forecast_widget.ui \
    dialog_about_admb.ui \
    fleet_widget.ui \
    dialog_about_nft.ui \
    population_widget.ui \
    composition_widget.ui \
    errorfloatdialog.ui \
    dialog_run.ui \
    dialog_fileview.ui \
    newfleetdialog.ui \
    dialog_about_gui.ui \
    documentdialog.ui \
    dialog_runoptions.ui \
    chartdialog.ui \
    equationdialog.ui \
    dialoginfo.ui \
    dialoginputerror.ui \
    parameterview.ui \
    dialogequationview.ui \
    dialogparameterview.ui \
    dialog_copyright.ui \
    dialogtable.ui \
    mainwindowrun.ui \
    dialogdirectories.ui \
    dialogchosereport.ui \
    dialog_readme.ui

OTHER_FILES +=

RESOURCES += \
    stock_synth.qrc

DISTFILES += \
    ../SSI_readme.txt \
    ../SSI_readme_archive.txt \
    ../SSI_copyright.txt \
    SSI_icon.ico

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../Qt/5.12.2/static-mingw73_32/lib/ -lQt5Core \
 -lQt5Gui -lQt5Widgets -lQt5Charts -lQt5PrintSupport
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../Qt/5.12.2/static-mingw73_32/lib/ -lQt5Cored  \
 -lQt5Guid -lQt5Widgetsd -lQt5Chartsd -lQt5PrintSupportd
else:unix: LIBS += -L$$PWD/../../../../../../../Qt/5.12.2/static-mingw73_32/lib/ -lQt5Core \
 -lQt5Gui -lQt5Widgets -lQt5Charts -lQt5PrintSupport

INCLUDEPATH += $$PWD/../../../../../../../Qt/5.12.2/static-mingw73_32/include
DEPENDPATH += $$PWD/../../../../../../../Qt/5.12.2/static-mingw73_32/include
