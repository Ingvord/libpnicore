#load python modules
import os.path as path
import platform
import os

from smod import ProgramVersion
from smod import GCCVersionParser
from smod import CheckProgram
from smod import LibFileNames

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

#create library namesl
libname = LibFileNames(env["LIBNAME"],env["VERSION"],env["SOVERSION"])
env.Append(LIBFULLNAME = libname.full_name(env))
env.Append(LIBSONAME   = libname.so_name(env))
env.Append(LIBLINKNAME = libname.link_name(env))


#create installation paths
env.Append(INCINSTPATH = path.join(env["PREFIX"],"include/pni/utils"))
if platform.machine()=="x86_64":
    env.Append(LIBINSTPATH = path.join(env["PREFIX"],"lib"))
else:
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


nullptr_test_code="""
int main(int argc,char **argv){
    char *ptr=nullptr;
    return 0;
}
"""

def CheckNullPtr(context):
    context.Message("Checking if compiler supports nullptr idiom ...")
    result = context.TryCompile(nullptr_test_code,".cpp")
    context.Result(result)
    return result

foreach_test_code="""
#include<iostream>
#include<vector>
int main(int argc,char **arv){
std::vector<int> vec = {1,2,3,4};
for(int &v: vec){
   std::cout<<v<<std::endl; 
}
return 0;
}
"""

def CheckForEach(context):
    context.Message("Check if compiler supports foreach loops ...")
    result = context.TryCompile(foreach_test_code,".cpp")
    context.Result(result)
    return result
    
#-------------------------------------------------------------------------------   
#start with configuration
conf = Configure(env,custom_tests =
{"CheckProgram":CheckProgram,"CheckNullPtr":CheckNullPtr,"CheckForEach":CheckForEach},
clean=False)

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

#checking compiler capabilities
if not conf.CheckNullPtr():
    print "nullptr not supported - use NULL"
    env.Append(CXXFLAGS=["-Dnullptr=NULL"])

if not conf.CheckForEach():
    print "foreach construction not supported - use workaround"
    env.Append(CXXFLAGS=["-DNOFOREACH"])

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
	
if not conf.CheckLib("cppunit",language="C++"):
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
#the next line is necessary for the linker on Debian system - this needs 
#a bit more information
if os.name == "posix":
	build_env.Append(LINKFLAGS=["-Wl,-h$LIBSONAME"]) 

Export("build_env")
Export("test_build_env")


#build
SConscript(["src/SConscript"])
SConscript(["test/SConscript","debian/SConscript"])
SConscript(["doc/SConscript"])

#set the default target
Default("all")
