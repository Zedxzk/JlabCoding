# parameters for running the evio files


# If not neccessary， DO NOT MODIFY!
inputDir = "./evioFiles/"
outputDir = "./outputRootFiles/"
# If not neccessary， DO NOT MODIFY!

# type the command you want to run herem do not include the files, you can add other parameters
runCommand = "hd_root -PPLUGINS=cosmicRayTestEvio -PTT:NO_CCDB=1 -PTT:XML_FILENAME=tt_RunPeriod-2025-01-default.xml"
fileType = ".evio" #can be changed to ".hddm" or others.

runId = []

# High Voltage Configuration :
#    ******************  Attention!!!   *****************
hvTemplateFile = 'ECAL_HV_22_Jan_2025_20_55_07.snap' # please input your current HV config File!
#    ******************  Attention!!!   *****************

itemToConfig   = "v0set 1 "
# itemToConfig   = "trip 1 "
fitResDir      = "./DigitHits/res.txt"
#  ADC Countes = A * ( hv ) ^ alpha
#  Here A is canceled out and not needed
aimADC         = 10
alpha          = 8
hvCap          = 1050
hvBot          = 850

# Set config for each channel. If you use detailed settings, please set the following parameters, and run -generateSettings command first. The setting file will be based on the universal settings above.
# And you can modify this file.
# If you use the same settings for all channels, please set the following parameters to None.
# If you use settings for each channel, the settings above for universal settings will be ignored. You can only use part of these settings.
aimADCSettings = None
alphaSettings = None
hvCapSettings = None
hvBotSettings = None
# aimADCSettings = "aimADCSettings.txt"
# alphaSettings = "alphaSettings.txt"
# hvCapSettings = "hvCapSettings.txt"
# hvBotSettings = "hvBotSettings.txt"


#    ***********  Set specifc Item ***********
# itemSettings = "tripSettings.txt"
itemSettings = None

itemValue   = 1
# itemValue   = None
