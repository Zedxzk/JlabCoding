#!/bin/tcsh 

source /group/halld/Software/build_scripts/gluex_env_jlab.csh   /work/halld/home/zhikun/lumi_skim/version_5.20.0.xml

setenv HALLD_MY /work/halld/home/zhikun/lumi_skim

setenv JANA_CALIB_CONTEXT "variation=default"

setenv JANA_PLUGIN_PATH /work/halld/home/zhikun/lumi_skim:$JANA_PLUGIN_PATH

setenv LD_LIBRARY_PATH /work/halld/home/zhikun/lumi_skim:$LD_LIBRARY_PATH

setenv PATH /work/halld/home/zhikun/lumi_skim:$PATH

