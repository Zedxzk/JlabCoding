# parameters for running the evio files


# If not neccessary， DO NOT MODIFY!
inputDir = "./evioFiles/"
outputDir = "./outputRootFiles/"
# If not neccessary， DO NOT MODIFY!


# type the command you want to run herem do not include the files, you can add other parameters
runCommand = "hd_root -PTT:XML_FILENAME=tt_RunPeriod-2025-01-default.xml -PPLUGINS=cosmicRayTestEvio "
fileType = ".evio" #can be changed to ".hddm"

#if want to run in personal folder, leave publicFileDir empty.
#if want to run in /cache, type in the whole path that includes the folder
# Eg. '/cache/halld/RunPeriod-2025-01/rawdata/Run130069/'
publicFileDir = ['']
runId = [(),]