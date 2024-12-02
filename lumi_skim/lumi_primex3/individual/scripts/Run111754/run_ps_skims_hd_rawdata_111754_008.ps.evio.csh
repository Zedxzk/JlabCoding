#!/bin/tcsh 

source /group/halld/Software/build_scripts/gluex_env_jlab.csh  /work/halld/home/zhikun/lumi_skim/version_5.20.0.xml 

setenv HALLD_MY /work/halld/home/zhikun/lumi_skim 

# setenv JANA_CALIB_URL mysql://ccdb_user@hallddb.jlab.org/ccdb

   
export CCDB_CONNECTION="mysql://ccdb_user@hallddb-farm.jlab.org/ccdb"

export JANA_CALIB_URL="mysql://ccdb_user@hallddb-farm.jlab.org/ccdb"

setenv JANA_CALIB_CONTEXT "variation=default"


setenv JANA_PLUGIN_PATH /work/halld/home/zhikun/lumi_skim:$JANA_PLUGIN_PATH

setenv LD_LIBRARY_PATH /work/halld/home/zhikun/lumi_skim:$LD_LIBRARY_PATH

setenv PATH /work/halld/home/zhikun/lumi_skim:$PATH




# cd  /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/Run111754

cd  /volatile/halld/home/zhikun/ver05_lumi/Run111754


hd_root -PPLUGINS=lumi_skim -PRun111754/hd_rawdata_111754_008.ps.evio:SKIP_F250_FORMAT_ERROR=1 -PLUMI_PRIMEX:DIR="/volatile/halld/home/zhikun/ver05_lumi/Run111754/lumi_ps_111754_008.root" -PNTHREADS=4 -o /volatile/halld/home/zhikun/ver05_lumi/Run111754/ps_111754_008.root /cache/halld/RunPeriod-2022-08/calib/ver01/PS/Run111754/hd_rawdata_111754_008.ps.evio  >& /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run111754/ps_111754_008.log

#hd_root -PPLUGINS=lumi_skim -PLUMI_PRIMEX:DIR="/volatile/halld/home/zhikun/ver05_lumi/Run111754/lumi_ps_111754_008.root" -PNTHREADS=4 -o /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/Run111754/ps_111754_008.root /cache/halld/RunPeriod-2022-08/recon/ver01/ps/Run111754/hd_rawdata_111754_008.ps.evio  >& /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run111754/ps_111754_008.log
