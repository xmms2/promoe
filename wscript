# encoding: utf-8
# vim:set syntax=python expandtab :

"""
This file is a part of Promoe, an XMMS2 Client

Copyright (C) 2009 XMMS2 Team

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

APPVERSION = '0.1.0'
APPNAME = 'promoe'

srcdir = '.'
blddir = '_build_'

import os.path

def set_options(opt):
    opt.tool_options('gnu_dirs')
    opt.tool_options('qt4')

    opt.sub_options('lib')
    opt.sub_options('src')


def configure(conf):
    conf.check_tool('gnu_dirs')
    conf.check_tool('g++')
    conf.check_tool('qt4')

    conf.check_cfg(package='xmms2-client-cpp', args="--cflags --libs",
                   mandatory=1)

    conf.define('VERSION', APPVERSION)
    conf.define('PROMOE_VERSION', APPVERSION)
    conf.define('PROMOE_DATADIR', os.path.join(conf.env['DATADIR'], 'promoe'))

    # temprary, until all usages are removed
    conf.env['CXXDEFINES'] =[]
    conf.env['CXXDEFINES'].append('PROMOE_VERSION="%s"'%APPVERSION)
    conf.env['CXXDEFINES'].append('DATADIR="%s/promoe"'%conf.env['DATADIR'])

    # Path for 'promoe_config.h'
    conf.env.prepend_value("CPPPATH", conf.srcdir)

    conf.sub_config('lib')
    # Path needed to find library headers for local static library
    conf.env.prepend_value("CPPPATH", os.path.join(conf.srcdir, "lib"))

    conf.sub_config('src')

    conf.write_config_header('promoe_config.h')

def build(bld):
    bld.add_subdirs('lib')
    bld.add_subdirs('src')

    bld.install_files('${MANDIR}/man1', 'promoe.1')
    bld.install_files('${DATADIR}/applications', 'promoe.desktop')

