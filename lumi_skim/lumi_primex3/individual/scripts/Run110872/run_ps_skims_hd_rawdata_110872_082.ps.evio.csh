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




# cd  /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/Run110872

cd  /volatile/halld/home/zhikun/ver05_lumi/Run110872


hd_root -PPLUGINS=lumi_skim -PRun110872/hd_rawdata_110872_082.ps.evio:SKIP_F250_FORMAT_ERROR=1 -PLUMI_PRIMEX:DIR="/volatile/halld/home/zhikun/ver05_lumi/Run110872/lumi_ps_110872_082.root" -PNTHREADS=4 -o /volatile/halld/home/zhikun/ver05_lumi/Run110872/ps_110872_082.root /cache/halld/RunPeriod-2022-08/calib/ver01/PS/Run110872/hd_rawdata_110872_082.ps.evio  >& /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run110872/ps_110872_082.log

#hd_root -PPLUGINS=lumi_skim -PLUMI_PRIMEX:DIR="/volatile/halld/home/zhikun/ver05_lumi/Run110872/lumi_ps_110872_082.root" -PNTHREADS=4 -o /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/Run110872/ps_110872_082.root /cache/halld/RunPeriod-2022-08/recon/ver01/ps/Run110872/hd_rawdata_110872_082.ps.evio  >& /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run110872/ps_110872_082.log
