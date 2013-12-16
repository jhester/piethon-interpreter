#!/usr/bin/python

import os, sys
import subprocess

class TestHarness(object):
  def __init__(self, dir = "testsuite"):
    self.testcases = []
    self.failed = []
    files = os.listdir(dir)
    for file in files:
      if file[-3:] == ".py":
        testfile = os.path.join("testsuite", file)
        returnCode = subprocess.call("./run < " + testfile, shell=True)
        if returnCode > 0:
          self.failed.append(testfile)
        else:
          self.testcases.append(testfile)
  def __repr__(self):
    msg = '\n'
    for x in self.testcases:
      msg += "Passed: " + x + '\n'
    msg += '\n'
    for x in self.failed:
      msg += "***FAILED***:" + x + '\n'
    return msg

if __name__ == "__main__":
  try:
    tester = None
    if len(sys.argv) == 2:
      tester = TestHarness(sys.argv[1])
    else:
      tester = TestHarness()
    print tester
  except OSError: 
    print sys.argv[1], "not found"
  except Exception: 
    print "Oops!"
