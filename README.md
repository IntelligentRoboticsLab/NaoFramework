NaoFramework
============

A modular framework for working with NAO robots.


The idea behind the framework, which is pretty much the same idea behind
the other frameworks, is to have multiple threads that concurrently do stuff
and to make them talk. The way the current framework is structured is
that we have a Brain class which handles these threads, which are currently
called BrainWave(s). Each BrainWave represents a thread, and you can put
as many as you wish in the program. The naoTH (for what I know) uses
two (Motion and Cognition), and BHuman (for what I read), has at least
4 (Motion, Cognition, Debug, and a small server detached that moves the
robot).

Each BrainWave has the task of managing a bunch of modules, and it
calls them in a loop forever. In this framework modules are loaded as shared
libraries, which are assumed to be extended from the DynamicModuleInter-
face class. Another class Blackboard, has the task of making everything
communicate. More info about these classes is in the code (you can doxy-
gen it).

The way this framework is intended to be used is to have some big
library of data-types, which are simple structs (hopefully no methods) that
contain data, and that are used to share informations by multiple modules.
This library would reside in a separate repo. Each module would then also
reside in their separate repo, and would import the necessary headers from
the framework and the big library of data-structures that are needed, and
include whatever it needs to do its work. It would then be compiled as a
dynamic library, which would then be fed to the framework in order to do
its job.

You can see that having modules be dynamic libraries makes it really
easy to run arbitrary code in the framework, while at the same time keep-
ing an extremely clean workspace as the runnable project would only need
to include the dll's of the modules, the executable of the framework and
whatever code you are working with, and not ALL the modules's code.

Currently there are two prototype modules in the repo, in the folder
moduleExamples. Each can be compiled using the script in the folder, like
so:

    ./compileModule Writer
    ./compileModule Reader

The idea is that their code would in the future reside in different repos.
Also it would be cool to have a general-module-repo where a default module
is kept with all the possible documentation ever about how to do things, so
one can fork that and in 15 minutes already do work on new things with
minimal experience.
