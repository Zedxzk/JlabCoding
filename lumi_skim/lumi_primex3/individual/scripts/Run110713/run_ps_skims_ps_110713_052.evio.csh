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




# cd  /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/Run110713

cd  /volatile/halld/home/zhikun/ver05_lumi/Run110713


hd_root -PPLUGINS=lumi_skim -P110713/ps_110713_052.evio:SKIP_F250_FORMAT_ERROR=1 -PLUMI_PRIMEX:DIR="/volatile/halld/home/zhikun/ver05_lumi/Run110713/lumi_ps_110713_052.root" -PNTHREADS=4 -o /volatile/halld/home/zhikun/ver05_lumi/Run110713/ps_110713_052.root /cache/halld/RunPeriod-2022-08/calib/ver01/PS/110713/ps_110713_052.evio  >& /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run110713/ps_110713_052.log

#hd_root -PPLUGINS=lumi_skim -PLUMI_PRIMEX:DIR="/volatile/halld/home/zhikun/ver05_lumi/Run110713/lumi_ps_110713_052.root" -PNTHREADS=4 -o /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/Run110713/ps_110713_052.root /cache/halld/RunPeriod-2022-08/recon/ver01/ps/110713/ps_110713_052.evio  >& /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run110713/ps_110713_052.log
