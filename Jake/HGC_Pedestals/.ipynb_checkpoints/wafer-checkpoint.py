#! /usr/bin/env python3
# Author: Izaak Neutelings (September 2024)
# Description: Help functions for plotting wafers
# Sources:
#   https://gitlab.cern.ch/hgcal-dpg/hgcal-sysval-offline/-/merge_requests/13
#   https://github.com/ywkao/hexagonal_histograms/tree/main/waferMaps
#   https://gitlab.cern.ch/hgcal-dpg/hgcal-comm/-/tree/master/DQM/data
#   https://gitlab.cern.ch/hgcal-dpg/hgcal-sysval-offline/-/tree/master/data/geometry
# Instructions to run as script and create plotly templates for the wafers:
#  cd $CMSSW_BASE/src/HGCalCommissioning/LocalCalibration
#  python3 python/plot/wafer.py
import os, re
import json

script_dir = os.path.dirname(os.path.abspath(__file__))

def getlimits(xvals,xmin=1e10,xmax=-1e10,margin=0):
  """Help function to set x & y axis limits from list of lists."""
  for x in xvals:
    xmin_ = min(x)
    xmax_ = max(x)
    if xmin_<xmin: xmin = xmin_
    elif xmax_>xmax: xmax = xmax_
  if margin!=0:
    span = xmax-xmin
    xmin -= margin*span
    xmax += margin*span
  return xmin, xmax
  

def getzlimits(zvals,margin=0):
  """Help function to set z axis limits from list."""
  zmin, zmax = min(zvals), max(zvals)
  if zmin==0 and zmax==0:
    zmax = 1
  elif zmin==zmax:
    span  = abs(zmax)
    zmin -= 0.1*span
    zmax += 0.1*span
  elif margin!=0:
    span = zmax-zmin
    zmin -= margin*span
    zmax += margin*span
  return zmin, zmax
  

def fill_wafer_hist(ch_values,moduletype='ML_L'):
    """
    This method takes care of instatiating a hexplot for a given module type and fill it with the values for the required channels
    ch_values is a dict of (channel number, value)
    module_type is a string with the module type to be used in the hexplot
    """
    import ROOT
    hex_plot = ROOT.TH2Poly()
    hex_plot.SetDirectory(0)
    hex_plot.SetOption('COLZ') # draw with color bar by default
    hex_plot.GetXaxis().SetTitle("x [cm]")
    hex_plot.GetYaxis().SetTitle("y [cm]")
    file = ROOT.TFile.Open(f'{script_dir}/geometry_ML_F_wafer.root','R')
    iobj = 0
    for key in file.GetListOfKeys():
      obj = key.ReadObj()
      if not obj.InheritsFrom("TGraph") : continue
      
      # ignore CM
      isCM = (iobj % 39 == 37) or (iobj % 39 == 38)
      if isCM:
        iobj += 1
        continue
      
      hex_plot.AddBin(obj)
      eRx = int(iobj/39)
      idx = iobj - eRx*2 #take out 2 CM per eRx to get the proper idx
      #print(ch_values,moduletype)
      if idx < len(ch_values):
        hex_plot.SetBinContent(idx+1,ch_values[idx])
      else:
        raise ValueError(f'Length of values {len(ch_values)} does not accomodate for #obj={iobj} eRx={eRx} idx={idx}')
      iobj += 1
      
    file.Close()
    return hex_plot
    
