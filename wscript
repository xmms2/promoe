# encoding: utf-8
# vim:set syntax=python expandtab :

"""
This file is a part of Promoe, an XMMS2 Client

Copyright (C) 2009-2010 XMMS2 Team

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
"""

from waflib.TaskGen import feature, before, after
import os

# Automatically pick up include paths
@feature('cxx')
@after('process_source')
@before('apply_incpaths')
def add_includes_paths(self):
    incs = set(self.to_list(getattr(self, 'includes', '')))
    for x in self.compiled_tasks:
        incs.add(x.inputs[0].parent.path_from(self.path))
    self.includes = list(incs)

APPVERSION = '0.2.0-devel'
APPNAME = 'promoe'

def options(opt):
    opt.load('gnu_dirs compiler_cxx')
    try:
        opt.load('qt5')
    except:
        opt.load('qt4')

def configure(conf):
    conf.load('gnu_dirs compiler_cxx')

    try:
        conf.load('qt5')
    except:
        conf.load('qt4')

    conf.check_cfg(package='xmms2-client-cpp', args='--cflags --libs')
    conf.check_cfg(package='libarchive', args="--cflags --libs", mandatory=False)

    conf.check_cfg(package='avahi-client', args="--cflags --libs", mandatory=False)
    conf.check_cfg(package='avahi-qt4', args="--cflags --libs", mandatory=False)
    # if bld.env['LIB_AVAHI-QT4'] and bld.env['LIB_AVAHI-CLIENT']:
    #     conf.define('HAVE_SERVERBROWSER', 1)

    conf.env.append_unique('CXXFLAGS', ['-fPIC', '-DPIC'])

    conf.define('VERSION', APPVERSION)
    conf.define('PROMOE_VERSION', APPVERSION)
    conf.define('PROMOE_DATADIR', os.path.join(conf.env['DATADIR'], 'promoe'))
    conf.define('PROMOE_SKINDIR', os.path.join(conf.env['DATADIR'], 'promoe', 'skins'))

    conf.write_config_header('promoe_config.h')

def build(bld):
    bld.recurse('backend_xmmsclient++ dir_iterator data src')

