# parameters for running the evio files


# If not neccessary， DO NOT MODIFY!
inputDir = "./run130043/"
outputDir = "./outputRootFiles/"
# If not neccessary， DO NOT MODIFY!


# type the command you want to run herem do not include the files, you can add other parameters
runCommand = "hd_root -PPLUGINS=pluginOfLMS"
# runCommand = "hd_root -PPLUGINS=pluginOfLMS -PTT:NO_CCDB=1 -PTT:XML_FILENAME=tt_RunPeriod-2025-01-default.xml "
fileType = ".evio" #can be changed to ".hddm"


# used for specifit runs
runId = [(130042, 130042)]
runPerfix = "Run"
RunDir = ["/cache/halld/RunPeriod-2025-01/rawdata/"]
#



# High Voltage Configuration :
#    ******************  Attention!!!   *****************
hvTemplateFile = 'temp.snap' # please input your current HV config File!
#    ******************  Attention!!!   *****************
itemToConfig   = "v0set 1 "
fitResDir      = "../muonsCommission/DigitHits/res.txt"
aimADC         = 10
#  ADC Countes = A * ( hv ) ^ alpha
#  Here A is canceled out and not needed
alpha          = 8
hvCap          = 1050
hvBot          = 950