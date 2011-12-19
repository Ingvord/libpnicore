#load python modules
import os.path as path
import platform
import os

from smod import ProgramVersion
from smod import GCCVersionParser
from smod import CheckProgram


debug = ARGUMENTS.get("DEBUG",0)


var = Variables('BuildConfig.py')
if os.name == "nt":
	var.Add(PathVariable("PREFIX","set installation prefix",
                         "C:\\Program Files\\libpniutils",
                         PathVariable.PathAccept))
elif os.name=="posix":
	var.Add(PathVariable("PREFIX","set installation prefix","/usr"))

if os.name == "nt":
	var.Add(PathVariable("BOOSTPREFIX",
            "set the installation prefix for boost",
            "C:\\Program Files\\boost",PathVariable.PathAccept))
elif os.name == "posix":	
	var.Add(PathVariable("BOOSTPREFIX","set the installation prefix for boost",
                         "/usr"))
	
var.Add("VERSION","library version","0.0.0")
var.Add("LIBNAME","library name","pniutils")
var.Add("SOVERSION","SOVersion of the library (binary interface version)","0")
var.Add("CXX","set the compiler to use","g++")
var.Add("MAINTAINER","package maintainer for the project","Eugen Wintersberger")
var.Add("MAINTAINER_MAIL","e-mail of the package maintainer","eugen.wintersberger@desy.de")
var.Add("DOCDIR","installation directory for the documentation","")
var.Add("MANDIR","installation directory for man pages","share/man")
var.Add("LIBSONAME","name of the library including the SO-version","")
var.Add("LIBLINKNAME","name of the library used for linking","")
var.Add("LIBFULLNAME","full name of the library binary","")
var.Add("INCINSTPATH","installation path for header files","")
var.Add("LIBINSTPATH","library installation path","")
var.Add("PKGNAMEROOT","root package name (actually only used for Debian packages)","")
var.Add("PKGNAME","name of the package for installation","")

#need now to create the proper library suffix

#create the build environment
env = Environment(variables=var,ENV={'PATH':os.environ['PATH']},
				  tools=['default','packaging','textfile'])

#create library names
if os.name == "posix":
	env.Append(LIBFULLNAME = env["LIBPREFIX"]+env["LIBNAME"]+env["SHLIBSUFFIX"]+"."
    	                     +env["SOVERSION"]+"."+env["VERSION"])
	env.Append(LIBSONAME = env["LIBPREFIX"]+env["LIBNAME"]+env["SHLIBSUFFIX"]+"."+
    	                     env["SOVERSION"])
	env.Append(LIBLINKNAME = env["LIBPREFIX"]+env["LIBNAME"]+env["SHLIBSUFFIX"])
elif os.name == "nt":
	env.Append(LIBFULLNAME = env["LIBPREFIX"]+env["LIBNAME"]+"."+env["SOVERSION"]+
	                         "."+env["VERSION"]+env["SHLIBSUFFIX"])
	env.Append(LIBSONAME = env["LIBPREFIX"]+env["LIBNAME"]+"."+env["SOVERSION"]+env["SHLIBSUFFIX"])
	env.Append(LIBLINKNAME = env["LIBPREFIX"]+env["LIBNAME"]+env["SHLIBSUFFIX"])

#create installation paths
env.Append(INCINSTPATH = path.join(env["PREFIX"],"include/pni/utils"))
env.Append(LIBINSTPATH = path.join(env["PREFIX"],"lib"))

if env["DOCDIR"] == "":
    #set default documentation directory for installation
    env.Append(DOCDIR = path.join(env["PREFIX"],"share/doc/"+
                                  env["LIBPREFIX"]+env["LIBNAME"]
                                  +env["SOVERSION"]+"-doc"))

if env["PKGNAMEROOT"] == "":
    env.Append(PKGNAMEROOT = env["LIBPREFIX"]+env["LIBNAME"]+env["SOVERSION"])
    env.Append(PKGNAME = env["LIBPREFIX"]+env["LIBNAME"]+env["SOVERSION"])




#set default compiler flags
env.Append(CXXFLAGS = ["-Wall","-std=c++0x"])
env.Append(LIBPATH=path.join(env["BOOSTPREFIX"],"lib"))
env.Append(CPPPATH=path.join(env["BOOSTPREFIX"],"include"))

#set the proper compiler - this should be changed to something 
#more general - independent of the underlying operating system
env.Replace(CXX = env["CXX"])


gcc_version = GCCVersionParser().parse(prog=env["CXX"])
#set some flags depending on the compiler versions
if gcc_version < ProgramVersion(4,6,0):    
    env.Append(CXXFLAGS=["-Dnullptr=NULL"])
    
#-------------------------------------------------------------------------------   
#start with configuration
conf = Configure(env,custom_tests = {"CheckProgram":CheckProgram})

#check available programs
if not conf.CheckProgram("pdflatex -v"):
	print "pdflatex not installed!"
	Exit(1)
	
if not conf.CheckProgram("dot -V"):
	print "graphviz not installed!"
	Exit(1)
	
if not conf.CheckProgram("perl -v"):
	print "perl not installed!"
	Exit(1)

#check for header files
if not conf.CheckCXXHeader("boost/numeric/conversion/cast.hpp"):
	print "BOOST header file cast.hpp does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("boost/shared_ptr.hpp"):
	print "BOOST header shared_ptr.hpp does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("boost/static_assert.hpp"):
	print "BOOST header static_assert.hpp does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/TestFixture.h"):
	print "CPPUNIT header TestFixture.h does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/TestRunner.h"):
	print "CPPUNIT header TestRunner.h does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/extensions/HelperMacros.h"):
	print "CPPUNIT header HelperMacros.h does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/TestCaller.h"):
	print "CPPUNIT header TestCaller.h does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/TestResult.h"):
	print "CPPUNIT header TestResult.h does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/TextTestProgressListener.h"):
	print "CPPUNIT header TextTestProgressListener.h does not exist!"
	Exit(1)
	
if not conf.CheckCXXHeader("cppunit/ui/text/TextTestRunner.h"):
	print "CPPUNIT header TextTestRunner.h does not exist!"
	Exit(1)
	
if not conf.CheckCHeader("plplot/plplot.h"):
	print "PLPLOT header plplot.h does not exist!";
	Exit(1)
	
if not conf.CheckCXXHeader("plplot/plstream.h"):
	print "PLPLOT header plstream.h does not exist!";
	Exit(1)
	
#check for libraries
if not conf.CheckLib("plplotcxxd"):
	print "PLPLOT C++ bindings are not installed!"
	Exit(1)
	
if not conf.CheckLib("cppunit"):
	print "CPPUNIT unit test libraray is not installed!"
	Exit(1)
	
	
env = conf.Finish()


#create optimized environment
opt_env = env.Clone()
opt_env.Append(CXXFLAGS = ["-O2"])


#create debugging environment
dbg_env = env.Clone()
dbg_env.Append(CXXFLAGS = ["-O0","-g"])

if debug:
    build_env = dbg_env.Clone()
else:
    build_env = opt_env.Clone()


test_build_env = build_env.Clone()
python_build_env = build_env.Clone()
#the next line is necessary for the linker on Debian system - this needs 
#a bit more information
if os.name == "posix":
	build_env.Append(LINKFLAGS=["-Wl,-h$LIBSONAME"]) 

Export("build_env")
Export("test_build_env")
Export("python_build_env")


#build
SConscript(["src/SConscript"])
SConscript(["test/SConscript","debian/SConscript"])
SConscript(["doc/SConscript"])
SConscript(["python/SConscript"])

#set the default target
Default("all")
