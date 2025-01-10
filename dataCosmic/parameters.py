# parameters for running the evio files


# If not neccessary， DO NOT MODIFY!
inputDir = "./evioFiles/"
outputDir = "./outputRootFiles/"
# If not neccessary， DO NOT MODIFY!


# type the command you want to run herem do not include the files, you can add other parameters
runCommand = "hd_root -PPLUGINS=cosmicRayTestEvio -PTT:NO_CCDB=1 -PTT:XML_FILENAME=tt_RunPeriod-2025-01-default.xml"
fileType = ".evio" #can be changed to ".hddm"

runId = []


# High Voltage Configuration :
#    ******************  Attention!!!   *****************
hvTemplateFile = 'temp.snap' # please input your current HV config File!
#    ******************  Attention!!!   *****************
itemToConfig   = "v0set 1 "
fitResDir      = "./DigitHits/res.txt"
aimADC         = 8
#  ADC Countes = A * ( hv ) ^ alpha
#  Here A is canceled out and not needed
alpha          = 8
hvCap          = 1050
hvBot          = 850